﻿/*
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
// winstd::bstr
//////////////////////////////////////////////////////////////////////

winstd::bstr::~bstr()
{
    if (m_h != invalid)
        SysFreeString(m_h);
}


void winstd::bstr::free_internal() noexcept
{
    SysFreeString(m_h);
}


winstd::bstr::handle_type winstd::bstr::duplicate_internal(_In_ handle_type h) const noexcept
{
    return SysAllocStringLen(h, SysStringLen(h));
}


//////////////////////////////////////////////////////////////////////
// winstd::variant
//////////////////////////////////////////////////////////////////////

#pragma warning(suppress: 26432) // Copy constructor and assignment operator are also present, but not detected by code analysis as they are using base type source object reference.
winstd::variant::~variant()
{
    VariantClear(this);
}


//////////////////////////////////////////////////////////////////////
// winstd::com_initializer
//////////////////////////////////////////////////////////////////////

winstd::com_initializer::~com_initializer()
{
    if (SUCCEEDED(m_result))
        CoUninitialize();
}
