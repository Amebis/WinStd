/*
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

#include "PCH.h"


//////////////////////////////////////////////////////////////////////
// winstd::dc
//////////////////////////////////////////////////////////////////////

winstd::dc::~dc()
{
    if (m_h != invalid)
        DeleteDC(m_h);
}


void winstd::dc::free_internal() noexcept
{
    DeleteDC(m_h);
}


//////////////////////////////////////////////////////////////////////
// winstd::window_dc
//////////////////////////////////////////////////////////////////////

winstd::window_dc::~window_dc()
{
    if (m_h != invalid)
        ReleaseDC(m_hwnd, m_h);
}


void winstd::window_dc::free_internal() noexcept
{
    ReleaseDC(m_hwnd, m_h);
}


//////////////////////////////////////////////////////////////////////
// winstd::dc_selector
//////////////////////////////////////////////////////////////////////

winstd::dc_selector::~dc_selector()
{
    if (m_orig)
        SelectObject(m_hdc, m_orig);
}
