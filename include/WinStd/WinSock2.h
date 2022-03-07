/*
    SPDX-License-Identifier: MIT
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
*/

/// \file
/// Integrates WinStd classes with Microsoft WinSock2 API
///
/// \defgroup WinSock2API WinSock2 API
/// Integrates WinStd classes with Microsoft WinSock2 API

#pragma once

#include "Common.h"
#include <WinSock2.h>
#include <ws2def.h>
#include <WS2tcpip.h>

namespace winstd
{
    /// \addtogroup WinSock2API
    /// @{

    ///
    /// WinSock2 runtime error
    ///
    class ws2_runtime_error : public num_runtime_error<int>
    {
    public:
        ///
        /// Constructs an exception
        ///
        /// \param[in] num  WinSock2 error code
        /// \param[in] msg  Error message
        ///
        ws2_runtime_error(_In_ error_type num, _In_ const std::string& msg) : num_runtime_error<int>(num, msg)
        {
        }

        ///
        /// Constructs an exception
        ///
        /// \param[in] num  WinSock2 error code
        /// \param[in] msg  Error message
        ///
        ws2_runtime_error(_In_ error_type num, _In_opt_z_ const char *msg = nullptr) : num_runtime_error<int>(num, msg)
        {
        }

        ///
        /// Constructs an exception using `WSAGetLastError()`
        ///
        /// \param[in] msg  Error message
        ///
        ws2_runtime_error(_In_ const std::string& msg) : num_runtime_error<int>(WSAGetLastError(), msg)
        {
        }

        ///
        /// Constructs an exception using `WSAGetLastError()`
        ///
        /// \param[in] msg  Error message
        ///
        ws2_runtime_error(_In_opt_z_ const char *msg = nullptr) : num_runtime_error<int>(WSAGetLastError(), msg)
        {
        }

        ///
        /// Returns a user-readable Windows error message
        ///
        /// \sa [FormatMessage function](https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-formatmessage)
        ///
        tstring msg(_In_opt_ DWORD dwLanguageId = 0) const
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
    /// \sa [GetAddrInfoA function](https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo)
    ///
    class addrinfo : public handle<PADDRINFOA, NULL>
    {
        WINSTD_HANDLE_IMPL(addrinfo, NULL)

    public:
        ///
        /// Frees address information
        ///
        /// \sa [FreeAddrInfoA function](https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-freeaddrinfo)
        ///
        virtual ~addrinfo()
        {
            if (m_h != invalid)
                free_internal();
        }

    protected:
        ///
        /// Frees address information
        ///
        /// \sa [FreeAddrInfoA function](https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-freeaddrinfo)
        ///
        void free_internal() noexcept override
        {
            FreeAddrInfoA(m_h);
        }
    };

    ///
    /// SID wrapper class
    ///
    /// \sa [GetAddrInfoW function](https://docs.microsoft.com/en-us/windows/desktop/api/ws2tcpip/nf-ws2tcpip-getaddrinfow)
    ///
    class waddrinfo : public handle<PADDRINFOW, NULL>
    {
        WINSTD_HANDLE_IMPL(waddrinfo, NULL)

    public:
        ///
        /// Frees address information
        ///
        /// \sa [FreeAddrInfoW function](https://docs.microsoft.com/en-us/windows/desktop/api/ws2tcpip/nf-ws2tcpip-freeaddrinfow)
        ///
        virtual ~waddrinfo()
        {
            if (m_h != invalid)
                free_internal();
        }

    protected:
        ///
        /// Frees address information
        ///
        /// \sa [FreeAddrInfoW function](https://docs.microsoft.com/en-us/windows/desktop/api/ws2tcpip/nf-ws2tcpip-freeaddrinfow)
        ///
        void free_internal() noexcept override
        {
            FreeAddrInfoW(m_h);
        }
    };

    ///
    /// Multi-byte / Wide-character SID wrapper class (according to _UNICODE)
    ///
#ifdef _UNICODE
    typedef waddrinfo taddrinfo;
#else
    typedef addrinfo taddrinfo;
#endif

#endif

    /// @}
}

/// \addtogroup WinSock2API
/// @{

#pragma warning(push)
#pragma warning(disable: 4505) // Don't warn on unused code

/// @copydoc GetAddrInfoW()
static INT GetAddrInfoA(
    _In_opt_ PCSTR pNodeName,
    _In_opt_ PCSTR pServiceName,
    _In_opt_ const ADDRINFOA *pHints,
    _Inout_ winstd::addrinfo &result)
{
    PADDRINFOA h;
    INT iResult = GetAddrInfoA(pNodeName, pServiceName, pHints, &h);
    if (iResult == 0)
        result.attach(h);
    return iResult;
}

///
/// Provides protocol-independent translation from a host name to an address.
///
/// \sa [GetAddrInfoW function](https://docs.microsoft.com/en-us/windows/desktop/api/ws2tcpip/nf-ws2tcpip-getaddrinfow)
///
static INT GetAddrInfoW(
    _In_opt_ PCWSTR pNodeName,
    _In_opt_ PCWSTR pServiceName,
    _In_opt_ const ADDRINFOW *pHints,
    _Inout_ winstd::waddrinfo &result)
{
    PADDRINFOW h;
    INT iResult = GetAddrInfoW(pNodeName, pServiceName, pHints, &h);
    if (iResult == 0)
        result.attach(h);
    return iResult;
}

#pragma warning(pop)

/// @}
