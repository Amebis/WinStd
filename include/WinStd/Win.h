/*
    SPDX-License-Identifier: MIT
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
*/

/// \defgroup WinStdWinAPI Windows API

#pragma once

#include "Common.h"
#include <AclAPI.h>
#include <tlhelp32.h>
#include <string>
#include <vector>

#pragma warning(push)
#pragma warning(disable: 4505) // Don't warn on unused code

/// \addtogroup WinStdWinAPI
/// @{

/// @copydoc GetModuleFileNameW()
template<class _Traits, class _Ax>
static DWORD GetModuleFileNameA(_In_opt_ HMODULE hModule, _Out_ std::basic_string<char, _Traits, _Ax> &sValue) noexcept
{
    assert(0); // TODO: Test this code.

    char szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(char)];

    // Try with stack buffer first.
    DWORD dwResult = ::GetModuleFileNameA(hModule, szStackBuffer, _countof(szStackBuffer));
    if (dwResult < _countof(szStackBuffer)) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwResult);
        return dwResult;
    } else {
        for (DWORD dwCapacity = 2*WINSTD_STACK_BUFFER_BYTES/sizeof(char);; dwCapacity *= 2) {
            // Allocate on heap and retry.
            std::unique_ptr<char[]> szBuffer(new char[dwCapacity]);
            dwResult = ::GetModuleFileNameA(hModule, szBuffer.get(), dwCapacity);
            if (dwResult < dwCapacity) {
                sValue.assign(szBuffer.get(), dwResult);
                return dwResult;
            }
        }
    }
}

///
/// Retrieves the fully qualified path for the file that contains the specified module and stores it in a std::wstring string.
///
/// \sa [GetModuleFileName function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms683197.aspx)
///
template<class _Traits, class _Ax>
static DWORD GetModuleFileNameW(_In_opt_ HMODULE hModule, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sValue) noexcept
{
    wchar_t szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(wchar_t)];

    // Try with stack buffer first.
    DWORD dwResult = ::GetModuleFileNameW(hModule, szStackBuffer, _countof(szStackBuffer));
    if (dwResult < _countof(szStackBuffer)) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwResult);
        return dwResult;
    } else {
        for (DWORD dwCapacity = 2*WINSTD_STACK_BUFFER_BYTES/sizeof(wchar_t);; dwCapacity *= 2) {
            // Allocate on heap and retry.
            std::unique_ptr<wchar_t[]> szBuffer(new wchar_t[dwCapacity]);
            dwResult = ::GetModuleFileNameW(hModule, szBuffer.get(), dwCapacity);
            if (dwResult < dwCapacity) {
                sValue.assign(szBuffer.get(), dwResult);
                return dwResult;
            }
        }
    }
}

/// @copydoc GetWindowTextW()
template<class _Traits, class _Ax>
static _Success_(return != 0) int GetWindowTextA(_In_ HWND hWnd, _Out_ std::basic_string<char, _Traits, _Ax> &sValue) noexcept
{
    assert(0); // TODO: Test this code.

    int iResult;

    // Query the final string length first.
    iResult = ::GetWindowTextLengthA(hWnd);
    if (iResult > 0) {
        if (++iResult < WINSTD_STACK_BUFFER_BYTES/sizeof(char)) {
            // Read string data to stack.
            char szBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(char)];
            iResult = ::GetWindowTextA(hWnd, szBuffer, _countof(szBuffer));
            sValue.assign(szBuffer, iResult);
        } else {
            // Allocate buffer on heap and read the string data into it.
            std::unique_ptr<char[]> szBuffer(new char[++iResult]);
            iResult = ::GetWindowTextA(hWnd, szBuffer.get(), iResult);
            sValue.assign(szBuffer.get(), iResult);
        }
        return iResult;
    }

    sValue.clear();
    return 0;
}

///
/// Copies the text of the specified window's title bar (if it has one) into a std::wstring string.
///
/// \sa [GetWindowText function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms633520.aspx)
///
template<class _Traits, class _Ax>
static _Success_(return != 0) int GetWindowTextW(_In_ HWND hWnd, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sValue) noexcept
{
    assert(0); // TODO: Test this code.

    int iResult;

    // Query the final string length first.
    iResult = ::GetWindowTextLengthW(hWnd);
    if (iResult > 0) {
        if (++iResult < WINSTD_STACK_BUFFER_BYTES/sizeof(wchar_t)) {
            // Read string data to stack.
            wchar_t szBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(wchar_t)];
            iResult = ::GetWindowTextW(hWnd, szBuffer, _countof(szBuffer));
            sValue.assign(szBuffer, iResult);
        } else {
            // Allocate buffer on heap and read the string data into it.
            std::unique_ptr<wchar_t[]> szBuffer(new wchar_t[++iResult]);
            iResult = ::GetWindowTextW(hWnd, szBuffer.get(), iResult);
            sValue.assign(szBuffer.get(), iResult);
        }
        return iResult;
    }

    sValue.clear();
    return 0;
}

