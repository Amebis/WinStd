/*
    SPDX-License-Identifier: MIT
    Copyright © 1991-2024 Amebis
    Copyright © 2016 GÉANT
*/

/// \defgroup WinStdWinHTTP Windows HTTP Client

#pragma once

#include "Common.h"
#include <winhttp.h>
#include <string>

/// \addtogroup WinStdWinHTTP
/// @{

///
/// Retrieves header information associated with an HTTP request.
///
/// \sa [WinHttpQueryHeaders function](https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpqueryheaders)
///
inline _Success_(return) BOOL WinHttpQueryHeaders(_In_ HINTERNET hRequest, _In_ DWORD dwInfoLevel, _Out_ DWORD& dwData)
{
    DWORD dwSize = sizeof(dwData);
    if (WinHttpQueryHeaders(hRequest, dwInfoLevel | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &dwData, &dwSize, WINHTTP_NO_HEADER_INDEX)) {
        assert(dwSize == sizeof(dwData));
        return TRUE;
    }
    return FALSE;
}

///
/// Retrieves header information associated with an HTTP request.
///
/// \sa [WinHttpQueryHeaders function](https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpqueryheaders)
///
inline _Success_(return) BOOL WinHttpQueryHeaders(_In_ HINTERNET hRequest, _In_ DWORD dwInfoLevel, _Inout_ std::wstring& sData)
{
    DWORD dwSize = 0x100;
    for (;;) {
        sData.resize(dwSize - 1);
        dwSize *= sizeof(WCHAR);
        if (WinHttpQueryHeaders(hRequest, dwInfoLevel, WINHTTP_HEADER_NAME_BY_INDEX, sData.data(), &dwSize, WINHTTP_NO_HEADER_INDEX)) {
            dwSize /= sizeof(WCHAR);
            sData.resize(dwSize);
            return TRUE;
        }
        DWORD result = GetLastError();
        if (result == ERROR_NOT_ENOUGH_MEMORY) {
            dwSize /= sizeof(WCHAR);
            dwSize *= 2;
        }
        else
            return FALSE;
    }
}

/// @}

namespace winstd
{
    /// \addtogroup WinStdWinHTTP
    /// @{

    ///
    /// HTTP handle wrapper class
    ///
    /// \sa [WinHttpOpen function](https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpopen)
    ///
    class http : public handle<HINTERNET, NULL>
    {
        WINSTD_HANDLE_IMPL(http, HINTERNET, NULL)

    public:
        ///
        /// Closes a handle to the HTTP.
        ///
        /// \sa [WinHttpCloseHandle function](https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpclosehandle)
        ///
        virtual ~http()
        {
            if (m_h != invalid)
                free_internal();
        }

    protected:
        ///
        /// Closes a handle to the HTTP.
        ///
        /// \sa [WinHttpCloseHandle function](https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpclosehandle)
        ///
        void free_internal() noexcept override
        {
            WinHttpCloseHandle(m_h);
        }
    };

    /// @}
}
