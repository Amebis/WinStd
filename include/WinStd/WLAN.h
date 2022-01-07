/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
*/

///
/// \defgroup WinStdWLANAPI WLAN API
/// Integrates WinStd classes with Microsoft WLAN API
///

#include "Common.h"

#include <wlanapi.h>

#include <string>

// Must not statically link to Wlanapi.dll as it is not available on Windows
// without a WLAN interface.
extern DWORD (WINAPI *pfnWlanReasonCodeToString)(__in DWORD dwReasonCode, __in DWORD dwBufferSize, __in_ecount(dwBufferSize) PWCHAR pStringBuffer, __reserved PVOID pReserved);

namespace winstd {
    template <class _Ty> struct WlanFreeMemory_delete;
    template <class _Ty> struct WlanFreeMemory_delete<_Ty[]>;
    class wlan_handle;
}

/// \addtogroup WinStdWLANAPI
/// @{

///
/// Retrieves a string that describes a specified reason code and stores it in a std::wstring string.
///
/// \sa [WlanReasonCodeToString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms706768.aspx)
///
/// \note
/// Since Wlanapi.dll is not always present, the `pfnWlanReasonCodeToString` pointer to `WlanReasonCodeToString()`
/// function must be loaded dynamically.
///
template<class _Elem, class _Traits, class _Ax> inline DWORD WlanReasonCodeToString(_In_ DWORD dwReasonCode, _Inout_ std::basic_string<_Elem, _Traits, _Ax> &sValue, __reserved PVOID pReserved);

/// @}

#pragma once


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
    class wlan_handle : public handle<HANDLE, NULL>
    {
        HANDLE_IMPL(wlan_handle, NULL)

    public:
        ///
        /// Closes a connection to the server.
        ///
        /// \sa [WlanCloseHandle function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms706610(v=vs.85).aspx)
        ///
        virtual ~wlan_handle();

        ///
        /// Opens a connection to the server.
        ///
        /// \sa [WlanOpenHandle function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms706759.aspx)
        ///
        /// \return
        /// - \c true when succeeds;
        /// - \c false when fails. Use `GetLastError()` for failure reason.
        ///
        inline bool open(_In_ DWORD dwClientVersion, _Out_ PDWORD pdwNegotiatedVersion) noexcept
        {
            handle_type h;
            const DWORD dwResult = WlanOpenHandle(dwClientVersion, 0, pdwNegotiatedVersion, &h);
            if (dwResult == ERROR_SUCCESS) {
                attach(h);
                return true;
            } else {
                SetLastError(dwResult);
                return false;
            }
        }

    protected:
        ///
        /// Closes a connection to the server.
        ///
        /// \sa [WlanCloseHandle function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms706610(v=vs.85).aspx)
        ///
        void free_internal() noexcept override;
    };

    /// @}
}


template<class _Elem, class _Traits, class _Ax>
inline DWORD WlanReasonCodeToString(_In_ DWORD dwReasonCode, _Inout_ std::basic_string<_Elem, _Traits, _Ax> &sValue, __reserved PVOID pReserved)
{
    DWORD dwSize = 0;

    if (!::pfnWlanReasonCodeToString)
        return ERROR_CALL_NOT_IMPLEMENTED;

    for (;;) {
        // Increment size and allocate buffer.
        dwSize += 1024;
        std::unique_ptr<_Elem[]> szBuffer(new _Elem[dwSize]);

        // Try!
        DWORD dwResult = ::pfnWlanReasonCodeToString(dwReasonCode, dwSize, szBuffer.get(), pReserved);
        if (dwResult == ERROR_SUCCESS) {
            DWORD dwLength = (DWORD)wcsnlen(szBuffer.get(), dwSize);
            if (dwLength < dwSize - 1) {
                // Buffer was long enough.
                sValue.assign(szBuffer.get(), dwLength);
                return ERROR_SUCCESS;
            }
        } else {
            // Return error code.
            return dwResult;
        }
    }
}
