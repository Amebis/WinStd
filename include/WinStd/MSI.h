/*
    SPDX-License-Identifier: MIT
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
*/

/// \defgroup WinStdMSIAPI Microsoft Installer API

#pragma once

#include "Common.h"
#include <MsiQuery.h>
#include <string>
#include <vector>

/// \addtogroup WinStdMSIAPI
/// @{

/// @copydoc MsiGetPropertyW()
template<class _Traits, class _Ax>
static UINT MsiGetPropertyA(_In_ MSIHANDLE hInstall, _In_z_ LPCSTR szName, _Inout_ std::basic_string<char, _Traits, _Ax> &sValue)
{
    assert(0); // TODO: Test this code.

    char szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(char)];
    DWORD dwSize = _countof(szStackBuffer);
    UINT uiResult;

    // Try with stack buffer first.
    uiResult = ::MsiGetPropertyA(hInstall, szName, szStackBuffer, &dwSize);
    if (uiResult == ERROR_SUCCESS) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return ERROR_SUCCESS;
    } else if (uiResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap to read the string data into and read it.
        std::unique_ptr<char[]> szBuffer(new char[++dwSize]);
        uiResult = ::MsiGetPropertyA(hInstall, szName, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), uiResult == ERROR_SUCCESS ? dwSize : 0);
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
template<class _Traits, class _Ax>
static UINT MsiGetPropertyW(_In_ MSIHANDLE hInstall, _In_z_ LPCWSTR szName, _Inout_ std::basic_string<wchar_t, _Traits, _Ax> &sValue)
{
    wchar_t szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(wchar_t)];
    DWORD dwSize = _countof(szStackBuffer);
    UINT uiResult;

    // Try with stack buffer first.
    uiResult = ::MsiGetPropertyW(hInstall, szName, szStackBuffer, &dwSize);
    if (uiResult == ERROR_SUCCESS) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return ERROR_SUCCESS;
    } else if (uiResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap to read the string data into and read it.
        std::unique_ptr<wchar_t[]> szBuffer(new wchar_t[++dwSize]);
        uiResult = ::MsiGetPropertyW(hInstall, szName, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), uiResult == ERROR_SUCCESS ? dwSize : 0);
        return uiResult;
    } else {
        // Return error code.
        return uiResult;
    }
}

/// @copydoc MsiRecordGetStringW()
template<class _Traits, class _Ax>
static UINT MsiRecordGetStringA(_In_ MSIHANDLE hRecord, _In_ unsigned int iField, _Inout_ std::basic_string<char, _Traits, _Ax> &sValue)
{
    assert(0); // TODO: Test this code.

    char szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(char)];
    DWORD dwSize = _countof(szStackBuffer);
    UINT uiResult;

    // Try with stack buffer first.
    uiResult = ::MsiRecordGetStringA(hRecord, iField, szStackBuffer, &dwSize);
    if (uiResult == ERROR_SUCCESS) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return ERROR_SUCCESS;
    } else if (uiResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap to read the string data into and read it.
        std::unique_ptr<char[]> szBuffer(new char[++dwSize]);
        uiResult = ::MsiRecordGetStringA(hRecord, iField, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), uiResult == ERROR_SUCCESS ? dwSize : 0);
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
template<class _Traits, class _Ax>
static UINT MsiRecordGetStringW(_In_ MSIHANDLE hRecord, _In_ unsigned int iField, _Inout_ std::basic_string<wchar_t, _Traits, _Ax> &sValue)
{
    wchar_t szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(wchar_t)];
    DWORD dwSize = _countof(szStackBuffer);
    UINT uiResult;

    // Try with stack buffer first.
    uiResult = ::MsiRecordGetStringW(hRecord, iField, szStackBuffer, &dwSize);
    if (uiResult == ERROR_SUCCESS) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return ERROR_SUCCESS;
    } else if (uiResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap to read the string data into and read it.
        std::unique_ptr<wchar_t[]> szBuffer(new wchar_t[++dwSize]);
        uiResult = ::MsiRecordGetStringW(hRecord, iField, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), uiResult == ERROR_SUCCESS ? dwSize : 0);
        return uiResult;
    } else {
        // Return error code.
        return uiResult;
    }
}

/// @copydoc MsiFormatRecordW()
template<class _Traits, class _Ax>
static UINT MsiFormatRecordA(_In_ MSIHANDLE hInstall, _In_ MSIHANDLE hRecord, _Inout_ std::basic_string<char, _Traits, _Ax> &sValue)
{
    assert(0); // TODO: Test this code.

    char szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(char)];
    DWORD dwSize = _countof(szStackBuffer);
    UINT uiResult;

    // Try with stack buffer first.
    uiResult = ::MsiFormatRecordA(hInstall, hRecord, szStackBuffer, &dwSize);
    if (uiResult == ERROR_SUCCESS) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return ERROR_SUCCESS;
    } else if (uiResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap to format the string data into and read it.
        std::unique_ptr<char[]> szBuffer(new char[++dwSize]);
        uiResult = ::MsiFormatRecordA(hInstall, hRecord, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), uiResult == ERROR_SUCCESS ? dwSize : 0);
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
template<class _Traits, class _Ax>
static UINT MsiFormatRecordW(_In_ MSIHANDLE hInstall, _In_ MSIHANDLE hRecord, _Inout_ std::basic_string<wchar_t, _Traits, _Ax> &sValue)
{
    wchar_t szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(wchar_t)];
    DWORD dwSize = _countof(szStackBuffer);
    UINT uiResult;

    // Try with stack buffer first.
    uiResult = ::MsiFormatRecordW(hInstall, hRecord, szStackBuffer, &dwSize);
    if (uiResult == ERROR_SUCCESS) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return ERROR_SUCCESS;
    } else if (uiResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap to format the string data into and read it.
        std::unique_ptr<wchar_t[]> szBuffer(new wchar_t[++dwSize]);
        uiResult = ::MsiFormatRecordW(hInstall, hRecord, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), uiResult == ERROR_SUCCESS ? dwSize : 0);
        return uiResult;
    } else {
        // Return error code.
        return uiResult;
    }
}

