/*
    Copyright 1991-2020 Amebis
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

///
/// \defgroup WinStdShellWAPI Shell API
/// Integrates WinStd classes with Microsoft Shell API
///
#include "Common.h"

#include <Shlwapi.h>

#include <string>

/// \addtogroup WinStdShellWAPI
/// @{

/// @copydoc PathCanonicalizeW()
template<class _Elem, class _Traits, class _Ax> inline BOOL PathCanonicalizeA(_Inout_ std::basic_string<_Elem, _Traits, _Ax> &sValue, _In_ LPCSTR pszPath);

///
/// Simplifies a path by removing navigation elements such as "." and ".." to produce a direct, well-formed path, and stores it in a std::wstring string.
///
/// \sa [PathCanonicalize function](https://msdn.microsoft.com/en-us/library/windows/desktop/bb773569.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline BOOL PathCanonicalizeW(_Inout_ std::basic_string<_Elem, _Traits, _Ax> &sValue, _In_ LPCWSTR pszPath);

/// @}

#pragma once


template<class _Elem, class _Traits, class _Ax>
inline BOOL PathCanonicalizeA(_Inout_ std::basic_string<_Elem, _Traits, _Ax> &sValue, _In_ LPCSTR pszPath)
{
    assert(0); // TODO: Test this code.

    // Allocate buffer on heap and read into it.
    _Elem szBuffer[MAX_PATH + 1];
    BOOL bResult = ::PathCanonicalizeA(szBuffer, pszPath);
    sValue.assign(szBuffer, bResult ? MAX_PATH : 0);
    return bResult;
}


template<class _Elem, class _Traits, class _Ax>
inline BOOL PathCanonicalizeW(_Inout_ std::basic_string<_Elem, _Traits, _Ax> &sValue, _In_ LPCWSTR pszPath)
{
    assert(0); // TODO: Test this code.

    _Elem szBuffer[MAX_PATH + 1];
    BOOL bResult = ::PathCanonicalizeW(szBuffer, pszPath);
    sValue.assign(szBuffer, bResult ? MAX_PATH : 0);
    return bResult;
}