/// @copydoc GetFileVersionInfoW()
template<class _Ty, class _Ax>
static _Success_(return != 0) BOOL GetFileVersionInfoA(_In_z_ LPCSTR lptstrFilename, __reserved DWORD dwHandle, _Out_ std::vector<_Ty, _Ax> &aValue) noexcept
{
    assert(0); // TODO: Test this code.

    // Get version info size.
    DWORD dwVerInfoSize = ::GetFileVersionInfoSizeA(lptstrFilename, &dwHandle);
    if (dwVerInfoSize != 0) {
        // Read version info.
        aValue.resize((dwVerInfoSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        return ::GetFileVersionInfoA(lptstrFilename, dwHandle, dwVerInfoSize, aValue.data());
    } else
        return FALSE;
}

///
/// Retrieves version information for the specified file and stores it in a std::vector buffer.
///
/// \sa [GetFileVersionInfo function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms647003.aspx)
///
template<class _Ty, class _Ax>
static _Success_(return != 0) BOOL GetFileVersionInfoW(_In_z_ LPCWSTR lptstrFilename, __reserved DWORD dwHandle, _Out_ std::vector<_Ty, _Ax> &aValue) noexcept
{
    assert(0); // TODO: Test this code.

    // Get version info size.
    DWORD dwVerInfoSize = ::GetFileVersionInfoSizeW(lptstrFilename, &dwHandle);
    if (dwVerInfoSize != 0) {
        // Read version info.
        aValue.resize((dwVerInfoSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        return ::GetFileVersionInfoW(lptstrFilename, dwHandle, dwVerInfoSize, aValue.data());
    } else
        return FALSE;
}

/// @copydoc ExpandEnvironmentStringsW()
template<class _Traits, class _Ax>
static _Success_(return != 0) DWORD ExpandEnvironmentStringsA(_In_z_ LPCSTR lpSrc, _Out_ std::basic_string<char, _Traits, _Ax> &sValue) noexcept
{
    assert(0); // TODO: Test this code.

    for (DWORD dwSizeOut = (DWORD)strlen(lpSrc) + 0x100;;) {
        DWORD dwSizeIn = dwSizeOut;
        std::unique_ptr<char[]> szBuffer(new char[(size_t)dwSizeIn + 2]); // Note: ANSI version requires one extra char.
        dwSizeOut = ::ExpandEnvironmentStringsA(lpSrc, szBuffer.get(), dwSizeIn);
        if (dwSizeOut == 0) {
            // Error or zero-length input.
            break;
        } else if (dwSizeOut <= dwSizeIn) {
            // The buffer was sufficient.
            sValue.assign(szBuffer.get(), dwSizeOut - 1);
            return dwSizeOut;
        }
    }

    sValue.clear();
    return 0;
}

///
/// Expands environment-variable strings, replaces them with the values defined for the current user, and stores it in a std::wstring string.
///
/// \sa [ExpandEnvironmentStrings function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724265.aspx)
///
template<class _Traits, class _Ax>
static _Success_(return != 0) DWORD ExpandEnvironmentStringsW(_In_z_ LPCWSTR lpSrc, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sValue) noexcept
{
    for (DWORD dwSizeOut = (DWORD)wcslen(lpSrc) + 0x100;;) {
        DWORD dwSizeIn = dwSizeOut;
        std::unique_ptr<wchar_t[]> szBuffer(new wchar_t[(size_t)dwSizeIn + 1]);
        dwSizeOut = ::ExpandEnvironmentStringsW(lpSrc, szBuffer.get(), dwSizeIn);
        if (dwSizeOut == 0) {
            // Error or zero-length input.
            break;
        } else if (dwSizeOut <= dwSizeIn) {
            // The buffer was sufficient.
            sValue.assign(szBuffer.get(), dwSizeOut - 1);
            return dwSizeOut;
        }
    }

    sValue.clear();
    return 0;
}

/// @copydoc GuidToStringW()
template<class _Traits, class _Ax>
static VOID GuidToStringA(_In_ LPCGUID lpGuid, _Out_ std::basic_string<char, _Traits, _Ax> &str) noexcept
{
    assert(0); // TODO: Test this code.

    sprintf(str, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
        lpGuid->Data1,
        lpGuid->Data2,
        lpGuid->Data3,
        lpGuid->Data4[0], lpGuid->Data4[1],
        lpGuid->Data4[2], lpGuid->Data4[3], lpGuid->Data4[4], lpGuid->Data4[5], lpGuid->Data4[6], lpGuid->Data4[7]);
}

///
/// Formats GUID and stores it in a std::wstring string.
///
/// \param[in ] lpGuid  Pointer to GUID
/// \param[out] str     String to store the result to
///
template<class _Traits, class _Ax>
static VOID GuidToStringW(_In_ LPCGUID lpGuid, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &str) noexcept
{
    assert(0); // TODO: Test this code.

    sprintf(str, L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
        lpGuid->Data1,
        lpGuid->Data2,
        lpGuid->Data3,
        lpGuid->Data4[0], lpGuid->Data4[1],
        lpGuid->Data4[2], lpGuid->Data4[3], lpGuid->Data4[4], lpGuid->Data4[5], lpGuid->Data4[6], lpGuid->Data4[7]);
}

/// @copydoc GuidToStringW()
#ifdef _UNICODE
#define GuidToString GuidToStringW
#else
#define GuidToString GuidToStringA
#endif

/// @copydoc StringToGuidW()
static _Success_(return) BOOL StringToGuidA(_In_z_ LPCSTR lpszGuid, _Out_ LPGUID lpGuid, _Out_opt_ LPCSTR *lpszGuidEnd = NULL) noexcept
{
    GUID g;
    LPSTR lpszEnd;
    unsigned long ulTmp;
    unsigned long long ullTmp;

    if (!lpszGuid || !lpGuid || *lpszGuid != '{') return FALSE;
    lpszGuid++;

    g.Data1 = strtoul(lpszGuid, &lpszEnd, 16);
    if (errno == ERANGE) return FALSE;
    lpszGuid = lpszEnd;

    if (*lpszGuid != '-') return FALSE;
    lpszGuid++;

    ulTmp = strtoul(lpszGuid, &lpszEnd, 16);
    if (errno == ERANGE || ulTmp > 0xFFFF) return FALSE;
    g.Data2 = static_cast<unsigned short>(ulTmp);
    lpszGuid = lpszEnd;

    if (*lpszGuid != '-') return FALSE;
    lpszGuid++;

    ulTmp = strtoul(lpszGuid, &lpszEnd, 16);
    if (errno == ERANGE || ulTmp > 0xFFFF) return FALSE;
    g.Data3 = static_cast<unsigned short>(ulTmp);
    lpszGuid = lpszEnd;

    if (*lpszGuid != '-') return FALSE;
    lpszGuid++;

    ulTmp = strtoul(lpszGuid, &lpszEnd, 16);
    if (errno == ERANGE || ulTmp > 0xFFFF) return FALSE;
    g.Data4[0] = static_cast<unsigned char>((ulTmp >> 8) & 0xff);
    g.Data4[1] = static_cast<unsigned char>( ulTmp       & 0xff);
    lpszGuid = lpszEnd;

    if (*lpszGuid != '-') return FALSE;
    lpszGuid++;

    ullTmp = _strtoui64(lpszGuid, &lpszEnd, 16);
    if (errno == ERANGE || ullTmp > 0xFFFFFFFFFFFF) return FALSE;
    g.Data4[2] = static_cast<unsigned char>((ullTmp >> 40) & 0xff);
    g.Data4[3] = static_cast<unsigned char>((ullTmp >> 32) & 0xff);
    g.Data4[4] = static_cast<unsigned char>((ullTmp >> 24) & 0xff);
    g.Data4[5] = static_cast<unsigned char>((ullTmp >> 16) & 0xff);
    g.Data4[6] = static_cast<unsigned char>((ullTmp >>  8) & 0xff);
    g.Data4[7] = static_cast<unsigned char>( ullTmp        & 0xff);
    lpszGuid = lpszEnd;

    if (*lpszGuid != '}') return FALSE;
    lpszGuid++;

    if (lpszGuidEnd)
        *lpszGuidEnd = lpszGuid;

    *lpGuid = g;
    return TRUE;
}

///
/// Parses string with GUID and stores it to GUID
///
/// \param[in ] lpszGuid     String with GUID
/// \param[out] lpGuid       GUID to store the result to
/// \param[out] lpszGuidEnd  If non-NULL the pointer to the end of parsed GUID within `lpszGuid` is returned
///
/// \returns
/// - `TRUE` if GUID successfuly parsed;
/// - `FALSE` otherwise.
///
static _Success_(return) BOOL StringToGuidW(_In_z_ LPCWSTR lpszGuid, _Out_ LPGUID lpGuid, _Out_opt_ LPCWSTR *lpszGuidEnd = NULL) noexcept
{
    GUID g;
    LPWSTR lpszEnd;
    unsigned long ulTmp;
    unsigned long long ullTmp;

    if (!lpszGuid || !lpGuid || *lpszGuid != '{') return FALSE;
    lpszGuid++;

    g.Data1 = wcstoul(lpszGuid, &lpszEnd, 16);
    if (errno == ERANGE) return FALSE;
    lpszGuid = lpszEnd;

    if (*lpszGuid != '-') return FALSE;
    lpszGuid++;

    ulTmp = wcstoul(lpszGuid, &lpszEnd, 16);
    if (errno == ERANGE || ulTmp > 0xFFFF) return FALSE;
    g.Data2 = static_cast<unsigned short>(ulTmp);
    lpszGuid = lpszEnd;

    if (*lpszGuid != '-') return FALSE;
    lpszGuid++;

    ulTmp = wcstoul(lpszGuid, &lpszEnd, 16);
    if (errno == ERANGE || ulTmp > 0xFFFF) return FALSE;
    g.Data3 = static_cast<unsigned short>(ulTmp);
    lpszGuid = lpszEnd;

    if (*lpszGuid != '-') return FALSE;
    lpszGuid++;

    ulTmp = wcstoul(lpszGuid, &lpszEnd, 16);
    if (errno == ERANGE || ulTmp > 0xFFFF) return FALSE;
    g.Data4[0] = static_cast<unsigned char>((ulTmp >> 8) & 0xff);
    g.Data4[1] = static_cast<unsigned char>( ulTmp       & 0xff);
    lpszGuid = lpszEnd;

    if (*lpszGuid != '-') return FALSE;
    lpszGuid++;

    ullTmp = _wcstoui64(lpszGuid, &lpszEnd, 16);
    if (errno == ERANGE || ullTmp > 0xFFFFFFFFFFFF) return FALSE;
    g.Data4[2] = static_cast<unsigned char>((ullTmp >> 40) & 0xff);
    g.Data4[3] = static_cast<unsigned char>((ullTmp >> 32) & 0xff);
    g.Data4[4] = static_cast<unsigned char>((ullTmp >> 24) & 0xff);
    g.Data4[5] = static_cast<unsigned char>((ullTmp >> 16) & 0xff);
    g.Data4[6] = static_cast<unsigned char>((ullTmp >>  8) & 0xff);
    g.Data4[7] = static_cast<unsigned char>( ullTmp        & 0xff);
    lpszGuid = lpszEnd;

    if (*lpszGuid != '}') return FALSE;
    lpszGuid++;

    if (lpszGuidEnd)
        *lpszGuidEnd = lpszGuid;

    *lpGuid = g;
    return TRUE;
}

/// @copydoc StringToGuidW()
#ifdef _UNICODE
#define StringToGuid StringToGuidW
#else
#define StringToGuid StringToGuidA
#endif

///
/// Queries for a string value in the registry and stores it in a std::string string.
///
/// `REG_EXPAND_SZ` are expanded using `ExpandEnvironmentStrings()` before storing to sValue.
///
/// \param[in ] hReg     A handle to an open registry key. The key must have been opened with the KEY_QUERY_VALUE access right.
/// \param[in ] pszName  The name of the registry value. If lpValueName is NULL or an empty string, "", the function retrieves the type and data for the key's unnamed or default value, if any.
/// \param[out] sValue   String to store the value to
///
/// \return
/// - `ERROR_SUCCESS` when query succeeds;
/// - `ERROR_INVALID_DATA` when the registy value type is not `REG_SZ`, `REG_MULTI_SZ`, or `REG_EXPAND_SZ`;
/// - `ERROR_OUTOFMEMORY` when the memory allocation for the sValue buffer fails;
/// - Error code when query fails. See `RegQueryValueEx()` for the list of error codes.
///
/// \sa [RegQueryValueEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724911.aspx)
/// \sa [ExpandEnvironmentStrings function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724265.aspx)
///
template<class _Traits, class _Ax>
static LSTATUS RegQueryStringValue(_In_ HKEY hReg, _In_z_ LPCSTR pszName, _Out_ std::basic_string<char, _Traits, _Ax> &sValue) noexcept
{
    LSTATUS lResult;
    BYTE aStackBuffer[WINSTD_STACK_BUFFER_BYTES];
    DWORD dwSize = sizeof(aStackBuffer), dwType;

    // Try with stack buffer first.
    lResult = ::RegQueryValueExA(hReg, pszName, NULL, &dwType, aStackBuffer, &dwSize);
    if (lResult == ERROR_SUCCESS) {
        if (dwType == REG_SZ || dwType == REG_MULTI_SZ) {
            // The value is REG_SZ or REG_MULTI_SZ.
            dwSize /= sizeof(CHAR);
            sValue.assign(reinterpret_cast<LPCSTR>(aStackBuffer), dwSize && reinterpret_cast<LPCSTR>(aStackBuffer)[dwSize - 1] == 0 ? dwSize - 1 : dwSize);
        } else if (dwType == REG_EXPAND_SZ) {
            // The value is REG_EXPAND_SZ. Expand it from stack buffer.
            if (::ExpandEnvironmentStringsA(reinterpret_cast<LPCSTR>(aStackBuffer), sValue) == 0)
                lResult = ::GetLastError();
        } else {
            // The value is not a string type.
            lResult = ERROR_INVALID_DATA;
        }
    } else if (lResult == ERROR_MORE_DATA) {
        if (dwType == REG_SZ || dwType == REG_MULTI_SZ) {
            // The value is REG_SZ or REG_MULTI_SZ. Read it now.
            std::unique_ptr<CHAR[]> szBuffer(new CHAR[dwSize / sizeof(CHAR)]);
            if ((lResult = ::RegQueryValueExA(hReg, pszName, NULL, NULL, reinterpret_cast<LPBYTE>(szBuffer.get()), &dwSize)) == ERROR_SUCCESS) {
                dwSize /= sizeof(CHAR);
                sValue.assign(szBuffer.get(), dwSize && szBuffer[dwSize - 1] == 0 ? dwSize - 1 : dwSize);
            }
        } else if (dwType == REG_EXPAND_SZ) {
            // The value is REG_EXPAND_SZ. Read it and expand environment variables.
            std::unique_ptr<CHAR[]> szBuffer(new CHAR[dwSize / sizeof(CHAR)]);
            if ((lResult = ::RegQueryValueExA(hReg, pszName, NULL, NULL, reinterpret_cast<LPBYTE>(szBuffer.get()), &dwSize)) == ERROR_SUCCESS) {
                if (::ExpandEnvironmentStringsA(szBuffer.get(), sValue) == 0)
                    lResult = ::GetLastError();
            }
        } else {
            // The value is not a string type.
            lResult = ERROR_INVALID_DATA;
        }
    }

    return lResult;
}

///
/// Queries for a string value in the registry and stores it in a std::wstring string.
///
/// `REG_EXPAND_SZ` are expanded using `ExpandEnvironmentStrings()` before storing to sValue.
///
/// \param[in ] hReg     A handle to an open registry key. The key must have been opened with the KEY_QUERY_VALUE access right.
/// \param[in ] pszName  The name of the registry value. If lpValueName is NULL or an empty string, "", the function retrieves the type and data for the key's unnamed or default value, if any.
/// \param[out] sValue   String to store the value to
///
/// \return
/// - `ERROR_SUCCESS` when query succeeds;
/// - `ERROR_INVALID_DATA` when the registy value type is not `REG_SZ`, `REG_MULTI_SZ`, or `REG_EXPAND_SZ`;
/// - `ERROR_OUTOFMEMORY` when the memory allocation for the sValue buffer fails;
/// - Error code when query fails. See `RegQueryValueEx()` for the list of error codes.
///
/// \sa [RegQueryValueEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724911.aspx)
/// \sa [ExpandEnvironmentStrings function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724265.aspx)
///
template<class _Traits, class _Ax>
static LSTATUS RegQueryStringValue(_In_ HKEY hReg, _In_z_ LPCWSTR pszName, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sValue) noexcept
{
    LSTATUS lResult;
    BYTE aStackBuffer[WINSTD_STACK_BUFFER_BYTES];
    DWORD dwSize = sizeof(aStackBuffer), dwType;

    // Try with stack buffer first.
    lResult = ::RegQueryValueExW(hReg, pszName, NULL, &dwType, aStackBuffer, &dwSize);
    if (lResult == ERROR_SUCCESS) {
        if (dwType == REG_SZ || dwType == REG_MULTI_SZ) {
            // The value is REG_SZ or REG_MULTI_SZ.
            dwSize /= sizeof(WCHAR);
            sValue.assign(reinterpret_cast<LPCWSTR>(aStackBuffer), dwSize && reinterpret_cast<LPCWSTR>(aStackBuffer)[dwSize - 1] == 0 ? dwSize - 1 : dwSize);
        } else if (dwType == REG_EXPAND_SZ) {
            // The value is REG_EXPAND_SZ. Expand it from stack buffer.
            if (::ExpandEnvironmentStringsW(reinterpret_cast<LPCWSTR>(aStackBuffer), sValue) == 0)
                lResult = ::GetLastError();
        } else {
            // The value is not a string type.
            lResult = ERROR_INVALID_DATA;
        }
    } else if (lResult == ERROR_MORE_DATA) {
        if (dwType == REG_SZ || dwType == REG_MULTI_SZ) {
            // The value is REG_SZ or REG_MULTI_SZ. Read it now.
            std::unique_ptr<WCHAR[]> szBuffer(new WCHAR[dwSize / sizeof(WCHAR)]);
            if ((lResult = ::RegQueryValueExW(hReg, pszName, NULL, NULL, reinterpret_cast<LPBYTE>(szBuffer.get()), &dwSize)) == ERROR_SUCCESS) {
                dwSize /= sizeof(WCHAR);
                sValue.assign(szBuffer.get(), dwSize && szBuffer[dwSize - 1] == 0 ? dwSize - 1 : dwSize);
            }
        } else if (dwType == REG_EXPAND_SZ) {
            // The value is REG_EXPAND_SZ. Read it and expand environment variables.
            std::unique_ptr<WCHAR[]> szBuffer(new WCHAR[dwSize / sizeof(WCHAR)]);
            if ((lResult = ::RegQueryValueExW(hReg, pszName, NULL, NULL, reinterpret_cast<LPBYTE>(szBuffer.get()), &dwSize)) == ERROR_SUCCESS) {
                if (::ExpandEnvironmentStringsW(szBuffer.get(), sValue) == 0)
                    lResult = ::GetLastError();
            }
        } else {
            // The value is not a string type.
            lResult = ERROR_INVALID_DATA;
        }
    }

    return lResult;
}

/// @copydoc RegQueryValueExW()
template<class _Ty, class _Ax>
static LSTATUS RegQueryValueExA(_In_ HKEY hKey, _In_opt_z_ LPCSTR lpValueName, __reserved LPDWORD lpReserved, _Out_opt_ LPDWORD lpType, _Out_ std::vector<_Ty, _Ax> &aData) noexcept
{
    LSTATUS lResult;
    BYTE aStackBuffer[WINSTD_STACK_BUFFER_BYTES];
    DWORD dwSize = sizeof(aStackBuffer);

    // Try with stack buffer first.
    lResult = RegQueryValueExA(hKey, lpValueName, lpReserved, lpType, aStackBuffer, &dwSize);
    if (lResult == ERROR_SUCCESS) {
        // Copy from stack buffer.
        aData.resize((dwSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        memcpy(aData.data(), aStackBuffer, dwSize);
    } else if (lResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap and retry.
        aData.resize((dwSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        lResult = RegQueryValueExA(hKey, lpValueName, lpReserved, NULL, reinterpret_cast<LPBYTE>(aData.data()), &dwSize);
    }

    return lResult;
}

///
/// Retrieves the type and data for the specified value name associated with an open registry key and stores the data in a std::vector buffer.
///
/// \sa [RegQueryValueEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724911.aspx)
///
template<class _Ty, class _Ax>
static LSTATUS RegQueryValueExW(_In_ HKEY hKey, _In_opt_z_ LPCWSTR lpValueName, __reserved LPDWORD lpReserved, _Out_opt_ LPDWORD lpType, _Out_ std::vector<_Ty, _Ax> &aData) noexcept
{
    LSTATUS lResult;
    BYTE aStackBuffer[WINSTD_STACK_BUFFER_BYTES];
    DWORD dwSize = sizeof(aStackBuffer);

    // Try with stack buffer first.
    lResult = RegQueryValueExW(hKey, lpValueName, lpReserved, lpType, aStackBuffer, &dwSize);
    if (lResult == ERROR_SUCCESS) {
        // Copy from stack buffer.
        aData.resize((dwSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        memcpy(aData.data(), aStackBuffer, dwSize);
    } else if (lResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap and retry.
        aData.resize((dwSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        lResult = RegQueryValueExW(hKey, lpValueName, lpReserved, NULL, reinterpret_cast<LPBYTE>(aData.data()), &dwSize);
    }

    return lResult;
}

#if _WIN32_WINNT >= _WIN32_WINNT_VISTA

/// @copydoc RegLoadMUIStringW()
template<class _Traits, class _Ax>
static LSTATUS RegLoadMUIStringA(_In_ HKEY hKey, _In_opt_z_ LPCSTR pszValue, _Out_ std::basic_string<char, _Traits, _Ax> &sOut, _In_ DWORD Flags, _In_opt_z_ LPCSTR pszDirectory) noexcept
{
    // According to "Remarks" section in MSDN documentation of RegLoadMUIString(),
    // this function is defined but not implemented as ANSI variation.
    assert(0);
    return ERROR_CALL_NOT_IMPLEMENTED;
}

///
/// Loads the specified string from the specified key and subkey, and stores it in a std::wstring string.
///
/// \sa [RegLoadMUIString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724890.aspx)
///
template<class _Traits, class _Ax>
static LSTATUS RegLoadMUIStringW(_In_ HKEY hKey, _In_opt_z_ LPCWSTR pszValue, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sOut, _In_ DWORD Flags, _In_opt_z_ LPCWSTR pszDirectory) noexcept
{
    LSTATUS lResult;
    wchar_t szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(wchar_t)];
    DWORD dwSize;

    Flags &= ~REG_MUI_STRING_TRUNCATE;

    // Try with stack buffer first.
    lResult = RegLoadMUIStringW(hKey, pszValue, szStackBuffer, sizeof(szStackBuffer), &dwSize, Flags, pszDirectory);
    if (lResult == ERROR_SUCCESS) {
        // Copy from stack buffer.
        sOut.assign(szStackBuffer, wcsnlen(szStackBuffer, dwSize/sizeof(wchar_t)));
    } else if (lResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap and retry.
        std::unique_ptr<wchar_t[]> szBuffer(new wchar_t[(dwSize + sizeof(wchar_t) - 1)/sizeof(wchar_t)]);
        sOut.assign(szBuffer.get(), (lResult = RegLoadMUIStringW(hKey, pszValue, szBuffer.get(), dwSize, &dwSize, Flags, pszDirectory)) == ERROR_SUCCESS ? wcsnlen(szBuffer.get(), dwSize/sizeof(wchar_t)) : 0);
    }

    return lResult;
}

#endif

///
/// Maps a UTF-16 (wide character) string to a std::string. The new character string is not necessarily from a multibyte character set.
///
/// \sa [WideCharToMultiByte function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130.aspx)
///
template<class _Traits, class _Ax>
static _Success_(return != 0) int WideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cchWideChar) LPCWSTR lpWideCharStr, _In_ int cchWideChar, _Out_ std::basic_string<char, _Traits, _Ax> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept
{
    CHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(CHAR)];

    // Try to convert to stack buffer first.
    int cch = ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, szStackBuffer, _countof(szStackBuffer), lpDefaultChar, lpUsedDefaultChar);
    if (cch) {
        // Copy from stack. Be careful not to include zero terminator.
        sMultiByteStr.assign(szStackBuffer, cchWideChar != -1 ? strnlen(szStackBuffer, cch) : (size_t)cch - 1);
    } else if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Query the required output size. Allocate buffer. Then convert again.
        cch = ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, NULL, 0, lpDefaultChar, lpUsedDefaultChar);
        std::unique_ptr<CHAR[]> szBuffer(new CHAR[cch]);
        cch = ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, szBuffer.get(), cch, lpDefaultChar, lpUsedDefaultChar);
        sMultiByteStr.assign(szBuffer.get(), cchWideChar != -1 ? strnlen(szBuffer.get(), cch) : (size_t)cch - 1);
    }

    return cch;
}

///
/// Maps a UTF-16 (wide character) string to a std::vector. The new character vector is not necessarily from a multibyte character set.
///
/// \sa [WideCharToMultiByte function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130.aspx)
///
template<class _Ax>
static _Success_(return != 0) int WideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cchWideChar) LPCWSTR lpWideCharStr, _In_ int cchWideChar, _Out_ std::vector<char, _Ax> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept
{
    CHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(CHAR)];

    // Try to convert to stack buffer first.
    int cch = ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, szStackBuffer, _countof(szStackBuffer), lpDefaultChar, lpUsedDefaultChar);
    if (cch) {
        // Copy from stack.
        sMultiByteStr.assign(szStackBuffer, szStackBuffer + cch);
    } else if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Query the required output size. Allocate buffer. Then convert again.
        cch = ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, NULL, 0, lpDefaultChar, lpUsedDefaultChar);
        sMultiByteStr.resize(cch);
        cch = ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, sMultiByteStr.data(), cch, lpDefaultChar, lpUsedDefaultChar);
    }

    return cch;
}