///
/// Reads bytes from a record stream field into a std::vector buffer.
///
/// \sa [MsiRecordReadStream function](https://msdn.microsoft.com/en-us/library/aa370370.aspx)
///
template<class _Ty, class _Ax>
static UINT MsiRecordReadStream(_In_ MSIHANDLE hRecord, _In_ unsigned int iField, _Inout_ std::vector<_Ty, _Ax> &binData)
{
    assert(0); // TODO: Test this code.

    DWORD dwSize = 0;
    UINT uiResult;

    // Query the actual data length first.
    uiResult = ::MsiRecordReadStream(hRecord, iField, NULL, &dwSize);
    if (uiResult == ERROR_SUCCESS) {
        binData.resize((dwSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        return ::MsiRecordReadStream(hRecord, iField, reinterpret_cast<char*>(binData.data()), &dwSize);
    } else {
        // Return error code.
        return uiResult;
    }
}

/// @copydoc MsiGetTargetPathW()
template<class _Traits, class _Ax>
static UINT MsiGetTargetPathA(_In_ MSIHANDLE hInstall, _In_z_ LPCSTR szFolder, _Out_ std::basic_string<char, _Traits, _Ax> &sValue)
{
    assert(0); // TODO: Test this code.

    char szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(char)];
    DWORD dwSize = _countof(szStackBuffer);
    UINT uiResult;

    // Try with stack buffer first.
    uiResult = ::MsiGetTargetPathA(hInstall, szFolder, szStackBuffer, &dwSize);
    if (uiResult == ERROR_SUCCESS) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return ERROR_SUCCESS;
    } else if (uiResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap to format the string data into and read it.
        std::unique_ptr<char[]> szBuffer(new char[++dwSize]);
        uiResult = ::MsiGetTargetPathA(hInstall, szFolder, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), uiResult == ERROR_SUCCESS ? dwSize : 0);
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
template<class _Traits, class _Ax>
static UINT MsiGetTargetPathW(_In_ MSIHANDLE hInstall, _In_z_ LPCWSTR szFolder, _Inout_ std::basic_string<wchar_t, _Traits, _Ax> &sValue)
{
    wchar_t szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(wchar_t)];
    DWORD dwSize = _countof(szStackBuffer);
    UINT uiResult;

    // Try with stack buffer first.
    uiResult = ::MsiGetTargetPathW(hInstall, szFolder, szStackBuffer, &dwSize);
    if (uiResult == ERROR_SUCCESS) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return ERROR_SUCCESS;
    } else if (uiResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap to format the string data into and read it.
        std::unique_ptr<wchar_t[]> szBuffer(new wchar_t[++dwSize]);
        uiResult = ::MsiGetTargetPathW(hInstall, szFolder, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), uiResult == ERROR_SUCCESS ? dwSize : 0);
        return uiResult;
    } else {
        // Return error code.
        return uiResult;
    }
}

/// @copydoc MsiGetComponentPathW()
template<class _Traits, class _Ax>
static INSTALLSTATE MsiGetComponentPathA(_In_z_ LPCSTR szProduct, _In_z_ LPCSTR szComponent, _Inout_ std::basic_string<char, _Traits, _Ax> &sValue)
{
    char szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(char)];
    DWORD dwSize = _countof(szStackBuffer);
    INSTALLSTATE state;

    // Try with stack buffer first.
    state = ::MsiGetComponentPathA(szProduct, szComponent, szStackBuffer, &dwSize);
    if (state >= INSTALLSTATE_BROKEN) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return state;
    } else if (state == INSTALLSTATE_MOREDATA) {
        // Allocate buffer on heap to format the string data into and read it.
        std::unique_ptr<char[]> szBuffer(new char[++dwSize]);
        state = ::MsiGetComponentPathA(szProduct, szComponent, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), state >= INSTALLSTATE_BROKEN ? dwSize : 0);
        return state;
    } else {
        // Return error code.
        return state;
    }
}

///
/// Returns the full path to an installed component. If the key path for the component is a registry key then the registry key is returned.
///
/// \sa [MsiGetComponentPath function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa370112.aspx)
///
template<class _Traits, class _Ax>
static INSTALLSTATE MsiGetComponentPathW(_In_z_ LPCWSTR szProduct, _In_z_ LPCWSTR szComponent, _Inout_ std::basic_string<wchar_t, _Traits, _Ax> &sValue)
{
    wchar_t szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(wchar_t)];
    DWORD dwSize = _countof(szStackBuffer);
    INSTALLSTATE state;

    // Try with stack buffer first.
    state = ::MsiGetComponentPathW(szProduct, szComponent, szStackBuffer, &dwSize);
    if (state >= INSTALLSTATE_BROKEN) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwSize);
        return state;
    } else if (state == INSTALLSTATE_MOREDATA) {
        // Allocate buffer on heap to format the string data into and read it.
        std::unique_ptr<wchar_t[]> szBuffer(new wchar_t[++dwSize]);
        state = ::MsiGetComponentPathW(szProduct, szComponent, szBuffer.get(), &dwSize);
        sValue.assign(szBuffer.get(), state >= INSTALLSTATE_BROKEN ? dwSize : 0);
        return state;
    } else {
        // Return error code.
        return state;
    }
}

/// @}
