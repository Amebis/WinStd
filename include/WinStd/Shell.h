/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
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
template<class _Elem, class _Traits, class _Ax>
static BOOL PathCanonicalizeA(_Inout_ std::basic_string<_Elem, _Traits, _Ax> &sValue, _In_ LPCSTR pszPath);

///
/// Simplifies a path by removing navigation elements such as "." and ".." to produce a direct, well-formed path, and stores it in a std::wstring string.
///
/// \sa [PathCanonicalize function](https://msdn.microsoft.com/en-us/library/windows/desktop/bb773569.aspx)
///
template<class _Elem, class _Traits, class _Ax>
static BOOL PathCanonicalizeW(_Inout_ std::basic_string<_Elem, _Traits, _Ax> &sValue, _In_ LPCWSTR pszPath);

/// @}

#pragma once


template<class _Elem, class _Traits, class _Ax>
static BOOL PathCanonicalizeA(_Inout_ std::basic_string<_Elem, _Traits, _Ax> &sValue, _In_ LPCSTR pszPath)
{
    assert(0); // TODO: Test this code.

    // Allocate buffer on heap and read into it.
    _Elem szBuffer[MAX_PATH + 1];
    BOOL bResult = ::PathCanonicalizeA(szBuffer, pszPath);
    sValue.assign(szBuffer, bResult ? MAX_PATH : 0);
    return bResult;
}


template<class _Elem, class _Traits, class _Ax>
static BOOL PathCanonicalizeW(_Inout_ std::basic_string<_Elem, _Traits, _Ax> &sValue, _In_ LPCWSTR pszPath)
{
    assert(0); // TODO: Test this code.

    _Elem szBuffer[MAX_PATH + 1];
    BOOL bResult = ::PathCanonicalizeW(szBuffer, pszPath);
    sValue.assign(szBuffer, bResult ? MAX_PATH : 0);
    return bResult;
}