///
/// Maps a UTF-16 (wide character) string to a std::string. The new character string is not necessarily from a multibyte character set.
///
/// \sa [WideCharToMultiByte function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130.aspx)
///
template<class _Traits1, class _Ax1, class _Traits2, class _Ax2>
static _Success_(return != 0) int WideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_ std::basic_string<wchar_t, _Traits1, _Ax1> sWideCharStr, _Out_ std::basic_string<char, _Traits2, _Ax2> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept
{
    CHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(CHAR)];

    // Try to convert to stack buffer first.
    int cch = ::WideCharToMultiByte(CodePage, dwFlags, sWideCharStr.c_str(), (int)sWideCharStr.length(), szStackBuffer, _countof(szStackBuffer), lpDefaultChar, lpUsedDefaultChar);
    if (cch) {
        // Copy from stack.
        sMultiByteStr.assign(szStackBuffer, cch);
    } else if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Query the required output size. Allocate buffer. Then convert again.
        cch = ::WideCharToMultiByte(CodePage, dwFlags, sWideCharStr.c_str(), (int)sWideCharStr.length(), NULL, 0, lpDefaultChar, lpUsedDefaultChar);
        std::unique_ptr<CHAR[]> szBuffer(new CHAR[cch]);
        cch = ::WideCharToMultiByte(CodePage, dwFlags, sWideCharStr.c_str(), (int)sWideCharStr.length(), szBuffer.get(), cch, lpDefaultChar, lpUsedDefaultChar);
        sMultiByteStr.assign(szBuffer.get(), cch);
    }

    return cch;
}

///
/// Maps a UTF-16 (wide character) string to a std::string. The new character string is not necessarily from a multibyte character set.
///
/// \note This function cleans all internal buffers using SecureZeroMemory() before returning.
///
/// \sa [WideCharToMultiByte function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130.aspx)
///
template<class _Traits, class _Ax>
static _Success_(return != 0) int SecureWideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cchWideChar) LPCWSTR lpWideCharStr, _In_ int cchWideChar, _Out_ std::basic_string<char, _Traits, _Ax> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept
{
    CHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(CHAR)];

    // Try to convert to stack buffer first.
    int cch = ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, szStackBuffer, _countof(szStackBuffer), lpDefaultChar, lpUsedDefaultChar);
    if (cch) {
        // Copy from stack. Be careful not to include zero terminator.
        sMultiByteStr.assign(szStackBuffer, cchWideChar != -1 ? strnlen(szStackBuffer, cch) : (size_t)cch - 1);
    } else if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Query the required output size. Allocate buffer. Then convert again.
        cch = ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, NULL, 0, lpDefaultChar, lpUsedDefaultChar);
        std::unique_ptr<CHAR[]> szBuffer(new CHAR[cch]);
        cch = ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, szBuffer.get(), cch, lpDefaultChar, lpUsedDefaultChar);
        sMultiByteStr.assign(szBuffer.get(), cchWideChar != -1 ? strnlen(szBuffer.get(), cch) : (size_t)cch - 1);
        SecureZeroMemory(szBuffer.get(), sizeof(CHAR)*cch);
    }

    SecureZeroMemory(szStackBuffer, sizeof(szStackBuffer));

    return cch;
}

///
/// Maps a UTF-16 (wide character) string to a std::vector. The new character vector is not necessarily from a multibyte character set.
///
/// \note This function cleans all internal buffers using SecureZeroMemory() before returning.
///
/// \sa [WideCharToMultiByte function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130.aspx)
///
template<class _Ax>
static _Success_(return != 0) int SecureWideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cchWideChar) LPCWSTR lpWideCharStr, _In_ int cchWideChar, _Out_ std::vector<char, _Ax> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept
{
    CHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(CHAR)];

    // Try to convert to stack buffer first.
    int cch = ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, szStackBuffer, _countof(szStackBuffer), lpDefaultChar, lpUsedDefaultChar);
    if (cch) {
        // Copy from stack.
        sMultiByteStr.assign(szStackBuffer, szStackBuffer + cch);
    } else if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Query the required output size. Allocate buffer. Then convert again.
        cch = ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, NULL, 0, lpDefaultChar, lpUsedDefaultChar);
        sMultiByteStr.resize(cch);
        cch = ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, sMultiByteStr.data(), cch, lpDefaultChar, lpUsedDefaultChar);
    }

    SecureZeroMemory(szStackBuffer, sizeof(szStackBuffer));

    return cch;
}

