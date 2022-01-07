/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
*/

#include "PCH.h"

#pragma comment(lib, "Setupapi.lib")


//////////////////////////////////////////////////////////////////////
// winstd::setup_device_info_list
//////////////////////////////////////////////////////////////////////

winstd::setup_device_info_list::~setup_device_info_list()
{
    if (m_h != invalid)
        SetupDiDestroyDeviceInfoList(m_h);
}


void winstd::setup_device_info_list::free_internal() noexcept
{
    SetupDiDestroyDeviceInfoList(m_h);
}


//////////////////////////////////////////////////////////////////////
// winstd::setup_device_info_list
//////////////////////////////////////////////////////////////////////

winstd::setup_driver_info_list_builder::~setup_driver_info_list_builder()
{
    if (m_result)
        SetupDiDestroyDriverInfoList(m_DeviceInfoSet, m_DeviceInfoData, m_DriverType);
}
