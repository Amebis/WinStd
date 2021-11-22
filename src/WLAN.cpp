/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 1991-2021 Amebis
    Copyright © 2016 GÉANT
*/

#include "PCH.h"


//////////////////////////////////////////////////////////////////////
// winstd::wlan_handle
//////////////////////////////////////////////////////////////////////

winstd::wlan_handle::~wlan_handle()
{
    if (m_h != invalid)
        WlanCloseHandle(m_h, NULL);
}


void winstd::wlan_handle::free_internal() noexcept
{
    WlanCloseHandle(m_h, NULL);
}
