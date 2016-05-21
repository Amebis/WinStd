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

##Debugging
For user friendlier display of variables of WinStd types in Visual Studio 2010 debugger, find the file _autoexp.dat_ in your _C:\Program Files (x86)\Microsoft Visual Studio 2010_ and open it with Notepad.
Locate the `[AutoExpand]` section and add the following lines:
```
winstd::variant=$BUILTIN(VARIANT)
```
Locate the `[Visualizer]` section and add the following lines:
```
; WinStd
winstd::com_obj<*>|winstd::handle<*>|winstd::dplhandle<*>|winstd::cert_context|winstd::cert_chain_context|winstd::cert_store|winstd::crypt_prov|winstd::crypt_hash|winstd::crypt_key|winstd::event_provider|winstd::library|winstd::heap{
		preview			([$e.m_h])
}
winstd::bstr{
		preview			([$e.m_h,su])
		stringview		([$e.m_h,sub])
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