///
/// Maps a UTF-16 (wide character) string to a std::string. The new character string is not necessarily from a multibyte character set.
///
/// \note This function cleans all internal buffers using SecureZeroMemory() before returning.
///
/// \sa [WideCharToMultiByte function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130.aspx)
///
template<class _Traits1, class _Ax1, class _Traits2, class _Ax2>
static _Success_(return != 0) int SecureWideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _Out_ std::basic_string<wchar_t, _Traits1, _Ax1> sWideCharStr, _Out_ std::basic_string<char, _Traits2, _Ax2> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept
{
    CHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(CHAR)];

    // Try to convert to stack buffer first.
    int cch = ::WideCharToMultiByte(CodePage, dwFlags, sWideCharStr.c_str(), (int)sWideCharStr.length(), szStackBuffer, _countof(szStackBuffer), lpDefaultChar, lpUsedDefaultChar);
    if (cch) {
        // Copy from stack.
        sMultiByteStr.assign(szStackBuffer, cch);
    } else if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Query the required output size. Allocate buffer. Then convert again.
        cch = ::WideCharToMultiByte(CodePage, dwFlags, sWideCharStr.c_str(), (int)sWideCharStr.length(), NULL, 0, lpDefaultChar, lpUsedDefaultChar);
        std::unique_ptr<CHAR[]> szBuffer(new CHAR[cch]);
        cch = ::WideCharToMultiByte(CodePage, dwFlags, sWideCharStr.c_str(), (int)sWideCharStr.length(), szBuffer.get(), cch, lpDefaultChar, lpUsedDefaultChar);
        sMultiByteStr.assign(szBuffer.get(), cch);
        SecureZeroMemory(szBuffer.get(), sizeof(CHAR)*cch);
    }

    SecureZeroMemory(szStackBuffer, sizeof(szStackBuffer));

    return cch;
}

///
/// Maps a character string to a UTF-16 (wide character) std::wstring. The character string is not necessarily from a multibyte character set.
///
/// \sa [MultiByteToWideChar function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd319072.aspx)
///
template<class _Traits, class _Ax>
static _Success_(return != 0) int MultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cbMultiByte) LPCSTR lpMultiByteStr, _In_ int cbMultiByte, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sWideCharStr) noexcept
{
    WCHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(WCHAR)];

    // Try to convert to stack buffer first.
    int cch = ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, szStackBuffer, _countof(szStackBuffer));
    if (cch) {
        // Copy from stack.
        sWideCharStr.assign(szStackBuffer, cbMultiByte != -1 ? wcsnlen(szStackBuffer, cch) : (size_t)cch - 1);
    } else if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Query the required output size. Allocate buffer. Then convert again.
        cch = ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, NULL, 0);
        std::unique_ptr<WCHAR[]> szBuffer(new WCHAR[cch]);
        cch = ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, szBuffer.get(), cch);
        sWideCharStr.assign(szBuffer.get(), cbMultiByte != -1 ? wcsnlen(szBuffer.get(), cch) : (size_t)cch - 1);
    }

    return cch;
}

///
/// Maps a character string to a UTF-16 (wide character) std::vector. The character vector is not necessarily from a multibyte character set.
///
/// \sa [MultiByteToWideChar function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd319072.aspx)
///
template<class _Ax>
static _Success_(return != 0) int MultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cbMultiByte) LPCSTR lpMultiByteStr, _In_ int cbMultiByte, _Out_ std::vector<wchar_t, _Ax> &sWideCharStr) noexcept
{
    WCHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(WCHAR)];

    // Try to convert to stack buffer first.
    int cch = ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, szStackBuffer, _countof(szStackBuffer));
    if (cch) {
        // Copy from stack.
        sWideCharStr.assign(szStackBuffer, szStackBuffer + cch);
    } else if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Query the required output size. Allocate buffer. Then convert again.
        cch = ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, NULL, 0);
        sWideCharStr.resize(cch);
        cch = ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, sWideCharStr.data(), cch);
    }

    return cch;
}

///
/// Maps a character string to a UTF-16 (wide character) std::wstring. The character string is not necessarily from a multibyte character set.
///
/// \sa [MultiByteToWideChar function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd319072.aspx)
///
template<class _Traits1, class _Ax1, class _Traits2, class _Ax2>
static _Success_(return != 0) int MultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_ const std::basic_string<char, _Traits1, _Ax1> &sMultiByteStr, _Out_ std::basic_string<wchar_t, _Traits2, _Ax2> &sWideCharStr) noexcept
{
    WCHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(WCHAR)];

    // Try to convert to stack buffer first.
    int cch = ::MultiByteToWideChar(CodePage, dwFlags, sMultiByteStr.c_str(), (int)sMultiByteStr.length(), szStackBuffer, _countof(szStackBuffer));
    if (cch) {
        // Copy from stack.
        sWideCharStr.assign(szStackBuffer, cch);
    } else if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Query the required output size. Allocate buffer. Then convert again.
        cch = ::MultiByteToWideChar(CodePage, dwFlags, sMultiByteStr.c_str(), (int)sMultiByteStr.length(), NULL, 0);
        std::unique_ptr<WCHAR[]> szBuffer(new WCHAR[cch]);
        cch = ::MultiByteToWideChar(CodePage, dwFlags, sMultiByteStr.c_str(), (int)sMultiByteStr.length(), szBuffer.get(), cch);
        sWideCharStr.assign(szBuffer.get(), cch);
    }

    return cch;
}

///
/// Maps a character string to a UTF-16 (wide character) std::wstring. The character string is not necessarily from a multibyte character set.
///
/// \note This function cleans all internal buffers using SecureZeroMemory() before returning.
///
/// \sa [MultiByteToWideChar function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd319072.aspx)
///
template<class _Traits, class _Ax>
static _Success_(return != 0) int SecureMultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cbMultiByte) LPCSTR lpMultiByteStr, _In_ int cbMultiByte, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sWideCharStr) noexcept
{
    WCHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(WCHAR)];

    // Try to convert to stack buffer first.
    int cch = ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, szStackBuffer, _countof(szStackBuffer));
    if (cch) {
        // Copy from stack.
        sWideCharStr.assign(szStackBuffer, cbMultiByte != -1 ? wcsnlen(szStackBuffer, cch) : (size_t)cch - 1);
    } else if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Query the required output size. Allocate buffer. Then convert again.
        cch = ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, NULL, 0);
        std::unique_ptr<WCHAR[]> szBuffer(new WCHAR[cch]);
        cch = ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, szBuffer.get(), cch);
        sWideCharStr.assign(szBuffer.get(), cbMultiByte != -1 ? wcsnlen(szBuffer.get(), cch) : (size_t)cch - 1);
        SecureZeroMemory(szBuffer.get(), sizeof(WCHAR)*cch);
    }

    SecureZeroMemory(szStackBuffer, sizeof(szStackBuffer));

    return cch;
}

///
/// Maps a character string to a UTF-16 (wide character) std::vector. The character vector is not necessarily from a multibyte character set.
///
/// \note This function cleans all internal buffers using SecureZeroMemory() before returning.
///
/// \sa [MultiByteToWideChar function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd319072.aspx)
///
template<class _Ax>
static _Success_(return != 0) int SecureMultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cbMultiByte) LPCSTR lpMultiByteStr, _In_ int cbMultiByte, _Out_ std::vector<wchar_t, _Ax> &sWideCharStr) noexcept
{
    WCHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(WCHAR)];

    // Try to convert to stack buffer first.
    int cch = ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, szStackBuffer, _countof(szStackBuffer));
    if (cch) {
        // Copy from stack.
        sWideCharStr.assign(szStackBuffer, szStackBuffer + cch);
    } else if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Query the required output size. Allocate buffer. Then convert again.
        cch = ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, NULL, 0);
        sWideCharStr.resize(cch);
        cch = ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, sWideCharStr.data(), cch);
    }

    SecureZeroMemory(szStackBuffer, sizeof(szStackBuffer));

    return cch;
}

///
/// Maps a character string to a UTF-16 (wide character) std::wstring. The character string is not necessarily from a multibyte character set.
///
/// \note This function cleans all internal buffers using SecureZeroMemory() before returning.
///
/// \sa [MultiByteToWideChar function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd319072.aspx)
///
template<class _Traits1, class _Ax1, class _Traits2, class _Ax2>
static _Success_(return != 0) int SecureMultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_ const std::basic_string<char, _Traits1, _Ax1> &sMultiByteStr, _Out_ std::basic_string<wchar_t, _Traits2, _Ax2> &sWideCharStr) noexcept
{
    WCHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(WCHAR)];

    // Try to convert to stack buffer first.
    int cch = ::MultiByteToWideChar(CodePage, dwFlags, sMultiByteStr.c_str(), (int)sMultiByteStr.length(), szStackBuffer, _countof(szStackBuffer));
    if (cch) {
        // Copy from stack.
        sWideCharStr.assign(szStackBuffer, cch);
    } else if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Query the required output size. Allocate buffer. Then convert again.
        cch = ::MultiByteToWideChar(CodePage, dwFlags, sMultiByteStr.c_str(), (int)sMultiByteStr.length(), NULL, 0);
        std::unique_ptr<WCHAR[]> szBuffer(new WCHAR[cch]);
        cch = ::MultiByteToWideChar(CodePage, dwFlags, sMultiByteStr.c_str(), (int)sMultiByteStr.length(), szBuffer.get(), cch);
        sWideCharStr.assign(szBuffer.get(), cch);
        SecureZeroMemory(szBuffer.get(), sizeof(WCHAR)*cch);
    }

    SecureZeroMemory(szStackBuffer, sizeof(szStackBuffer));

    return cch;
}

///
/// Normalizes characters of a text string according to Unicode 4.0 TR#15.
///
/// \sa [NormalizeString function](https://docs.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-normalizestring)
///
template<class _Traits, class _Ax>
static _Success_(return > 0) int NormalizeString(_In_ NORM_FORM NormForm, _In_ LPCWSTR lpSrcString, _In_ int cwSrcLength, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sDstString) noexcept
{
    WCHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(WCHAR)];

    // Try to convert to stack buffer first.
    int cch = ::NormalizeString(NormForm, lpSrcString, cwSrcLength, szStackBuffer, _countof(szStackBuffer));
    if (cch > 0) {
        // Copy from stack.
        sDstString.assign(szStackBuffer, cwSrcLength != -1 ? wcsnlen(szStackBuffer, cch) : (size_t)cch - 1);
    } else {
        switch (::GetLastError()) {
            case ERROR_INSUFFICIENT_BUFFER:
                for (int i = 10; i--;) {
                    // Allocate buffer. Then convert again.
                    cch = -cch;
                    std::unique_ptr<WCHAR[]> szBuffer(new WCHAR[cch]);
                    cch = ::NormalizeString(NormForm, lpSrcString, cwSrcLength, szBuffer.get(), cch);
                    if (cch > 0) {
                        sDstString.assign(szBuffer.get(), cwSrcLength != -1 ? wcsnlen(szStackBuffer, cch) : (size_t)cch - 1);
                        break;
                    }
                    if (::GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
                        sDstString.clear();
                        break;
                    }
                }
                break;

            case ERROR_SUCCESS:
                sDstString.clear();
                break;
        }
    }

    return cch;
}

