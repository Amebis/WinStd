/*
    SPDX-License-Identifier: MIT
    Copyright © 1991-2023 Amebis
    Copyright © 2016 GÉANT
*/

/// \defgroup WinStdCryptoAPI Cryptography API

#pragma once

#include "Common.h"
#include <assert.h>
#include <WinCrypt.h>
#include <algorithm>
#include <string>
#include <vector>

/// \addtogroup WinStdCryptoAPI
/// @{

/// @copydoc CertGetNameStringW()
template<class _Traits, class _Ax>
static DWORD CertGetNameStringA(_In_ PCCERT_CONTEXT pCertContext, _In_ DWORD dwType, _In_ DWORD dwFlags, _In_opt_ void *pvTypePara, _Out_ std::basic_string<char, _Traits, _Ax> &sNameString)
{
    // Query the final string length first.
    DWORD dwSize = ::CertGetNameStringA(pCertContext, dwType, dwFlags, pvTypePara, NULL, 0);

    // Allocate buffer on heap to format the string data into and read it.
    std::unique_ptr<char[]> szBuffer(new char[dwSize]);
    dwSize = ::CertGetNameStringA(pCertContext, dwType, dwFlags, pvTypePara, szBuffer.get(), dwSize);
    sNameString.assign(szBuffer.get(), dwSize - 1);
    return dwSize;
}

///
/// Obtains the subject or issuer name from a certificate [CERT_CONTEXT](https://msdn.microsoft.com/en-us/library/windows/desktop/aa377189.aspx) structure and stores it in a std::wstring string.
///
/// \sa [CertGetNameString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376086.aspx)
///
template<class _Traits, class _Ax>
static DWORD CertGetNameStringW(_In_ PCCERT_CONTEXT pCertContext, _In_ DWORD dwType, _In_ DWORD dwFlags, _In_opt_ void *pvTypePara, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sNameString)
{
    // Query the final string length first.
    DWORD dwSize = ::CertGetNameStringW(pCertContext, dwType, dwFlags, pvTypePara, NULL, 0);

    // Allocate buffer on heap to format the string data into and read it.
    std::unique_ptr<wchar_t[]> szBuffer(new wchar_t[dwSize]);
    dwSize = ::CertGetNameStringW(pCertContext, dwType, dwFlags, pvTypePara, szBuffer.get(), dwSize);
    sNameString.assign(szBuffer.get(), dwSize - 1);
    return dwSize;
}

///
/// Retrieves the information contained in an extended property of a certificate context.
///
/// \sa [CertGetCertificateContextProperty function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376079.aspx)
///
template<class _Ty, class _Ax>
static _Success_(return != 0) BOOL WINAPI CertGetCertificateContextProperty(_In_ PCCERT_CONTEXT pCertContext, _In_ DWORD dwPropId, _Out_ std::vector<_Ty, _Ax> &aData)
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
        if (CertGetCertificateContextProperty(pCertContext, dwPropId, aData.data(), &dwSize))
            return TRUE;
    }

    return FALSE;
}

///
/// Retrieves data that governs the operations of a hash object. The actual hash value can be retrieved by using this function.
///
/// \sa [CryptGetHashParam function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379947.aspx)
///
template<class _Ty, class _Ax>
static _Success_(return != 0) BOOL CryptGetHashParam(_In_ HCRYPTHASH hHash, _In_ DWORD dwParam, _Out_ std::vector<_Ty, _Ax> &aData, _In_ DWORD dwFlags)
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
        if (CryptGetHashParam(hHash, dwParam, reinterpret_cast<BYTE*>(aData.data()), &dwSize, dwFlags))
            return TRUE;
    }

    return FALSE;
}

///
/// Retrieves data that governs the operations of a hash object. The actual hash value can be retrieved by using this function.
///
/// \sa [CryptGetHashParam function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379947.aspx)
///
template<class T>
static _Success_(return != 0) BOOL CryptGetHashParam(_In_ HCRYPTHASH hHash, _In_ DWORD dwParam, _Out_ T &data, _In_ DWORD dwFlags)
{
    DWORD dwSize = sizeof(T);
    return CryptGetHashParam(hHash, dwParam, (BYTE*)&data, &dwSize, dwFlags);
}

