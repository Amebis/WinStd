#WinStd
Provides additional templates and function helpers for Windows API using Standard C++

##Building
- The _.h_ files can be used individually. However, we do encourage you to include the entire library project and reference it in dependant projects of your solution, as WinStd might develop some non-inline code over time.
- The _WinStd.vcxproj_ requires Microsoft Visual Studio 2010 SP1 and _..\\..\\include_ folder with _common.props_, _Debug.props_, _Release.props_, _Win32.props_, and _x64.props_ files to customize building process for individual applications.

##Usage
1. Clone the repository into your solution folder.
2. Add the _WinStd.vcxproj_ to your solution.
3. Add WinStd's _include_ folder to _Additional Include Directories_ in your project's C/C++ settings.
4. Add a new reference to WinStd project from your project's common properties.
5. Include _.h_ files from WinStd as needed:
```C
#include <WinStd/Shell.h>
#include <string>
#include <iostream>

void main()
{
  std::wstring path;
  PathCanonicalize(path, _T("C:\\Windows\\Temp\\test\\.."));
  std::cout << (LPCTSTR)path << std::endl;
}
```