///
/// Normalizes characters of a text string according to Unicode 4.0 TR#15.
///
/// \sa [NormalizeString function](https://docs.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-normalizestring)
///
template<class _Traits1, class _Ax1, class _Traits2, class _Ax2>
static _Success_(return > 0) int NormalizeString(_In_ NORM_FORM NormForm, _In_ const std::basic_string<wchar_t, _Traits1, _Ax1> &sSrcString, _Out_ std::basic_string<wchar_t, _Traits2, _Ax2> &sDstString) noexcept
{
    WCHAR szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(WCHAR)];

    // Try to convert to stack buffer first.
    int cch = ::NormalizeString(NormForm, sSrcString.c_str(), (int)sSrcString.length(), szStackBuffer, _countof(szStackBuffer));
    if (cch > 0) {
        // Copy from stack.
        sDstString.assign(szStackBuffer, cch);
    } else {
        switch (::GetLastError()) {
            case ERROR_INSUFFICIENT_BUFFER:
                for (int i = 10; i--;) {
                    // Allocate buffer. Then convert again.
                    cch = -cch;
                    std::unique_ptr<WCHAR[]> szBuffer(new WCHAR[cch]);
                    cch = ::NormalizeString(NormForm, sSrcString.c_str(), (int)sSrcString.length(), szBuffer.get(), cch);
                    if (cch > 0) {
                        sDstString.assign(szBuffer.get(), cch);
                        break;
                    }
                    if (::GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
                        sDstString.clear();
                        break;
                    }
                }
                break;

            case ERROR_SUCCESS:
                sDstString.clear();
                break;
        }
    }

    return cch;
}

/// @copydoc LoadStringW
template<class _Traits, class _Ax>
static _Success_(return != 0) int WINAPI LoadStringA(_In_opt_ HINSTANCE hInstance, _In_ UINT uID, _Out_ std::basic_string<char, _Traits, _Ax> &sBuffer) noexcept
{
    // Get read-only pointer to string resource.
    LPCSTR pszStr;
    int i = LoadStringA(hInstance, uID, reinterpret_cast<LPSTR>(&pszStr), 0);
    if (i) {
        sBuffer.assign(pszStr, i);
        return i;
    } else
        return 0;
}

///
/// Loads a string resource from the executable file associated with a specified module.
///
/// \sa [LoadString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms647486.aspx)
///
template<class _Traits, class _Ax>
static _Success_(return != 0) int WINAPI LoadStringW(_In_opt_ HINSTANCE hInstance, _In_ UINT uID, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sBuffer) noexcept
{
    // Get read-only pointer to string resource.
    LPCWSTR pszStr;
    int i = LoadStringW(hInstance, uID, reinterpret_cast<LPWSTR>(&pszStr), 0);
    if (i) {
        sBuffer.assign(pszStr, i);
        return i;
    } else
        return 0;
}

///
/// Formats and sends a string to the debugger for display.
///
/// \sa [OutputDebugString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363362.aspx)
///
static VOID OutputDebugStrV(_In_z_ LPCSTR lpOutputString, _In_ va_list arg) noexcept
{
    std::string str;
    try { vsprintf(str, lpOutputString, arg); } catch (...) { return; }
    OutputDebugStringA(str.c_str());
}

///
/// Formats and sends a string to the debugger for display.
///
/// \sa [OutputDebugString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363362.aspx)
///
static VOID OutputDebugStrV(_In_z_ LPCWSTR lpOutputString, _In_ va_list arg) noexcept
{
    std::wstring str;
    try { vsprintf(str, lpOutputString, arg); } catch (...) { return; }
    OutputDebugStringW(str.c_str());
}

///
/// Formats and sends a string to the debugger for display.
///
/// \sa [OutputDebugString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363362.aspx)
///
static VOID OutputDebugStr(_In_z_ LPCSTR lpOutputString, ...) noexcept
{
    va_list arg;
    va_start(arg, lpOutputString);
    OutputDebugStrV(lpOutputString, arg);
    va_end(arg);
}

///
/// Formats and sends a string to the debugger for display.
///
/// \sa [OutputDebugString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363362.aspx)
///
static VOID OutputDebugStr(_In_z_ LPCWSTR lpOutputString, ...) noexcept
{
    va_list arg;
    va_start(arg, lpOutputString);
    OutputDebugStrV(lpOutputString, arg);
    va_end(arg);
}

/// @copydoc GetDateFormatW()
template<class _Traits, class _Ax>
static _Success_(return != 0) int GetDateFormatA(_In_ LCID Locale, _In_ DWORD dwFlags, _In_opt_ const SYSTEMTIME *lpDate, _In_opt_z_ LPCSTR lpFormat, _Out_ std::basic_string<char, _Traits, _Ax> &sDate) noexcept
{
    int iResult = GetDateFormatA(Locale, dwFlags, lpDate, lpFormat, NULL, 0);
    if (iResult) {
        // Allocate buffer on heap and retry.
        std::unique_ptr<char[]> szBuffer(new char[iResult]);
        iResult = GetDateFormatA(Locale, dwFlags, lpDate, lpFormat, szBuffer.get(), iResult);
        sDate.assign(szBuffer.get(), iResult ? iResult - 1 : 0);
        return iResult;
    }

    return iResult;
}

///
/// Formats a date as a date string for a locale specified by the locale identifier. The function formats either a specified date or the local system date. 
///
/// \sa [GetDateFormat function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd318086.aspx)
///
template<class _Traits, class _Ax>
static _Success_(return != 0) int GetDateFormatW(_In_ LCID Locale, _In_ DWORD dwFlags, _In_opt_ const SYSTEMTIME *lpDate, _In_opt_z_ LPCWSTR lpFormat, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sDate) noexcept
{
    int iResult = GetDateFormatW(Locale, dwFlags, lpDate, lpFormat, NULL, 0);
    if (iResult) {
        // Allocate buffer on heap and retry.
        std::unique_ptr<wchar_t[]> szBuffer(new wchar_t[iResult]);
        iResult = GetDateFormatW(Locale, dwFlags, lpDate, lpFormat, szBuffer.get(), iResult);
        sDate.assign(szBuffer.get(), iResult ? iResult - 1 : 0);
        return iResult;
    }

    return iResult;
}

/// @copydoc LookupAccountSidW()
template<class _Traits, class _Ax>
static _Success_(return != 0) BOOL LookupAccountSidA(_In_opt_z_ LPCSTR lpSystemName, _In_ PSID lpSid, _Out_opt_ std::basic_string<char, _Traits, _Ax> *sName, _Out_opt_ std::basic_string<char, _Traits, _Ax> *sReferencedDomainName, _Out_ PSID_NAME_USE peUse) noexcept
{
    assert(0); // TODO: Test this code.

    DWORD dwNameLen = 0, dwRefDomainLen = 0;

    if (LookupAccountSidA(lpSystemName, lpSid,
        NULL, &dwNameLen     ,
        NULL, &dwRefDomainLen,
        peUse))
    {
        // Name and domain is blank.
        if (sName                ) sName                ->clear();
        if (sReferencedDomainName) sReferencedDomainName->clear();
        return TRUE;
    } else if (GetLastError() == ERROR_MORE_DATA) {
        // Allocate on heap and retry.
        std::unique_ptr<char[]> bufName     (new char[dwNameLen     ]);
        std::unique_ptr<char[]> bufRefDomain(new char[dwRefDomainLen]);
        if (LookupAccountSidA(lpSystemName, lpSid,
            bufName     .get(), &dwNameLen     ,
            bufRefDomain.get(), &dwRefDomainLen,
            peUse))
        {
            if (sName                ) sName                ->assign(bufName     .get(), dwNameLen      - 1);
            if (sReferencedDomainName) sReferencedDomainName->assign(bufRefDomain.get(), dwRefDomainLen - 1);
            return TRUE;
        }
    }

    return FALSE;
}

///
/// Retrieves the name of the account for this SID and the name of the first domain on which this SID is found.
///
/// \sa [LookupAccountSid function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379166.aspx)
///
template<class _Traits, class _Ax>
static _Success_(return != 0) BOOL LookupAccountSidW(_In_opt_z_ LPCWSTR lpSystemName, _In_ PSID lpSid, _Out_opt_ std::basic_string<wchar_t, _Traits, _Ax> *sName, _Out_opt_ std::basic_string<wchar_t, _Traits, _Ax> *sReferencedDomainName, _Out_ PSID_NAME_USE peUse) noexcept
{
    assert(0); // TODO: Test this code.

    DWORD dwNameLen = 0, dwRefDomainLen = 0;

    if (LookupAccountSidW(lpSystemName, lpSid,
        NULL, &dwNameLen     ,
        NULL, &dwRefDomainLen,
        peUse))
    {
        // Name and domain is blank.
        if (sName                ) sName                ->clear();
        if (sReferencedDomainName) sReferencedDomainName->clear();
        return TRUE;
    } else if (GetLastError() == ERROR_MORE_DATA) {
        // Allocate on heap and retry.
        std::unique_ptr<wchar_t[]> bufName     (new wchar_t[dwNameLen     ]);
        std::unique_ptr<wchar_t[]> bufRefDomain(new wchar_t[dwRefDomainLen]);
        if (LookupAccountSidW(lpSystemName, lpSid,
            bufName     .get(), &dwNameLen     ,
            bufRefDomain.get(), &dwRefDomainLen,
            peUse))
        {
            if (sName                ) sName                ->assign(bufName     .get(), dwNameLen      - 1);
            if (sReferencedDomainName) sReferencedDomainName->assign(bufRefDomain.get(), dwRefDomainLen - 1);
            return TRUE;
        }
    }

    return FALSE;
}

///
/// Creates a SID for predefined aliases.
///
/// \sa [CreateWellKnownSid function](https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-createwellknownsid)
///
static _Success_(return != FALSE) BOOL CreateWellKnownSid(_In_ WELL_KNOWN_SID_TYPE WellKnownSidType, _In_opt_ PSID DomainSid, _Inout_ std::unique_ptr<SID> &Sid)
{
    BYTE szStackBuffer[WINSTD_STACK_BUFFER_BYTES];
    DWORD dwSize = sizeof(szStackBuffer);

    if (CreateWellKnownSid(WellKnownSidType, DomainSid, szStackBuffer, &dwSize)) {
        // The stack buffer was big enough to retrieve complete data. Alloc and copy.
        Sid.reset((SID*)new BYTE[dwSize]);
        memcpy(Sid.get(), szStackBuffer, dwSize);
        return TRUE;
    } else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // The stack buffer was too small to retrieve complete data. Alloc and retry.
        Sid.reset((SID*)new BYTE[dwSize]);
        return CreateWellKnownSid(WellKnownSidType, DomainSid, Sid.get(), &dwSize);
    } else
        return FALSE;
}

///
/// Retrieves a specified type of information about an access token. The calling process must have appropriate access rights to obtain the information.
///
/// \sa [GetTokenInformation function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa446671.aspx)
///
template<class _Ty>
static _Success_(return != 0) BOOL GetTokenInformation(_In_ HANDLE TokenHandle, _In_ TOKEN_INFORMATION_CLASS TokenInformationClass, _Out_ std::unique_ptr<_Ty> &TokenInformation) noexcept
{
    BYTE szStackBuffer[WINSTD_STACK_BUFFER_BYTES];
    DWORD dwSize;

    if (GetTokenInformation(TokenHandle, TokenInformationClass, szStackBuffer, sizeof(szStackBuffer), &dwSize)) {
        // The stack buffer was big enough to retrieve complete data. Alloc and copy.
        TokenInformation.reset((_Ty*)(new BYTE[dwSize]));
        memcpy(TokenInformation.get(), szStackBuffer, dwSize);
        return TRUE;
    } else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // The stack buffer was too small to retrieve complete data. Alloc and retry.
        TokenInformation.reset((_Ty*)(new BYTE[dwSize]));
        return GetTokenInformation(TokenHandle, TokenInformationClass, TokenInformation.get(), dwSize, &dwSize);
    } else
        return FALSE;
}

