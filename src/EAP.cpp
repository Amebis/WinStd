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

#pragma comment(lib, "Eappcfg.lib")


//////////////////////////////////////////////////////////////////////
// winstd::eap_attr
//////////////////////////////////////////////////////////////////////

winstd::eap_attr::~eap_attr()
{
    if (pValue)
        delete []pValue;
}


void winstd::eap_attr::create_ms_mppe_key(_In_ BYTE bVendorType, _In_count_(nKeySize) LPCBYTE pbKey, _In_ BYTE nKeySize)
{
    BYTE nPaddingLength = (BYTE)((16 - (1 + (DWORD)nKeySize)) % 16);
    DWORD dwLengthNew =
        4              + // Vendor-Id
        1              + // Vendor type
        1              + // Vendor length
        2              + // Salt
        1              + // Key-Length
        nKeySize       + // Key
        nPaddingLength;  // Padding

    LPBYTE p = new BYTE[dwLengthNew];
    p[0] = 0x00;                                    // Vendor-Id (0x137 = 311 = Microsoft)
    p[1] = 0x00;                                    // --|
    p[2] = 0x01;                                    // --|
    p[3] = 0x37;                                    // --^
    p[4] = bVendorType;                             // Vendor type
    p[5] = (BYTE)(dwLengthNew - 4);                 // Vendor length
    p[6] = 0x00;                                    // Salt
    p[7] = 0x00;                                    // --^
    p[8] = nKeySize;                                // Key-Length
    memcpy(p + 9, pbKey, nKeySize);                 // Key
    memset(p + 9 + nKeySize, 0, nPaddingLength);    // Padding

    if (pValue)
        delete [] pValue;

    eaType   = eatVendorSpecific;
    dwLength = dwLengthNew;
    pValue   = p;
}


const EAP_ATTRIBUTE winstd::eap_attr::blank = {};


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


//////////////////////////////////////////////////////////////////////
// winstd::eap_method_info_array
//////////////////////////////////////////////////////////////////////

winstd::eap_method_info_array::~eap_method_info_array()
{
    if (pEapMethods)
        free_internal();
}


void winstd::eap_method_info_array::free_internal()
{
    for (DWORD i = 0; i < dwNumberOfMethods; i++)
        free_internal(pEapMethods + i);

    EapHostPeerFreeMemory((BYTE*)pEapMethods);
}


void winstd::eap_method_info_array::free_internal(_In_ EAP_METHOD_INFO *pMethodInfo)
{
    if (pMethodInfo->pInnerMethodInfo)
        free_internal(pMethodInfo->pInnerMethodInfo);

    EapHostPeerFreeMemory((BYTE*)pMethodInfo->pwszAuthorName);
    EapHostPeerFreeMemory((BYTE*)pMethodInfo->pwszFriendlyName);
}
