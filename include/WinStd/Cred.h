/*
    SPDX-License-Identifier: MIT
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
*/

///
/// \defgroup WinStdCryptoAPI Cryptography API
/// Integrates WinStd classes with Microsoft Cryptography API
///

#include "Common.h"

#include <wincred.h>

#include <memory>

namespace winstd
{
    template <class _Ty> struct CredFree_delete;
    template <class _Ty> struct CredFree_delete<_Ty[]>;
}


/// \addtogroup WinStdCryptoAPI
/// @{

///
/// Enumerates the credentials from the user's credential set. The credential set used is the one associated with the logon session of the current token. The token must not have the user's SID disabled.
///
/// \sa [CredEnumerate function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa374794.aspx)
///
static BOOL CredEnumerate(_In_z_ LPCTSTR Filter, _Reserved_ DWORD Flags, _Out_ DWORD *Count, _Inout_ std::unique_ptr<PCREDENTIAL[], winstd::CredFree_delete<PCREDENTIAL[]> > &cCredentials) noexcept;

/// @copydoc CredProtectW()
template<class _Traits, class _Ax>
static BOOL CredProtectA(_In_ BOOL fAsSelf, _In_count_(cchCredentials) LPCSTR pszCredentials, _In_ DWORD cchCredentials, _Inout_ std::basic_string<char, _Traits, _Ax> &sProtectedCredentials, _Out_ CRED_PROTECTION_TYPE *ProtectionType);

///
/// Encrypts the specified credentials so that only the current security context can decrypt them.
///
/// \sa [CredProtect function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa374803.aspx)
///
template<class _Traits, class _Ax>
static BOOL CredProtectW(_In_ BOOL fAsSelf, _In_count_(cchCredentials) LPCWSTR pszCredentials, _In_ DWORD cchCredentials, _Inout_ std::basic_string<wchar_t, _Traits, _Ax> &sProtectedCredentials, _Out_ CRED_PROTECTION_TYPE *ProtectionType);

/// @copydoc CredUnprotectW()
template<class _Traits, class _Ax>
static BOOL CredUnprotectA(_In_ BOOL fAsSelf, _In_count_(cchCredentials) LPCSTR pszProtectedCredentials, _In_ DWORD cchCredentials, _Inout_ std::basic_string<char, _Traits, _Ax> &sCredentials);

///
/// Decrypts credentials that were previously encrypted by using the CredProtect function.
///
/// \sa [CredUnprotect function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa375186.aspx)
///
template<class _Traits, class _Ax>
static BOOL CredUnprotectW(_In_ BOOL fAsSelf, _In_count_(cchCredentials) LPCWSTR pszProtectedCredentials, _In_ DWORD cchCredentials, _Inout_ std::basic_string<wchar_t, _Traits, _Ax> &sCredentials);

/// @}

#pragma once


namespace winstd
{
    /// \addtogroup WinStdCryptoAPI
    /// @{

    ///
    /// Deleter for unique_ptr using CredFree
    ///
    template <class _Ty> struct CredFree_delete
    {
        typedef CredFree_delete<_Ty> _Myt; ///< This type

        ///
        /// Default construct
        ///
        CredFree_delete() {}

        ///
        /// Construct from another CredFree_delete
        ///
        template <class _Ty2> CredFree_delete(const CredFree_delete<_Ty2>&) {}

        ///
        /// Delete a pointer
        ///
        /// \sa [CredFree function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa374796.aspx)
        ///
        void operator()(_Ty *_Ptr) const
        {
            CredFree(_Ptr);
        }
    };


    ///
    /// Deleter for unique_ptr to array of unknown size using CredFree
    ///
    template <class _Ty> struct CredFree_delete<_Ty[]>
    {
        typedef CredFree_delete<_Ty> _Myt; ///< This type

        ///
        /// Default construct
        ///
        CredFree_delete() {}

        ///
        /// Delete a pointer
        ///
        /// \sa [CredFree function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa374796.aspx)
        ///
        void operator()(_Ty *_Ptr) const noexcept
        {
            CredFree(_Ptr);
        }

        ///
        /// Delete a pointer of another type
        ///
        /// \sa [CredFree function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa374796.aspx)
        ///
        template<class _Other>
        void operator()(_Other *) const
        {
            CredFree(_Ptr);
        }
    };

    /// @}
}


#pragma warning(push)
#pragma warning(disable: 4505) // Don't warn on unused code


static BOOL CredEnumerate(_In_z_ LPCTSTR Filter, _Reserved_ DWORD Flags, _Out_ DWORD *Count, _Inout_ std::unique_ptr<PCREDENTIAL[], winstd::CredFree_delete<PCREDENTIAL[]> > &cCredentials) noexcept
{
    PCREDENTIAL *pCredentials;
    if (CredEnumerate(Filter, Flags, Count, &pCredentials)) {
        cCredentials.reset(pCredentials);
        return TRUE;
    }

    return FALSE;
}