///
/// Retrieves the full name of the executable image for the specified process.
///
/// \sa [QueryFullProcessImageNameA function](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-queryfullprocessimagenamea)
///
template<class _Traits, class _Ax>
static _Success_(return != 0) BOOL QueryFullProcessImageNameA(_In_ HANDLE hProcess, _In_ DWORD dwFlags, _Inout_ std::basic_string<char, _Traits, _Ax>& sExeName)
{
    char szStackBuffer[WINSTD_STACK_BUFFER_BYTES / sizeof(char)];
    DWORD dwSize = _countof(szStackBuffer);

    // Try with stack buffer first.
    if (::QueryFullProcessImageNameA(hProcess, dwFlags, szStackBuffer, &dwSize)) {
        // Copy from stack.
        sExeName.assign(szStackBuffer, dwSize);
        return TRUE;
    }
    for (DWORD dwCapacity = 2 * WINSTD_STACK_BUFFER_BYTES / sizeof(char); GetLastError() == ERROR_INSUFFICIENT_BUFFER; dwCapacity *= 2) {
        // Allocate on heap and retry.
        std::unique_ptr<char[]> szBuffer(new char[dwCapacity]);
        dwSize = dwCapacity;
        if (::QueryFullProcessImageNameA(hProcess, dwFlags, szBuffer.get(), &dwSize)) {
            sExeName.assign(szBuffer.get(), dwSize);
            return TRUE;
        }
    }
    return FALSE;
}

///
/// Retrieves the full name of the executable image for the specified process.
///
/// \sa [QueryFullProcessImageNameW function](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-queryfullprocessimagenamew)
///
template<class _Traits, class _Ax>
static _Success_(return != 0) BOOL QueryFullProcessImageNameW(_In_ HANDLE hProcess, _In_ DWORD dwFlags, _Inout_ std::basic_string<wchar_t, _Traits, _Ax>& sExeName)
{
    wchar_t szStackBuffer[WINSTD_STACK_BUFFER_BYTES / sizeof(wchar_t)];
    DWORD dwSize = _countof(szStackBuffer);

    // Try with stack buffer first.
    if (::QueryFullProcessImageNameW(hProcess, dwFlags, szStackBuffer, &dwSize)) {
        // Copy from stack.
        sExeName.assign(szStackBuffer, dwSize);
        return TRUE;
    }
    for (DWORD dwCapacity = 2 * WINSTD_STACK_BUFFER_BYTES / sizeof(wchar_t); GetLastError() == ERROR_INSUFFICIENT_BUFFER; dwCapacity *= 2) {
        // Allocate on heap and retry.
        std::unique_ptr<wchar_t[]> szBuffer(new wchar_t[dwCapacity]);
        dwSize = dwCapacity;
        if (::QueryFullProcessImageNameW(hProcess, dwFlags, szBuffer.get(), &dwSize)) {
            sExeName.assign(szBuffer.get(), dwSize);
            return TRUE;
        }
    }
    return FALSE;
}

/// @}

#pragma warning(pop)

namespace winstd
{
    /// \addtogroup WinStdWinAPI
    /// @{

    ///
    /// Windows HANDLE wrapper class
    ///
    template<HANDLE INVALID>
    class win_handle : public handle<HANDLE, INVALID>
    {
        WINSTD_HANDLE_IMPL(win_handle, INVALID)

    public:
        ///
        /// Closes an open object handle.
        ///
        /// \sa [CloseHandle function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724211.aspx)
        ///
        virtual ~win_handle()
        {
            if (m_h != invalid)
                free_internal();
        }

    protected:
        ///
        /// Closes an open object handle.
        ///
        /// \sa [CloseHandle function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724211.aspx)
        ///
        void free_internal() noexcept override
        {
            CloseHandle(m_h);
        }
    };

    ///
    /// Module handle wrapper
    ///
    /// \sa [LoadLibraryEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms684179.aspx)
    ///
    class library : public handle<HMODULE, NULL>
    {
        WINSTD_HANDLE_IMPL(library, NULL)

    public:
        ///
        /// Frees the module.
        ///
        /// \sa [FreeLibrary function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms683152.aspx)
        ///
        virtual ~library()
        {
            if (m_h != invalid)
                free_internal();
        }

    protected:
        ///
        /// Frees the module.
        ///
        /// \sa [FreeLibrary function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms683152.aspx)
        ///
        void free_internal() noexcept override
        {
            FreeLibrary(m_h);
        }
    };

    ///
    /// Process handle wrapper
    ///
    /// \sa [OpenProcess function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms684320.aspx)
    ///
    typedef win_handle<NULL> process;

    ///
    /// Thread handle wrapper
    ///
    /// \sa [CreateThread function](https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread)
    ///
    typedef win_handle<NULL> thread;

    ///
    /// Process snapshot handle wrapper
    ///
    /// \sa [CreateToolhelp32Snapshot function](https://docs.microsoft.com/en-us/windows/win32/api/tlhelp32/nf-tlhelp32-createtoolhelp32snapshot)
    ///
    typedef win_handle<INVALID_HANDLE_VALUE> process_snapshot;

    ///
    /// File handle wrapper
    ///
    /// \sa [CreateFile function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363858.aspx)
    /// \sa [CreateNamedPipe function](https://docs.microsoft.com/en-us/windows/win32/api/namedpipeapi/nf-namedpipeapi-createnamedpipew)
    ///
    typedef win_handle<INVALID_HANDLE_VALUE> file;

    ///
    /// File mapping
    ///
    /// \sa [CreateFileMapping function](https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-createfilemappingw)
    ///
    typedef win_handle<NULL> file_mapping;

    ///
    /// Deleter for unique_ptr using UnmapViewOfFile
    ///
    template <class _Ty> struct UnmapViewOfFile_delete
    {
        typedef UnmapViewOfFile_delete<_Ty> _Myt; ///< This type

        ///
        /// Default construct
        ///
        UnmapViewOfFile_delete() {}

        ///
        /// Construct from another UnmapViewOfFile_delete
        ///
        template <class _Ty2> UnmapViewOfFile_delete(const UnmapViewOfFile_delete<_Ty2>&) {}

        ///
        /// Delete a pointer
        ///
        void operator()(_Ty* _Ptr) const
        {
            if (!UnmapViewOfFile(_Ptr))
                throw win_runtime_error("UnmapViewOfFile failed");
        }
    };

    ///
    /// Deleter for unique_ptr to array of unknown size using UnmapViewOfFile
    ///
    template <class _Ty> struct UnmapViewOfFile_delete<_Ty[]>
    {
        typedef UnmapViewOfFile_delete<_Ty> _Myt; ///< This type

        ///
        /// Default construct
        ///
        UnmapViewOfFile_delete() {}

        ///
        /// Delete a pointer
        ///
        void operator()(_Ty* _Ptr) const
        {
            if (!UnmapViewOfFile(_Ptr))
                throw win_runtime_error("UnmapViewOfFile failed");
        }

        ///
        /// Delete a pointer of another type
        ///
        template<class _Other>
        void operator()(_Other*) const
        {
            if (!UnmapViewOfFile(_Ptr))
                throw win_runtime_error("UnmapViewOfFile failed");
        }
    };

    ///
    /// Event handle wrapper
    ///
    /// \sa [CreateEventW function](https://docs.microsoft.com/en-us/windows/desktop/api/synchapi/nf-synchapi-createeventw)
    /// \sa [OpenEventW function](https://docs.microsoft.com/en-us/windows/desktop/api/synchapi/nf-synchapi-openeventw)
    ///
    typedef win_handle<NULL> event;

    ///
    /// Critical section wrapper
    ///
    class critical_section
    {
        WINSTD_NONCOPYABLE(critical_section)
        WINSTD_NONMOVABLE(critical_section)

    public:
        ///
        /// Construct the object and initializes a critical section object
        ///
        /// \sa [InitializeCriticalSection function](https://docs.microsoft.com/en-us/windows/desktop/api/synchapi/nf-synchapi-initializecriticalsection)
        ///
        critical_section() noexcept
        {
            InitializeCriticalSection(&m_data);
        }

        ///
        /// Releases all resources used by an unowned critical section object.
        ///
        /// \sa [DeleteCriticalSection function](https://docs.microsoft.com/en-us/windows/desktop/api/synchapi/nf-synchapi-deletecriticalsection)
        ///
        virtual ~critical_section()
        {
            DeleteCriticalSection(&m_data);
        }

        ///
        /// Auto-typecasting operator
        ///
        /// \return Pointer to critical section
        ///
        operator LPCRITICAL_SECTION() noexcept
        {
            return &m_data;
        }

    protected:
        CRITICAL_SECTION m_data;    ///< Critical section struct
    };

    ///
    /// Find-file handle wrapper
    ///
    /// \sa [FindFirstFile function](https://docs.microsoft.com/en-us/windows/desktop/api/fileapi/nf-fileapi-findfirstfilew)
    ///
    class find_file : public handle<HANDLE, INVALID_HANDLE_VALUE>
    {
        WINSTD_HANDLE_IMPL(find_file, INVALID_HANDLE_VALUE)

    public:
        ///
        /// Closes a file search handle.
        ///
        /// \sa [FindClose function](https://docs.microsoft.com/en-us/windows/desktop/api/fileapi/nf-fileapi-findclose)
        ///
        virtual ~find_file()
        {
            if (m_h != invalid)
                free_internal();
        }

    protected:
        ///
        /// Closes a file search handle.
        ///
        /// \sa [FindClose function](https://docs.microsoft.com/en-us/windows/desktop/api/fileapi/nf-fileapi-findclose)
        ///
        void free_internal() noexcept override
        {
            FindClose(m_h);
        }
    };

    ///
    /// Heap handle wrapper
    ///
    /// \sa [HeapCreate function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa366599.aspx)
    ///
    class heap : public handle<HANDLE, NULL>
    {
        WINSTD_HANDLE_IMPL(heap, NULL)

    public:
        ///
        /// Destroys the heap.
        ///
        /// \sa [HeapDestroy function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa366700.aspx)
        ///
        virtual ~heap()
        {
            if (m_h != invalid)
                free_internal();
        }

        ///
        /// Enumerates allocated heap blocks using `OutputDebugString()`
        ///
        /// \returns
        /// - `true` if any blocks found;
        /// - `false` otherwise.
        ///
        bool enumerate() noexcept
        {
            assert(m_h != invalid);

            bool found = false;

            // Lock the heap for exclusive access.
            HeapLock(m_h);

            PROCESS_HEAP_ENTRY e;
            e.lpData = NULL;
            while (HeapWalk(m_h, &e) != FALSE) {
                if ((e.wFlags & PROCESS_HEAP_ENTRY_BUSY) != 0) {
                    OutputDebugStr(
                        _T("Allocated block%s%s\n")
                        _T("  Data portion begins at: %#p\n  Size: %d bytes\n")
                        _T("  Overhead: %d bytes\n  Region index: %d\n"),
                            (e.wFlags & PROCESS_HEAP_ENTRY_MOVEABLE) != 0 ? tstring_printf(_T(", movable with HANDLE %#p"), e.Block.hMem).c_str() : _T(""),
                            (e.wFlags & PROCESS_HEAP_ENTRY_DDESHARE) != 0 ? _T(", DDESHARE") : _T(""),
                             e.lpData,
                             e.cbData,
                             e.cbOverhead,
                             e.iRegionIndex);

                    found = true;
                }
            }

            const DWORD dwResult = GetLastError();
            if (dwResult != ERROR_NO_MORE_ITEMS)
                OutputDebugStr(_T("HeapWalk failed (error %u).\n"), dwResult);

            // Unlock the heap.
            HeapUnlock(m_h);

            return found;
        }

    protected:
        ///
        /// Destroys the heap.
        ///
        /// \sa [HeapDestroy function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa366700.aspx)
        ///
        void free_internal() noexcept override
        {
            enumerate();
            HeapDestroy(m_h);
        }
    };

    ///
    /// HeapAlloc allocator
    ///
    template <class _Ty>
    class heap_allocator
    {
    public:
        typedef typename _Ty value_type;    ///< A type that is managed by the allocator

