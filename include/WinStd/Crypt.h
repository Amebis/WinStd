﻿/*
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

#include <WinCrypt.h>

#include <string>
#include <vector>

namespace winstd
{
    class cert_context;
    class cert_chain_context;
    class cert_store;
    class crypt_prov;
    class crypt_hash;
    class crypt_key;
    class data_blob;
}

/// \addtogroup WinStdCryptoAPI
/// @{

/// @copydoc CertGetNameStringW()
template<class _Elem, class _Traits, class _Ax> inline DWORD CertGetNameStringA(_In_ PCCERT_CONTEXT pCertContext, _In_ DWORD dwType, _In_ DWORD dwFlags, _In_opt_ void *pvTypePara, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sNameString);

///
/// Obtains the subject or issuer name from a certificate [CERT_CONTEXT](https://msdn.microsoft.com/en-us/library/windows/desktop/aa377189.aspx) structure and stores it in a std::wstring string.
///
/// \sa [CertGetNameString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376086.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline DWORD CertGetNameStringW(_In_ PCCERT_CONTEXT pCertContext, _In_ DWORD dwType, _In_ DWORD dwFlags, _In_opt_ void *pvTypePara, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sNameString);

///
/// Retrieves the information contained in an extended property of a certificate context.
///
/// \sa [CertGetCertificateContextProperty function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376079.aspx)
///
template<class _Ty, class _Ax> inline _Success_(return != 0) BOOL WINAPI CertGetCertificateContextProperty(_In_ PCCERT_CONTEXT pCertContext, _In_ DWORD dwPropId, _Out_ std::vector<_Ty, _Ax> &aData);

///
/// Retrieves data that governs the operations of a hash object. The actual hash value can be retrieved by using this function.
///
/// \sa [CryptGetHashParam function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379947.aspx)
///
template<class _Ty, class _Ax> inline _Success_(return != 0) BOOL CryptGetHashParam(_In_ HCRYPTHASH hHash, _In_ DWORD dwParam, _Out_ std::vector<_Ty, _Ax> &aData, _In_ DWORD dwFlags);

///
/// Retrieves data that governs the operations of a key.
///
/// \sa [CryptGetKeyParam function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379949.aspx)
///
template<class _Ty, class _Ax> inline _Success_(return != 0) BOOL CryptGetKeyParam(_In_ HCRYPTKEY hKey, _In_ DWORD dwParam, _Out_ std::vector<_Ty, _Ax> &aData, _In_ DWORD dwFlags);

///
/// Retrieves data that governs the operations of a key.
///
/// \sa [CryptGetKeyParam function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379949.aspx)
///
template<class T> inline _Success_(return != 0) BOOL CryptGetKeyParam(_In_ HCRYPTKEY hKey, _In_ DWORD dwParam, _Out_ T &data, _In_ DWORD dwFlags);

///
/// Exports a cryptographic key or a key pair from a cryptographic service provider (CSP) in a secure manner.
///
/// \sa [CryptExportKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379931.aspx)
///
template<class _Ty, class _Ax> inline _Success_(return != 0) BOOL CryptExportKey(_In_ HCRYPTKEY hKey, _In_ HCRYPTKEY hExpKey, _In_ DWORD dwBlobType, _In_ DWORD dwFlags, _Out_ std::vector<_Ty, _Ax> &aData);

///
/// Encrypts data.
///
/// \sa [CryptEncrypt function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379924.aspx)
///
template<class _Ty, class _Ax> inline _Success_(return != 0) BOOL CryptEncrypt(_In_ HCRYPTKEY hKey, _In_opt_ HCRYPTHASH hHash, _In_ BOOL Final, _In_ DWORD dwFlags, _Inout_ std::vector<_Ty, _Ax> &aData);

///
/// Decrypts data previously encrypted by using the CryptEncrypt function.
///
/// \sa [CryptDecrypt function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379913.aspx)
///
template<class _Ty, class _Ax> inline _Success_(return != 0) BOOL CryptDecrypt(_In_ HCRYPTKEY hKey, _In_opt_ HCRYPTHASH hHash, _In_ BOOL Final, _In_ DWORD dwFlags, _Inout_ std::vector<_Ty, _Ax> &aData);

/// @}

#pragma once

#include <assert.h>

#include <algorithm>


namespace winstd
{
    /// \addtogroup WinStdCryptoAPI
    /// @{

    ///
    /// PCCERT_CONTEXT wrapper class
    ///
    class cert_context : public dplhandle<PCCERT_CONTEXT, NULL>
    {
        DPLHANDLE_IMPL(cert_context, NULL)

    public:
        ///
        /// Destroys the certificate context.
        ///
        /// \sa [CertFreeCertificateContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376075.aspx)
        ///
        virtual ~cert_context();

        ///
        /// Creates the certificate context.
        ///
        /// \return
        /// - true when creation succeeds;
        /// - false when creation fails. For extended error information, call `GetLastError()`.
        ///
        /// \sa [CertCreateCertificateContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376033.aspx)
        ///
        inline bool create(_In_  DWORD dwCertEncodingType, _In_  LPCBYTE pbCertEncoded, _In_  DWORD cbCertEncoded) noexcept
        {
            handle_type h = CertCreateCertificateContext(dwCertEncodingType, pbCertEncoded, cbCertEncoded);
            if (h != invalid) {
                attach(h);
                return true;
            } else
                return false;
        }

        ///
        /// Is certificate equal to?
        ///
        /// \param[in] other  Certificate to compare against
        /// \return
        /// - Non zero when certificate is equal to \p other;
        /// - Zero otherwise.
        ///
        inline bool operator==(_In_ const handle_type &other) const noexcept
        {
            // TODO: [Crypto] Make constant time.
            return
                m_h == other ||
                m_h->cbCertEncoded == other->cbCertEncoded && memcmp(m_h->pbCertEncoded, other->pbCertEncoded, m_h->cbCertEncoded) == 0;
        }

        ///
        /// Is certificate not equal to?
        ///
        /// \param[in] other  Certificate to compare against
        /// \return
        /// - Non zero when certificate is not equal to \p other;
        /// - Zero otherwise.
        ///
        inline bool operator!=(_In_ const handle_type &other) const noexcept
        {
            return !operator==(other);
        }

        ///
        /// Is certificate less than?
        ///
        /// \param[in] other  Certificate to compare against
        /// \return
        /// - Non zero when certificate is less than \p other;
        /// - Zero otherwise.
        ///
        inline bool operator<(_In_ const handle_type &other) const noexcept
        {
            // TODO: [Crypto] Make constant time.
            const int r = memcmp(m_h->pbCertEncoded, other->pbCertEncoded, std::min<DWORD>(m_h->cbCertEncoded, other->cbCertEncoded));
            return r < 0 || r == 0 && m_h->cbCertEncoded < other->cbCertEncoded;
        }

        ///
        /// Is certificate greater than?
        ///
        /// \param[in] other  Certificate to compare against
        /// \return
        /// - Non zero when certificate is greater than \p other;
        /// - Zero otherwise.
        ///
        inline bool operator>(_In_ const handle_type &other) const noexcept
        {
            // TODO: [Crypto] Make constant time.
            const int r = memcmp(m_h->pbCertEncoded, other->pbCertEncoded, std::min<DWORD>(m_h->cbCertEncoded, other->cbCertEncoded));
            return r > 0 || r == 0 && m_h->cbCertEncoded > other->cbCertEncoded;
        }

        ///
        /// Is certificate less than or equal?
        ///
        /// \param[in] other  Certificate to compare against
        /// \return
        /// - Non zero when certificate is less than \p other;
        /// - Zero otherwise.
        ///
        inline bool operator<=(_In_ const handle_type &other) const noexcept
        {
            return !operator>(other);
        }

        ///
        /// Is certificate greater than or equal?
        ///
        /// \param[in] other  Certificate to compare against
        /// \return
        /// - Non zero when certificate is greater than \p other;
        /// - Zero otherwise.
        ///
        inline bool operator>=(_In_ const handle_type &other) const noexcept
        {
            return !operator<(other);
        }

    protected:
        ///
        /// Destroys the certificate context.
        ///
        /// \sa [CertFreeCertificateContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376075.aspx)
        ///
        void free_internal() noexcept override;

        ///
        /// Duplicates the certificate context.
        ///
        /// \param[in] h  Object handle of existing certificate context
        ///
        /// \return Duplicated certificate context handle
        ///
        /// \sa [CertDuplicateCertificateContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376045.aspx)
        ///
        handle_type duplicate_internal(_In_ handle_type h) const noexcept override;
    };


    ///
    /// PCCERT_CHAIN_CONTEXT wrapper class
    ///
    class cert_chain_context : public dplhandle<PCCERT_CHAIN_CONTEXT, NULL>
    {
        DPLHANDLE_IMPL(cert_chain_context, NULL)

    public:
        ///
        /// Destroys the certificate chain context.
        ///
        /// \sa [CertFreeCertificateChain function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376073.aspx)
        ///
        virtual ~cert_chain_context();

        ///
        /// Creates the certificate chain context.
        ///
        /// \return
        /// - true when creation succeeds;
        /// - false when creation fails. For extended error information, call `GetLastError()`.
        ///
        /// \sa [CertGetCertificateChain function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376078.aspx)
        ///
        inline bool create(_In_opt_ HCERTCHAINENGINE hChainEngine, _In_ PCCERT_CONTEXT pCertContext, _In_opt_ LPFILETIME pTime, _In_opt_ HCERTSTORE hAdditionalStore, _In_ PCERT_CHAIN_PARA pChainPara, _In_ DWORD dwFlags, __reserved LPVOID pvReserved = NULL) noexcept
        {
            handle_type h;
            if (CertGetCertificateChain(hChainEngine, pCertContext, pTime, hAdditionalStore, pChainPara, dwFlags, pvReserved, &h)) {
                attach(h);
                return true;
            } else
                return false;
        }

    protected:
        ///
        /// Destroys the certificate chain context.
        ///
        /// \sa [CertFreeCertificateChain function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376073.aspx)
        ///
        void free_internal() noexcept override;

        ///
        /// Duplicates the certificate chain context.
        ///
        /// \param[in] h  Object handle of existing certificate chain context
        ///
        /// \return Duplicated certificate chain context handle
        ///
        /// \sa [CertDuplicateCertificateContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376045.aspx)
        ///
        handle_type duplicate_internal(_In_ handle_type h) const noexcept override;
    };


    ///
    /// HCERTSTORE wrapper class
    ///
    class cert_store : public handle<HCERTSTORE, NULL>
    {
        HANDLE_IMPL(cert_store, NULL)

    public:
        ///
        /// Closes the certificate store.
        ///
        /// \sa [CertCloseStore function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376026.aspx)
        ///
        virtual ~cert_store();

        ///
        /// Opens the certificate store.
        ///
        /// \return
        /// - true when creation succeeds;
        /// - false when creation fails. For extended error information, call `GetLastError()`.
        ///
        /// \sa [CertOpenStore function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376559.aspx)
        ///
        inline bool create(_In_ LPCSTR lpszStoreProvider, _In_ DWORD dwEncodingType, _In_opt_ HCRYPTPROV_LEGACY hCryptProv, _In_ DWORD dwFlags, _In_opt_ const void *pvPara) noexcept
        {
            handle_type h = CertOpenStore(lpszStoreProvider, dwEncodingType, hCryptProv, dwFlags, pvPara);
            if (h != invalid) {
                attach(h);
                return true;
            } else
                return false;
        }

        ///
        /// Opens the most common system certificate store. To open certificate stores with more complex requirements, such as file-based or memory-based stores, use `create()`.
        ///
        /// \return
        /// - true when creation succeeds;
        /// - false when creation fails. For extended error information, call `GetLastError()`.
        ///
        /// \sa [CertOpenSystemStore function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376560.aspx)
        ///
        inline bool create(_In_opt_ HCRYPTPROV_LEGACY hCryptProv, _In_z_ LPCTSTR szSubsystemProtocol) noexcept
        {
            handle_type h = CertOpenSystemStore(hCryptProv, szSubsystemProtocol);
            if (h != invalid) {
                attach(h);
                return true;
            } else
                return false;
        }

    protected:
        ///
        /// Closes the certificate store.
        ///
        /// \sa [CertCloseStore function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376026.aspx)
        ///
        void free_internal() noexcept override;
    };


    ///
    /// HCRYPTPROV wrapper class
    ///
    class crypt_prov : public handle<HCRYPTPROV, NULL>
    {
        HANDLE_IMPL(crypt_prov, NULL)

    public:
        ///
        /// Releases the cryptographic context.
        ///
        /// \sa [CryptReleaseContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa380268.aspx)
        ///
        virtual ~crypt_prov();

        ///
        /// Acquires the cryptographic context.
        ///
        /// \return
        /// - true when creation succeeds;
        /// - false when creation fails. For extended error information, call `GetLastError()`.
        ///
        /// \sa [CryptAcquireContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379886.aspx)
        ///
        inline bool create(_In_opt_z_ LPCTSTR szContainer, _In_opt_z_ LPCTSTR szProvider, _In_ DWORD dwProvType, _In_ DWORD dwFlags = 0) noexcept
        {
            handle_type h;
            if (CryptAcquireContext(&h, szContainer, szProvider, dwProvType, dwFlags)) {
                attach(h);
                return true;
            } else
                return false;
        }

    protected:
        ///
        /// Releases the cryptographic context.
        ///
        /// \sa [CryptReleaseContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa380268.aspx)
        ///
        void free_internal() noexcept override;
    };


    ///
    /// HCRYPTHASH wrapper class
    ///
    class crypt_hash : public dplhandle<HCRYPTHASH, NULL>
    {
        DPLHANDLE_IMPL(crypt_hash, NULL)

    public:
        ///
        /// Destroys the hash context.
        ///
        /// \sa [CryptDestroyHash function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379917.aspx)
        ///
        virtual ~crypt_hash();

        ///
        /// Creates the hash context.
        ///
        /// \return
        /// - true when creation succeeds;
        /// - false when creation fails. For extended error information, call `GetLastError()`.
        ///
        /// \sa [CryptCreateHash function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379908.aspx)
        ///
        inline bool create(_In_ HCRYPTPROV  hProv, _In_ ALG_ID Algid, _In_opt_ HCRYPTKEY hKey = NULL, _In_opt_ DWORD dwFlags = 0) noexcept
        {
            handle_type h;
            if (CryptCreateHash(hProv, Algid, hKey, dwFlags, &h)) {
                attach(h);
                return true;
            } else
                return false;
        }

    protected:
        ///
        /// Destroys the hash context.
        ///
        /// \sa [CryptDestroyHash function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379917.aspx)
        ///
        void free_internal() noexcept override;

        ///
        /// Duplicates the hash context.
        ///
        /// \param[in] h  Object handle of existing hash context
        ///
        /// \return Duplicated hash context handle
        ///
        /// \sa [CryptDuplicateHash function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379919.aspx)
        ///
        handle_type duplicate_internal(_In_ handle_type h) const noexcept override;
    };


    ///
    /// HCRYPTKEY wrapper class
    ///
    class crypt_key : public dplhandle<HCRYPTKEY, NULL>
    {
        DPLHANDLE_IMPL(crypt_key, NULL)

    public:
        ///
        /// Destroys the key.
        ///
        /// \sa [CryptDestroyKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379918.aspx)
        ///
        virtual ~crypt_key();

        ///
        /// Generates the key.
        ///
        /// \sa [CryptGenKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379941.aspx)
        ///
        inline bool generate(_In_ HCRYPTPROV hProv, _In_ ALG_ID Algid, _In_ DWORD dwFlags) noexcept
        {
            handle_type h;
            if (CryptGenKey(hProv, Algid, dwFlags, &h)) {
                attach(h);
                return true;
            } else
                return false;
        }

        ///
        /// Imports the key.
        ///
        /// \sa [CryptImportKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa380207.aspx)
        ///
        inline bool import(_In_ HCRYPTPROV hProv, __in_bcount(dwDataLen) LPCBYTE pbData, _In_ DWORD dwDataLen, _In_ HCRYPTKEY hPubKey, _In_ DWORD dwFlags) noexcept
        {
            handle_type h;
            if (CryptImportKey(hProv, pbData, dwDataLen, hPubKey, dwFlags, &h)) {
                attach(h);
                return true;
            } else
                return false;
        }

        ///
        /// Imports the public key.
        ///
        /// \sa [CryptImportPublicKeyInfo function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa380209.aspx)
        ///
        inline bool import_public(_In_ HCRYPTPROV hCryptProv, _In_ DWORD dwCertEncodingType, _In_ PCERT_PUBLIC_KEY_INFO pInfo) noexcept
        {
            handle_type h;
            if (CryptImportPublicKeyInfo(hCryptProv, dwCertEncodingType, pInfo, &h)) {
                attach(h);
                return true;
            } else
                return false;
        }

        ///
        /// Generates cryptographic session keys derived from a base data value.
        ///
        /// \sa [CryptDeriveKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379916.aspx)
        ///
        inline bool derive(_In_ HCRYPTPROV hProv, _In_ ALG_ID Algid, _In_ HCRYPTHASH hBaseData, _In_ DWORD dwFlags) noexcept
        {
            handle_type h;
            if (CryptDeriveKey(hProv, Algid, hBaseData, dwFlags, &h)) {
                attach(h);
                return true;
            } else
                return false;
        }

        ///
        /// Creates Exponent-of-one key
        ///
        /// \sa [How to export and import plain text session keys by using CryptoAPI](https://support.microsoft.com/en-us/kb/228786)
        ///
        /// \param[in] hProv      Handle of cryptographics provider to use
        /// \param[in] dwKeySpec  Key specification (`AT_KEYEXCHANGE` or `AT_SIGNATURE`)
        ///
        bool create_exp1(_In_ HCRYPTPROV hProv, _In_ DWORD dwKeySpec);

    protected:
        ///
        /// Destroys the key.
        ///
        /// \sa [CryptDestroyKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379918.aspx)
        ///
        void free_internal() noexcept override;

        ///
        /// Duplicates the key.
        ///
        /// \param[in] h  Object handle of existing key
        ///
        /// \return Duplicated key handle
        ///
        /// \sa [CryptDuplicateKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379920.aspx)
        ///
        handle_type duplicate_internal(_In_ handle_type h) const noexcept override;
    };

    ///
    /// DATA_BLOB wrapper class
    ///
    #pragma warning(push)
    #pragma warning(disable: 26432) // Copy constructor and assignment operator are also present, but not detected by code analysis as they are using base type source object reference.
    class data_blob : public DATA_BLOB
    {
    public:
        ///
        /// Initializes an empty BLOB.
        ///
        inline data_blob() noexcept
        {
            cbData = 0;
            pbData = NULL;
        }

        ///
        /// Initializes a BLOB from existing data.
        ///
        inline data_blob(_In_count_(size) BYTE *data, _In_ DWORD size) noexcept
        {
            cbData = size;
            pbData = data;
        }

        ///
        /// Duplicate an existing BLOB.
        ///
        inline data_blob(_In_ const DATA_BLOB &other)
        {
            cbData = other.cbData;
            if (cbData) {
                pbData = static_cast<BYTE*>(LocalAlloc(LMEM_FIXED, other.cbData));
                if (!pbData) throw win_runtime_error("LocalAlloc failed.");
                memcpy(pbData, other.pbData, other.cbData);
            } else
                pbData = NULL;
        }

        ///
        /// Move an existing BLOB.
        ///
        inline data_blob(_Inout_ data_blob &&other) noexcept
        {
            cbData = other.cbData;
            pbData = other.pbData;
            other.cbData = 0;
            other.pbData = NULL;
        }

        ///
        /// Destroys the BLOB.
        ///
        virtual ~data_blob();

        ///
        /// Copy an existing BLOB.
        ///
        inline data_blob& operator=(_In_ const DATA_BLOB &other)
        {
            if (this != &other) {
                cbData = other.cbData;
                if (pbData)
                    LocalFree(pbData);
                if (cbData) {
                    pbData = static_cast<BYTE*>(LocalAlloc(LMEM_FIXED, other.cbData));
                    if (!pbData) throw win_runtime_error("LocalAlloc failed.");
                    memcpy(pbData, other.pbData, other.cbData);
                } else
                    pbData = NULL;
            }

            return *this;
        }

        ///
        /// Move an existing BLOB.
        ///
        inline data_blob& operator=(_Inout_ data_blob &&other) noexcept
        {
            if (this != &other) {
                cbData = other.cbData;
                if (pbData)
                    LocalFree(pbData);
                pbData = other.pbData;
                other.cbData = 0;
                other.pbData = NULL;
            }

            return *this;
        }

        ///
        /// Get BLOB size.
        ///
        inline DWORD size() const noexcept
        {
            return cbData;
        }

        ///
        /// Get BLOB buffer.
        ///
        inline const BYTE* data() const noexcept
        {
            return pbData;
        }

        ///
        /// Get BLOB buffer.
        ///
        inline BYTE* data() noexcept
        {
            return pbData;
        }
    };
    #pragma warning(pop)

    /// @}
}


template<class _Elem, class _Traits, class _Ax>
inline DWORD CertGetNameStringA(_In_ PCCERT_CONTEXT pCertContext, _In_ DWORD dwType, _In_ DWORD dwFlags, _In_opt_ void *pvTypePara, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sNameString)
{
    // Query the final string length first.
    DWORD dwSize = ::CertGetNameStringA(pCertContext, dwType, dwFlags, pvTypePara, NULL, 0);

    // Allocate buffer on heap to format the string data into and read it.
    std::unique_ptr<_Elem[]> szBuffer(new _Elem[dwSize]);
    dwSize = ::CertGetNameStringA(pCertContext, dwType, dwFlags, pvTypePara, szBuffer.get(), dwSize);
    sNameString.assign(szBuffer.get(), dwSize - 1);
    return dwSize;
}


template<class _Elem, class _Traits, class _Ax>
inline DWORD CertGetNameStringW(_In_ PCCERT_CONTEXT pCertContext, _In_ DWORD dwType, _In_ DWORD dwFlags, _In_opt_ void *pvTypePara, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sNameString)
{
    // Query the final string length first.
    DWORD dwSize = ::CertGetNameStringW(pCertContext, dwType, dwFlags, pvTypePara, NULL, 0);

    // Allocate buffer on heap to format the string data into and read it.
    std::unique_ptr<_Elem[]> szBuffer(new _Elem[dwSize]);
    dwSize = ::CertGetNameStringW(pCertContext, dwType, dwFlags, pvTypePara, szBuffer.get(), dwSize);
    sNameString.assign(szBuffer.get(), dwSize - 1);
    return dwSize;
}


template<class _Ty, class _Ax>
inline _Success_(return != 0) BOOL WINAPI CertGetCertificateContextProperty(_In_ PCCERT_CONTEXT pCertContext, _In_ DWORD dwPropId, _Out_ std::vector<_Ty, _Ax> &aData)
{
    BYTE buf[WINSTD_STACK_BUFFER_BYTES];
    DWORD dwSize = WINSTD_STACK_BUFFER_BYTES;

    // Try with the stack buffer first.
    if (CertGetCertificateContextProperty(pCertContext, dwPropId, buf, &dwSize)) {
        // Copy from stack.
        aData.assign((const _Ty*)buf, (const _Ty*)buf + (dwSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        return TRUE;
    } else if (GetLastError() == ERROR_MORE_DATA) {
        aData.resize((dwSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        if (CertGetCertificateContextProperty(pCertContext, dwPropId, (BYTE*)aData.data(), &dwSize))
            return TRUE;
    }

    return FALSE;
}


template<class _Ty, class _Ax>
inline _Success_(return != 0) BOOL CryptGetHashParam(_In_ HCRYPTHASH hHash, _In_ DWORD dwParam, _Out_ std::vector<_Ty, _Ax> &aData, _In_ DWORD dwFlags)
{
    BYTE buf[WINSTD_STACK_BUFFER_BYTES];
    DWORD dwSize = WINSTD_STACK_BUFFER_BYTES;

    // Try with the stack buffer first.
    if (CryptGetHashParam(hHash, dwParam, buf, &dwSize, dwFlags)) {
        // Copy from stack.
        aData.assign((const _Ty*)buf, (const _Ty*)buf + (dwSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        return TRUE;
    } else if (GetLastError() == ERROR_MORE_DATA) {
        aData.resize((dwSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        if (CryptGetHashParam(hHash, dwParam, (BYTE*)aData.data(), &dwSize, dwFlags))
            return TRUE;
    }

    return FALSE;
}


template<class T>
inline BOOL CryptGetHashParam(_In_ HCRYPTHASH hHash, _In_ DWORD dwParam, _Out_ T &data, _In_ DWORD dwFlags)
{
    DWORD dwSize = sizeof(T);
    return CryptGetHashParam(hHash, dwParam, (BYTE*)&data, &dwSize, dwFlags);
}


template<class _Ty, class _Ax>
inline _Success_(return != 0) BOOL CryptGetKeyParam(_In_ HCRYPTKEY hKey, _In_ DWORD dwParam, _Out_ std::vector<_Ty, _Ax> &aData, _In_ DWORD dwFlags)
{
    BYTE buf[WINSTD_STACK_BUFFER_BYTES];
    DWORD dwSize = WINSTD_STACK_BUFFER_BYTES;

    // Try with the stack buffer first.
    if (CryptGetKeyParam(hKey, dwParam, buf, &dwSize, dwFlags)) {
        // Copy from stack.
        aData.assign((const _Ty*)buf, (const _Ty*)buf + (dwSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        return TRUE;
    } else if (GetLastError() == ERROR_MORE_DATA) {
        aData.resize((dwSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        if (CryptGetKeyParam(hKey, dwParam, (BYTE*)aData.data(), &dwSize, dwFlags))
            return TRUE;
    }

    return FALSE;
}


template<class T>
inline BOOL CryptGetKeyParam(_In_ HCRYPTKEY hKey, _In_ DWORD dwParam, _Out_ T &data, _In_ DWORD dwFlags)
{
    DWORD dwSize = sizeof(T);
    return CryptGetKeyParam(hKey, dwParam, (BYTE*)&data, &dwSize, dwFlags);
}


template<class _Ty, class _Ax>
inline _Success_(return != 0) BOOL CryptExportKey(_In_ HCRYPTKEY hKey, _In_ HCRYPTKEY hExpKey, _In_ DWORD dwBlobType, _In_ DWORD dwFlags, _Out_ std::vector<_Ty, _Ax> &aData)
{
    DWORD dwKeyBLOBSize = 0;

    if (CryptExportKey(hKey, hExpKey, dwBlobType, dwFlags, NULL, &dwKeyBLOBSize)) {
        aData.resize((dwKeyBLOBSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        if (CryptExportKey(hKey, hExpKey, dwBlobType, dwFlags, aData.data(), &dwKeyBLOBSize))
            return TRUE;
    }

    return FALSE;
}


template<class _Ty, class _Ax>
inline _Success_(return != 0) BOOL CryptEncrypt(_In_ HCRYPTKEY hKey, _In_opt_ HCRYPTHASH hHash, _In_ BOOL Final, _In_ DWORD dwFlags, _Inout_ std::vector<_Ty, _Ax> &aData)
{
    DWORD
        dwDataLen = (DWORD)(aData.size()     * sizeof(_Ty)),
        dwBufLen  = (DWORD)(aData.capacity() * sizeof(_Ty)),
        dwEncLen  = dwDataLen,
        dwResult;

    if (dwBufLen) {
        aData.resize(dwBufLen);
        if (CryptEncrypt(hKey, hHash, Final, dwFlags, (BYTE*)aData.data(), &dwEncLen, dwBufLen)) {
            // Encryption succeeded.
            assert(dwEncLen <= dwBufLen);
            if (dwEncLen < dwBufLen)
                aData.resize((dwEncLen + sizeof(_Ty) - 1) / sizeof(_Ty));
            return TRUE;
        } else
            dwResult = GetLastError();
    } else if (CryptEncrypt(hKey, NULL, Final, dwFlags, NULL, &dwEncLen, 0)) {
        // CryptEncrypt() always succeeds for output data size queries.
        // dwEncLen contains required output data size. Continue as if the buffer was to small. Actually, the output buffer _was_ too small!
        dwResult = ERROR_MORE_DATA;
    } else
        dwResult = GetLastError();

    if (dwResult == ERROR_MORE_DATA) {
        // Encrypted data will be longer. Reserve more space and retry.
        aData.resize(((dwBufLen = dwEncLen) + sizeof(_Ty) - 1) / sizeof(_Ty));
        dwEncLen = dwDataLen;
        if (CryptEncrypt(hKey, hHash, Final, dwFlags, (BYTE*)aData.data(), &dwEncLen, dwBufLen)) {
            // Encryption succeeded.
            assert(dwEncLen <= dwBufLen);
            if (dwEncLen < dwBufLen)
                aData.resize((dwEncLen + sizeof(_Ty) - 1) / sizeof(_Ty));
            return TRUE;
        }
    } else {
        // Resize back to data length.
        aData.resize((dwDataLen + sizeof(_Ty) - 1) / sizeof(_Ty));
    }

    return FALSE;
}


template<class _Ty, class _Ax>
inline _Success_(return != 0) BOOL CryptDecrypt(_In_ HCRYPTKEY hKey, _In_opt_ HCRYPTHASH hHash, _In_ BOOL Final, _In_ DWORD dwFlags, _Inout_ std::vector<_Ty, _Ax> &aData)
{
    DWORD dwDataLen = (DWORD)(aData.size() * sizeof(_Ty));

    if (CryptDecrypt(hKey, hHash, Final, dwFlags, (BYTE*)aData.data(), &dwDataLen)) {
        // Decryption succeeded.
        aData.resize((dwDataLen + sizeof(_Ty) - 1) / sizeof(_Ty));
        return TRUE;
    }

    return FALSE;
}
