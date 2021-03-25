/*
    Copyright © 1991-2021 Amebis
    Copyright © 2016 GÉANT

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
inline BOOL CredEnumerate(_In_z_ LPCTSTR Filter, _Reserved_ DWORD Flags, _Out_ DWORD *Count, _Inout_ std::unique_ptr<PCREDENTIAL[], winstd::CredFree_delete<PCREDENTIAL[]> > &cCredentials) noexcept;

/// @copydoc CredProtectW()
template<class _Elem, class _Traits, class _Ax> inline BOOL CredProtectA(_In_ BOOL fAsSelf, _In_count_(cchCredentials) LPCSTR pszCredentials, _In_ DWORD cchCredentials, _Inout_ std::basic_string<_Elem, _Traits, _Ax> &sProtectedCredentials, _Out_ CRED_PROTECTION_TYPE *ProtectionType);

///
/// Encrypts the specified credentials so that only the current security context can decrypt them.
///
/// \sa [CredProtect function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa374803.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline BOOL CredProtectW(_In_ BOOL fAsSelf, _In_count_(cchCredentials) LPCWSTR pszCredentials, _In_ DWORD cchCredentials, _Inout_ std::basic_string<_Elem, _Traits, _Ax> &sProtectedCredentials, _Out_ CRED_PROTECTION_TYPE *ProtectionType);

/// @copydoc CredUnprotectW()
template<class _Elem, class _Traits, class _Ax> inline BOOL CredUnprotectA(_In_ BOOL fAsSelf, _In_count_(cchCredentials) LPCSTR pszProtectedCredentials, _In_ DWORD cchCredentials, _Inout_ std::basic_string<_Elem, _Traits, _Ax> &sCredentials);

///
/// Decrypts credentials that were previously encrypted by using the CredProtect function.
///
/// \sa [CredUnprotect function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa375186.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline BOOL CredUnprotectW(_In_ BOOL fAsSelf, _In_count_(cchCredentials) LPCWSTR pszProtectedCredentials, _In_ DWORD cchCredentials, _Inout_ std::basic_string<_Elem, _Traits, _Ax> &sCredentials);

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


inline BOOL CredEnumerate(_In_z_ LPCTSTR Filter, _Reserved_ DWORD Flags, _Out_ DWORD *Count, _Inout_ std::unique_ptr<PCREDENTIAL[], winstd::CredFree_delete<PCREDENTIAL[]> > &cCredentials) noexcept
{
    PCREDENTIAL *pCredentials;
    if (CredEnumerate(Filter, Flags, Count, &pCredentials)) {
        cCredentials.reset(pCredentials);
        return TRUE;
    }

    return FALSE;
}


template<class _Elem, class _Traits, class _Ax>
inline BOOL CredProtectA(_In_ BOOL fAsSelf, _In_count_(cchCredentials) LPCSTR pszCredentials, _In_ DWORD cchCredentials, _Inout_ std::basic_string<_Elem, _Traits, _Ax> &sProtectedCredentials, _Out_ CRED_PROTECTION_TYPE *ProtectionType)
{
    _Elem buf[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];
    DWORD dwSize = _countof(buf);

    // Try with the stack buffer first.
    if (CredProtectA(fAsSelf, const_cast<LPSTR>(pszCredentials), cchCredentials, buf, &dwSize, ProtectionType)) {
        // Copy from stack.
        sProtectedCredentials.assign(buf, dwSize - 1);
        return TRUE;
    } else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Allocate on heap and retry.
        std::unique_ptr<_Elem[]> buf(new _Elem[dwSize]);
        if (CredProtectA(fAsSelf, const_cast<LPSTR>(pszCredentials), cchCredentials, buf.get(), &dwSize, ProtectionType)) {
            sProtectedCredentials.assign(buf.get(), dwSize - 1);
            return TRUE;
        }
    }

    return FALSE;
}


template<class _Elem, class _Traits, class _Ax>
inline BOOL CredProtectW(_In_ BOOL fAsSelf, _In_count_(cchCredentials) LPCWSTR pszCredentials, _In_ DWORD cchCredentials, _Inout_ std::basic_string<_Elem, _Traits, _Ax> &sProtectedCredentials, _Out_ CRED_PROTECTION_TYPE *ProtectionType)
{
    _Elem buf[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];
    DWORD dwSize = _countof(buf);

    // Try with the stack buffer first.
    if (CredProtectW(fAsSelf, const_cast<LPWSTR>(pszCredentials), cchCredentials, buf, &dwSize, ProtectionType)) {
        // Copy from stack.
        sProtectedCredentials.assign(buf, dwSize - 1);
        return TRUE;
    } else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Allocate on heap and retry.
        std::unique_ptr<_Elem[]> buf(new _Elem[dwSize]);
        if (CredProtectW(fAsSelf, const_cast<LPWSTR>(pszCredentials), cchCredentials, buf.get(), &dwSize, ProtectionType)) {
            sProtectedCredentials.assign(buf.get(), dwSize - 1);
            return TRUE;
        }
    }

    return FALSE;
}


template<class _Elem, class _Traits, class _Ax>
inline BOOL CredUnprotectA(_In_ BOOL fAsSelf, _In_count_(cchCredentials) LPCSTR pszProtectedCredentials, _In_ DWORD cchCredentials, _Inout_ std::basic_string<_Elem, _Traits, _Ax> &sCredentials)
{
    _Elem buf[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];
    DWORD dwSize = _countof(buf);

    // Try with the stack buffer first.
    if (CredUnprotectA(fAsSelf, const_cast<LPSTR>(pszProtectedCredentials), cchCredentials, buf, &dwSize)) {
        // Copy from stack.
        sCredentials.assign(buf, dwSize);
        return TRUE;
    } else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Allocate on heap and retry.
        std::unique_ptr<_Elem[]> buf(new _Elem[dwSize]);
        if (CredUnprotectA(fAsSelf, const_cast<LPSTR>(pszProtectedCredentials), cchCredentials, buf.get(), &dwSize)) {
            sCredentials.assign(buf.get(), dwSize);
            return TRUE;
        }
    }

    return FALSE;
}


template<class _Elem, class _Traits, class _Ax>
inline BOOL CredUnprotectW(_In_ BOOL fAsSelf, _In_count_(cchCredentials) LPCWSTR pszProtectedCredentials, _In_ DWORD cchCredentials, _Inout_ std::basic_string<_Elem, _Traits, _Ax> &sCredentials)
{
    _Elem buf[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];
    DWORD dwSize = _countof(buf);

    // Try with the stack buffer first.
    if (CredUnprotectW(fAsSelf, const_cast<LPWSTR>(pszProtectedCredentials), cchCredentials, buf, &dwSize)) {
        // Copy from stack.
        sCredentials.assign(buf, dwSize);
        return TRUE;
    } else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Allocate on heap and retry.
        std::unique_ptr<_Elem[]> buf(new _Elem[dwSize]);
        if (CredUnprotectW(fAsSelf, const_cast<LPWSTR>(pszProtectedCredentials), cchCredentials, buf.get(), &dwSize)) {
            sCredentials.assign(buf.get(), dwSize);
            return TRUE;
        }
    }

    return FALSE;
}