template<class _Traits, class _Ax>
static BOOL CredProtectA(_In_ BOOL fAsSelf, _In_count_(cchCredentials) LPCSTR pszCredentials, _In_ DWORD cchCredentials, _Inout_ std::basic_string<char, _Traits, _Ax> &sProtectedCredentials, _Out_ CRED_PROTECTION_TYPE *ProtectionType)
{
    char buf[WINSTD_STACK_BUFFER_BYTES/sizeof(char)];
    DWORD dwSize = _countof(buf);

    // Try with the stack buffer first.
    if (CredProtectA(fAsSelf, const_cast<LPSTR>(pszCredentials), cchCredentials, buf, &dwSize, ProtectionType)) {
        // Copy from stack.
        sProtectedCredentials.assign(buf, dwSize - 1);
        return TRUE;
    } else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Allocate on heap and retry.
        std::unique_ptr<char[]> buf(new char[dwSize]);
        if (CredProtectA(fAsSelf, const_cast<LPSTR>(pszCredentials), cchCredentials, buf.get(), &dwSize, ProtectionType)) {
            sProtectedCredentials.assign(buf.get(), dwSize - 1);
            return TRUE;
        }
    }

    return FALSE;
}


template<class _Traits, class _Ax>
static BOOL CredProtectW(_In_ BOOL fAsSelf, _In_count_(cchCredentials) LPCWSTR pszCredentials, _In_ DWORD cchCredentials, _Inout_ std::basic_string<wchar_t, _Traits, _Ax> &sProtectedCredentials, _Out_ CRED_PROTECTION_TYPE *ProtectionType)
{
    wchar_t buf[WINSTD_STACK_BUFFER_BYTES/sizeof(wchar_t)];
    DWORD dwSize = _countof(buf);

    // Try with the stack buffer first.
    if (CredProtectW(fAsSelf, const_cast<LPWSTR>(pszCredentials), cchCredentials, buf, &dwSize, ProtectionType)) {
        // Copy from stack.
        sProtectedCredentials.assign(buf, dwSize - 1);
        return TRUE;
    } else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Allocate on heap and retry.
        std::unique_ptr<wchar_t[]> buf(new wchar_t[dwSize]);
        if (CredProtectW(fAsSelf, const_cast<LPWSTR>(pszCredentials), cchCredentials, buf.get(), &dwSize, ProtectionType)) {
            sProtectedCredentials.assign(buf.get(), dwSize - 1);
            return TRUE;
        }
    }

    return FALSE;
}


template<class _Traits, class _Ax>
static BOOL CredUnprotectA(_In_ BOOL fAsSelf, _In_count_(cchCredentials) LPCSTR pszProtectedCredentials, _In_ DWORD cchCredentials, _Inout_ std::basic_string<char, _Traits, _Ax> &sCredentials)
{
    char buf[WINSTD_STACK_BUFFER_BYTES/sizeof(char)];
    DWORD dwSize = _countof(buf);

    // Try with the stack buffer first.
    if (CredUnprotectA(fAsSelf, const_cast<LPSTR>(pszProtectedCredentials), cchCredentials, buf, &dwSize)) {
        // Copy from stack.
        sCredentials.assign(buf, dwSize);
        return TRUE;
    } else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Allocate on heap and retry.
        std::unique_ptr<char[]> buf(new char[dwSize]);
        if (CredUnprotectA(fAsSelf, const_cast<LPSTR>(pszProtectedCredentials), cchCredentials, buf.get(), &dwSize)) {
            sCredentials.assign(buf.get(), dwSize);
            return TRUE;
        }
    }

    return FALSE;
}


template<class _Traits, class _Ax>
static BOOL CredUnprotectW(_In_ BOOL fAsSelf, _In_count_(cchCredentials) LPCWSTR pszProtectedCredentials, _In_ DWORD cchCredentials, _Inout_ std::basic_string<wchar_t, _Traits, _Ax> &sCredentials)
{
    wchar_t buf[WINSTD_STACK_BUFFER_BYTES/sizeof(wchar_t)];
    DWORD dwSize = _countof(buf);

    // Try with the stack buffer first.
    if (CredUnprotectW(fAsSelf, const_cast<LPWSTR>(pszProtectedCredentials), cchCredentials, buf, &dwSize)) {
        // Copy from stack.
        sCredentials.assign(buf, dwSize);
        return TRUE;
    } else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Allocate on heap and retry.
        std::unique_ptr<wchar_t[]> buf(new wchar_t[dwSize]);
        if (CredUnprotectW(fAsSelf, const_cast<LPWSTR>(pszProtectedCredentials), cchCredentials, buf.get(), &dwSize)) {
            sCredentials.assign(buf.get(), dwSize);
            return TRUE;
        }
    }

    return FALSE;
}

#pragma warning(pop)
