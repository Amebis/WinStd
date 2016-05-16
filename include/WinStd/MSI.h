/*
    Copyright 1991-2016 Amebis

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

#pragma once

#include "Common.h"

#include <MsiQuery.h>

#include <string>
#include <vector>


///
/// \defgroup WinStdMSIAPI Microsoft Installer API
/// Integrates WinStd classes with Microsoft Installer API
///
/// @{

///
/// Gets the value for an installer property and stores it in a std::string string.
///
/// \sa [MsiGetProperty function](https://msdn.microsoft.com/en-us/library/aa370134.aspx)
///
inline UINT MsiGetPropertyA(_In_ MSIHANDLE hInstall, _In_ LPCSTR szName, _Out_ std::string &sValue)
{
    CHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(CHAR)];
    DWORD dwSize = _countof(szStackBuffer);
    UINT uiResult;

    // Try with stack buffer first.
    uiResult = ::MsiGetPropertyA(hInstall, szName, szStackBuffer, &dwSize);
    if (uiResult == NO_ERROR) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return NO_ERROR;
    } else if (uiResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap to read the string data into and read it.
        auto szBuffer = std::unique_ptr<CHAR[]>(new CHAR[++dwSize]);
        uiResult = ::MsiGetPropertyA(hInstall, szName, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), uiResult == NO_ERROR ? dwSize : 0);
        return uiResult;
    } else {
        // Return error code.
        return uiResult;
    }
}


///
/// Gets the value for an installer property and stores it in a std::wstring string.
///
/// \sa [MsiGetProperty function](https://msdn.microsoft.com/en-us/library/aa370134.aspx)
///
inline UINT MsiGetPropertyW(_In_ MSIHANDLE hInstall, _In_ LPCWSTR szName, _Out_ std::wstring &sValue)
{
    WCHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(WCHAR)];
    DWORD dwSize = _countof(szStackBuffer);
    UINT uiResult;

    // Try with stack buffer first.
    uiResult = ::MsiGetPropertyW(hInstall, szName, szStackBuffer, &dwSize);
    if (uiResult == NO_ERROR) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return NO_ERROR;
    } else if (uiResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap to read the string data into and read it.
        auto szBuffer = std::unique_ptr<WCHAR[]>(new WCHAR[++dwSize]);
        uiResult = ::MsiGetPropertyW(hInstall, szName, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), uiResult == NO_ERROR ? dwSize : 0);
        return uiResult;
    } else {
        // Return error code.
        return uiResult;
    }
}


///
/// Returns the string value of a record field and stores it in a std::string string.
///
/// \sa [MsiRecordGetString function](https://msdn.microsoft.com/en-us/library/aa370368.aspx)
///
inline UINT MsiRecordGetStringA(_In_ MSIHANDLE hRecord, _In_ unsigned int iField, _Out_ std::string &sValue)
{
    CHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(CHAR)];
    DWORD dwSize = _countof(szStackBuffer);
    UINT uiResult;

    // Try with stack buffer first.
    uiResult = ::MsiRecordGetStringA(hRecord, iField, szStackBuffer, &dwSize);
    if (uiResult == NO_ERROR) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return NO_ERROR;
    } else if (uiResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap to read the string data into and read it.
        auto szBuffer = std::unique_ptr<CHAR[]>(new CHAR[++dwSize]);
        uiResult = ::MsiRecordGetStringA(hRecord, iField, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), uiResult == NO_ERROR ? dwSize : 0);
        return uiResult;
    } else {
        // Return error code.
        return uiResult;
    }
}


///
/// Returns the string value of a record field and stores it in a std::wstring string.
///
/// \sa [MsiRecordGetString function](https://msdn.microsoft.com/en-us/library/aa370368.aspx)
///
inline UINT MsiRecordGetStringW(_In_ MSIHANDLE hRecord, _In_ unsigned int iField, _Out_ std::wstring &sValue)
{
    WCHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(WCHAR)];
    DWORD dwSize = _countof(szStackBuffer);
    UINT uiResult;

    // Try with stack buffer first.
    uiResult = ::MsiRecordGetStringW(hRecord, iField, szStackBuffer, &dwSize);
    if (uiResult == NO_ERROR) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return NO_ERROR;
    } else if (uiResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap to read the string data into and read it.
        auto szBuffer = std::unique_ptr<WCHAR[]>(new WCHAR[++dwSize]);
        uiResult = ::MsiRecordGetStringW(hRecord, iField, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), uiResult == NO_ERROR ? dwSize : 0);
        return uiResult;
    } else {
        // Return error code.
        return uiResult;
    }
}


///
/// Formats record field data and properties using a format string and stores it in a std::string string.
///
/// \sa [MsiFormatRecord function](https://msdn.microsoft.com/en-us/library/aa370109.aspx)
///
inline UINT MsiFormatRecordA(MSIHANDLE hInstall, MSIHANDLE hRecord, std::string &sValue)
{
    CHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(CHAR)];
    DWORD dwSize = _countof(szStackBuffer);
    UINT uiResult;

    // Try with stack buffer first.
    uiResult = ::MsiFormatRecordA(hInstall, hRecord, szStackBuffer, &dwSize);
    if (uiResult == NO_ERROR) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return NO_ERROR;
    } else if (uiResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap to format the string data into and read it.
        auto szBuffer = std::unique_ptr<CHAR[]>(new CHAR[++dwSize]);
        uiResult = ::MsiFormatRecordA(hInstall, hRecord, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), uiResult == NO_ERROR ? dwSize : 0);
        return uiResult;
    } else {
        // Return error code.
        return uiResult;
    }
}


///
/// Formats record field data and properties using a format string and stores it in a std::wstring string.
///
/// \sa [MsiFormatRecord function](https://msdn.microsoft.com/en-us/library/aa370109.aspx)
///
inline UINT MsiFormatRecordW(MSIHANDLE hInstall, MSIHANDLE hRecord, std::wstring &sValue)
{
    WCHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(WCHAR)];
    DWORD dwSize = _countof(szStackBuffer);
    UINT uiResult;

    // Try with stack buffer first.
    uiResult = ::MsiFormatRecordW(hInstall, hRecord, szStackBuffer, &dwSize);
    if (uiResult == NO_ERROR) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return NO_ERROR;
    } else if (uiResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap to format the string data into and read it.
        auto szBuffer = std::unique_ptr<WCHAR[]>(new WCHAR[++dwSize]);
        uiResult = ::MsiFormatRecordW(hInstall, hRecord, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), uiResult == NO_ERROR ? dwSize : 0);
        return uiResult;
    } else {
        // Return error code.
        return uiResult;
    }
}


///
/// Reads bytes from a record stream field into a std::vector<BYTE> buffer.
///
/// \sa [MsiRecordReadStream function](https://msdn.microsoft.com/en-us/library/aa370370.aspx)
///
inline UINT MsiRecordReadStream(_In_ MSIHANDLE hRecord, _In_ unsigned int iField, _Out_ std::vector<BYTE> &binData)
{
    DWORD dwSize = 0;
    UINT uiResult;

    // Query the actual data length first.
    uiResult = ::MsiRecordReadStream(hRecord, iField, NULL, &dwSize);
    if (uiResult == NO_ERROR) {
        binData.resize(dwSize);
        return ::MsiRecordReadStream(hRecord, iField, (char*)binData.data(), &dwSize);
    } else {
        // Return error code.
        return uiResult;
    }
}


///
/// Returns the full target path for a folder in the Directory table and stores it in a std::string string.
///
/// \sa [MsiGetTargetPath function](https://msdn.microsoft.com/en-us/library/aa370303.aspx)
///
inline UINT MsiGetTargetPathA(_In_ MSIHANDLE hInstall, _In_ LPCSTR szFolder, _Out_ std::string &sValue)
{
    CHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(CHAR)];
    DWORD dwSize = _countof(szStackBuffer);
    UINT uiResult;

    // Try with stack buffer first.
    uiResult = ::MsiGetTargetPathA(hInstall, szFolder, szStackBuffer, &dwSize);
    if (uiResult == NO_ERROR) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return NO_ERROR;
    } else if (uiResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap to format the string data into and read it.
        auto szBuffer = std::unique_ptr<CHAR[]>(new CHAR[++dwSize]);
        uiResult = ::MsiGetTargetPathA(hInstall, szFolder, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), uiResult == NO_ERROR ? dwSize : 0);
        return uiResult;
    } else {
        // Return error code.
        return uiResult;
    }
}


///
/// Returns the full target path for a folder in the Directory table and stores it in a std::wstring string.
///
/// \sa [MsiGetTargetPath function](https://msdn.microsoft.com/en-us/library/aa370303.aspx)
///
inline UINT MsiGetTargetPathW(_In_ MSIHANDLE hInstall, _In_ LPCWSTR szFolder, _Out_ std::wstring &sValue)
{
    WCHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(WCHAR)];
    DWORD dwSize = _countof(szStackBuffer);
    UINT uiResult;

    // Try with stack buffer first.
    uiResult = ::MsiGetTargetPathW(hInstall, szFolder, szStackBuffer, &dwSize);
    if (uiResult == NO_ERROR) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return NO_ERROR;
    } else if (uiResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap to format the string data into and read it.
        auto szBuffer = std::unique_ptr<WCHAR[]>(new WCHAR[++dwSize]);
        uiResult = ::MsiGetTargetPathW(hInstall, szFolder, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), uiResult == NO_ERROR ? dwSize : 0);
        return uiResult;
    } else {
        // Return error code.
        return uiResult;
    }
}

/// @}
