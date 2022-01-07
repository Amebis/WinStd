/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
*/

#include "PCH.h"

#pragma comment(lib, "Eappcfg.lib")


//////////////////////////////////////////////////////////////////////
// winstd::eap_attr
//////////////////////////////////////////////////////////////////////

#pragma warning(suppress: 26432) // Copy constructor and assignment operator are also present, but not detected by code analysis as they are using base type source object reference.
winstd::eap_attr::~eap_attr()
{
    if (pValue)
        delete [] pValue;
}


void winstd::eap_attr::create_ms_mppe_key(_In_ BYTE bVendorType, _In_count_(nKeySize) LPCBYTE pbKey, _In_ BYTE nKeySize)
{
    const BYTE nPaddingLength = static_cast<BYTE>((16 - (1 + static_cast<DWORD>(nKeySize))) % 16);
    const DWORD dwLengthNew =
        4              + // Vendor-Id
        1              + // Vendor type
        1              + // Vendor length
        2              + // Salt
        1              + // Key-Length
        nKeySize       + // Key
        nPaddingLength;  // Padding

#pragma warning(push)
#pragma warning(disable: 6386)
    LPBYTE p = new BYTE[dwLengthNew];
    p[0] = 0x00;                                    // Vendor-Id (0x137 = 311 = Microsoft)
    p[1] = 0x00;                                    // --|
    p[2] = 0x01;                                    // --|
    p[3] = 0x37;                                    // --^
    p[4] = bVendorType;                             // Vendor type
    p[5] = static_cast<BYTE>(dwLengthNew - 4);      // Vendor length
    p[6] = 0x00;                                    // Salt
    p[7] = 0x00;                                    // --^
    p[8] = nKeySize;                                // Key-Length
#pragma warning(pop)
    memcpy(p + 9, pbKey, nKeySize);                 // Key
    memset(p + 9 + nKeySize, 0, nPaddingLength);    // Padding

    if (pValue)
        delete [] pValue;

    #pragma warning(suppress: 26812) // EAP_ATTRIBUTE_TYPE is unscoped.
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
    if (m_h != invalid)
        HeapFree(GetProcessHeap(), 0, m_h);
}


void winstd::eap_packet::free_internal() noexcept
{
    HeapFree(GetProcessHeap(), 0, m_h);
}


winstd::eap_packet::handle_type winstd::eap_packet::duplicate_internal(_In_ handle_type h) const noexcept
{
    const WORD n = ntohs(*reinterpret_cast<WORD*>(h->Length));
    handle_type h2 = static_cast<handle_type>(HeapAlloc(GetProcessHeap(), 0, n));
    if (h2 == invalid) {
        SetLastError(ERROR_OUTOFMEMORY);
        return invalid;
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


/// \cond internal

void winstd::eap_method_info_array::free_internal() noexcept
{
    for (DWORD i = 0; i < dwNumberOfMethods; i++)
        free_internal(pEapMethods + i);

    EapHostPeerFreeMemory(reinterpret_cast<BYTE*>(pEapMethods));
}


void winstd::eap_method_info_array::free_internal(_In_ EAP_METHOD_INFO *pMethodInfo) noexcept
{
    if (pMethodInfo->pInnerMethodInfo)
        free_internal(pMethodInfo->pInnerMethodInfo);

    EapHostPeerFreeMemory(reinterpret_cast<BYTE*>(pMethodInfo->pwszAuthorName));
    EapHostPeerFreeMemory(reinterpret_cast<BYTE*>(pMethodInfo->pwszFriendlyName));
}

/// \endcond
