/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 1991-2021 Amebis
    Copyright © 2016 GÉANT
*/

#include "PCH.h"


#if defined(SECURITY_WIN32) || defined(SECURITY_KERNEL) || defined(SECURITY_MAC)

//////////////////////////////////////////////////////////////////////
// winstd::sec_credentials
//////////////////////////////////////////////////////////////////////

winstd::sec_credentials::~sec_credentials()
{
    if (m_h != invalid) {
        FreeCredentialsHandle(m_h);
        delete m_h;
    }
}


void winstd::sec_credentials::free_internal() noexcept
{
    FreeCredentialsHandle(m_h);
    delete m_h;
}


//////////////////////////////////////////////////////////////////////
// winstd::sec_context
//////////////////////////////////////////////////////////////////////

winstd::sec_context::~sec_context()
{
    if (m_h != invalid) {
        DeleteSecurityContext(m_h);
        delete m_h;
    }
}


void winstd::sec_context::free_internal() noexcept
{
    DeleteSecurityContext(m_h);
    delete m_h;
}


//////////////////////////////////////////////////////////////////////
// winstd::sec_buffer_desc
//////////////////////////////////////////////////////////////////////

winstd::sec_buffer_desc::~sec_buffer_desc()
{
    for (ULONG i = 0; i < cBuffers; i++) {
        if (pBuffers[i].pvBuffer)
            FreeContextBuffer(pBuffers[i].pvBuffer);
    }
}

#endif
