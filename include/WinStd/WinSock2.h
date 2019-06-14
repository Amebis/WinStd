/*
    Copyright 1991-2019 Amebis
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
/// \defgroup WinSock2API WinSock2 API
/// Integrates WinStd classes with Microsoft WinSock2 API
///

#include "Common.h"

namespace winstd
{
    class WINSTD_API ws2_runtime_error;
#if (NTDDI_VERSION >= NTDDI_WINXPSP2) || (_WIN32_WINNT >= 0x0502)
    class WINSTD_API addrinfo;
#endif
}

#pragma once

#include <WinSock2.h>
#include <ws2def.h>
#include <WS2tcpip.h>


namespace winstd
{
    /// \addtogroup WinStdExceptions
    /// @{

    ///
    /// WinSock2 runtime error
    ///
    class WINSTD_API ws2_runtime_error : public num_runtime_error<int>
    {
    public:
        ///
        /// Constructs an exception
        ///
        /// \param[in] num  WinSock2 error code
        /// \param[in] msg  Error message
        ///
        inline ws2_runtime_error(_In_ error_type num, _In_ const std::string& msg) : num_runtime_error<int>(num, msg)
        {
        }


        ///
        /// Constructs an exception
        ///
        /// \param[in] num  WinSock2 error code
        /// \param[in] msg  Error message
        ///
        inline ws2_runtime_error(_In_ error_type num, _In_opt_z_ const char *msg = nullptr) : num_runtime_error<int>(num, msg)
        {
        }


        ///
        /// Constructs an exception using `WSAGetLastError()`
        ///
        /// \param[in] msg  Error message
        ///
        inline ws2_runtime_error(_In_ const std::string& msg) : num_runtime_error<int>(WSAGetLastError(), msg)
        {
        }


        ///
        /// Constructs an exception using `WSAGetLastError()`
        ///
        /// \param[in] msg  Error message
        ///
        inline ws2_runtime_error(_In_opt_z_ const char *msg = nullptr) : num_runtime_error<int>(WSAGetLastError(), msg)
        {
        }


        ///
        /// Copies an exception
        ///
        /// \param[in] other  Exception to copy from
        ///
        inline ws2_runtime_error(const ws2_runtime_error &other) : num_runtime_error<int>(other)
        {
        }


        ///
        /// Returns a user-readable Windows error message
        ///
        /// \sa [FormatMessage function](https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-formatmessage)
        ///
        inline tstring msg(_In_opt_ DWORD dwLanguageId = 0) const
        {
            tstring str;
            if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0, m_num, dwLanguageId, str, NULL)) {
                // Stock Windows error messages contain CRLF. Well... Trim all the trailing white space.
                str.erase(str.find_last_not_of(_T(" \t\n\r\f\v")) + 1);
            } else
                sprintf(str, m_num >= 0x10000 ? _T("Error 0x%X") : _T("Error %u"), m_num);
            return str;
        }
    };

    /// @}


    /// \addtogroup WinSock2API
    /// @{

#if (NTDDI_VERSION >= NTDDI_WINXPSP2) || (_WIN32_WINNT >= 0x0502)

    ///
    /// SID wrapper class
    ///
    class WINSTD_API addrinfo : public handle<PADDRINFOT, NULL>
    {
        HANDLE_IMPL(addrinfo, NULL)

    public:
        ///
        /// Provides protocol-independent translation from a host name to an address.
        ///
        /// \sa [GetAddrInfoW function](https://docs.microsoft.com/en-us/windows/desktop/api/ws2tcpip/nf-ws2tcpip-getaddrinfow)
        ///
        inline bool get(
            _In_opt_ PCTSTR          pNodeName,
            _In_opt_ PCTSTR          pServiceName,
            _In_opt_ const ADDRINFOT *pHints)
        {
            handle_type h;
            if (GetAddrInfo(pNodeName, pServiceName, pHints, &h) == 0) {
                attach(h);
                return true;
            } else
                return false;
        }

        ///
        /// Frees address information
        ///
        /// \sa [FreeAddrInfoW function](https://docs.microsoft.com/en-us/windows/desktop/api/ws2tcpip/nf-ws2tcpip-freeaddrinfow)
        ///
        virtual ~addrinfo();

    protected:
        ///
        /// Frees address information
        ///
        /// \sa [FreeAddrInfoW function](https://docs.microsoft.com/en-us/windows/desktop/api/ws2tcpip/nf-ws2tcpip-freeaddrinfow)
        ///
        virtual void free_internal();
    };

#endif

    /// @}
}
