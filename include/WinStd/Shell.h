/*
    SPDX-License-Identifier: MIT
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
*/

/// \defgroup WinStdShellWAPI Shell API

#pragma once

#include "Common.h"
#include <Shlwapi.h>
#include <string>

/// \addtogroup WinStdShellWAPI
/// @{

/// @copydoc PathCanonicalizeW()
template<class _Traits, class _Ax>
static BOOL PathCanonicalizeA(_Inout_ std::basic_string<char, _Traits, _Ax> &sValue, _In_ LPCSTR pszPath)
{
    char szBuffer[MAX_PATH + 1];
    BOOL bResult = ::PathCanonicalizeA(szBuffer, pszPath);
    sValue.assign(szBuffer, bResult ? MAX_PATH : 0);
    return bResult;
}

///
/// Simplifies a path by removing navigation elements such as "." and ".." to produce a direct, well-formed path, and stores it in a std::wstring string.
///
/// \sa [PathCanonicalize function](https://msdn.microsoft.com/en-us/library/windows/desktop/bb773569.aspx)
///
template<class _Traits, class _Ax>
static BOOL PathCanonicalizeW(_Inout_ std::basic_string<wchar_t, _Traits, _Ax> &sValue, _In_ LPCWSTR pszPath)
{
    wchar_t szBuffer[MAX_PATH + 1];
    BOOL bResult = ::PathCanonicalizeW(szBuffer, pszPath);
    sValue.assign(szBuffer, bResult ? MAX_PATH : 0);
    return bResult;
}

/// @}
