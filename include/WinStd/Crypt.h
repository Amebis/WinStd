/*
    Copyright 1991-2016 Amebis
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

#include "Common.h"

#include <WinCrypt.h>

#include <string>
#include <vector>

template<class _Elem, class _Traits, class _Ax> inline DWORD CertGetNameStringA(_In_ PCCERT_CONTEXT pCertContext, _In_ DWORD dwType, _In_ DWORD dwFlags, _In_ void *pvTypePara, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sNameString);
template<class _Elem, class _Traits, class _Ax> inline DWORD CertGetNameStringW(_In_ PCCERT_CONTEXT pCertContext, _In_ DWORD dwType, _In_ DWORD dwFlags, _In_ void *pvTypePara, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sNameString);
template<class _Ty, class _Ax> inline BOOL CryptGetHashParam(_In_ HCRYPTHASH  hHash, _In_ DWORD dwParam, _Out_ std::vector<_Ty, _Ax> &aData, _In_ DWORD dwFlags);
template<class _Ty, class _Ax> inline BOOL CryptExportKey(_In_ HCRYPTKEY hKey, _In_ HCRYPTKEY hExpKey, _In_ DWORD dwBlobType, _In_ DWORD dwFlags, _Out_ std::vector<_Ty, _Ax> &aData);
namespace winstd
{
    class cert_context;
    class cert_chain_context;
    class cert_store;
    class crypt_prov;
    class crypt_hash;
    class crypt_key;
}

#pragma once


///
/// \defgroup WinStdCryptoAPI Cryptography API
/// Integrates WinStd classes with Microsoft Cryptography API
///
/// @{

///
/// Obtains the subject or issuer name from a certificate [CERT_CONTEXT](https://msdn.microsoft.com/en-us/library/windows/desktop/aa377189.aspx) structure and stores it in a std::string string.
///
/// \sa [CertGetNameString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376086.aspx)
///
template<class _Elem, class _Traits, class _Ax>
inline DWORD CertGetNameStringA(_In_ PCCERT_CONTEXT pCertContext, _In_ DWORD dwType, _In_ DWORD dwFlags, _In_ void *pvTypePara, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sNameString)
{
    assert(0); // TODO: Test this code.

    // Query the final string length first.
    DWORD dwSize = ::CertGetNameStringA(pCertContext, dwType, dwFlags, pvTypePara, NULL, 0);

    // Allocate buffer on heap to format the string data into and read it.
    auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[dwSize]);
    dwSize = ::CertGetNameStringA(pCertContext, dwType, dwFlags, pvTypePara, szBuffer.get(), dwSize);
    sNameString.assign(szBuffer.get(), dwSize);
    return dwSize;
}


///
/// Obtains the subject or issuer name from a certificate [CERT_CONTEXT](https://msdn.microsoft.com/en-us/library/windows/desktop/aa377189.aspx) structure and stores it in a std::wstring string.
///
/// \sa [CertGetNameString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376086.aspx)
///
template<class _Elem, class _Traits, class _Ax>
inline DWORD CertGetNameStringW(_In_ PCCERT_CONTEXT pCertContext, _In_ DWORD dwType, _In_ DWORD dwFlags, _In_ void *pvTypePara, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sNameString)
{
    assert(0); // TODO: Test this code.

    // Query the final string length first.
    DWORD dwSize = ::CertGetNameStringW(pCertContext, dwType, dwFlags, pvTypePara, NULL, 0);

    // Allocate buffer on heap to format the string data into and read it.
    auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[dwSize]);
    dwSize = ::CertGetNameStringW(pCertContext, dwType, dwFlags, pvTypePara, szBuffer.get(), dwSize);
    sNameString.assign(szBuffer.get(), dwSize);
    return dwSize;
}


///
/// Retrieves data that governs the operations of a hash object. The actual hash value can be retrieved by using this function.
///
/// \sa [CryptGetHashParam function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379947.aspx)
///
template<class _Ty, class _Ax>
inline BOOL CryptGetHashParam(_In_ HCRYPTHASH  hHash, _In_ DWORD dwParam, _Out_ std::vector<_Ty, _Ax> &aData, _In_ DWORD dwFlags)
{
    assert(0); // TODO: Test this code.

    DWORD dwHashSize;

    if (CryptGetHashParam(hHash, dwParam, NULL, &dwHashSize, dwFlags)) {
        aData.resize((dwHashSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        if (CryptGetHashParam(hHash, dwParam, aData.data(), &dwHashSize, dwFlags))
            return TRUE;
    }

    aData.clear();
    return FALSE;
}


///
/// Exports a cryptographic key or a key pair from a cryptographic service provider (CSP) in a secure manner.
///
/// \sa [CryptExportKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379931.aspx)
///
template<class _Ty, class _Ax>
inline BOOL CryptExportKey(_In_ HCRYPTKEY hKey, _In_ HCRYPTKEY hExpKey, _In_ DWORD dwBlobType, _In_ DWORD dwFlags, _Out_ std::vector<_Ty, _Ax> &aData)
{
    assert(0); // TODO: Test this code.

    DWORD dwKeyBLOBSize;

    if (CryptExportKey(hKey, hExpKey, dwBlobType, dwFlags, NULL, &dwKeyBLOBSize)) {
        aData.resize((dwKeyBLOBSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        if (CryptExportKey(hKey, hExpKey, dwBlobType, dwFlags, aData.data(), &dwKeyBLOBSize))
            return TRUE;
    }

    aData.clear();
    return FALSE;
}

/// @}


namespace winstd
{
    /// \addtogroup ATLCryptoAPI
    /// @{

    ///
    /// PCCERT_CONTEXT wrapper class
    ///
    class cert_context : public dplhandle<PCCERT_CONTEXT>
    {
    public:
        ///
        /// Destroys the certificate context.
        ///
        /// \sa [CertFreeCertificateContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376075.aspx)
        ///
        virtual ~cert_context()
        {
            if (m_h)
                CertFreeCertificateContext(m_h);
        }

        ///
        /// Creates the certificate context.
        ///
        /// \return
        /// - true when creation succeeds;
        /// - false when creation fails. For extended error information, call `GetLastError()`.
        /// \sa [CertCreateCertificateContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376033.aspx)
        ///
        inline bool create(_In_  DWORD dwCertEncodingType, _In_  const unsigned char *pbCertEncoded, _In_  DWORD cbCertEncoded)
        {
            handle_type h = CertCreateCertificateContext(dwCertEncodingType, pbCertEncoded, cbCertEncoded);
            if (h) {
                attach(h);
                return true;
            } else
                return false;
        }

    protected:
        ///
        /// Destroys the certificate context.
        ///
        /// \sa [CertFreeCertificateContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376075.aspx)
        ///
        virtual void free_internal()
        {
            CertFreeCertificateContext(m_h);
        }

        ///
        /// Duplicates the certificate context.
        ///
        /// \param[in] h  Object handle of existing certificate context
        ///
        /// \return Duplicated certificate context handle
        ///
        /// \sa [CertDuplicateCertificateContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376045.aspx)
        ///
        virtual handle_type duplicate_internal(_In_ handle_type h) const
        {
            return CertDuplicateCertificateContext(h);
        }
    };


    ///
    /// PCCERT_CHAIN_CONTEXT wrapper class
    ///
    class cert_chain_context : public dplhandle<PCCERT_CHAIN_CONTEXT>
    {
    public:
        ///
        /// Destroys the certificate chain context.
        ///
        /// \sa [CertFreeCertificateChain function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376073.aspx)
        ///
        virtual ~cert_chain_context()
        {
            if (m_h)
                CertFreeCertificateChain(m_h);
        }

        ///
        /// Creates the certificate chain context.
        ///
        /// \return
        /// - true when creation succeeds;
        /// - false when creation fails. For extended error information, call `GetLastError()`.
        /// \sa [CertGetCertificateChain function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376078.aspx)
        ///
        inline bool create(_In_opt_ HCERTCHAINENGINE hChainEngine, _In_ PCCERT_CONTEXT pCertContext, _In_opt_ LPFILETIME pTime, _In_opt_ HCERTSTORE hAdditionalStore, _In_ PCERT_CHAIN_PARA pChainPara, _In_ DWORD dwFlags, __reserved LPVOID pvReserved)
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
        virtual void free_internal()
        {
            CertFreeCertificateChain(m_h);
        }

        ///
        /// Duplicates the certificate chain context.
        ///
        /// \param[in] h  Object handle of existing certificate chain context
        ///
        /// \return Duplicated certificate chain context handle
        ///
        /// \sa [CertDuplicateCertificateContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376045.aspx)
        ///
        virtual handle_type duplicate_internal(_In_ handle_type h) const
        {
            return CertDuplicateCertificateChain(h);
        }
    };


    ///
    /// HCERTSTORE wrapper class
    ///
    class cert_store : public handle<HCERTSTORE>
    {
    public:
        ///
        /// Closes the certificate store.
        ///
        /// \sa [CertCloseStore function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376026.aspx)
        ///
        virtual ~cert_store()
        {
            if (m_h)
                CertCloseStore(m_h, 0);
        }

        ///
        /// Opens the certificate store.
        ///
        /// \return
        /// - true when creation succeeds;
        /// - false when creation fails. For extended error information, call `GetLastError()`.
        /// \sa [CertOpenStore function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376559.aspx)
        ///
        inline bool create(_In_ LPCSTR lpszStoreProvider, _In_ DWORD dwEncodingType, _In_opt_ HCRYPTPROV_LEGACY hCryptProv, _In_ DWORD dwFlags, _In_opt_ const void *pvPara)
        {
            handle_type h = CertOpenStore(lpszStoreProvider, dwEncodingType, hCryptProv, dwFlags, pvPara);
            if (h) {
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
        virtual void free_internal()
        {
            CertCloseStore(m_h, 0);
        }
    };


    ///
    /// HCRYPTPROV wrapper class
    ///
    class crypt_prov : public handle<HCRYPTPROV>
    {
    public:
        ///
        /// Releases the cryptographi context.
        ///
        /// \sa [CryptReleaseContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa380268.aspx)
        ///
        virtual ~crypt_prov()
        {
            if (m_h)
                CryptReleaseContext(m_h, 0);
        }

        ///
        /// Acquires the cryptographic context.
        ///
        /// \return
        /// - true when creation succeeds;
        /// - false when creation fails. For extended error information, call `GetLastError()`.
        /// \sa [CryptAcquireContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379886.aspx)
        ///
        inline bool create(_In_opt_ LPCTSTR szContainer, _In_opt_ LPCTSTR szProvider, _In_ DWORD dwProvType, _In_ DWORD dwFlags)
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
        virtual void free_internal()
        {
            CryptReleaseContext(m_h, 0);
        }
    };


    ///
    /// HCRYPTHASH wrapper class
    ///
    class crypt_hash : public dplhandle<HCRYPTHASH>
    {
    public:
        ///
        /// Destroys the hash context.
        ///
        /// \sa [CryptDestroyHash function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379917.aspx)
        ///
        virtual ~crypt_hash()
        {
            if (m_h)
                CryptDestroyHash(m_h);
        }

        ///
        /// Creates the hash context.
        ///
        /// \return
        /// - true when creation succeeds;
        /// - false when creation fails. For extended error information, call `GetLastError()`.
        /// \sa [CryptCreateHash function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379908.aspx)
        ///
        inline bool create(_In_ HCRYPTPROV  hProv, _In_ ALG_ID Algid, _In_ HCRYPTKEY hKey, _In_ DWORD dwFlags)
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
        virtual void free_internal()
        {
            CryptDestroyHash(m_h);
        }

        ///
        /// Duplicates the hash context.
        ///
        /// \param[in] h  Object handle of existing hash context
        ///
        /// \return Duplicated hash context handle
        ///
        /// \sa [CryptDuplicateHash function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379919.aspx)
        ///
        virtual handle_type duplicate_internal(_In_ handle_type h) const
        {
            handle_type hNew = NULL;
            return CryptDuplicateHash(h, NULL, 0, &hNew) ? hNew : NULL;
        }
    };


    ///
    /// HCRYPTKEY wrapper class
    ///
    class crypt_key : public dplhandle<HCRYPTKEY>
    {
    public:
        ///
        /// Destroys the key.
        ///
        /// \sa [CryptDestroyKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379918.aspx)
        ///
        virtual ~crypt_key()
        {
            if (m_h)
                CryptDestroyKey(m_h);
        }

        ///
        /// Generates the key.
        ///
        /// \sa [CryptGenKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379941.aspx)
        ///
        inline bool generate(_In_ HCRYPTPROV hProv, _In_ ALG_ID Algid, _In_ DWORD dwFlags)
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
        inline bool import(_In_ HCRYPTPROV hProv, __in_bcount(dwDataLen) const unsigned char *pbData, _In_ DWORD dwDataLen, _In_ HCRYPTKEY hPubKey, _In_ DWORD dwFlags)
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
        inline bool import_public(_In_ HCRYPTPROV hCryptProv, _In_ DWORD dwCertEncodingType, _In_ PCERT_PUBLIC_KEY_INFO pInfo)
        {
            handle_type h;
            if (CryptImportPublicKeyInfo(hCryptProv, dwCertEncodingType, pInfo, &h)) {
                attach(h);
                return true;
            } else
                return false;
        }

    protected:
        ///
        /// Destroys the key.
        ///
        /// \sa [CryptDestroyKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379918.aspx)
        ///
        virtual void free_internal()
        {
            CryptDestroyKey(m_h);
        }

        ///
        /// Duplicates the key.
        ///
        /// \param[in] h  Object handle of existing key
        ///
        /// \return Duplicated key handle
        ///
        /// \sa [CryptDuplicateKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379920.aspx)
        ///
        virtual handle_type duplicate_internal(_In_ handle_type h) const
        {
            handle_type hNew = NULL;
            return CryptDuplicateKey(h, NULL, 0, &hNew) ? hNew : NULL;
        }
    };

    /// @}
}
