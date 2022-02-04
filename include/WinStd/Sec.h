/*
    SPDX-License-Identifier: MIT
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
*/

///
/// \defgroup WinStdSecurityAPI Security API
/// Integrates WinStd classes with Microsoft Security API
///

#include "Common.h"

#include <Security.h>

#include <string>

namespace winstd
{
    class sec_credentials;
    class sec_context;
    class sec_buffer_desc;
    class sec_runtime_error;
}

#if defined(SECURITY_WIN32) || defined(SECURITY_KERNEL)

/// \addtogroup WinStdSecurityAPI
/// @{

/// @copydoc GetUserNameExW()
template<class _Traits, class _Ax>
static BOOLEAN GetUserNameExA(_In_ EXTENDED_NAME_FORMAT NameFormat, _Inout_ std::basic_string<char, _Traits, _Ax> &sName);

///
/// Retrieves the name of the user or other security principal associated with the calling thread and stores it in a std::wstring string.
///
/// \sa [GetUserNameEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724435.aspx)
///
template<class _Traits, class _Ax>
static BOOLEAN GetUserNameExW(_In_ EXTENDED_NAME_FORMAT NameFormat, _Inout_ std::basic_string<wchar_t, _Traits, _Ax> &sName);

#endif

/// @}

#pragma once


namespace winstd
{
    /// \addtogroup WinStdSecurityAPI
    /// @{

    ///
    /// PCredHandle wrapper class
    ///
    class sec_credentials : public handle<PCredHandle, NULL>
    {
        WINSTD_NONCOPYABLE(sec_credentials)

    public:
        ///
        /// Initializes a new class instance with the object handle set to NULL.
        ///
        sec_credentials()
        {
            m_expires.QuadPart = -1;
        }

        ///
        /// Initializes a new class with an already available object handle.
        ///
        /// \param[in] h        Initial class handle value
        /// \param[in] expires  Credentials expiration
        ///
        sec_credentials(_In_opt_ handle_type h, _In_ const TimeStamp expires) :
            m_expires(expires),
            handle(h)
        {
        }

        ///
        /// Move constructor
        ///
        /// \param[inout] h  A rvalue reference of another object
        ///
        sec_credentials(_Inout_ sec_credentials &&h) noexcept :
            m_expires(std::move(h.m_expires)),
            handle<PCredHandle, NULL>(std::move(h))
        {
        }

        ///
        /// Frees the security credentials.
        ///
        /// \sa [FreeCredentialsHandle function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa375417.aspx)
        ///
        virtual ~sec_credentials()
        {
            if (m_h != invalid)
                free_internal();
        }

        ///
        /// Move assignment
        ///
        /// \param[inout] h  A rvalue reference of another object
        ///
        sec_credentials& operator=(_Inout_ sec_credentials &&h) noexcept
        {
            if (this != std::addressof(h)) {
                *(handle<handle_type, NULL>*)this = std::move(h);
                m_expires = std::move(h.m_expires);
            }
            return *this;
        }

        ///
        /// Acquires the security credentials.
        ///
        /// \return
        /// - \c SEC_E_OK when succeeds;
        /// - Error code when fails.
        ///
        /// \sa [AcquireCredentialsHandle (General) function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa374712.aspx)
        ///
        SECURITY_STATUS acquire(
            _In_opt_ LPTSTR         pszPrincipal,
            _In_     LPTSTR         pszPackage,
            _In_     unsigned long  fCredentialUse,
            _In_opt_ void           *pvLogonId,
            _In_opt_ void           *pAuthData,
            _In_opt_ SEC_GET_KEY_FN pGetKeyFn = NULL,
            _In_opt_ void           *pvGetKeyArgument = NULL)
        {
            handle_type h = new CredHandle;
            TimeStamp exp;
            SECURITY_STATUS res = AcquireCredentialsHandle(pszPrincipal, pszPackage, fCredentialUse, pvLogonId, pAuthData, pGetKeyFn, pvGetKeyArgument, h, &exp);
            if (SUCCEEDED(res)) {
                attach(h);
                m_expires = exp;
            } else
                delete h;
            return res;
        }

