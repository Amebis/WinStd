/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
*/

 ///
 /// \defgroup SetupAPI Setup API
 /// Integrates WinStd classes with Microsoft Setup API
 ///

#include "Common.h"

namespace winstd
{
    class setup_device_info_list;
    class setup_driver_info_list_builder;
}


#pragma once

#include <SetupAPI.h>


namespace winstd
{
    /// \addtogroup SetupAPI
    /// @{

    ///
    /// HDEVINFO wrapper class
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
                SetupDiDestroyDeviceInfoList(m_h);
        }

        ///
        /// Creates an empty device information set and optionally associates the set with a device setup class and a top-level window.
        ///
        /// \return
        /// - true when creation succeeds;
        /// - false when creation fails. For extended error information, call `GetLastError()`.
        ///
        /// \sa [SetupDiCreateDeviceInfoList function](https://docs.microsoft.com/en-us/windows/desktop/api/setupapi/nf-setupapi-setupdicreatedeviceinfolist)
        ///
        inline bool create(
            _In_opt_ const GUID * ClassGuid,
            _In_opt_ HWND         hwndParent) noexcept
        {
            handle_type h = SetupDiCreateDeviceInfoList(ClassGuid, hwndParent);
            if (h != invalid) {
                attach(h);
                return true;
            }
            else
                return false;
        }

        ///
        /// Creates a device information set that contains requested device information elements for a local or a remote computer.
        ///
        /// \return
        /// - true when creation succeeds;
        /// - false when creation fails. For extended error information, call `GetLastError()`.
        ///
        /// \sa [SetupDiGetClassDevsExW function](https://docs.microsoft.com/en-us/windows/desktop/api/setupapi/nf-setupapi-setupdigetclassdevsexw)
        ///
        inline bool create(
            _In_opt_   const GUID * ClassGuid,
            _In_opt_   PCTSTR       Enumerator,
            _In_opt_   HWND         hwndParent,
            _In_       DWORD        Flags,
            _In_opt_   HDEVINFO     DeviceInfoSet,
            _In_opt_   PCTSTR       MachineName,
            _Reserved_ PVOID        Reserved) noexcept
        {
            handle_type h = SetupDiGetClassDevsEx(ClassGuid, Enumerator, hwndParent, Flags, DeviceInfoSet, MachineName, Reserved);
            if (h != invalid) {
                attach(h);
                return true;
            }
            else
                return false;
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
        inline setup_driver_info_list_builder(
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
        inline BOOL status() const noexcept
        {
            return m_result;
        }

    protected:
        HDEVINFO         m_DeviceInfoSet;
        PSP_DEVINFO_DATA m_DeviceInfoData;
        DWORD            m_DriverType;
        BOOL             m_result;          ///< Did building succeed?
    };

    /// @}
}
