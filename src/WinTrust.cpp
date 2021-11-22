/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 1991-2021 Amebis
    Copyright © 2016 GÉANT
*/

#include "PCH.h"

#pragma comment(lib, "Wintrust.lib")


//////////////////////////////////////////////////////////////////////
// winstd::wintrust
//////////////////////////////////////////////////////////////////////

winstd::wintrust::~wintrust()
{
    m_wtd.dwStateAction = WTD_STATEACTION_CLOSE;
    WinVerifyTrust(m_hwnd, &m_action, &m_wtd);
}