    protected:
        ///
        /// Frees the security credentials.
        ///
        /// \sa [FreeCredentialsHandle function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa375417.aspx)
        ///
        void free_internal() noexcept override
        {
            FreeCredentialsHandle(m_h);
            delete m_h;
        }

    public:
        TimeStamp m_expires;    ///< Credentials expiration time
    };


    ///
    /// PCtxtHandle wrapper class
    ///
    class sec_context : public handle<PCtxtHandle, NULL>
    {
    public:
        ///
        /// Initializes a new class instance with the object handle set to NULL.
        ///
        sec_context() :
            m_attrib(0),
            handle<PCtxtHandle, NULL>()
        {
            m_expires.QuadPart = -1;
        }

        ///
        /// Move constructor
        ///
        /// \param[inout] h  A rvalue reference of another object
        ///
        sec_context(_Inout_ sec_context &&h) noexcept :
            m_attrib (std::move(h.m_attrib )),
            m_expires(std::move(h.m_expires)),
            handle<PCtxtHandle, NULL>(std::move(h))
        {
        }

        ///
        /// Frees the security context.
        ///
        /// \sa [DeleteSecurityContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa375354.aspx)
        ///
        virtual ~sec_context()
        {
            if (m_h != invalid)
                free_internal();
        }

        ///
        /// Move assignment
        ///
        /// \param[inout] h  A rvalue reference of another object
        ///
        sec_context& operator=(_Inout_ sec_context &&h) noexcept
        {
            if (this != std::addressof(h)) {
                *(handle<handle_type, NULL>*)this = std::move(h);
                m_attrib  = std::move(h.m_attrib);
                m_expires = std::move(h.m_expires);
            }
            return *this;
        }

        ///
        /// Initializes security context.
        ///
        /// \return
        /// - \c SEC_E_OK when succeeds;
        /// - Error code when fails.
        ///
        /// \sa [InitializeSecurityContext (General) function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa375506.aspx)
        ///
        SECURITY_STATUS initialize(
            _In_opt_    PCredHandle    phCredential,
            _In_opt_z_  LPCTSTR        pszTargetName,
            _In_        ULONG          fContextReq,
            _In_        ULONG          TargetDataRep,
            _In_opt_    PSecBufferDesc pInput,
            _Inout_opt_ PSecBufferDesc pOutput)
        {
            handle_type h = new CtxtHandle;
            h->dwUpper = 0;
            h->dwLower = 0;
            ULONG attr;
            TimeStamp exp;
            SECURITY_STATUS res = InitializeSecurityContext(phCredential, NULL, const_cast<LPTSTR>(pszTargetName), fContextReq, 0, TargetDataRep, pInput, 0, h, pOutput, &attr, &exp);
            if (SUCCEEDED(res)) {
                attach(h);
                m_attrib  = attr;
                m_expires = exp;
            } else
                delete h;
            return res;
        }

        ///
        /// Continue security context.
        ///
        /// \return
        /// - \c SEC_E_OK when succeeds;
        /// - Error code when fails.
        ///
        /// \sa [InitializeSecurityContext (General) function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa375506.aspx)
        ///
        SECURITY_STATUS process(
            _In_opt_    PCredHandle    phCredential,
            _In_opt_z_  LPCTSTR        pszTargetName,
            _In_        ULONG          fContextReq,
            _In_        ULONG          TargetDataRep,
            _In_opt_    PSecBufferDesc pInput,
            _Inout_opt_ PSecBufferDesc pOutput)
        {
            return InitializeSecurityContext(phCredential, m_h, const_cast<LPTSTR>(pszTargetName), fContextReq, 0, TargetDataRep, pInput, 0, NULL, pOutput, &m_attrib, &m_expires);
        }

    protected:
        ///
        /// Frees the security context.
        ///
        /// \sa [DeleteSecurityContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa375354.aspx)
        ///
        void free_internal() noexcept override
        {
            DeleteSecurityContext(m_h);
            delete m_h;
        }

    public:
        ULONG     m_attrib;     ///< Context attributes
        TimeStamp m_expires;    ///< Context expiration time
    };


