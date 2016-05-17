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

#include <Security.h>

#include <string>

template<class _Elem, class _Traits, class _Ax> BOOLEAN GetUserNameExA(_In_ EXTENDED_NAME_FORMAT NameFormat, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sName);
template<class _Elem, class _Traits, class _Ax> BOOLEAN GetUserNameExW(_In_ EXTENDED_NAME_FORMAT NameFormat, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sName);

#pragma once


///
/// \defgroup WinStdSecurityAPI Security API
/// Integrates WinStd classes with Microsoft Security API
///
/// @{

///
/// Retrieves the name of the user or other security principal associated with the calling thread and stores it in a std::string string.
///
/// \sa [GetUserNameEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724435.aspx)
///
template<class _Elem, class _Traits, class _Ax>
BOOLEAN GetUserNameExA(_In_ EXTENDED_NAME_FORMAT NameFormat, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sName)
{
    assert(0); // TODO: Test this code.

    _Elem szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];
    ULONG ulSize = _countof(szStackBuffer);

    // Try with stack buffer first.
    if (::GetUserNameExA(NameFormat, szStackBuffer, &ulSize)) {
        // Copy from stack.
        sName.assign(szStackBuffer, ulSize);
        return TRUE;
    } else {
        if (::GetLastError() == ERROR_MORE_DATA) {
            // Allocate buffer on heap and retry.
            auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[ulSize]);
            if (::GetUserNameExA(NameFormat, szBuffer.get(), &ulSize)) {
                sName.assign(szBuffer.get(), ulSize);
                return TRUE;
            }
        }
    }

    sName.clear();
    return FALSE;
}


///
/// Retrieves the name of the user or other security principal associated with the calling thread and stores it in a std::wstring string.
///
/// \sa [GetUserNameEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724435.aspx)
///
template<class _Elem, class _Traits, class _Ax>
BOOLEAN GetUserNameExW(_In_ EXTENDED_NAME_FORMAT NameFormat, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sName)
{
    assert(0); // TODO: Test this code.

    _Elem szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];
    ULONG ulSize = _countof(szStackBuffer);

    // Try with stack buffer first.
    if (::GetUserNameExW(NameFormat, szStackBuffer, &ulSize)) {
        // Copy from stack.
        sName.assign(szStackBuffer, ulSize);
        return TRUE;
    } else {
        if (::GetLastError() == ERROR_MORE_DATA) {
            // Allocate buffer on heap and retry.
            auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[ulSize]);
            if (::GetUserNameExW(NameFormat, szBuffer.get(), &ulSize)) {
                sName.assign(szBuffer.get(), ulSize);
                return TRUE;
            }
        }
    }

    sName.clear();
    return FALSE;
}

/// @}
