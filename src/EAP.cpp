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
// winstd::eap_attr
//////////////////////////////////////////////////////////////////////

winstd::eap_attr::~eap_attr()
{
    if (pValue)
        delete []pValue;
}


//////////////////////////////////////////////////////////////////////
// winstd::eap_packet
//////////////////////////////////////////////////////////////////////

winstd::eap_packet::~eap_packet()
{
    if (m_h)
        HeapFree(GetProcessHeap(), 0, m_h);
}


void winstd::eap_packet::free_internal()
{
    HeapFree(GetProcessHeap(), 0, m_h);
}


winstd::eap_packet::handle_type winstd::eap_packet::duplicate_internal(_In_ handle_type h) const
{
    WORD n = ntohs(*(WORD*)h->Length);
    handle_type h2 = (handle_type)HeapAlloc(GetProcessHeap(), 0, n);
    if (!h2) {
        SetLastError(ERROR_OUTOFMEMORY);
        return NULL;
    }

    memcpy(h2, h, n);

    return h2;
}