    ///
    /// SecBufferDesc wrapper class
    ///
    class sec_buffer_desc : public SecBufferDesc
    {
    public:
        ///
        /// Initializes security buffer descriptor.
        ///
        sec_buffer_desc(_Inout_count_(count) PSecBuffer buf, ULONG count, _In_ ULONG version = SECBUFFER_VERSION)
        {
            ulVersion = version;
            cBuffers  = count;
            pBuffers  = buf;
        }

        ///
        /// Frees the security buffer descriptor.
        ///
        /// \sa [FreeContextBuffer function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa375416.aspx)
        ///
        virtual ~sec_buffer_desc()
        {
            for (ULONG i = 0; i < cBuffers; i++) {
                if (pBuffers[i].pvBuffer)
                    FreeContextBuffer(pBuffers[i].pvBuffer);
            }
        }
    };

    /// @}

    ///
    /// \defgroup WinStdExceptions Exceptions
    /// Additional exceptions
    ///
    /// @{

    ///
    /// Security runtime error
    ///
    /// \note Must be defined as derived class from num_runtime_error<> to allow correct type info for dynamic typecasting and prevent folding with other derivates of num_runtime_error<>.
    ///
    class sec_runtime_error : public num_runtime_error<SECURITY_STATUS>
    {
    public:
        ///
        /// Constructs an exception
        ///
        /// \param[in] num  Security provider error code
        /// \param[in] msg  Error message
        ///
        sec_runtime_error(_In_ error_type num, _In_ const std::string& msg) : num_runtime_error<SECURITY_STATUS>(num, msg)
        {
        }


        ///
        /// Constructs an exception
        ///
        /// \param[in] num  Security provider error code
        /// \param[in] msg  Error message
        ///
        sec_runtime_error(_In_ error_type num, _In_opt_z_ const char *msg = nullptr) : num_runtime_error<SECURITY_STATUS>(num, msg)
        {
        }


        ///
        /// Copies an exception
        ///
        /// \param[in] other  Exception to copy from
        ///
        sec_runtime_error(const sec_runtime_error &other) : num_runtime_error<SECURITY_STATUS>(other)
        {
        }
    };

    /// @}
}


#if defined(SECURITY_WIN32) || defined(SECURITY_KERNEL)

template<class _Traits, class _Ax>
static BOOLEAN GetUserNameExA(_In_ EXTENDED_NAME_FORMAT NameFormat, _Inout_ std::basic_string<char, _Traits, _Ax> &sName)
{
    assert(0); // TODO: Test this code.

    char szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(char)];
    ULONG ulSize = _countof(szStackBuffer);

    // Try with stack buffer first.
    if (::GetUserNameExA(NameFormat, szStackBuffer, &ulSize)) {
        // Copy from stack.
        sName.assign(szStackBuffer, ulSize);
        return TRUE;
    } else {
        if (::GetLastError() == ERROR_MORE_DATA) {
            // Allocate buffer on heap and retry.
            std::unique_ptr<char[]> szBuffer(new char[ulSize]);
            if (::GetUserNameExA(NameFormat, szBuffer.get(), &ulSize)) {
                sName.assign(szBuffer.get(), ulSize);
                return TRUE;
            }
        }
    }

    return FALSE;
}


template<class _Traits, class _Ax>
static BOOLEAN GetUserNameExW(_In_ EXTENDED_NAME_FORMAT NameFormat, _Inout_ std::basic_string<wchar_t, _Traits, _Ax> &sName)
{
    assert(0); // TODO: Test this code.

    wchar_t szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(wchar_t)];
    ULONG ulSize = _countof(szStackBuffer);

    // Try with stack buffer first.
    if (::GetUserNameExW(NameFormat, szStackBuffer, &ulSize)) {
        // Copy from stack.
        sName.assign(szStackBuffer, ulSize);
        return TRUE;
    } else {
        if (::GetLastError() == ERROR_MORE_DATA) {
            // Allocate buffer on heap and retry.
            std::unique_ptr<wchar_t[]> szBuffer(new wchar_t[ulSize]);
            if (::GetUserNameExW(NameFormat, szBuffer.get(), &ulSize)) {
                sName.assign(szBuffer.get(), ulSize);
                return TRUE;
            }
        }
    }

    return FALSE;
}

#endif
