/*
    Copyright 1991-2020 Amebis
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


void winstd::setup_device_info_list::free_internal()
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
