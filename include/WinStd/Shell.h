/*
    Copyright 1991-2016 Amebis
    Copyright 2016 GÉANT

    This file is part of WinStd.

    Setup is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Setup is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Setup. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <atlstr.h>
#include <Shlwapi.h>

///
/// \defgroup WinStdShellWAPI Shell API
/// Integrates WinStd classes with Microsoft Shell API
///
/// @{

///
/// Simplifies a path by removing navigation elements such as "." and ".." to produce a direct, well-formed path, and stores it in a std::string string.
///
/// \sa [PathCanonicalize function](https://msdn.microsoft.com/en-us/library/windows/desktop/bb773569.aspx)
///
inline BOOL PathCanonicalizeA(__out std::string &sValue, __in LPCSTR pszPath)
{
    // Allocate buffer on heap and read into it.
    CHAR szBuffer[MAX_PATH + 1];
    BOOL bResult = ::PathCanonicalizeA(szBuffer, pszPath);
    sValue.assign(szBuffer, bResult ? MAX_PATH : 0);
    return bResult;
}


///
/// Simplifies a path by removing navigation elements such as "." and ".." to produce a direct, well-formed path, and stores it in a std::wstring string.
///
/// \sa [PathCanonicalize function](https://msdn.microsoft.com/en-us/library/windows/desktop/bb773569.aspx)
///
inline BOOL PathCanonicalizeW(__out std::wstring &sValue, __in LPCWSTR pszPath)
{
    WCHAR szBuffer[MAX_PATH + 1];
    BOOL bResult = ::PathCanonicalizeW(szBuffer, pszPath);
    sValue.assign(szBuffer, bResult ? MAX_PATH : 0);
    return bResult;
}

/// @}
