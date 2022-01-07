/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
*/

#include "PCH.h"


#if (NTDDI_VERSION >= NTDDI_WINXPSP2) || (_WIN32_WINNT >= 0x0502)

//////////////////////////////////////////////////////////////////////
// winstd::addrinfo
//////////////////////////////////////////////////////////////////////

winstd::addrinfo::~addrinfo()
{
    if (m_h != invalid)
        FreeAddrInfo(m_h);
}


void winstd::addrinfo::free_internal() noexcept
{
    FreeAddrInfo(m_h);
}

#endif
