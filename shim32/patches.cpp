#include "framework.h"

/**
 * This code comes from various sources, so I'm not entirely sure who wrote it originally
 */

static BYTE originalCode[5];
static PBYTE originalEP = 0;

void Imperialism_ApplyYearDemoPatches() {
	// from
	// - jge loc_576DB8
	// to
	// - nop
	// - jmp loc_576DB8
	*(BYTE*)0x576929 = 0x80;
	*(BYTE*)0x57692A = 0xE9;
}

void Imperialism_ApplyPatches() {
	// make the code section writable so we can happyly apply our patches

	Imperialism_ApplyYearDemoPatches();
}

void Main_UnprotectModule(HMODULE hModule)
{
	PIMAGE_DOS_HEADER header = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((DWORD)hModule + header->e_lfanew);

	// unprotect the entire PE image
	SIZE_T size = ntHeader->OptionalHeader.SizeOfImage;
	DWORD oldProtect;
	VirtualProtect((LPVOID)hModule, size, PAGE_EXECUTE_READWRITE, &oldProtect);
}

void Main_DoInit()
{
	// unprotect our entire PE image
	HMODULE hModule;
	if (SUCCEEDED(GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)Main_DoInit, &hModule)))
	{
		Main_UnprotectModule(hModule);
	}

	// Patch the game memory to get the game version
	Imperialism_ApplyPatches();

	// return to the original EP
	memcpy(originalEP, &originalCode, sizeof(originalCode));
	__asm jmp originalEP
}

void Imperialism_Init()
{
	// find the entry point for the executable process, set page access, and replace the EP
	HMODULE hModule = GetModuleHandle(NULL); // passing NULL should be safe even with the loader lock being held (according to ReactOS ldr.c)

	if (hModule)
	{
		PIMAGE_DOS_HEADER header = (PIMAGE_DOS_HEADER)hModule;
		PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((DWORD)hModule + header->e_lfanew);

		Main_UnprotectModule(hModule);

		// back up original code
		PBYTE ep = (PBYTE)((DWORD)hModule + ntHeader->OptionalHeader.AddressOfEntryPoint);
		memcpy(originalCode, ep, sizeof(originalCode));

		// patch to call our EP
		int newEP = (int)Main_DoInit - ((int)ep + 5);
		ep[0] = 0xE9; // for some reason this doesn't work properly when run under the debugger
		memcpy(&ep[1], &newEP, 4);

		originalEP = ep;
	}
}