///
/// Retrieves data that governs the operations of a key.
///
/// \sa [CryptGetKeyParam function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379949.aspx)
///
template<class _Ty, class _Ax>
static _Success_(return != 0) BOOL CryptGetKeyParam(_In_ HCRYPTKEY hKey, _In_ DWORD dwParam, _Out_ std::vector<_Ty, _Ax> &aData, _In_ DWORD dwFlags)
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
        if (CryptGetKeyParam(hKey, dwParam, reinterpret_cast<BYTE*>(aData.data()), &dwSize, dwFlags))
            return TRUE;
    }

    return FALSE;
}

///
/// Retrieves data that governs the operations of a key.
///
/// \sa [CryptGetKeyParam function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379949.aspx)
///
template<class T>
static BOOL CryptGetKeyParam(_In_ HCRYPTKEY hKey, _In_ DWORD dwParam, _Out_ T &data, _In_ DWORD dwFlags)
{
    DWORD dwSize = sizeof(T);
    return CryptGetKeyParam(hKey, dwParam, (BYTE*)&data, &dwSize, dwFlags);
}

///
/// Exports a cryptographic key or a key pair from a cryptographic service provider (CSP) in a secure manner.
///
/// \sa [CryptExportKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379931.aspx)
///
template<class _Ty, class _Ax>
static _Success_(return != 0) BOOL CryptExportKey(_In_ HCRYPTKEY hKey, _In_ HCRYPTKEY hExpKey, _In_ DWORD dwBlobType, _In_ DWORD dwFlags, _Out_ std::vector<_Ty, _Ax> &aData)
{
    DWORD dwKeyBLOBSize = 0;

    if (CryptExportKey(hKey, hExpKey, dwBlobType, dwFlags, NULL, &dwKeyBLOBSize)) {
        aData.resize((dwKeyBLOBSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        if (CryptExportKey(hKey, hExpKey, dwBlobType, dwFlags, reinterpret_cast<BYTE*>(aData.data()), &dwKeyBLOBSize))
            return TRUE;
    }

    return FALSE;
}

///
/// Encrypts data.
///
/// \sa [CryptEncrypt function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379924.aspx)
///
template<class _Ty, class _Ax>
static _Success_(return != 0) BOOL CryptEncrypt(_In_ HCRYPTKEY hKey, _In_opt_ HCRYPTHASH hHash, _In_ BOOL Final, _In_ DWORD dwFlags, _Inout_ std::vector<_Ty, _Ax> &aData)
{
    SIZE_T
        sDataLen = SIZETMult(aData.size(), sizeof(_Ty)),
        sBufLen  = SIZETMult(aData.capacity(), sizeof(_Ty));
    if (sDataLen > DWORD_MAX || sBufLen > DWORD_MAX)
        throw std::invalid_argument("Data too big");
    DWORD
        dwDataLen = static_cast<DWORD>(sDataLen),
        dwBufLen  = static_cast<DWORD>(sBufLen),
        dwEncLen  = dwDataLen,
        dwResult;

    if (dwBufLen) {
        aData.resize(dwBufLen);
        if (CryptEncrypt(hKey, hHash, Final, dwFlags, reinterpret_cast<BYTE*>(aData.data()), &dwEncLen, dwBufLen)) {
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
        if (CryptEncrypt(hKey, hHash, Final, dwFlags, reinterpret_cast<BYTE*>(aData.data()), &dwEncLen, dwBufLen)) {
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

///
/// Decrypts data previously encrypted by using the CryptEncrypt function.
///
/// \sa [CryptDecrypt function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379913.aspx)
///
template<class _Ty, class _Ax>
static _Success_(return != 0) BOOL CryptDecrypt(_In_ HCRYPTKEY hKey, _In_opt_ HCRYPTHASH hHash, _In_ BOOL Final, _In_ DWORD dwFlags, _Inout_ std::vector<_Ty, _Ax> &aData)
{
    SIZE_T sDataLen = SIZETMult(aData.size(), sizeof(_Ty));
    if (sDataLen > DWORD_MAX)
        throw std::invalid_argument("Data too big");
    DWORD dwDataLen = static_cast<DWORD>(sDataLen);

    if (CryptDecrypt(hKey, hHash, Final, dwFlags, reinterpret_cast<BYTE*>(aData.data()), &dwDataLen)) {
        // Decryption succeeded.
        aData.resize((dwDataLen + sizeof(_Ty) - 1) / sizeof(_Ty));
        return TRUE;
    }

    return FALSE;
}

/// @}

namespace winstd
{
    /// \addtogroup WinStdCryptoAPI
    /// @{

    ///
    /// PCCERT_CONTEXT wrapper class
    ///
    /// \sa [CertCreateCertificateContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376033.aspx)
    ///
    class cert_context : public dplhandle<PCCERT_CONTEXT, NULL>
    {
        WINSTD_DPLHANDLE_IMPL(cert_context, NULL)

    public:
        ///
        /// Destroys the certificate context.
        ///
        /// \sa [CertFreeCertificateContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376075.aspx)
        ///
        virtual ~cert_context()
        {
            if (m_h != invalid)
                free_internal();
        }

        ///
        /// Is certificate equal to?
        ///
        /// \param[in] other  Certificate to compare against
        /// \return
        /// - Non zero when certificate is equal to \p other;
        /// - Zero otherwise.
        ///
        bool operator==(_In_ const handle_type &other) const noexcept
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
        bool operator!=(_In_ const handle_type &other) const noexcept
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
        bool operator<(_In_ const handle_type &other) const noexcept
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
        bool operator>(_In_ const handle_type &other) const noexcept
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
        bool operator<=(_In_ const handle_type &other) const noexcept
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
        bool operator>=(_In_ const handle_type &other) const noexcept
        {
            return !operator<(other);
        }

    protected:
        ///
        /// Destroys the certificate context.
        ///
        /// \sa [CertFreeCertificateContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376075.aspx)
        ///
        void free_internal() noexcept override
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
        handle_type duplicate_internal(_In_ handle_type h) const override
        {
            // As per doc, this only increases refcounter. Should never fail.
            return CertDuplicateCertificateContext(h);
        }
    };

    ///
    /// PCCERT_CHAIN_CONTEXT wrapper class
    ///
    /// \sa [CertGetCertificateChain function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376078.aspx)
    ///
    class cert_chain_context : public dplhandle<PCCERT_CHAIN_CONTEXT, NULL>
    {
        WINSTD_DPLHANDLE_IMPL(cert_chain_context, NULL)

    public:
        ///
        /// Destroys the certificate chain context.
        ///
        /// \sa [CertFreeCertificateChain function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376073.aspx)
        ///
        virtual ~cert_chain_context()
        {
            if (m_h != invalid)
                free_internal();
        }

    protected:
        ///
        /// Destroys the certificate chain context.
        ///
        /// \sa [CertFreeCertificateChain function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376073.aspx)
        ///
        void free_internal() noexcept override
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
        /// \sa [CertDuplicateCertificateChain function](https://learn.microsoft.com/en-us/windows/win32/api/wincrypt/nf-wincrypt-certduplicatecertificatechain)
        ///
        handle_type duplicate_internal(_In_ handle_type h) const override
        {
            // As per doc, this only increases refcounter. Should never fail.
            return CertDuplicateCertificateChain(h);
        }
    };

    ///
    /// HCERTSTORE wrapper class
    ///
    /// \sa [CertOpenStore function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376559.aspx)
    /// \sa [CertOpenSystemStore function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376560.aspx)
    ///
    class cert_store : public handle<HCERTSTORE, NULL>
    {
        WINSTD_HANDLE_IMPL(cert_store, NULL)

    public:
        ///
        /// Closes the certificate store.
        ///
        /// \sa [CertCloseStore function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376026.aspx)
        ///
        virtual ~cert_store()
        {
            if (m_h != invalid)
                free_internal();
        }

    protected:
        ///
        /// Closes the certificate store.
        ///
        /// \sa [CertCloseStore function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376026.aspx)
        ///
        void free_internal() noexcept override
        {
            CertCloseStore(m_h, 0);
        }
    };

    ///
    /// HCRYPTPROV wrapper class
    ///
    /// \sa [CryptAcquireContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379886.aspx)
    ///
    class crypt_prov : public handle<HCRYPTPROV, NULL>
    {
        WINSTD_HANDLE_IMPL(crypt_prov, NULL)

    public:
        ///
        /// Releases the cryptographic context.
        ///
        /// \sa [CryptReleaseContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa380268.aspx)
        ///
        virtual ~crypt_prov()
        {
            if (m_h != invalid)
                free_internal();
        }

    protected:
        ///
        /// Releases the cryptographic context.
        ///
        /// \sa [CryptReleaseContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa380268.aspx)
        ///
        void free_internal() noexcept override
        {
            CryptReleaseContext(m_h, 0);
        }
    };

    ///
    /// HCRYPTHASH wrapper class
    ///
    /// \sa [CryptCreateHash function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379908.aspx)
    ///
    class crypt_hash : public dplhandle<HCRYPTHASH, NULL>
    {
        WINSTD_DPLHANDLE_IMPL(crypt_hash, NULL)

    public:
        ///
        /// Destroys the hash context.
        ///
        /// \sa [CryptDestroyHash function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379917.aspx)
        ///
        virtual ~crypt_hash()
        {
            if (m_h != invalid)
                free_internal();
        }

    protected:
        ///
        /// Destroys the hash context.
        ///
        /// \sa [CryptDestroyHash function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379917.aspx)
        ///
        void free_internal() noexcept override
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
        handle_type duplicate_internal(_In_ handle_type h) const override
        {
            handle_type hNew;
            if (CryptDuplicateHash(h, NULL, 0, &hNew))
                return hNew;
            throw win_runtime_error("CryptDuplicateHash failed");
        }
    };

    ///
    /// HCRYPTKEY wrapper class
    ///
    /// \sa [CryptGenKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379941.aspx)
    /// \sa [CryptImportKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa380207.aspx)
    /// \sa [CryptImportPublicKeyInfo function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa380209.aspx)
    /// \sa [CryptDeriveKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379916.aspx)
    ///
    class crypt_key : public dplhandle<HCRYPTKEY, NULL>
    {
        WINSTD_DPLHANDLE_IMPL(crypt_key, NULL)

    public:
        ///
        /// Destroys the key.
        ///
        /// \sa [CryptDestroyKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379918.aspx)
        ///
        virtual ~crypt_key()
        {
            if (m_h != invalid)
                free_internal();
        }

        ///
        /// Creates Exponent-of-one key
        ///
        /// \sa [How to export and import plain text session keys by using CryptoAPI](https://support.microsoft.com/en-us/kb/228786)
        ///
        /// \param[in] hProv      Handle of cryptographics provider to use
        /// \param[in] dwKeySpec  Key specification (`AT_KEYEXCHANGE` or `AT_SIGNATURE`)
        ///
        bool create_exp1(_In_ HCRYPTPROV hProv, _In_ DWORD dwKeySpec)
        {
            if (dwKeySpec != AT_KEYEXCHANGE && dwKeySpec != AT_SIGNATURE) {
                SetLastError(ERROR_INVALID_PARAMETER);
                return false;
            }

            // Generate the private key.
            handle_type h;
            if (CryptGenKey(hProv, dwKeySpec, CRYPT_EXPORTABLE, &h)) {
                // Export the private key, we'll convert it to a private exponent of one key.
                std::vector<BYTE, sanitizing_allocator<BYTE>> key_blob;
                if (CryptExportKey(h, 0, PRIVATEKEYBLOB, 0, key_blob)) {
                    CryptDestroyKey(h);

                    // Get the byte length of the key.
                    size_t
                        size_key   = *reinterpret_cast<DWORD*>(&key_blob[12])/8,
                        size_prime = size_key/2;

                    // Modify the Exponent in Key BLOB format
                    // Key BLOB format is documented in SDK

                    // Convert pubexp in rsapubkey to 1
                    LPBYTE ptr = &key_blob[16];
                    *reinterpret_cast<DWORD*>(ptr) = 1;
                    ptr += sizeof(DWORD);

                    // Skip modulus, prime1, prime2
                    ptr += size_key;
                    ptr += size_prime;
                    ptr += size_prime;

                    // Convert exponent1 to 1
                    ptr[0] = 1;
                    memset(ptr + 1, 0, size_prime - 1);
                    ptr += size_prime;

                    // Convert exponent2 to 1
                    ptr[0] = 1;
                    memset(ptr + 1, 0, size_prime - 1);
                    ptr += size_prime;

                    // Skip coefficient
                    ptr += size_prime;

                    // Convert privateExponent to 1
                    ptr[0] = 1;
                    memset(ptr + 1, 0, size_key - 1);

                    // Import the exponent-of-one private key.
                    if (CryptImportKey(hProv, key_blob.data(), static_cast<DWORD>(key_blob.size()), 0, 0, &h)) {
                        attach(h);
                        return true;
                    }
                } else
                    CryptDestroyKey(h);
            }

            return false;
        }

    protected:
        ///
        /// Destroys the key.
        ///
        /// \sa [CryptDestroyKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379918.aspx)
        ///
        void free_internal() noexcept override
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
        handle_type duplicate_internal(_In_ handle_type h) const override
        {
            handle_type hNew;
            if (CryptDuplicateKey(h, NULL, 0, &hNew))
                return hNew;
            throw win_runtime_error("CryptDuplicateKey failed");
        }
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
        data_blob() noexcept
        {
            cbData = 0;
            pbData = NULL;
        }

        ///
        /// Initializes a BLOB from existing data.
        ///
        data_blob(_In_count_(size) BYTE *data, _In_ DWORD size) noexcept
        {
            cbData = size;
            pbData = data;
        }

        ///
        /// Duplicate an existing BLOB.
        ///
        data_blob(_In_ const DATA_BLOB &other)
        {
            cbData = other.cbData;
            if (cbData) {
                pbData = static_cast<BYTE*>(LocalAlloc(LMEM_FIXED, other.cbData));
                if (!pbData) throw win_runtime_error("LocalAlloc failed");
                memcpy(pbData, other.pbData, other.cbData);
            } else
                pbData = NULL;
        }

        ///
        /// Move an existing BLOB.
        ///
        data_blob(_Inout_ data_blob &&other) noexcept
        {
            cbData = other.cbData;
            pbData = other.pbData;
            other.cbData = 0;
            other.pbData = NULL;
        }

        ///
        /// Destroys the BLOB.
        ///
        virtual ~data_blob()
        {
            if (pbData != NULL)
                LocalFree(pbData);
        }

        ///
        /// Copy an existing BLOB.
        ///
        data_blob& operator=(_In_ const DATA_BLOB &other)
        {
            if (this != &other) {
                cbData = other.cbData;
                if (pbData)
                    LocalFree(pbData);
                if (cbData) {
                    pbData = static_cast<BYTE*>(LocalAlloc(LMEM_FIXED, other.cbData));
                    if (!pbData) throw win_runtime_error("LocalAlloc failed");
                    memcpy(pbData, other.pbData, other.cbData);
                } else
                    pbData = NULL;
            }

            return *this;
        }

        ///
        /// Move an existing BLOB.
        ///
        data_blob& operator=(_Inout_ data_blob &&other) noexcept
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
        DWORD size() const noexcept
        {
            return cbData;
        }

        ///
        /// Get BLOB buffer.
        ///
        const BYTE* data() const noexcept
        {
            return pbData;
        }

        ///
        /// Get BLOB buffer.
        ///
        BYTE* data() noexcept
        {
            return pbData;
        }
    };
    #pragma warning(pop)

    /// @}
}

/// \addtogroup WinStdCryptoAPI
/// @{

#pragma warning(push)
#pragma warning(disable: 4505) // Don't warn on unused code

///
/// The CertGetCertificateChain function builds a certificate chain context starting from an end certificate and going back, if possible, to a trusted root certificate.
///
/// \sa [CertGetCertificateChain function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376078.aspx)
///
static BOOL CertGetCertificateChain(_In_opt_ HCERTCHAINENGINE hChainEngine, _In_ PCCERT_CONTEXT pCertContext, _In_opt_ LPFILETIME pTime, _In_opt_ HCERTSTORE hAdditionalStore, _In_ PCERT_CHAIN_PARA pChainPara, _In_ DWORD dwFlags, _Reserved_ LPVOID pvReserved, _Inout_ winstd::cert_chain_context &ctx)
{
    PCCERT_CHAIN_CONTEXT pChainContext;
    BOOL bResult = CertGetCertificateChain(hChainEngine, pCertContext, pTime, hAdditionalStore, pChainPara, dwFlags, pvReserved, &pChainContext);
    if (bResult)
        ctx.attach(pChainContext);
    return bResult;
}

/// @copydoc CryptAcquireContextW()
static BOOL CryptAcquireContextA(_Inout_ winstd::crypt_prov &prov, _In_opt_ LPCSTR szContainer, _In_opt_ LPCSTR szProvider, _In_ DWORD dwProvType, _In_ DWORD dwFlags)
{
    HCRYPTPROV h;
    BOOL bResult = CryptAcquireContextA(&h, szContainer, szProvider, dwProvType, dwFlags);
    if (bResult)
        prov.attach(h);
    return bResult;
}

///
/// Acquires the cryptographic context.
///
/// \sa [CryptAcquireContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379886.aspx)
///
static BOOL CryptAcquireContextW(_Inout_ winstd::crypt_prov &prov, _In_opt_ LPCWSTR szContainer, _In_opt_ LPCWSTR szProvider, _In_ DWORD dwProvType, _In_ DWORD dwFlags)
{
    HCRYPTPROV h;
    BOOL bResult = CryptAcquireContextW(&h, szContainer, szProvider, dwProvType, dwFlags);
    if (bResult)
        prov.attach(h);
    return bResult;
}

///
/// Creates the hash context.
///
/// \sa [CryptCreateHash function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379908.aspx)
///
static BOOL CryptCreateHash(_In_ HCRYPTPROV hProv, _In_ ALG_ID Algid, _In_ HCRYPTKEY hKey, _In_ DWORD dwFlags, _Inout_ winstd::crypt_hash &hash)
{
    HCRYPTHASH h;
    BOOL bResult = CryptCreateHash(hProv, Algid, hKey, dwFlags, &h);
    if (bResult)
        hash.attach(h);
    return bResult;
}

///
/// Generates the key.
///
/// \sa [CryptGenKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379941.aspx)
///
static BOOL CryptGenKey(_In_ HCRYPTPROV hProv, _In_ ALG_ID Algid, _In_ DWORD dwFlags, _Inout_ winstd::crypt_key &key)
{
    HCRYPTKEY h;
    BOOL bResult = CryptGenKey(hProv, Algid, dwFlags, &h);
    if (bResult)
        key.attach(h);
    return bResult;
}

///
/// Imports the key.
///
/// \sa [CryptImportKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa380207.aspx)
///
static bool CryptImportKey(_In_ HCRYPTPROV hProv, __in_bcount(dwDataLen) LPCBYTE pbData, _In_ DWORD dwDataLen, _In_ HCRYPTKEY hPubKey, _In_ DWORD dwFlags, _Inout_ winstd::crypt_key &key)
{
    HCRYPTKEY h;
    BOOL bResult = CryptImportKey(hProv, pbData, dwDataLen, hPubKey, dwFlags, &h);
    if (bResult)
        key.attach(h);
    return bResult;
}

///
/// Imports the public key.
///
/// \sa [CryptImportPublicKeyInfo function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa380209.aspx)
///
static bool CryptImportPublicKeyInfo(_In_ HCRYPTPROV hCryptProv, _In_ DWORD dwCertEncodingType, _In_ PCERT_PUBLIC_KEY_INFO pInfo, _Inout_ winstd::crypt_key &key)
{
    HCRYPTKEY h;
    BOOL bResult = CryptImportPublicKeyInfo(hCryptProv, dwCertEncodingType, pInfo, &h);
    if (bResult)
        key.attach(h);
    return bResult;
}

///
/// Generates cryptographic session keys derived from a base data value.
///
/// \sa [CryptDeriveKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379916.aspx)
///
static bool CryptDeriveKey(_In_ HCRYPTPROV hProv, _In_ ALG_ID Algid, _In_ HCRYPTHASH hBaseData, _In_ DWORD dwFlags, _Inout_ winstd::crypt_key &key)
{
    HCRYPTKEY h;
    BOOL bResult = CryptDeriveKey(hProv, Algid, hBaseData, dwFlags, &h);
    if (bResult)
        key.attach(h);
    return bResult;
}

#pragma warning(pop)

/// @}
