/*
    Copyright 1991-2020 Amebis
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

#include "PCH.h"

#pragma comment(lib, "Crypt32.lib")


//////////////////////////////////////////////////////////////////////
// winstd::cert_context
//////////////////////////////////////////////////////////////////////

winstd::cert_context::~cert_context()
{
    if (m_h != invalid)
        CertFreeCertificateContext(m_h);
}


void winstd::cert_context::free_internal() noexcept
{
    CertFreeCertificateContext(m_h);
}


winstd::cert_context::handle_type winstd::cert_context::duplicate_internal(_In_ handle_type h) const noexcept
{
    return CertDuplicateCertificateContext(h);
}


//////////////////////////////////////////////////////////////////////
// winstd::cert_chain_context
//////////////////////////////////////////////////////////////////////

winstd::cert_chain_context::~cert_chain_context()
{
    if (m_h != invalid)
        CertFreeCertificateChain(m_h);
}


void winstd::cert_chain_context::free_internal() noexcept
{
    CertFreeCertificateChain(m_h);
}


winstd::cert_chain_context::handle_type winstd::cert_chain_context::duplicate_internal(_In_ handle_type h) const noexcept
{
    return CertDuplicateCertificateChain(h);
}


//////////////////////////////////////////////////////////////////////
// winstd::cert_store
//////////////////////////////////////////////////////////////////////

winstd::cert_store::~cert_store()
{
    if (m_h != invalid)
        CertCloseStore(m_h, 0);
}


void winstd::cert_store::free_internal() noexcept
{
    CertCloseStore(m_h, 0);
}


//////////////////////////////////////////////////////////////////////
// winstd::crypt_prov
//////////////////////////////////////////////////////////////////////

winstd::crypt_prov::~crypt_prov()
{
    if (m_h != invalid)
        CryptReleaseContext(m_h, 0);
}


void winstd::crypt_prov::free_internal() noexcept
{
    CryptReleaseContext(m_h, 0);
}


//////////////////////////////////////////////////////////////////////
// winstd::crypt_hash
//////////////////////////////////////////////////////////////////////

winstd::crypt_hash::~crypt_hash()
{
    if (m_h != invalid)
        CryptDestroyHash(m_h);
}


void winstd::crypt_hash::free_internal() noexcept
{
    CryptDestroyHash(m_h);
}


winstd::crypt_hash::handle_type winstd::crypt_hash::duplicate_internal(_In_ handle_type h) const noexcept
{
    handle_type hNew = invalid;
    return CryptDuplicateHash(h, NULL, 0, &hNew) ? hNew : invalid;
}


//////////////////////////////////////////////////////////////////////
// winstd::crypt_key
//////////////////////////////////////////////////////////////////////

winstd::crypt_key::~crypt_key()
{
    if (m_h != invalid)
        CryptDestroyKey(m_h);
}


bool winstd::crypt_key::create_exp1(_In_ HCRYPTPROV hProv, _In_ DWORD dwKeySpec)
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


void winstd::crypt_key::free_internal() noexcept
{
    CryptDestroyKey(m_h);
}


winstd::crypt_key::handle_type winstd::crypt_key::duplicate_internal(_In_ handle_type h) const noexcept
{
    handle_type hNew = invalid;
    return CryptDuplicateKey(h, NULL, 0, &hNew) ? hNew : invalid;
}


//////////////////////////////////////////////////////////////////////
// winstd::data_blob
//////////////////////////////////////////////////////////////////////

#pragma warning(suppress: 26432) // Copy constructor and assignment operator are also present, but not detected by code analysis as they are using base type source object reference.
winstd::data_blob::~data_blob()
{
    if (pbData != NULL)
        LocalFree(pbData);
}