        typedef _Ty *pointer;               ///< A type that provides a pointer to the type of object managed by the allocator
        typedef _Ty& reference;             ///< A type that provides a reference to the type of object managed by the allocator
        typedef const _Ty *const_pointer;   ///< A type that provides a constant pointer to the type of object managed by the allocator
        typedef const _Ty& const_reference; ///< A type that provides a constant reference to type of object managed by the allocator

        typedef SIZE_T size_type;           ///< An unsigned integral type that can represent the length of any sequence that an object of template class `heap_allocator` can allocate
        typedef ptrdiff_t difference_type;  ///< A signed integral type that can represent the difference between values of pointers to the type of object managed by the allocator

        ///
        /// A structure that enables an allocator for objects of one type to allocate storage for objects of another type.
        ///
        template <class _Other>
        struct rebind
        {
            typedef heap_allocator<_Other> other;   ///< Other allocator type
        };

    public:
        ///
        /// Constructs allocator
        ///
        /// \param[in] heap  Handle to existing heap
        ///
        heap_allocator(_In_ HANDLE heap) : m_heap(heap)
        {
        }

        ///
        /// Constructs allocator from another type
        ///
        /// \param[in] other  Another allocator of the heap_allocator kind
        ///
        template <class _Other>
        heap_allocator(_In_ const heap_allocator<_Other> &other) : m_heap(other.m_heap)
        {
        }

        ///
        /// Allocates a new memory block
        ///
        /// \param[in] count  Number of elements
        ///
        /// \returns Pointer to new memory block
        ///
        pointer allocate(_In_ size_type count)
        {
            assert(m_heap);
            return (pointer)HeapAlloc(m_heap, 0, count * sizeof(_Ty));
        }

        ///
        /// Frees memory block
        ///
        /// \param[in] ptr   Pointer to memory block
        /// \param[in] size  Size of memory block (in bytes)
        ///
        void deallocate(_In_ pointer ptr, _In_ size_type size)
        {
            UNREFERENCED_PARAMETER(size);
            assert(m_heap);
            HeapFree(m_heap, 0, ptr);
        }

        ///
        /// Calls copying constructor for the element
        ///
        /// \param[in] ptr  Pointer to memory block
        /// \param[in] val  Source element
        ///
        void construct(_Inout_ pointer ptr, _In_ const _Ty& val)
        {
            ::new ((void*)ptr) _Ty(val);
        }

        ///
        /// Calls moving constructor for the element
        ///
        /// \param[in] ptr  Pointer to memory block
        /// \param[in] val  Source element
        ///
        void construct(_Inout_ pointer ptr, _Inout_ _Ty&& val)
        {
            ::new ((void*)ptr) _Ty(std::forward<_Ty>(val));
        }

        ///
        /// Calls destructor for the element
        ///
        /// \param[in] ptr  Pointer to memory block
        ///
        void destroy(_Inout_ pointer ptr)
        {
            ptr->_Ty::~_Ty();
        }

        ///
        /// Returns maximum memory block size
        ///
        size_type max_size() const
        {
            return (SIZE_T)-1;
        }

    public:
        HANDLE m_heap;  ///< Heap handle
    };

    ///
    /// Activates given activation context in constructor and deactivates it in destructor
    ///
    class actctx_activator
    {
        WINSTD_NONCOPYABLE(actctx_activator)
        WINSTD_NONMOVABLE(actctx_activator)

    public:
        ///
        /// Construct the activator and activates the given activation context
        ///
        /// \param[in] hActCtx  Activation context
        ///
        /// \sa [ActivateActCtx function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa374151.aspx)
        ///
        actctx_activator(_In_ HANDLE hActCtx) noexcept
        {
            if (!ActivateActCtx(hActCtx, &m_cookie))
                m_cookie = 0;
        }

        ///
        /// Deactivates activation context and destructs the activator
        ///
        /// \sa [DeactivateActCtx function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa375140.aspx)
        ///
        virtual ~actctx_activator()
        {
            if (m_cookie)
                DeactivateActCtx(0, m_cookie);
        }

    protected:
        ULONG_PTR m_cookie; ///< Cookie for context deactivation
    };

    ///
    /// Base class for thread impersonation of another security context
    ///
    class impersonator
    {
    public:
        ///
        /// Construct the impersonator
        ///
        impersonator() noexcept : m_cookie(FALSE) {}

        ///
        /// Reverts to current user and destructs the impersonator
        ///
        /// \sa [RevertToSelf function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379317.aspx)
        ///
        virtual ~impersonator()
        {
            if (m_cookie)
                RevertToSelf();
        }

        ///
        /// Did impersonation succeed?
        ///
        operator bool () const { return m_cookie; }

    protected:
        BOOL m_cookie; ///< Did impersonation succeed?
    };

    ///
    /// Lets the calling thread impersonate the security context of a logged-on user
    ///
    class user_impersonator : public impersonator
    {
        WINSTD_NONCOPYABLE(user_impersonator)
        WINSTD_NONMOVABLE(user_impersonator)

    public:
        ///
        /// Construct the impersonator and impersonates the given user
        ///
        /// \param[in] hToken  A handle to a primary or impersonation access token that represents a logged-on user or NULL for no impersonation.
        ///
        /// \sa [ImpersonateLoggedOnUser function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa378612.aspx)
        ///
        user_impersonator(_In_opt_ HANDLE hToken) noexcept
        {
            m_cookie = hToken && ImpersonateLoggedOnUser(hToken);
        }
    };

    ///
    /// Lets the calling thread impersonate the security context of the SYSTEM user
    ///
    class system_impersonator : public impersonator
    {
        WINSTD_NONCOPYABLE(system_impersonator)
        WINSTD_NONMOVABLE(system_impersonator)

    public:
        ///
        /// Construct the impersonator and impersonates the SYSTEM user
        ///
        system_impersonator() noexcept
        {
            TOKEN_PRIVILEGES privileges = { 1, {{{ 0, 0 }, SE_PRIVILEGE_ENABLED }} };
            if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &privileges.Privileges[0].Luid) ||
                !ImpersonateSelf(SecurityImpersonation))
                return;

            {
                HANDLE h;
                if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES, FALSE, &h))
                    goto revert;
                win_handle<INVALID_HANDLE_VALUE> thread_token(h);
                if (!AdjustTokenPrivileges(thread_token, FALSE, &privileges, sizeof(privileges), NULL, NULL))
                    goto revert;
                process_snapshot process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                if (!process_snapshot)
                    goto revert;
                PROCESSENTRY32 entry = { sizeof(PROCESSENTRY32) };
                if (!Process32First(process_snapshot, &entry))
                    goto revert;
                while (_tcsicmp(entry.szExeFile, TEXT("winlogon.exe")) != 0)
                    if (!Process32Next(process_snapshot, &entry))
                        goto revert;
                process winlogon_process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, entry.th32ProcessID);
                if (!winlogon_process)
                    goto revert;
                if (!OpenProcessToken(winlogon_process, TOKEN_IMPERSONATE | TOKEN_DUPLICATE, &h))
                    goto revert;
                win_handle<INVALID_HANDLE_VALUE> winlogon_token(h);
                if (!DuplicateToken(winlogon_token, SecurityImpersonation, &h))
                    goto revert;
                win_handle<INVALID_HANDLE_VALUE> duplicated_token(h);
                if (!SetThreadToken(NULL, duplicated_token))
                    goto revert;
            }

            m_cookie = TRUE;
            return;

        revert:
            DWORD dwResult = GetLastError();
            RevertToSelf();
            SetLastError(dwResult);
        }
    };

    ///
    /// Console control handler stack management
    ///
    class console_ctrl_handler
    {
        WINSTD_NONCOPYABLE(console_ctrl_handler)
        WINSTD_NONMOVABLE(console_ctrl_handler)

    public:
        ///
        /// Construct the console control handler object and pushes the given handler to the console control handler stack
        ///
        /// \param[in] HandlerRoutine  A pointer to the application-defined HandlerRoutine function to be added.
        ///
        /// \sa [SetConsoleCtrlHandler function](https://docs.microsoft.com/en-us/windows/console/setconsolectrlhandler)
        ///
        console_ctrl_handler(_In_opt_ PHANDLER_ROUTINE HandlerRoutine) noexcept : m_handler(HandlerRoutine)
        {
            m_cookie = SetConsoleCtrlHandler(m_handler, TRUE);
        }

        ///
        /// Pops console control handler from the console control handler stack
        ///
        /// \sa [SetConsoleCtrlHandler function](https://docs.microsoft.com/en-us/windows/console/setconsolectrlhandler)
        ///
        virtual ~console_ctrl_handler()
        {
            if (m_cookie)
                SetConsoleCtrlHandler(m_handler, FALSE);
        }

    protected:
        BOOL m_cookie;              ///< Did pushing the console control handler succeed?
        PHANDLER_ROUTINE m_handler; ///< Pointer to console control handler
    };

    ///
    /// Memory in virtual address space of a process handle wrapper
    ///
    class vmemory : public handle<LPVOID, NULL>
    {
        WINSTD_NONCOPYABLE(vmemory)

    public:
        ///
        /// Initializes a new class instance with the memory handle set to INVAL.
        ///
        vmemory() noexcept : m_proc(NULL)
        {
        }

        ///
        /// Initializes a new class instance with an already available object handle.
        ///
        /// \param[in] proc  Handle of process the memory belongs to
        /// \param[in] h     Initial object handle value
        ///
        vmemory(_In_ handle_type h, _In_ HANDLE proc) noexcept :
            m_proc(proc),
            handle<LPVOID, NULL>(h)
        {
        }

        ///
        /// Move constructor
        ///
        /// \param[inout] h  A rvalue reference of another object
        ///
        vmemory(_Inout_ vmemory &&h) noexcept :
            m_proc(std::move(h.m_proc)),
            handle<LPVOID, NULL>(std::move(h))
        {
        }

        ///
        /// Frees the memory.
        ///
        /// \sa [VirtualFreeEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa366894.aspx)
        ///
        virtual ~vmemory()
        {
            if (m_h != invalid)
                VirtualFreeEx(m_proc, m_h, 0, MEM_RELEASE);
        }

        ///
        /// Move assignment
        ///
        /// \param[inout] other  A rvalue reference of another object
        ///
        vmemory& operator=(_Inout_ vmemory &&other) noexcept
        {
            if (this != std::addressof(other)) {
                (handle<handle_type, NULL>&&)*this = std::move(other);
                m_proc                             = std::move(other.m_proc);
            }
            return *this;
        }

        ///
        /// Sets a new memory handle for the class
        ///
        /// When the current object handle of the class is non-INVAL, the object is destroyed first.
        ///
        /// \param[in] proc  Handle of process the memory belongs to
        /// \param[in] h     Initial object handle value
        ///
        void attach(_In_ HANDLE proc, _In_opt_ handle_type h) noexcept
        {
            m_proc = proc;
            if (m_h != invalid)
                free_internal();
            m_h    = h;
        }

        ///
        /// Reserves, commits, or changes the state of a region of memory within the virtual address space of a specified process. The function initializes the memory it allocates to zero.
        ///
        /// \sa [VirtualAllocEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa366890.aspx)
        ///
        /// \return
        /// - \c true when succeeds;
        /// - \c false when fails. Use `GetLastError()` for failure reason.
        ///
        bool alloc(
            _In_     HANDLE hProcess,
            _In_opt_ LPVOID lpAddress,
            _In_     SIZE_T dwSize,
            _In_     DWORD  flAllocationType,
            _In_     DWORD  flProtect) noexcept
        {
            handle_type h = VirtualAllocEx(hProcess, lpAddress, dwSize, flAllocationType, flProtect);
            if (h != invalid) {
                attach(hProcess, h);
                return true;
            } else
                return false;
        }

    protected:
        ///
        /// Frees the memory.
        ///
        /// \sa [VirtualFreeEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa366894.aspx)
        ///
        void free_internal() noexcept override
        {
            VirtualFreeEx(m_proc, m_h, 0, MEM_RELEASE);
        }

    protected:
        HANDLE m_proc;  ///< Handle of memory's process
    };

    ///
    /// Registry key wrapper class
    ///
    /// \sa [RegCreateKeyEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724844.aspx)
    /// \sa [RegOpenKeyEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724897.aspx)
    ///
    class reg_key : public handle<HKEY, NULL>
    {
        WINSTD_HANDLE_IMPL(reg_key, NULL)

    public:
        ///
        /// Closes a handle to the registry key.
        ///
        /// \sa [RegCloseKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724837.aspx)
        ///
        virtual ~reg_key()
        {
            if (m_h != invalid)
                free_internal();
        }

        ///
        /// Deletes the specified registry subkey.
        ///
        /// \param[in] szSubkey  Name of the subkey to delete
        ///
        /// \return
        /// - true when creation succeeds;
        /// - false when creation fails. For extended error information, call `GetLastError()`.
        ///
        bool delete_subkey(_In_z_ LPCTSTR szSubkey)
        {
            LSTATUS s;

            s = RegDeleteKey(m_h, szSubkey);
            if (s == ERROR_SUCCESS || s == ERROR_FILE_NOT_FOUND)
                return true;

            {
                reg_key k;
                handle_type h;
                s = RegOpenKeyEx(m_h, szSubkey, 0, KEY_ENUMERATE_SUB_KEYS, &h);
                if (s == ERROR_SUCCESS)
                    k.attach(h);
                else {
                    SetLastError(s);
                    return false;
                }
                for (;;) {
                    TCHAR szName[MAX_PATH];
                    DWORD dwSize = _countof(szName);
                    s = RegEnumKeyEx(k, 0, szName, &dwSize, NULL, NULL, NULL, NULL);
                    if (s == ERROR_SUCCESS)
                        k.delete_subkey(szName);
                    else if (s == ERROR_NO_MORE_ITEMS)
                        break;
                    else {
                        SetLastError(s);
                        return false;
                    }
                }
            }

            s = RegDeleteKey(m_h, szSubkey);
            if (s == ERROR_SUCCESS)
                return true;
            else {
                SetLastError(s);
                return false;
            }
        }

    protected:
        ///
        /// Closes a handle to the registry key.
        ///
        /// \sa [RegCloseKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724837.aspx)
        ///
        void free_internal() noexcept override
        {
            RegCloseKey(m_h);
        }
    };

    ///
    /// SID wrapper class
    ///
    class security_id : public handle<PSID, NULL>
    {
        WINSTD_HANDLE_IMPL(security_id, NULL)

    public:
        ///
        /// Closes a handle to the SID.
        ///
        /// \sa [FreeSid function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa446631.aspx)
        ///
        virtual ~security_id()
        {
            if (m_h != invalid)
                free_internal();
        }

    protected:
        ///
        /// Closes a handle to the SID.
        ///
        /// \sa [FreeSid function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa446631.aspx)
        ///
        void free_internal() noexcept override
        {
            FreeSid(m_h);
        }
    };

    ///
    /// PROCESS_INFORMATION struct wrapper
    ///
    class process_information : public PROCESS_INFORMATION
    {
        WINSTD_NONCOPYABLE(process_information)
        WINSTD_NONMOVABLE(process_information)

    public:
        ///
        /// Constructs blank PROCESS_INFORMATION
        ///
        process_information() noexcept
        {
            hProcess    = INVALID_HANDLE_VALUE;
            hThread     = INVALID_HANDLE_VALUE;
            dwProcessId = 0;
            dwThreadId  = 0;
        }

        ///
        /// Closes process and thread handles.
        ///
        ~process_information()
        {
            #pragma warning(push)
            #pragma warning(disable: 6001) // Using uninitialized memory '*this'. << ???

            if (hProcess != INVALID_HANDLE_VALUE)
                CloseHandle(hProcess);

            if (hThread != INVALID_HANDLE_VALUE)
                CloseHandle(hThread);

            #pragma warning(pop)
        }
    };

    ///
    /// Event log handle wrapper
    ///
    /// \sa [RegisterEventSource function](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-registereventsourcew)
    ///
    class event_log : public handle<HANDLE, NULL>
    {
        WINSTD_HANDLE_IMPL(event_log, NULL)

    public:
        ///
        /// Closes an event log handle.
        ///
        /// \sa [DeregisterEventSource function](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-deregistereventsource)
        ///
        virtual ~event_log()
        {
            if (m_h != invalid)
                free_internal();
        }

    protected:
        ///
        /// Closes an event log handle.
        ///
        /// \sa [DeregisterEventSource function](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-deregistereventsource)
        ///
        void free_internal() noexcept override
        {
            DeregisterEventSource(m_h);
        }
    };

    ///
    /// SC_HANDLE wrapper class
    ///
    class sc_handle : public handle<SC_HANDLE, NULL>
    {
        WINSTD_HANDLE_IMPL(sc_handle, NULL)

    public:
        ///
        /// Closes an open object handle.
        ///
        /// \sa [CloseServiceHandle function](https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-closeservicehandle)
        ///
        virtual ~sc_handle()
        {
            if (m_h != invalid)
                free_internal();
        }

    protected:
        ///
        /// Closes an open object handle.
        ///
        /// \sa [CloseServiceHandle function](https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-closeservicehandle)
        ///
        void free_internal() noexcept override
        {
            CloseServiceHandle(m_h);
        }
    };

    /// @}
}

