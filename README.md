# WinStd [![Build status](https://ci.appveyor.com/api/projects/status/1e8868i88s10cp59?svg=true)](https://ci.appveyor.com/project/Amebis/winstd)

Provides additional templates and function helpers for Windows API using Standard C++ in Microsoft Visual C++ 2017-2019

## Features

### Portable

This project does not require building. Just `#include` individual files from this repository into your source code and get started.

### Lightweight Classes

...to simplify Windows allocated memory and resources focused on their release to prevent leakage

The classes provide unified create methods and free destructors. They are like _smart-pointers_ for various Windows resources. Once created, you use the class instance as a snap-in replacement for pointers/handles parameters in the standard Win32 API functions.

#### Example

```C++
// Load and set icon.
winstd::library lib_shell32;
if (lib_shell32.load(_T("shell32.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE))
    m_note_icon->SetIcon(wxLoadIconFromResource(lib_shell32, MAKEINTRESOURCE(48)));
```

### Functions and Templates

...to extend standard Win32 API functions for variable-size outputs

Different Win32 API functions have different ways of returning variable-sized data. Getting tired of carefully studying MSDN for each particular Win32 API function how to preallocate the output memory correctly? We too...

WinStd provides a subset of Win32 API identically named functions (C++ polymorphism to the rescue), where one can use `std::string`, `std::wstring`, `std::vector<>` etc. as an output parameter. WinStd handles all the dirty work with memory allocation for you, so you can focus on your code.

#### Example

```C++
// Encode response as OEM.
std::string response;
WideCharToMultiByte(CP_OEMCP, 0, L"Copyright \u00A9 2017", response, NULL, NULL);
std::cout << response.c_str() << std::endl;
```

### String Formatters

...for those situations where one must quckly compose a temporary string using `sprintf()` or `FormatMessage()`

#### Example

```C++
if (dwMaxSendPacketSize < sizeof(EapPacket))
    throw std::invalid_argument(
        winstd::string_printf(
            "Maximum packet size too small (minimum: %zu, available: %u).",
            sizeof(EapPacket) + 1,
            dwMaxSendPacketSize));
```

## Usage

1. Clone the repository into your solution folder.
2. Add WinStd's `include` folder to _Additional Include Directories_ in your project's C/C++ settings.
3. Include `.h` files from WinStd as needed:
```C++
#include <WinStd/Shell.h>
#include <string>
#include <iostream>

void main()
{
  std::wstring path;
  PathCanonicalize(path, _T("C:\\Windows\\Temp\\test\\.."));
  std::cout << path.c_str() << std::endl;
}
```

More examples and use-cases can be found in [GÉANTLink](https://github.com/Amebis/GEANTLink) and [ZRCola](https://github.com/Amebis/ZRCola) projects source code. They make heavy use of WinStd.
