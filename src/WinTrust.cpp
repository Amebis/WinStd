/*
    Copyright 1991-2018 Amebis
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

#pragma comment(lib, "Wintrust.lib")


//////////////////////////////////////////////////////////////////////
// winstd::wintrust
//////////////////////////////////////////////////////////////////////

winstd::wintrust::~wintrust()
{
    m_wtd.dwStateAction = WTD_STATEACTION_CLOSE;
    WinVerifyTrust(m_hwnd, (GUID*)&m_action, &m_wtd);
}
