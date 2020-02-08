# WinStd [![Build status](https://ci.appveyor.com/api/projects/status/1e8868i88s10cp59?svg=true)](https://ci.appveyor.com/project/Amebis/winstd)

Provides additional templates and function helpers for Windows API using Standard C++

## Features

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
            "Maximum packet size too small (minimum: %u, available: %u).",
            sizeof(EapPacket) + 1,
            dwMaxSendPacketSize));
```

## Building

The `WinStd.vcxproj` requires Microsoft Visual Studio 2010 SP1 and `..\..\include` folder with `common.props`, `Debug.props`, `Release.props`, `Win32.props`, and `x64.props` files to customize building process for individual applications.

## Usage

1. Clone the repository into your solution folder.
2. Add the `WinStd.vcxproj` to your solution.
3. Add WinStd's `include` folder to _Additional Include Directories_ in your project's C/C++ settings.
4. Add a new reference to WinStd project from your project's common properties.
5. Include `.h` files from WinStd as needed:
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

## Debugging

For user friendlier display of variables of WinStd types in Visual Studio 2010 debugger, find the file `autoexp.dat` in your `C:\Program Files (x86)\Microsoft Visual Studio 2010` and open it with Notepad.
Locate the `[AutoExpand]` section and add the following lines:
```
winstd::variant=$BUILTIN(VARIANT)
```
Locate the `[Visualizer]` section and add the following lines:
```
; WinStd
winstd::com_obj<*>|winstd::handle<*>|winstd::dplhandle<*>|winstd::cert_context|winstd::cert_chain_context|winstd::cert_store|winstd::crypt_prov|winstd::crypt_hash|winstd::crypt_key|winstd::event_provider|winstd::library|winstd::heap{
        preview            ([$e.m_h])
}
winstd::bstr{
        preview            ([$e.m_h,su])
        stringview        ([$e.m_h,sub])
}
winstd::vector_queue<*>{
    preview (
        #(
            "[",
            $e.m_count,
            "](",
            #array(
                expr: $e.m_data._Myptr[($e.m_head + $i)%$e.m_size_max],
                size: $e.m_count
            ),
            ")"
        )
    )

    children (
        #(
            #([size] : $e.m_count),
            #([capacity] : $e.m_size_max),
            #array(
                expr: $e.m_data._Myptr[($e.m_head + $i)%$e.m_size_max],
                size: $e.m_count
            )
        )
    )
}
winstd::sanitizing_vector<*>{
    preview (
        #(
            "S[",
            $e._Mylast - $e._Myfirst,
            "](",
            #array(
                expr: $e._Myfirst[$i],
                size: $e._Mylast - $e._Myfirst
            ),
            ")"
        )
    )

    children (
        #(
            #([size] : $e._Mylast - $e._Myfirst),
            #([capacity] : $e._Myend - $e._Myfirst),
            #array(
                expr: $e._Myfirst[$i],
                size: $e._Mylast - $e._Myfirst
            )
        )
    )
}
winstd::eap_attr{
    preview (
        #(
            $e.eaType,
            " [",
            $e.dwLength,
            "](",
            #array(
                expr: $e.pValue[$i],
                size: $e.dwLength
            ),
            ")"
        )
    )

    children (
        #(
            #([type] : $e.eaType),
            #([size] : $e.dwLength),
            #array(
                expr: $e.pValue[$i],
                size: $e.dwLength
            )
        )
    )
}
winstd::event_data{
    preview (
        #(
            "[",
            $e.Size,
            "](",
            #array(
                expr: [((unsigned char*)$e.Ptr)[$i],x],
                size: $e.Size
            ),
            ")"
        )
    )

    children (
        #(
            #([size] : $e.Size),
            #array(
                expr: [((unsigned char*)$e.Ptr)[$i],x],
                size: $e.Size
            )
        )
    )
}
```
