/*
    SPDX-License-Identifier: MIT
    Copyright © 1991-2023 Amebis
    Copyright © 2016 GÉANT
*/

/// \defgroup WinStdWLANAPI WLAN API

#pragma once

#include "Common.h"
#include <wlanapi.h>
#include <string>

/// \addtogroup WinStdWLANAPI
/// @{

/// \cond internal
// Must not statically link to Wlanapi.dll as it is not available on Windows
// without a WLAN interface.
extern DWORD (WINAPI *pfnWlanReasonCodeToString)(__in DWORD dwReasonCode, __in DWORD dwBufferSize, __in_ecount(dwBufferSize) PWCHAR pStringBuffer, __reserved PVOID pReserved);
/// \endcond

///
/// Retrieves a string that describes a specified reason code and stores it in a std::wstring string.
///
/// \sa [WlanReasonCodeToString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms706768.aspx)
///
/// \note
/// Since Wlanapi.dll is not always present, the `pfnWlanReasonCodeToString` pointer to `WlanReasonCodeToString()`
/// function must be loaded dynamically.
///
template<class _Traits, class _Ax>
static DWORD WlanReasonCodeToString(_In_ DWORD dwReasonCode, _Inout_ std::basic_string<wchar_t, _Traits, _Ax> &sValue, __reserved PVOID pReserved)
{
    SIZE_T sSize = 0;

    if (!::pfnWlanReasonCodeToString)
        return ERROR_CALL_NOT_IMPLEMENTED;

    for (;;) {
        // Increment size and allocate buffer.
        sSize = SIZETAdd(sSize, 1024);
        if (sSize > DWORD_MAX)
            throw std::runtime_exception("Data too big");
        sValue.resize(sSize - 1);

        // Try!
        DWORD dwResult = ::pfnWlanReasonCodeToString(dwReasonCode, static_cast<DWORD>(sSize), &sValue[0], pReserved);
        if (dwResult == ERROR_SUCCESS) {
            SIZE_T sLength = wcsnlen(&sValue[0], sSize);
            if (sLength < sSize - 1) {
                // Buffer was long enough.
                sValue.resize(sLength);
                return ERROR_SUCCESS;
            }
        } else {
            // Return error code.
            return dwResult;
        }
    }
}

/// @}

namespace winstd
{
    /// \addtogroup WinStdWLANAPI
    /// @{

    ///
    /// Deleter for unique_ptr using WlanFreeMemory
    ///
    template <class _Ty> struct WlanFreeMemory_delete
    {
        typedef WlanFreeMemory_delete<_Ty> _Myt; ///< This type

        ///
        /// Default construct
        ///
        WlanFreeMemory_delete() {}

        ///
        /// Construct from another WlanFreeMemory_delete
        ///
        template <class _Ty2> WlanFreeMemory_delete(const WlanFreeMemory_delete<_Ty2>&) {}

        ///
        /// Delete a pointer
        ///
        void operator()(_Ty *_Ptr) const
        {
            WlanFreeMemory(_Ptr);
        }
    };

    ///
    /// Deleter for unique_ptr to array of unknown size using WlanFreeMemory
    ///
    template <class _Ty> struct WlanFreeMemory_delete<_Ty[]>
    {
        typedef WlanFreeMemory_delete<_Ty> _Myt; ///< This type

        ///
        /// Default construct
        ///
        WlanFreeMemory_delete() {}

        ///
        /// Delete a pointer
        ///
        void operator()(_Ty *_Ptr) const
        {
            WlanFreeMemory(_Ptr);
        }

        ///
        /// Delete a pointer of another type
        ///
        template<class _Other>
        void operator()(_Other *) const
        {
            WlanFreeMemory(_Ptr);
        }
    };

    ///
    /// WLAN handle wrapper
    ///
    /// \sa [WlanOpenHandle function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms706759.aspx)
    ///
    class wlan_handle : public handle<HANDLE, NULL>
    {
        WINSTD_HANDLE_IMPL(wlan_handle, HANDLE, NULL)

    public:
        ///
        /// Closes a connection to the server.
        ///
        /// \sa [WlanCloseHandle function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms706610(v=vs.85).aspx)
        ///
        virtual ~wlan_handle()
        {
            if (m_h != invalid)
                free_internal();
        }

    protected:
        ///
        /// Closes a connection to the server.
        ///
        /// \sa [WlanCloseHandle function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms706610(v=vs.85).aspx)
        ///
        void free_internal() noexcept override
        {
            WlanCloseHandle(m_h, NULL);
        }
    };

    /// @}
}

/// \addtogroup WinStdWLANAPI
/// @{

///
/// Opens a connection to the server.
///
/// \sa [WlanOpenHandle function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms706759.aspx)
///
#pragma warning(suppress: 4505) // Don't warn on unused code
static DWORD WlanOpenHandle(
    _In_ DWORD dwClientVersion,
    _Reserved_ PVOID pReserved,
    _Out_ PDWORD pdwNegotiatedVersion,
    _Inout_ winstd::wlan_handle &handle)
{
    HANDLE h;
    DWORD dwResult = WlanOpenHandle(dwClientVersion, pReserved, pdwNegotiatedVersion, &h);
    if (dwResult == ERROR_SUCCESS)
        handle.attach(h);
    return dwResult;
}

/// @}
