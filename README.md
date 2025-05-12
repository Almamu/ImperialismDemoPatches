# user32.dll Shim for *Imperialism* Demo

This project provides a custom `user32.dll` **shim** (`shim32.dll`) for the *Imperialism* demo game. It is used to intercept specific Windows API calls and bypass certain demo-related checks and restrictions.

## Pre-requirements
- NASM

## Overview

The original *Imperialism* demo performs user interface checks via `USER32.DLL` that limit its functionality. This shim implements just enough of the required API to allow the game to run, while bypassing those limitations.

The shim is built using [ShimmySham](https://github.com/Miyuki333/ShimmySham), which generates stub implementations of Windows DLL exports for use in projects like this.

## Usage

### 1. Modify the Binary to Use `shim32.dll`

To redirect the game to use the shim instead of the real `USER32.DLL`, patch the import table in the binary:

#### Manual Binary Patch Instructions

1. Open the demo executable in a hex editor (e.g., HxD).
2. Search for the ASCII string `USER32.DLL` in the import section.
3. Replace it with `SHIM32.DLL` (10 characters total — including null terminator).
4. Save the binary.

> ⚠️ This will only work if the binary uses a standard dynamic import. Always test in a safe, local environment.

### 2. Place shim32.dll

Place `shim32.dll` in the same directory as the patched demo executable. This ensures the Windows loader will find your custom DLL before falling back to the system one.

### 3. Function Patching

The shim includes specific patches to stub or override particular `USER32.DLL` functions used by the demo to enforce restrictions. These may include:

* Overriding dialog box or message box behavior
* Bypassing window style checks
* Removing demo-mode nags or UI blocks

You can modify these in `shim32.c` to match other demo behaviors as needed.

## Building

> ❗ **Only 32-bit builds are supported.** The original game is a 32-bit application and will not load a 64-bit DLL.

To build:

1. Open the provided Visual Studio solution file: `shim32.sln`
2. Select the `x86` platform and the desired configuration (`Release` or `Debug`)
3. Build the solution using **Build → Build Solution** (`Ctrl+Shift+B`)

The output `shim32.dll` will be located in the corresponding `Release` or `Debug` folder inside the `x86` build directory.

## Legal Notice

This project is for educational and preservation purposes. The *Imperialism* demo is abandonware and this patch is intended to improve accessibility for research and historical interest.