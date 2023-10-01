/*
    SPDX-License-Identifier: MIT
    Copyright © 1991-2023 Amebis
    Copyright © 2016 GÉANT
*/

/// \defgroup WinStdWinHTTP Windows HTTP Client

#pragma once

#include "Common.h"
#include <winhttp.h>

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
        WINSTD_HANDLE_IMPL(http, NULL)

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