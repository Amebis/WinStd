# WinStd

Provides templates and function helpers for Windows Win32 API using Standard C++ in Microsoft Visual C++ 2017-2019

## Features

### Portable

This project does not require building. Just `#include` individual files from this repository into your source code and get started.

### Memory and Resource Helper Classes

Simplify memory and resource management. The classes release memory and resources automatically. They are like _smart-pointers_ for various Windows resources. Once created, you use the class instance as a snap-in replacement for pointers/handles parameters in the standard Win32 API function calls.

#### Example

```C++
// Load and set icon.
winstd::library lib_shell32(LoadLibraryEx(_T("shell32.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE));
if (!lib_shell32)
    throw winstd::win_runtime_error("LoadLibraryEx failed");
m_note_icon->SetIcon(wxLoadIconFromResource(lib_shell32, MAKEINTRESOURCE(48)));
```

### Functions and Templates

Different Win32 API functions have different ways of returning variable-sized data. Getting tired of carefully studying MSDN for each particular Win32 API function how to preallocate the output memory correctly? We too...

WinStd provides a subset of Win32 API identically named functions (C++ polymorphism to the rescue), where one can use `std::string`, `std::wstring`, `std::vector<>` etc. as an output parameter. WinStd handles all the dirty work with memory allocation for you, so you can focus on your code.

#### Example

```C++
// Encode response as OEM.
std::string response;
WideCharToMultiByte(CP_OEMCP, 0, L"Copyright \u00A9 2017", -1, response, NULL, NULL);
std::cout << response.c_str() << std::endl;
```

### String Formatters

For those situations where one must quckly compose a temporary string using `sprintf()` or `FormatMessage()`. Or, convert a GUID to a string on the fly.

#### Example

```C++
if (dwMaxSendPacketSize < sizeof(EapPacket))
    throw std::invalid_argument(
        winstd::string_printf(
            "Maximum packet size too small (minimum: %zu, available: %u).",
            sizeof(EapPacket) + 1,
            dwMaxSendPacketSize));
```

## What WinStd Is Not

WinStd is not trying to be a full-fledged object-oriented framework on top of Win32 API. We have Microsoft to publish those once every few years - and obsolete it when they loose interest. WinStd aims at augmenting Win32 API with a little bit of help from C++.

## Usage

1. Clone the repository into your solution folder.
2. Add WinStd's `include` folder to _Additional Include Directories_ in your project's C/C++ settings.
3. Include `.h` files from WinStd as needed:
```C++
#include <WinStd/Shell.h>
#include <string>
#include <iostream>

using namespace std;

void main()
{
    wstring path;
    PathCanonicalizeW(path, L"C:\\Windows\\Temp\\test\\..");
    wcout << path.c_str() << endl;
}
```

An auto-generated documentation is [here](https://amebis.github.io/WinStd/).

More examples and use-cases can be found in [GÉANTLink](https://github.com/Amebis/GEANTLink) and [ZRCola](https://github.com/Amebis/ZRCola) projects source code. They make heavy use of WinStd. Examples can also be found in the `UnitTests` project.

This is a one-man project for the time being, so the Win32 API support is far from complete. It is added as needed. Contributions are welcome.
