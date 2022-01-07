/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
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