/// \addtogroup WinStdWinAPI
/// @{

#pragma warning(push)
#pragma warning(disable: 4505) // Don't warn on unused code

/// @copydoc RegCreateKeyExW()
static LSTATUS RegCreateKeyExA(
    _In_ HKEY hKey,
    _In_ LPCSTR lpSubKey,
    _Reserved_ DWORD Reserved,
    _In_opt_ LPSTR lpClass,
    _In_ DWORD dwOptions,
    _In_ REGSAM samDesired,
    _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _Inout_ winstd::reg_key &result,
    _Out_opt_ LPDWORD lpdwDisposition)
{
    HKEY h;
    LSTATUS s = RegCreateKeyExA(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, &h, lpdwDisposition);
    if (s == ERROR_SUCCESS)
        result.attach(h);
    return s;
}

///
/// Creates the specified registry key. If the key already exists, the function opens it.
///
/// \sa [RegCreateKeyEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724844.aspx)
///
static LSTATUS RegCreateKeyExW(
    _In_ HKEY hKey,
    _In_ LPCWSTR lpSubKey,
    _Reserved_ DWORD Reserved,
    _In_opt_ LPWSTR lpClass,
    _In_ DWORD dwOptions,
    _In_ REGSAM samDesired,
    _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _Inout_ winstd::reg_key &result,
    _Out_opt_ LPDWORD lpdwDisposition)
{
    HKEY h;
    LSTATUS s = RegCreateKeyExW(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, &h, lpdwDisposition);
    if (s == ERROR_SUCCESS)
        result.attach(h);
    return s;
}

/// @copydoc RegOpenKeyExW()
static LSTATUS RegOpenKeyExA(
    _In_ HKEY hKey,
    _In_opt_ LPCSTR lpSubKey,
    _In_opt_ DWORD ulOptions,
    _In_ REGSAM samDesired,
    _Inout_ winstd::reg_key &result)
{
    HKEY h;
    LSTATUS s = RegOpenKeyExA(hKey, lpSubKey, ulOptions, samDesired, &h);
    if (s == ERROR_SUCCESS)
        result.attach(h);
    return s;
}

///
/// Opens the specified registry key.
///
/// \sa [RegOpenKeyEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724897.aspx)
///
static LSTATUS RegOpenKeyExW(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpSubKey,
    _In_opt_ DWORD ulOptions,
    _In_ REGSAM samDesired,
    _Inout_ winstd::reg_key &result)
{
    HKEY h;
    LSTATUS s = RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, &h);
    if (s == ERROR_SUCCESS)
        result.attach(h);
    return s;
}

///
/// Opens the access token associated with a process
///
/// \sa [OpenProcessToken function](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocesstoken)
///
static BOOL OpenProcessToken(_In_ HANDLE ProcessHandle, _In_ DWORD DesiredAccess, _Inout_ winstd::win_handle<NULL> &TokenHandle)
{
    HANDLE h;
    if (OpenProcessToken(ProcessHandle, DesiredAccess, &h)) {
        TokenHandle.attach(h);
        return TRUE;
    }
    return FALSE;
}

///
/// Creates a new access token that duplicates an existing token. This function can create either a primary token or an impersonation token.
///
/// \sa [DuplicateTokenEx function](https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-duplicatetokenex)
///
static BOOL DuplicateTokenEx(_In_ HANDLE hExistingToken, _In_ DWORD dwDesiredAccess, _In_opt_ LPSECURITY_ATTRIBUTES lpTokenAttributes, _In_ SECURITY_IMPERSONATION_LEVEL ImpersonationLevel, _In_ TOKEN_TYPE TokenType, _Inout_ winstd::win_handle<NULL> &NewToken)
{
    HANDLE h;
    if (DuplicateTokenEx(hExistingToken, dwDesiredAccess, lpTokenAttributes, ImpersonationLevel, TokenType, &h)) {
        NewToken.attach(h);
        return TRUE;
    }
    return FALSE;
}

///
/// Allocates and initializes a security identifier (SID) with up to eight subauthorities.
///
/// \sa [AllocateAndInitializeSid function](https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-allocateandinitializesid)
///
static BOOL AllocateAndInitializeSid(_In_ PSID_IDENTIFIER_AUTHORITY pIdentifierAuthority, _In_ BYTE nSubAuthorityCount, _In_ DWORD nSubAuthority0, _In_ DWORD nSubAuthority1, _In_ DWORD nSubAuthority2, _In_ DWORD nSubAuthority3, _In_ DWORD nSubAuthority4, _In_ DWORD nSubAuthority5, _In_ DWORD nSubAuthority6, _In_ DWORD nSubAuthority7, _Inout_ winstd::security_id& Sid)
{
    PSID h;
    if (AllocateAndInitializeSid(pIdentifierAuthority, nSubAuthorityCount, nSubAuthority0, nSubAuthority1, nSubAuthority2, nSubAuthority3, nSubAuthority4, nSubAuthority5, nSubAuthority6, nSubAuthority7, &h)) {
        Sid.attach(h);
        return TRUE;
    }
    return FALSE;
}

/// @copydoc SetEntriesInAclW()
static DWORD SetEntriesInAclA(_In_ ULONG cCountOfExplicitEntries, _In_reads_opt_(cCountOfExplicitEntries) PEXPLICIT_ACCESS_A pListOfExplicitEntries, _In_opt_ PACL OldAcl, _Inout_ std::unique_ptr<ACL, winstd::LocalFree_delete<ACL>>& Acl)
{
    PACL h;
    DWORD dwResult = SetEntriesInAclA(cCountOfExplicitEntries, pListOfExplicitEntries, OldAcl, &h);
    if (dwResult == ERROR_SUCCESS)
        Acl.reset(h);
    return ERROR_SUCCESS;
}

///
/// Creates a new access control list (ACL) by merging new access control or audit control information into an existing ACL structure.
///
/// \sa [SetEntriesInAclW function](https://learn.microsoft.com/en-us/windows/win32/api/aclapi/nf-aclapi-setentriesinaclw)
///
static DWORD SetEntriesInAclW(_In_ ULONG cCountOfExplicitEntries, _In_reads_opt_(cCountOfExplicitEntries) PEXPLICIT_ACCESS_W pListOfExplicitEntries, _In_opt_ PACL OldAcl, _Inout_ std::unique_ptr<ACL, winstd::LocalFree_delete<ACL>>& Acl)
{
    PACL h;
    DWORD dwResult = SetEntriesInAclW(cCountOfExplicitEntries, pListOfExplicitEntries, OldAcl, &h);
    if (dwResult == ERROR_SUCCESS)
        Acl.reset(h);
    return ERROR_SUCCESS;
}

#pragma warning(pop)

/// @}
