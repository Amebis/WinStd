/*
    SPDX-License-Identifier: MIT
    Copyright © 1991-2023 Amebis
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

/// @copydoc PathRemoveBackslashW()
template<class _Traits, class _Ax>
static void PathRemoveBackslashA(_Inout_ std::basic_string<char, _Traits, _Ax>& sValue)
{
    char szBuffer[MAX_PATH + 1];
    size_t len = sValue.length();
    if (len < _countof(szBuffer)) {
        memcpy(szBuffer, sValue.c_str(), len);
        szBuffer[len] = 0;
        PathRemoveBackslashA(szBuffer);
        sValue.assign(szBuffer);
    }
    else {
        std::unique_ptr<char[]> buf(new char[len + 1]);
        memcpy(buf.get(), sValue.c_str(), len);
        buf[len] = 0;
        PathRemoveBackslashA(buf.get());
        sValue.assign(buf.get());
    }
}

///
/// Removes the trailing backslash from a given path.
///
/// \sa [PathRemoveBackslashW function](https://learn.microsoft.com/en-us/windows/win32/api/shlwapi/nf-shlwapi-pathremovebackslashw)
///
template<class _Traits, class _Ax>
static void PathRemoveBackslashW(_Inout_ std::basic_string<wchar_t, _Traits, _Ax>& sValue)
{
    wchar_t szBuffer[MAX_PATH + 1];
    size_t len = sValue.length();
    if (len < _countof(szBuffer)) {
        wmemcpy(szBuffer, sValue.c_str(), len);
        szBuffer[len] = 0;
        PathRemoveBackslashW(szBuffer);
        sValue.assign(szBuffer);
    }
    else {
        std::unique_ptr<wchar_t[]> buf(new wchar_t[len + 1]);
        wmemcpy(buf.get(), sValue.c_str(), len);
        buf[len] = 0;
        PathRemoveBackslashW(buf.get());
        sValue.assign(buf.get());
    }
}

/// @}
