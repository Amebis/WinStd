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

#include "Common.h"

#include <wlanapi.h>

#include <string>

// Must not statically link to Wlanapi.dll as it is not available on Windows
// without a WLAN interface.
extern DWORD (WINAPI *pfnWlanReasonCodeToString)(__in DWORD dwReasonCode, __in DWORD dwBufferSize, __in_ecount(dwBufferSize) PWCHAR pStringBuffer, __reserved PVOID pReserved);

///
/// \defgroup WinStdWLANAPI WLAN API
/// Integrates WinStd classes with Microsoft WLAN API
///
/// @{

///
/// Retrieves a string that describes a specified reason code and stores it in a std::wstring string.
///
/// \sa [WlanReasonCodeToString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms706768.aspx)
///
/// \note
/// Since Wlanapi.dll is not always present, the \c pfnWlanReasonCodeToString pointer to \c WlanReasonCodeToString
/// function must be loaded dynamically.
///
template<class _Elem, class _Traits, class _Ax> inline DWORD WlanReasonCodeToString(_In_ DWORD dwReasonCode, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue, __reserved PVOID pReserved);

/// @}

#pragma once


template<class _Elem, class _Traits, class _Ax>
inline DWORD WlanReasonCodeToString(_In_ DWORD dwReasonCode, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue, __reserved PVOID pReserved)
{
    assert(0); // TODO: Test this code.

    DWORD dwSize = 0;

    if (!::pfnWlanReasonCodeToString)
        return ERROR_CALL_NOT_IMPLEMENTED;

    for (;;) {
        // Increment size and allocate buffer.
        auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[dwSize += 1024]);

        // Try!
        DWORD dwResult = ::pfnWlanReasonCodeToString(dwReasonCode, dwSize, szBuffer.get(), pReserved);
        if (dwResult == ERROR_SUCCESS) {
            DWORD dwLength = (DWORD)wcsnlen(szBuffer.get(), dwSize);
            if (dwLength < dwSize) {
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
