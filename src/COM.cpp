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
// winstd::bstr
//////////////////////////////////////////////////////////////////////

winstd::bstr::bstr() : handle<BSTR>()
{
}


winstd::bstr::bstr(_In_ LPCOLESTR src) : handle<BSTR>(SysAllocString(src))
{
}


winstd::bstr::bstr(_In_ LPCOLESTR src, _In_ UINT len) : handle<BSTR>(SysAllocStringLen(src, len))
{
}


winstd::bstr::~bstr()
{
    if (m_h)
        SysFreeString(m_h);
}


void winstd::bstr::free_internal()
{
    SysFreeString(m_h);
}


winstd::bstr::handle_type winstd::bstr::duplicate_internal(_In_ handle_type h) const
{
    return SysAllocStringLen(h, SysStringLen(h));
}


//////////////////////////////////////////////////////////////////////
// winstd::variant
//////////////////////////////////////////////////////////////////////

winstd::variant::variant()
{
    VariantInit(this);
}


winstd::variant::variant(_In_ const VARIANT& varSrc)
{
    vt = VT_EMPTY;
    VariantCopy(this, &varSrc);
}


winstd::variant::variant(_In_ bool bSrc)
{
    vt = VT_BOOL;
    boolVal = bSrc ? VARIANT_TRUE : VARIANT_FALSE;
}


winstd::variant::variant(_In_ char cSrc)
{
    vt = VT_I1;
    cVal = cSrc;
}


winstd::variant::variant(_In_ unsigned char nSrc)
{
    vt = VT_UI1;
    bVal = nSrc;
}


winstd::variant::variant(_In_ short nSrc)
{
    vt = VT_I2;
    iVal = nSrc;
}


winstd::variant::variant(_In_ unsigned short nSrc)
{
    vt = VT_UI2;
    uiVal = nSrc;
}


winstd::variant::variant(_In_ int nSrc, _In_ VARTYPE vtSrc)
{
    assert(vtSrc == VT_I4 || vtSrc == VT_INT);
    vt = vtSrc;
    intVal = nSrc;
}


winstd::variant::variant(_In_ unsigned int nSrc, _In_ VARTYPE vtSrc)
{
    assert(vtSrc == VT_UI4 || vtSrc == VT_UINT);
    vt = vtSrc;
    uintVal= nSrc;
}


winstd::variant::variant(_In_ long nSrc, _In_ VARTYPE vtSrc)
{
    assert(vtSrc == VT_I4 || vtSrc == VT_ERROR);
    vt = vtSrc;
    lVal = nSrc;
}


winstd::variant::variant(_In_ unsigned long nSrc)
{
    vt = VT_UI4;
    ulVal = nSrc;
}


winstd::variant::variant(_In_ float fltSrc)
{
    vt = VT_R4;
    fltVal = fltSrc;
}


winstd::variant::variant(_In_ double dblSrc, _In_ VARTYPE vtSrc)
{
    assert(vtSrc == VT_R8 || vtSrc == VT_DATE);
    vt = vtSrc;
    dblVal = dblSrc;
}


winstd::variant::variant(_In_ long long nSrc)
{
    vt = VT_I8;
    llVal = nSrc;
}


winstd::variant::variant(_In_ unsigned long long nSrc)
{
    vt = VT_UI8;
    ullVal = nSrc;
}


winstd::variant::variant(_In_ CY cySrc)
{
    vt = VT_CY;
    cyVal.Hi = cySrc.Hi;
    cyVal.Lo = cySrc.Lo;
}


winstd::variant::variant(_In_z_ LPCOLESTR lpszSrc)
{
    vt = VT_EMPTY;
    *this = lpszSrc;
}


winstd::variant::variant(_In_z_ BSTR bstr)
{
    vt = VT_EMPTY;
    *this = bstr;
}


winstd::variant::variant(_In_opt_ IDispatch* pSrc)
{
    vt = VT_DISPATCH;
    pdispVal = pSrc;

    if (pdispVal != NULL)
        pdispVal->AddRef();
}


winstd::variant::variant(_In_opt_ IUnknown* pSrc)
{
    vt = VT_UNKNOWN;
    punkVal = pSrc;

    if (punkVal != NULL)
        punkVal->AddRef();
}


winstd::variant::variant(_In_ const SAFEARRAY *pSrc)
{
    assert(pSrc != NULL);

    LPSAFEARRAY pCopy;
    HRESULT hRes = SafeArrayCopy((LPSAFEARRAY)pSrc, &pCopy);
    if (SUCCEEDED(hRes)) {
        SafeArrayGetVartype((LPSAFEARRAY)pSrc, &vt);
        vt |= VT_ARRAY;
        parray = pCopy;
    } else
        assert(0);
}


winstd::variant::~variant()
{
    VariantClear(this);
}
