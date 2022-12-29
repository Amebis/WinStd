/*
    SPDX-License-Identifier: MIT
    Copyright © 1991-2023 Amebis
    Copyright © 2016 GÉANT
*/

/// \defgroup SetupAPI Setup API

#pragma once

#include "Common.h"
#include <SetupAPI.h>

namespace winstd
{
    /// \addtogroup SetupAPI
    /// @{

    ///
    /// HDEVINFO wrapper class
    ///
    /// \sa [SetupDiCreateDeviceInfoList function](https://docs.microsoft.com/en-us/windows/desktop/api/setupapi/nf-setupapi-setupdicreatedeviceinfolist)
    /// \sa [SetupDiGetClassDevsExW function](https://docs.microsoft.com/en-us/windows/desktop/api/setupapi/nf-setupapi-setupdigetclassdevsexw)
    ///
    class setup_device_info_list : public handle<HDEVINFO, INVALID_HANDLE_VALUE>
    {
        WINSTD_HANDLE_IMPL(setup_device_info_list, INVALID_HANDLE_VALUE)

    public:
        ///
        /// Frees the device information set.
        ///
        /// \sa [SetupDiDestroyDeviceInfoList function](https://docs.microsoft.com/en-us/windows/desktop/api/setupapi/nf-setupapi-setupdidestroydeviceinfolist)
        ///
        virtual ~setup_device_info_list()
        {
            if (m_h != invalid)
                free_internal();
        }

    protected:
        ///
        /// Frees the device information set.
        ///
        /// \sa [SetupDiDestroyDeviceInfoList function](https://docs.microsoft.com/en-us/windows/desktop/api/setupapi/nf-setupapi-setupdidestroydeviceinfolist)
        ///
        void free_internal() noexcept override
        {
            SetupDiDestroyDeviceInfoList(m_h);
        }
    };

    ///
    /// Builds a list of drivers in constructor and deletes it in destructor
    ///
    class setup_driver_info_list_builder
    {
        WINSTD_NONCOPYABLE(setup_driver_info_list_builder)
        WINSTD_NONMOVABLE(setup_driver_info_list_builder)

    public:
        ///
        /// Construct the builder and builds a list of drivers that is associated with a specific device or with the global class driver list for a device information set.
        ///
        /// \sa [SetupDiBuildDriverInfoList function](https://docs.microsoft.com/en-us/windows/desktop/api/setupapi/nf-setupapi-setupdibuilddriverinfolist)
        ///
        setup_driver_info_list_builder(
            _In_        HDEVINFO         DeviceInfoSet,
            _Inout_opt_ PSP_DEVINFO_DATA DeviceInfoData,
            _In_        DWORD            DriverType) noexcept :
            m_DeviceInfoSet (DeviceInfoSet),
            m_DeviceInfoData(DeviceInfoData),
            m_DriverType    (DriverType)
        {
            m_result = SetupDiBuildDriverInfoList(m_DeviceInfoSet, m_DeviceInfoData, m_DriverType);
        }

        ///
        /// Deletes a driver list and destructs the builder.
        ///
        /// \sa [SetupDiDestroyDriverInfoList function](https://docs.microsoft.com/en-us/windows/desktop/api/setupapi/nf-setupapi-setupdidestroydriverinfolist)
        ///
        virtual ~setup_driver_info_list_builder()
        {
            if (m_result)
                SetupDiDestroyDriverInfoList(m_DeviceInfoSet, m_DeviceInfoData, m_DriverType);
        }

        ///
        /// Return result of `SetupDiBuildDriverInfoList()` call.
        ///
        /// \sa [SetupDiBuildDriverInfoList function](https://docs.microsoft.com/en-us/windows/desktop/api/setupapi/nf-setupapi-setupdibuilddriverinfolist)
        ///
        BOOL status() const noexcept
        {
            return m_result;
        }

    protected:
        /// \cond internal
        HDEVINFO         m_DeviceInfoSet;
        PSP_DEVINFO_DATA m_DeviceInfoData;
        DWORD            m_DriverType;
        BOOL             m_result;
        /// \endcond
    };

    /// @}
}
