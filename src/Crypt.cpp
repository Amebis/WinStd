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

#include "StdAfx.h"


//////////////////////////////////////////////////////////////////////
// winstd::cert_context
//////////////////////////////////////////////////////////////////////

winstd::cert_context::~cert_context()
{
    if (m_h)
        CertFreeCertificateContext(m_h);
}


void winstd::cert_context::free_internal()
{
    CertFreeCertificateContext(m_h);
}


winstd::cert_context::handle_type winstd::cert_context::duplicate_internal(_In_ handle_type h) const
{
    return CertDuplicateCertificateContext(h);
}


//////////////////////////////////////////////////////////////////////
// winstd::cert_chain_context
//////////////////////////////////////////////////////////////////////

winstd::cert_chain_context::~cert_chain_context()
{
    if (m_h)
        CertFreeCertificateChain(m_h);
}


void winstd::cert_chain_context::free_internal()
{
    CertFreeCertificateChain(m_h);
}


winstd::cert_chain_context::handle_type winstd::cert_chain_context::duplicate_internal(_In_ handle_type h) const
{
    return CertDuplicateCertificateChain(h);
}


//////////////////////////////////////////////////////////////////////
// winstd::cert_store
//////////////////////////////////////////////////////////////////////

winstd::cert_store::~cert_store()
{
    if (m_h)
        CertCloseStore(m_h, 0);
}


void winstd::cert_store::free_internal()
{
    CertCloseStore(m_h, 0);
}


//////////////////////////////////////////////////////////////////////
// winstd::crypt_prov
//////////////////////////////////////////////////////////////////////

winstd::crypt_prov::~crypt_prov()
{
    if (m_h)
        CryptReleaseContext(m_h, 0);
}


void winstd::crypt_prov::free_internal()
{
    CryptReleaseContext(m_h, 0);
}


//////////////////////////////////////////////////////////////////////
// winstd::crypt_hash
//////////////////////////////////////////////////////////////////////

winstd::crypt_hash::~crypt_hash()
{
    if (m_h)
        CryptDestroyHash(m_h);
}


void winstd::crypt_hash::free_internal()
{
    CryptDestroyHash(m_h);
}


winstd::crypt_hash::handle_type winstd::crypt_hash::duplicate_internal(_In_ handle_type h) const
{
    handle_type hNew = NULL;
    return CryptDuplicateHash(h, NULL, 0, &hNew) ? hNew : NULL;
}


//////////////////////////////////////////////////////////////////////
// winstd::crypt_key
//////////////////////////////////////////////////////////////////////

winstd::crypt_key::~crypt_key()
{
    if (m_h)
        CryptDestroyKey(m_h);
}


void winstd::crypt_key::free_internal()
{
    CryptDestroyKey(m_h);
}


winstd::crypt_key::handle_type winstd::crypt_key::duplicate_internal(_In_ handle_type h) const
{
    handle_type hNew = NULL;
    return CryptDuplicateKey(h, NULL, 0, &hNew) ? hNew : NULL;
}
