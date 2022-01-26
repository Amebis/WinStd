/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
*/

///
/// \defgroup WinStdWinAPI Windows API
/// Integrates WinStd classes with Microsoft Windows API
///

#include "Common.h"

#include <string>
#include <vector>

namespace winstd
{
    template<HANDLE INVALID> class win_handle;
    class library;
    class process;
    class file;
    class event;
    class critical_section;
    class heap;
    template <class _Ty> class heap_allocator;
    class actctx_activator;
    class user_impersonator;
    class console_ctrl_handler;
    class vmemory;
    class reg_key;
    class security_id;
    class process_information;
}


/// \addtogroup WinStdWinAPI
/// @{

/// @copydoc GetModuleFileNameW()
template<class _Elem, class _Traits, class _Ax> inline DWORD GetModuleFileNameA(_In_opt_ HMODULE hModule, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue) noexcept;

///
/// Retrieves the fully qualified path for the file that contains the specified module and stores it in a std::wstring string.
///
/// \sa [GetModuleFileName function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms683197.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline DWORD GetModuleFileNameW(_In_opt_ HMODULE hModule, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue) noexcept;

/// @copydoc GetWindowTextW()
template<class _Elem, class _Traits, class _Ax> inline _Success_(return != 0) int GetWindowTextA(_In_ HWND hWnd, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue) noexcept;

///
/// Copies the text of the specified window's title bar (if it has one) into a std::wstring string.
///
/// \sa [GetWindowText function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms633520.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline _Success_(return != 0) int GetWindowTextW(_In_ HWND hWnd, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue) noexcept;

/// @copydoc GetFileVersionInfoW()
template<class _Ty, class _Ax> inline _Success_(return != 0) BOOL GetFileVersionInfoA(_In_z_ LPCSTR lptstrFilename, __reserved DWORD dwHandle, _Out_ std::vector<_Ty, _Ax> &aValue) noexcept;

///
/// Retrieves version information for the specified file and stores it in a std::vector buffer.
///
/// \sa [GetFileVersionInfo function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms647003.aspx)
///
template<class _Ty, class _Ax> inline _Success_(return != 0) BOOL GetFileVersionInfoW(_In_z_ LPCWSTR lptstrFilename, __reserved DWORD dwHandle, _Out_ std::vector<_Ty, _Ax> &aValue) noexcept;

/// @copydoc ExpandEnvironmentStringsW()
template<class _Elem, class _Traits, class _Ax> inline _Success_(return != 0) DWORD ExpandEnvironmentStringsA(_In_z_ LPCSTR lpSrc, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue) noexcept;

///
/// Expands environment-variable strings, replaces them with the values defined for the current user, and stores it in a std::wstring string.
///
/// \sa [ExpandEnvironmentStrings function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724265.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline _Success_(return != 0) DWORD ExpandEnvironmentStringsW(_In_z_ LPCWSTR lpSrc, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue) noexcept;

/// @copydoc GuidToStringW()
template<class _Elem, class _Traits, class _Ax> inline VOID GuidToStringA(_In_ LPCGUID lpGuid, _Out_ std::basic_string<_Elem, _Traits, _Ax> &str) noexcept;

///
/// Formats GUID and stores it in a std::wstring string.
///
/// \param[in ] lpGuid  Pointer to GUID
/// \param[out] str     String to store the result to
///
template<class _Elem, class _Traits, class _Ax> inline VOID GuidToStringW(_In_ LPCGUID lpGuid, _Out_ std::basic_string<_Elem, _Traits, _Ax> &str) noexcept;
/// @copydoc GuidToStringW()
#ifdef _UNICODE
#define GuidToString GuidToStringW
#else
#define GuidToString GuidToStringA
#endif

/// @copydoc StringToGuidW()
_Success_(return) BOOL StringToGuidA(_In_z_ LPCSTR lpszGuid, _Out_ LPGUID lpGuid, _Out_opt_ LPCSTR *lpszGuidEnd = NULL) noexcept;

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
_Success_(return) BOOL StringToGuidW(_In_z_ LPCWSTR lpszGuid, _Out_ LPGUID lpGuid, _Out_opt_ LPCWSTR *lpszGuidEnd = NULL) noexcept;
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
template<class _Elem, class _Traits, class _Ax> inline LSTATUS RegQueryStringValue(_In_ HKEY hReg, _In_z_ LPCSTR pszName, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue) noexcept;

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
template<class _Elem, class _Traits, class _Ax> inline LSTATUS RegQueryStringValue(_In_ HKEY hReg, _In_z_ LPCWSTR pszName, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue) noexcept;

/// @copydoc RegQueryValueExW()
template<class _Ty, class _Ax> inline LSTATUS RegQueryValueExA(_In_ HKEY hKey, _In_opt_z_ LPCSTR lpValueName, __reserved LPDWORD lpReserved, _Out_opt_ LPDWORD lpType, _Out_ std::vector<_Ty, _Ax> &aData) noexcept;

///
/// Retrieves the type and data for the specified value name associated with an open registry key and stores the data in a std::vector buffer.
///
/// \sa [RegQueryValueEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724911.aspx)
///
template<class _Ty, class _Ax> inline LSTATUS RegQueryValueExW(_In_ HKEY hKey, _In_opt_z_ LPCWSTR lpValueName, __reserved LPDWORD lpReserved, _Out_opt_ LPDWORD lpType, _Out_ std::vector<_Ty, _Ax> &aData) noexcept;

#if _WIN32_WINNT >= _WIN32_WINNT_VISTA

/// @copydoc RegLoadMUIStringW()
template<class _Elem, class _Traits, class _Ax> inline LSTATUS RegLoadMUIStringA(_In_ HKEY hKey, _In_opt_z_ LPCSTR pszValue, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sOut, _In_ DWORD Flags, _In_opt_z_ LPCSTR pszDirectory) noexcept;

///
/// Loads the specified string from the specified key and subkey, and stores it in a std::wstring string.
///
/// \sa [RegLoadMUIString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724890.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline LSTATUS RegLoadMUIStringW(_In_ HKEY hKey, _In_opt_z_ LPCWSTR pszValue, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sOut, _In_ DWORD Flags, _In_opt_z_ LPCWSTR pszDirectory) noexcept;

#endif

///
/// Maps a UTF-16 (wide character) string to a std::string. The new character string is not necessarily from a multibyte character set.
///
/// \sa [WideCharToMultiByte function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130.aspx)
///
template<class _Traits, class _Ax> inline _Success_(return != 0) int WideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cchWideChar) LPCWSTR lpWideCharStr, _In_ int cchWideChar, _Out_ std::basic_string<char, _Traits, _Ax> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept;

///
/// Maps a UTF-16 (wide character) string to a std::vector. The new character vector is not necessarily from a multibyte character set.
///
/// \sa [WideCharToMultiByte function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130.aspx)
///
template<class _Ax> inline _Success_(return != 0) int WideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cchWideChar) LPCWSTR lpWideCharStr, _In_ int cchWideChar, _Out_ std::vector<char, _Ax> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept;

///
/// Maps a UTF-16 (wide character) string to a std::string. The new character string is not necessarily from a multibyte character set.
///
/// \sa [WideCharToMultiByte function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130.aspx)
///
template<class _Traits1, class _Ax1, class _Traits2, class _Ax2> inline _Success_(return != 0) int WideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _Out_ std::basic_string<wchar_t, _Traits1, _Ax1> sWideCharStr, _Out_ std::basic_string<char, _Traits2, _Ax2> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept;

///
/// Maps a UTF-16 (wide character) string to a std::string. The new character string is not necessarily from a multibyte character set.
///
/// \note This function cleans all internal buffers using SecureZeroMemory() before returning.
///
/// \sa [WideCharToMultiByte function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130.aspx)
///
template<class _Traits, class _Ax> inline _Success_(return != 0) int SecureWideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cchWideChar) LPCWSTR lpWideCharStr, _In_ int cchWideChar, _Out_ std::basic_string<char, _Traits, _Ax> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept;

///
/// Maps a UTF-16 (wide character) string to a std::vector. The new character vector is not necessarily from a multibyte character set.
///
/// \note This function cleans all internal buffers using SecureZeroMemory() before returning.
///
/// \sa [WideCharToMultiByte function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130.aspx)
///
template<class _Ax> inline _Success_(return != 0) int SecureWideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cchWideChar) LPCWSTR lpWideCharStr, _In_ int cchWideChar, _Out_ std::vector<char, _Ax> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept;

///
/// Maps a UTF-16 (wide character) string to a std::string. The new character string is not necessarily from a multibyte character set.
///
/// \note This function cleans all internal buffers using SecureZeroMemory() before returning.
///
/// \sa [WideCharToMultiByte function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130.aspx)
///
template<class _Traits1, class _Ax1, class _Traits2, class _Ax2> inline _Success_(return != 0) int SecureWideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _Out_ std::basic_string<wchar_t, _Traits1, _Ax1> sWideCharStr, _Out_ std::basic_string<char, _Traits2, _Ax2> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept;

///
/// Maps a character string to a UTF-16 (wide character) std::wstring. The character string is not necessarily from a multibyte character set.
///
/// \sa [MultiByteToWideChar function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd319072.aspx)
///
template<class _Traits, class _Ax> inline _Success_(return != 0) int MultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cbMultiByte) LPCSTR lpMultiByteStr, _In_ int cbMultiByte, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sWideCharStr) noexcept;

///
/// Maps a character string to a UTF-16 (wide character) std::vector. The character vector is not necessarily from a multibyte character set.
///
/// \sa [MultiByteToWideChar function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd319072.aspx)
///
template<class _Ax> inline _Success_(return != 0) int MultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cbMultiByte) LPCSTR lpMultiByteStr, _In_ int cbMultiByte, _Out_ std::vector<wchar_t, _Ax> &sWideCharStr) noexcept;

///
/// Maps a character string to a UTF-16 (wide character) std::wstring. The character string is not necessarily from a multibyte character set.
///
/// \sa [MultiByteToWideChar function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd319072.aspx)
///
template<class _Traits1, class _Ax1, class _Traits2, class _Ax2> inline _Success_(return != 0) int MultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_ const std::basic_string<char, _Traits1, _Ax1> &sMultiByteStr, _Out_ std::basic_string<wchar_t, _Traits2, _Ax2> &sWideCharStr) noexcept;

///
/// Maps a character string to a UTF-16 (wide character) std::wstring. The character string is not necessarily from a multibyte character set.
///
/// \note This function cleans all internal buffers using SecureZeroMemory() before returning.
///
/// \sa [MultiByteToWideChar function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd319072.aspx)
///
template<class _Traits, class _Ax> inline _Success_(return != 0) int SecureMultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cbMultiByte) LPCSTR lpMultiByteStr, _In_ int cbMultiByte, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sWideCharStr) noexcept;

///
/// Maps a character string to a UTF-16 (wide character) std::vector. The character vector is not necessarily from a multibyte character set.
///
/// \note This function cleans all internal buffers using SecureZeroMemory() before returning.
///
/// \sa [MultiByteToWideChar function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd319072.aspx)
///
template<class _Ax> inline _Success_(return != 0) int SecureMultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cbMultiByte) LPCSTR lpMultiByteStr, _In_ int cbMultiByte, _Out_ std::vector<wchar_t, _Ax> &sWideCharStr) noexcept;

///
/// Maps a character string to a UTF-16 (wide character) std::wstring. The character string is not necessarily from a multibyte character set.
///
/// \note This function cleans all internal buffers using SecureZeroMemory() before returning.
///
/// \sa [MultiByteToWideChar function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd319072.aspx)
///
template<class _Traits1, class _Ax1, class _Traits2, class _Ax2> inline _Success_(return != 0) int SecureMultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_ const std::basic_string<char, _Traits1, _Ax1> &sMultiByteStr, _Out_ std::basic_string<wchar_t, _Traits2, _Ax2> &sWideCharStr) noexcept;

///
/// Normalizes characters of a text string according to Unicode 4.0 TR#15.
///
/// \sa [NormalizeString function](https://docs.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-normalizestring)
///
template<class _Traits, class _Ax>
inline _Success_(return > 0) int NormalizeString(_In_ NORM_FORM NormForm, _In_ LPCWSTR lpSrcString, _In_ int cwSrcLength, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sDstString) noexcept;

///
/// Normalizes characters of a text string according to Unicode 4.0 TR#15.
///
/// \sa [NormalizeString function](https://docs.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-normalizestring)
///
template<class _Traits1, class _Ax1, class _Traits2, class _Ax2>
inline _Success_(return > 0) int NormalizeString(_In_ NORM_FORM NormForm, _In_ const std::basic_string<wchar_t, _Traits1, _Ax1> &sSrcString, _Out_ std::basic_string<wchar_t, _Traits2, _Ax2> &sDstString) noexcept;

/// @copydoc LoadStringW
template<class _Traits, class _Ax> inline _Success_(return != 0) int WINAPI LoadStringA(_In_opt_ HINSTANCE hInstance, _In_ UINT uID, _Out_ std::basic_string<char, _Traits, _Ax> &sBuffer) noexcept;

///
/// Loads a string resource from the executable file associated with a specified module.
///
/// \sa [LoadString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms647486.aspx)
///
template<class _Traits, class _Ax> inline _Success_(return != 0) int WINAPI LoadStringW(_In_opt_ HINSTANCE hInstance, _In_ UINT uID, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sBuffer) noexcept;

///
/// Formats and sends a string to the debugger for display.
///
/// \sa [OutputDebugString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363362.aspx)
///
inline VOID OutputDebugStrV(_In_z_ LPCSTR lpOutputString, _In_ va_list arg) noexcept;

///
/// Formats and sends a string to the debugger for display.
///
/// \sa [OutputDebugString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363362.aspx)
///
inline VOID OutputDebugStrV(_In_z_ LPCWSTR lpOutputString, _In_ va_list arg) noexcept;

///
/// Formats and sends a string to the debugger for display.
///
/// \sa [OutputDebugString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363362.aspx)
///
inline VOID OutputDebugStr(_In_z_ LPCSTR lpOutputString, ...) noexcept;

///
/// Formats and sends a string to the debugger for display.
///
/// \sa [OutputDebugString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363362.aspx)
///
inline VOID OutputDebugStr(_In_z_ LPCWSTR lpOutputString, ...) noexcept;

/// @copydoc GetDateFormatW()
template<class _Elem, class _Traits, class _Ax> inline _Success_(return != 0) int GetDateFormatA(_In_ LCID Locale, _In_ DWORD dwFlags, _In_opt_ const SYSTEMTIME *lpDate, _In_opt_z_ LPCSTR lpFormat, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sDate) noexcept;

///
/// Formats a date as a date string for a locale specified by the locale identifier. The function formats either a specified date or the local system date. 
///
/// \sa [GetDateFormat function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd318086.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline _Success_(return != 0) int GetDateFormatW(_In_ LCID Locale, _In_ DWORD dwFlags, _In_opt_ const SYSTEMTIME *lpDate, _In_opt_z_ LPCWSTR lpFormat, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sDate) noexcept;

/// @copydoc LookupAccountSidW()
template<class _Elem, class _Traits, class _Ax> inline _Success_(return != 0) BOOL LookupAccountSidA(_In_opt_z_ LPCSTR lpSystemName, _In_ PSID lpSid, _Out_opt_ std::basic_string<_Elem, _Traits, _Ax> *sName, _Out_opt_ std::basic_string<_Elem, _Traits, _Ax> *sReferencedDomainName, _Out_ PSID_NAME_USE peUse) noexcept;

///
/// Retrieves the name of the account for this SID and the name of the first domain on which this SID is found.
///
/// \sa [LookupAccountSid function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379166.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline _Success_(return != 0) BOOL LookupAccountSidW(_In_opt_z_ LPCWSTR lpSystemName, _In_ PSID lpSid, _Out_opt_ std::basic_string<_Elem, _Traits, _Ax> *sName, _Out_opt_ std::basic_string<_Elem, _Traits, _Ax> *sReferencedDomainName, _Out_ PSID_NAME_USE peUse) noexcept;

///
/// Retrieves a specified type of information about an access token. The calling process must have appropriate access rights to obtain the information.
///
/// \sa [GetTokenInformation function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa446671.aspx)
///
template<class _Ty> inline _Success_(return != 0) BOOL GetTokenInformation(_In_ HANDLE TokenHandle, _In_ TOKEN_INFORMATION_CLASS TokenInformationClass, _Out_ std::unique_ptr<_Ty> &TokenInformation) noexcept;

///
/// Retrieves the full name of the executable image for the specified process.
///
/// \sa [QueryFullProcessImageNameA function](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-queryfullprocessimagenamea)
///
template<class _Elem, class _Traits, class _Ax> inline _Success_(return != 0) BOOL QueryFullProcessImageNameA(_In_ HANDLE hProcess, _In_ DWORD dwFlags, _Inout_ std::basic_string<_Elem, _Traits, _Ax>& sExeName);

///
/// Retrieves the full name of the executable image for the specified process.
///
/// \sa [QueryFullProcessImageNameW function](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-queryfullprocessimagenamew)
///
template<class _Elem, class _Traits, class _Ax> inline _Success_(return != 0) BOOL QueryFullProcessImageNameW(_In_ HANDLE hProcess, _In_ DWORD dwFlags, _Inout_ std::basic_string<_Elem, _Traits, _Ax>& sExeName);

/// @}

#pragma once


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
        HANDLE_IMPL(win_handle, INVALID)

    public:
        ///
        /// Closes an open object handle.
        ///
        /// \sa [CloseHandle function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724211.aspx)
        ///
        virtual ~win_handle()
        {
            if (m_h != invalid)
                CloseHandle(m_h);
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
    class library : public handle<HMODULE, NULL>
    {
        HANDLE_IMPL(library, NULL)

    public:
        ///
        /// Frees the module.
        ///
        /// \sa [FreeLibrary function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms683152.aspx)
        ///
        virtual ~library();

        ///
        /// Loads the specified module into the address space of the calling process.
        ///
        /// \sa [LoadLibraryEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms684179.aspx)
        ///
        /// \return
        /// - \c true when succeeds;
        /// - \c false when fails. Use `GetLastError()` for failure reason.
        ///
        inline bool load(_In_z_ LPCTSTR lpFileName, __reserved handle_type hFile, _In_ DWORD dwFlags) noexcept
        {
            handle_type h = LoadLibraryEx(lpFileName, hFile, dwFlags);
            if (h != invalid) {
                attach(h);
                return true;
            } else
                return false;
        }

    protected:
        ///
        /// Frees the module.
        ///
        /// \sa [FreeLibrary function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms683152.aspx)
        ///
        void free_internal() noexcept override;
    };


    ///
    /// Process handle wrapper
    ///
    class process : public win_handle<NULL>
    {
    public:
        ///
        /// Opens process handle.
        ///
        /// \sa [OpenProcess function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms684320.aspx)
        ///
        /// \return
        /// - \c true when succeeds;
        /// - \c false when fails. Use `GetLastError()` for failure reason.
        ///
        inline bool open(_In_ DWORD dwDesiredAccess, _In_ BOOL bInheritHandle, _In_ DWORD dwProcessId) noexcept
        {
            handle_type h = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
            if (h != invalid) {
                attach(h);
                return true;
            } else
                return false;
        }
    };


    ///
    /// File handle wrapper
    ///
    class file : public win_handle<INVALID_HANDLE_VALUE>
    {
    public:
        ///
        /// Opens file handle.
        ///
        /// \sa [CreateFile function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363858.aspx)
        ///
        /// \return
        /// - \c true when succeeds;
        /// - \c false when fails. Use `GetLastError()` for failure reason.
        ///
        inline bool create(_In_z_ LPCTSTR lpFileName, _In_ DWORD dwDesiredAccess, _In_ DWORD dwShareMode, _In_ DWORD dwCreationDisposition, _In_opt_ DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, _In_opt_ HANDLE hTemplateFile = NULL) noexcept
        {
            handle_type h = CreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
            if (h != invalid) {
                attach(h);
                return true;
            } else
                return false;
        }
    };


    ///
    /// Event handle wrapper
    ///
    class event : public win_handle<NULL>
    {
    public:
        ///
        /// Creates or opens a named or unnamed event object.
        ///
        /// \sa [CreateEventW function](https://docs.microsoft.com/en-us/windows/desktop/api/synchapi/nf-synchapi-createeventw)
        ///
        /// \return
        /// - \c true when succeeds;
        /// - \c false when fails. Use `GetLastError()` for failure reason.
        ///
        inline bool create(_In_ BOOL bManualReset, _In_ BOOL bInitialState, _In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes = NULL, _In_opt_z_ LPCTSTR lpName = NULL) noexcept
        {
            handle_type h = CreateEvent(lpEventAttributes, bManualReset, bInitialState, lpName);
            if (h != invalid) {
                attach(h);
                return true;
            } else
                return false;
        }

        ///
        /// Opens an existing named event object.
        ///
        /// \sa [OpenEventW function](https://docs.microsoft.com/en-us/windows/desktop/api/synchapi/nf-synchapi-openeventw)
        ///
        /// \return
        /// - \c true when succeeds;
        /// - \c false when fails. Use `GetLastError()` for failure reason.
        ///
        inline bool open(_In_ DWORD dwDesiredAccess, _In_ BOOL bInheritHandle, _In_z_ LPCTSTR lpName) noexcept
        {
            handle_type h = OpenEvent(dwDesiredAccess, bInheritHandle, lpName);
            if (h != invalid) {
                attach(h);
                return true;
            } else
                return false;
        }
    };


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
        critical_section();

        ///
        /// Releases all resources used by an unowned critical section object.
        ///
        /// \sa [DeleteCriticalSection function](https://docs.microsoft.com/en-us/windows/desktop/api/synchapi/nf-synchapi-deletecriticalsection)
        ///
        virtual ~critical_section();

        ///
        /// Auto-typecasting operator
        ///
        /// \return Pointer to critical section
        ///
        inline operator LPCRITICAL_SECTION() noexcept
        {
            return &m_data;
        }

    protected:
        CRITICAL_SECTION m_data;    ///< Critical section struct
    };


    ///
    /// Find-file handle wrapper
    ///
    class find_file : public handle<HANDLE, INVALID_HANDLE_VALUE>
    {
        HANDLE_IMPL(find_file, INVALID_HANDLE_VALUE)

    public:
        ///
        /// Closes a file search handle.
        ///
        /// \sa [FindClose function](https://docs.microsoft.com/en-us/windows/desktop/api/fileapi/nf-fileapi-findclose)
        ///
        virtual ~find_file();

        ///
        /// Searches a directory for a file or subdirectory with a name that matches a specific name (or partial name if wildcards are used).
        ///
        /// \sa [FindFirstFile function](https://docs.microsoft.com/en-us/windows/desktop/api/fileapi/nf-fileapi-findfirstfilew)
        ///
        /// \return
        /// - \c true when succeeds;
        /// - \c false when fails. Use `GetLastError()` for failure reason.
        ///
        inline bool find(_In_ LPCTSTR lpFileName, _Out_ LPWIN32_FIND_DATA lpFindFileData) noexcept
        {
            handle_type h = FindFirstFile(lpFileName, lpFindFileData);
            if (h != invalid) {
                attach(h);
                return true;
            } else
                return false;
        }

    protected:
        ///
        /// Closes a file search handle.
        ///
        /// \sa [FindClose function](https://docs.microsoft.com/en-us/windows/desktop/api/fileapi/nf-fileapi-findclose)
        ///
        void free_internal() noexcept override;
    };


    ///
    /// Heap handle wrapper
    ///
    class heap : public handle<HANDLE, NULL>
    {
        HANDLE_IMPL(heap, NULL)

    public:
        ///
        /// Destroys the heap.
        ///
        /// \sa [HeapDestroy function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa366700.aspx)
        ///
        virtual ~heap();

        ///
        /// Creates the heap.
        ///
        /// \sa [HeapCreate function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa366599.aspx)
        ///
        /// \return
        /// - \c true when succeeds;
        /// - \c false when fails. Use `GetLastError()` for failure reason.
        ///
        inline bool create(_In_ DWORD flOptions, _In_ SIZE_T dwInitialSize, _In_ SIZE_T dwMaximumSize) noexcept
        {
            handle_type h = HeapCreate(flOptions, dwInitialSize, dwMaximumSize);
            if (h != invalid) {
                attach(h);
                return true;
            } else
                return false;
        }

        ///
        /// Enumerates allocated heap blocks using `OutputDebugString()`
        ///
        /// \returns
        /// - `true` if any blocks found;
        /// - `false` otherwise.
        ///
        bool enumerate() noexcept;

    protected:
        ///
        /// Destroys the heap.
        ///
        /// \sa [HeapDestroy function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa366700.aspx)
        ///
        void free_internal() noexcept override;
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
        inline heap_allocator(_In_ HANDLE heap) : m_heap(heap)
        {
        }

        ///
        /// Constructs allocator from another type
        ///
        /// \param[in] other  Another allocator of the heap_allocator kind
        ///
        template <class _Other>
        inline heap_allocator(_In_ const heap_allocator<_Other> &other) : m_heap(other.m_heap)
        {
        }

        ///
        /// Allocates a new memory block
        ///
        /// \param[in] count  Number of elements
        ///
        /// \returns Pointer to new memory block
        ///
        inline pointer allocate(_In_ size_type count)
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
        inline void deallocate(_In_ pointer ptr, _In_ size_type size)
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
        inline void construct(_Inout_ pointer ptr, _In_ const _Ty& val)
        {
            ::new ((void*)ptr) _Ty(val);
        }

        ///
        /// Calls moving constructor for the element
        ///
        /// \param[in] ptr  Pointer to memory block
        /// \param[in] val  Source element
        ///
        inline void construct(_Inout_ pointer ptr, _Inout_ _Ty&& val)
        {
            ::new ((void*)ptr) _Ty(std::forward<_Ty>(val));
        }

        ///
        /// Calls destructor for the element
        ///
        /// \param[in] ptr  Pointer to memory block
        ///
        inline void destroy(_Inout_ pointer ptr)
        {
            ptr->_Ty::~_Ty();
        }

        ///
        /// Returns maximum memory block size
        ///
        inline size_type max_size() const
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
        actctx_activator(_In_ HANDLE hActCtx) noexcept;

        ///
        /// Deactivates activation context and destructs the activator
        ///
        /// \sa [DeactivateActCtx function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa375140.aspx)
        ///
        virtual ~actctx_activator();

    protected:
        ULONG_PTR m_cookie; ///< Cookie for context deactivation
    };


    ///
    /// Lets the calling thread impersonate the security context of a logged-on user
    ///
    class user_impersonator
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
        user_impersonator(_In_opt_ HANDLE hToken) noexcept;

        ///
        /// Reverts to current user and destructs the impersonator
        ///
        /// \sa [RevertToSelf function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379317.aspx)
        ///
        virtual ~user_impersonator();

    protected:
        BOOL m_cookie; ///< Did impersonation succeed?
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
        console_ctrl_handler(_In_opt_ PHANDLER_ROUTINE HandlerRoutine) noexcept;

        ///
        /// Pops console control handler from the console control handler stack
        ///
        /// \sa [SetConsoleCtrlHandler function](https://docs.microsoft.com/en-us/windows/console/setconsolectrlhandler)
        ///
        virtual ~console_ctrl_handler();

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
        inline vmemory() noexcept : m_proc(NULL)
        {
        }

        ///
        /// Initializes a new class instance with an already available object handle.
        ///
        /// \param[in] proc  Handle of process the memory belongs to
        /// \param[in] h     Initial object handle value
        ///
        inline vmemory(_In_ handle_type h, _In_ HANDLE proc) noexcept :
            m_proc(proc),
            handle<LPVOID, NULL>(h)
        {
        }

        ///
        /// Move constructor
        ///
        /// \param[inout] h  A rvalue reference of another object
        ///
        inline vmemory(_Inout_ vmemory &&h) noexcept :
            m_proc(std::move(h.m_proc)),
            handle<LPVOID, NULL>(std::move(h))
        {
        }

        ///
        /// Frees the memory.
        ///
        /// \sa [VirtualFreeEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa366894.aspx)
        ///
        virtual ~vmemory();

        ///
        /// Move assignment
        ///
        /// \param[inout] other  A rvalue reference of another object
        ///
        inline vmemory& operator=(_Inout_ vmemory &&other) noexcept
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
        inline void attach(_In_ HANDLE proc, _In_opt_ handle_type h) noexcept
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
        inline bool alloc(
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
        void free_internal() noexcept override;

    protected:
        HANDLE m_proc;  ///< Handle of memory's process
    };


    ///
    /// Registry wrapper class
    ///
    class reg_key : public handle<HKEY, NULL>
    {
        HANDLE_IMPL(reg_key, NULL)

    public:
        ///
        /// Closes a handle to the registry key.
        ///
        /// \sa [RegCloseKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724837.aspx)
        ///
        virtual ~reg_key();

        ///
        /// Creates the specified registry key. If the key already exists, the function opens it.
        ///
        /// \return
        /// - true when creation succeeds;
        /// - false when creation fails. For extended error information, call `GetLastError()`.
        ///
        /// \sa [RegCreateKeyEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724844.aspx)
        ///
        inline bool create(
            _In_      HKEY                  hKey,
            _In_z_    LPCTSTR               lpSubKey,
            _In_opt_  LPTSTR                lpClass,
            _In_      DWORD                 dwOptions,
            _In_      REGSAM                samDesired,
            _In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL,
            _Out_opt_ LPDWORD               lpdwDisposition = NULL) noexcept
        {
            handle_type h;
            const LSTATUS s = RegCreateKeyEx(hKey, lpSubKey, 0, lpClass, dwOptions, samDesired, lpSecurityAttributes, &h, lpdwDisposition);
            if (s == ERROR_SUCCESS) {
                attach(h);
                return true;
            } else {
                SetLastError(s);
                return false;
            }
        }

        ///
        /// Opens the specified registry key.
        ///
        /// \return
        /// - true when creation succeeds;
        /// - false when creation fails. For extended error information, call `GetLastError()`.
        ///
        /// \sa [RegOpenKeyEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724897.aspx)
        ///
        inline bool open(
            _In_       HKEY    hKey,
            _In_opt_z_ LPCTSTR lpSubKey,
            _In_       DWORD   ulOptions,
            _In_       REGSAM  samDesired) noexcept
        {
            handle_type h;
            const LONG s = RegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, &h);
            if (s == ERROR_SUCCESS) {
                attach(h);
                return true;
            } else {
                SetLastError(s);
                return false;
            }
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
        bool delete_subkey(_In_z_ LPCTSTR szSubkey);

    protected:
        ///
        /// Closes a handle to the registry key.
        ///
        /// \sa [RegCloseKey function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724837.aspx)
        ///
        void free_internal() noexcept override;
    };


    ///
    /// SID wrapper class
    ///
    class security_id : public handle<PSID, NULL>
    {
        HANDLE_IMPL(security_id, NULL)

    public:
        ///
        /// Closes a handle to the SID.
        ///
        /// \sa [FreeSid function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa446631.aspx)
        ///
        virtual ~security_id();

    protected:
        ///
        /// Closes a handle to the SID.
        ///
        /// \sa [FreeSid function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa446631.aspx)
        ///
        void free_internal() noexcept override;
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
        inline process_information() noexcept
        {
            hProcess    = INVALID_HANDLE_VALUE;
            hThread     = INVALID_HANDLE_VALUE;
            dwProcessId = 0;
            dwThreadId  = 0;
        }

        ///
        /// Closes process and thread handles.
        ///
        inline ~process_information()
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

    /// @}
}


template<class _Elem, class _Traits, class _Ax>
inline DWORD GetModuleFileNameA(_In_opt_ HMODULE hModule, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue) noexcept
{
    assert(0); // TODO: Test this code.

    _Elem szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];

    // Try with stack buffer first.
    DWORD dwResult = ::GetModuleFileNameA(hModule, szStackBuffer, _countof(szStackBuffer));
    if (dwResult < _countof(szStackBuffer)) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwResult);
        return dwResult;
    } else {
        for (DWORD dwCapacity = 2*WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem);; dwCapacity *= 2) {
            // Allocate on heap and retry.
            std::unique_ptr<_Elem[]> szBuffer(new _Elem[dwCapacity]);
            dwResult = ::GetModuleFileNameA(hModule, szBuffer.get(), dwCapacity);
            if (dwResult < dwCapacity) {
                sValue.assign(szBuffer.get(), dwResult);
                return dwResult;
            }
        }
    }
}


template<class _Elem, class _Traits, class _Ax>
inline DWORD GetModuleFileNameW(_In_opt_ HMODULE hModule, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue) noexcept
{
    _Elem szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];

    // Try with stack buffer first.
    DWORD dwResult = ::GetModuleFileNameW(hModule, szStackBuffer, _countof(szStackBuffer));
    if (dwResult < _countof(szStackBuffer)) {
        // Copy from stack.
        sValue.assign(szStackBuffer, dwResult);
        return dwResult;
    } else {
        for (DWORD dwCapacity = 2*WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem);; dwCapacity *= 2) {
            // Allocate on heap and retry.
            std::unique_ptr<_Elem[]> szBuffer(new _Elem[dwCapacity]);
            dwResult = ::GetModuleFileNameW(hModule, szBuffer.get(), dwCapacity);
            if (dwResult < dwCapacity) {
                sValue.assign(szBuffer.get(), dwResult);
                return dwResult;
            }
        }
    }
}


template<class _Elem, class _Traits, class _Ax>
inline _Success_(return != 0) int GetWindowTextA(_In_ HWND hWnd, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue) noexcept
{
    assert(0); // TODO: Test this code.

    int iResult;

    // Query the final string length first.
    iResult = ::GetWindowTextLengthA(hWnd);
    if (iResult > 0) {
        if (++iResult < WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)) {
            // Read string data to stack.
            _Elem szBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];
            iResult = ::GetWindowTextA(hWnd, szBuffer, _countof(szBuffer));
            sValue.assign(szBuffer, iResult);
        } else {
            // Allocate buffer on heap and read the string data into it.
            std::unique_ptr<_Elem[]> szBuffer(new _Elem[++iResult]);
            iResult = ::GetWindowTextA(hWnd, szBuffer.get(), iResult);
            sValue.assign(szBuffer.get(), iResult);
        }
        return iResult;
    }

    sValue.clear();
    return 0;
}


template<class _Elem, class _Traits, class _Ax>
inline _Success_(return != 0) int GetWindowTextW(_In_ HWND hWnd, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue) noexcept
{
    assert(0); // TODO: Test this code.

    int iResult;

    // Query the final string length first.
    iResult = ::GetWindowTextLengthW(hWnd);
    if (iResult > 0) {
        if (++iResult < WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)) {
            // Read string data to stack.
            _Elem szBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];
            iResult = ::GetWindowTextW(hWnd, szBuffer, _countof(szBuffer));
            sValue.assign(szBuffer, iResult);
        } else {
            // Allocate buffer on heap and read the string data into it.
            std::unique_ptr<_Elem[]> szBuffer(new _Elem[++iResult]);
            iResult = ::GetWindowTextW(hWnd, szBuffer.get(), iResult);
            sValue.assign(szBuffer.get(), iResult);
        }
        return iResult;
    }

    sValue.clear();
    return 0;
}


template<class _Ty, class _Ax>
inline _Success_(return != 0) BOOL GetFileVersionInfoA(_In_z_ LPCSTR lptstrFilename, __reserved DWORD dwHandle, _Out_ std::vector<_Ty, _Ax> &aValue) noexcept
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


template<class _Ty, class _Ax>
inline _Success_(return != 0) BOOL GetFileVersionInfoW(_In_z_ LPCWSTR lptstrFilename, __reserved DWORD dwHandle, _Out_ std::vector<_Ty, _Ax> &aValue) noexcept
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


template<class _Elem, class _Traits, class _Ax>
inline _Success_(return != 0) DWORD ExpandEnvironmentStringsA(_In_z_ LPCSTR lpSrc, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue) noexcept
{
    assert(0); // TODO: Test this code.

    for (DWORD dwSizeOut = (DWORD)strlen(lpSrc) + 0x100;;) {
        DWORD dwSizeIn = dwSizeOut;
        std::unique_ptr<_Elem[]> szBuffer(new _Elem[(size_t)dwSizeIn + 2]); // Note: ANSI version requires one extra char.
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


template<class _Elem, class _Traits, class _Ax>
inline _Success_(return != 0) DWORD ExpandEnvironmentStringsW(_In_z_ LPCWSTR lpSrc, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue) noexcept
{
    for (DWORD dwSizeOut = (DWORD)wcslen(lpSrc) + 0x100;;) {
        DWORD dwSizeIn = dwSizeOut;
        std::unique_ptr<_Elem[]> szBuffer(new _Elem[(size_t)dwSizeIn + 1]);
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


template<class _Elem, class _Traits, class _Ax>
inline VOID GuidToStringA(_In_ LPCGUID lpGuid, _Out_ std::basic_string<_Elem, _Traits, _Ax> &str) noexcept
{
    assert(0); // TODO: Test this code.

    sprintf(str, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
        lpGuid->Data1,
        lpGuid->Data2,
        lpGuid->Data3,
        lpGuid->Data4[0], lpGuid->Data4[1],
        lpGuid->Data4[2], lpGuid->Data4[3], lpGuid->Data4[4], lpGuid->Data4[5], lpGuid->Data4[6], lpGuid->Data4[7]);
}


template<class _Elem, class _Traits, class _Ax>
inline VOID GuidToStringW(_In_ LPCGUID lpGuid, _Out_ std::basic_string<_Elem, _Traits, _Ax> &str) noexcept
{
    assert(0); // TODO: Test this code.

    sprintf(str, L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
        lpGuid->Data1,
        lpGuid->Data2,
        lpGuid->Data3,
        lpGuid->Data4[0], lpGuid->Data4[1],
        lpGuid->Data4[2], lpGuid->Data4[3], lpGuid->Data4[4], lpGuid->Data4[5], lpGuid->Data4[6], lpGuid->Data4[7]);
}


template<class _Elem, class _Traits, class _Ax>
inline LSTATUS RegQueryStringValue(_In_ HKEY hReg, _In_z_ LPCSTR pszName, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue) noexcept
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


template<class _Elem, class _Traits, class _Ax>
inline LSTATUS RegQueryStringValue(_In_ HKEY hReg, _In_z_ LPCWSTR pszName, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue) noexcept
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


template<class _Ty, class _Ax>
inline LSTATUS RegQueryValueExA(_In_ HKEY hKey, _In_opt_z_ LPCSTR lpValueName, __reserved LPDWORD lpReserved, _Out_opt_ LPDWORD lpType, _Out_ std::vector<_Ty, _Ax> &aData) noexcept
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
        lResult = RegQueryValueExA(hKey, lpValueName, lpReserved, NULL, aData.data(), &dwSize);
    }

    return lResult;
}


template<class _Ty, class _Ax>
inline LSTATUS RegQueryValueExW(_In_ HKEY hKey, _In_opt_z_ LPCWSTR lpValueName, __reserved LPDWORD lpReserved, _Out_opt_ LPDWORD lpType, _Out_ std::vector<_Ty, _Ax> &aData) noexcept
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
        lResult = RegQueryValueExW(hKey, lpValueName, lpReserved, NULL, aData.data(), &dwSize);
    }

    return lResult;
}


#if _WIN32_WINNT >= _WIN32_WINNT_VISTA

template<class _Elem, class _Traits, class _Ax>
inline LSTATUS RegLoadMUIStringA(_In_ HKEY hKey, _In_opt_z_ LPCSTR pszValue, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sOut, _In_ DWORD Flags, _In_opt_z_ LPCSTR pszDirectory) noexcept
{
    // According to "Remarks" section in MSDN documentation of RegLoadMUIString(),
    // this function is defined but not implemented as ANSI variation.
    assert(0);
    return ERROR_CALL_NOT_IMPLEMENTED;
}


template<class _Elem, class _Traits, class _Ax>
inline LSTATUS RegLoadMUIStringW(_In_ HKEY hKey, _In_opt_z_ LPCWSTR pszValue, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sOut, _In_ DWORD Flags, _In_opt_z_ LPCWSTR pszDirectory) noexcept
{
    LSTATUS lResult;
    _Elem szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];
    DWORD dwSize;

    Flags &= ~REG_MUI_STRING_TRUNCATE;

    // Try with stack buffer first.
    lResult = RegLoadMUIStringW(hKey, pszValue, szStackBuffer, sizeof(szStackBuffer), &dwSize, Flags, pszDirectory);
    if (lResult == ERROR_SUCCESS) {
        // Copy from stack buffer.
        sOut.assign(szStackBuffer, wcsnlen(szStackBuffer, dwSize/sizeof(_Elem)));
    } else if (lResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap and retry.
        std::unique_ptr<_Elem[]> szBuffer(new _Elem[(dwSize + sizeof(_Elem) - 1)/sizeof(_Elem)]);
        sOut.assign(szBuffer.get(), (lResult = RegLoadMUIStringW(hKey, pszValue, szBuffer.get(), dwSize, &dwSize, Flags, pszDirectory)) == ERROR_SUCCESS ? wcsnlen(szBuffer.get(), dwSize/sizeof(_Elem)) : 0);
    }

    return lResult;
}

#endif


template<class _Traits, class _Ax>
inline _Success_(return != 0) int WideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cchWideChar) LPCWSTR lpWideCharStr, _In_ int cchWideChar, _Out_ std::basic_string<char, _Traits, _Ax> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept
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


template<class _Ax>
inline _Success_(return != 0) int WideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cchWideChar) LPCWSTR lpWideCharStr, _In_ int cchWideChar, _Out_ std::vector<char, _Ax> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept
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


template<class _Traits1, class _Ax1, class _Traits2, class _Ax2>
inline _Success_(return != 0) int WideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_ std::basic_string<wchar_t, _Traits1, _Ax1> sWideCharStr, _Out_ std::basic_string<char, _Traits2, _Ax2> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept
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


template<class _Traits, class _Ax>
inline _Success_(return != 0) int SecureWideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cchWideChar) LPCWSTR lpWideCharStr, _In_ int cchWideChar, _Out_ std::basic_string<char, _Traits, _Ax> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept
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


template<class _Ax>
inline _Success_(return != 0) int SecureWideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cchWideChar) LPCWSTR lpWideCharStr, _In_ int cchWideChar, _Out_ std::vector<char, _Ax> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept
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


template<class _Traits1, class _Ax1, class _Traits2, class _Ax2>
inline _Success_(return != 0) int SecureWideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_ std::basic_string<wchar_t, _Traits1, _Ax1> sWideCharStr, _Out_ std::basic_string<char, _Traits2, _Ax2> &sMultiByteStr, _In_opt_z_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar) noexcept
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


template<class _Traits, class _Ax>
inline _Success_(return != 0) int MultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cbMultiByte) LPCSTR lpMultiByteStr, _In_ int cbMultiByte, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sWideCharStr) noexcept
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


template<class _Ax>
inline _Success_(return != 0) int MultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cbMultiByte) LPCSTR lpMultiByteStr, _In_ int cbMultiByte, _Out_ std::vector<wchar_t, _Ax> &sWideCharStr) noexcept
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


template<class _Traits1, class _Ax1, class _Traits2, class _Ax2>
inline _Success_(return != 0) int MultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_ const std::basic_string<char, _Traits1, _Ax1> &sMultiByteStr, _Out_ std::basic_string<wchar_t, _Traits2, _Ax2> &sWideCharStr) noexcept
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


template<class _Traits, class _Ax>
inline _Success_(return != 0) int SecureMultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cbMultiByte) LPCSTR lpMultiByteStr, _In_ int cbMultiByte, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sWideCharStr) noexcept
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


template<class _Ax>
inline _Success_(return != 0) int SecureMultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cbMultiByte) LPCSTR lpMultiByteStr, _In_ int cbMultiByte, _Out_ std::vector<wchar_t, _Ax> &sWideCharStr) noexcept
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


template<class _Traits1, class _Ax1, class _Traits2, class _Ax2>
inline _Success_(return != 0) int SecureMultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_ const std::basic_string<char, _Traits1, _Ax1> &sMultiByteStr, _Out_ std::basic_string<wchar_t, _Traits2, _Ax2> &sWideCharStr) noexcept
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


template<class _Traits, class _Ax>
inline _Success_(return > 0) int NormalizeString(_In_ NORM_FORM NormForm, _In_ LPCWSTR lpSrcString, _In_ int cwSrcLength, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sDstString) noexcept
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


template<class _Traits1, class _Ax1, class _Traits2, class _Ax2>
inline _Success_(return > 0) int NormalizeString(_In_ NORM_FORM NormForm, _In_ const std::basic_string<wchar_t, _Traits1, _Ax1> &sSrcString, _Out_ std::basic_string<wchar_t, _Traits2, _Ax2> &sDstString) noexcept
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


template<class _Traits, class _Ax>
inline _Success_(return != 0) int WINAPI LoadStringA(_In_opt_ HINSTANCE hInstance, _In_ UINT uID, _Out_ std::basic_string<char, _Traits, _Ax> &sBuffer) noexcept
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


template<class _Traits, class _Ax>
inline _Success_(return != 0) int WINAPI LoadStringW(_In_opt_ HINSTANCE hInstance, _In_ UINT uID, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sBuffer) noexcept
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


inline VOID OutputDebugStrV(_In_z_ LPCSTR lpOutputString, _In_ va_list arg) noexcept
{
    std::string str;
    try { vsprintf(str, lpOutputString, arg); } catch (...) { return; }
    OutputDebugStringA(str.c_str());
}


inline VOID OutputDebugStrV(_In_z_ LPCWSTR lpOutputString, _In_ va_list arg) noexcept
{
    std::wstring str;
    try { vsprintf(str, lpOutputString, arg); } catch (...) { return; }
    OutputDebugStringW(str.c_str());
}


inline VOID OutputDebugStr(_In_z_ LPCSTR lpOutputString, ...) noexcept
{
    va_list arg;
    va_start(arg, lpOutputString);
    OutputDebugStrV(lpOutputString, arg);
    va_end(arg);
}


inline VOID OutputDebugStr(_In_z_ LPCWSTR lpOutputString, ...) noexcept
{
    va_list arg;
    va_start(arg, lpOutputString);
    OutputDebugStrV(lpOutputString, arg);
    va_end(arg);
}


template<class _Elem, class _Traits, class _Ax> inline _Success_(return != 0) int GetDateFormatA(_In_ LCID Locale, _In_ DWORD dwFlags, _In_opt_ const SYSTEMTIME *lpDate, _In_opt_z_ LPCSTR lpFormat, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sDate) noexcept
{
    int iResult = GetDateFormatA(Locale, dwFlags, lpDate, lpFormat, NULL, 0);
    if (iResult) {
        // Allocate buffer on heap and retry.
        std::unique_ptr<_Elem[]> szBuffer(new _Elem[iResult]);
        iResult = GetDateFormatA(Locale, dwFlags, lpDate, lpFormat, szBuffer.get(), iResult);
        sDate.assign(szBuffer.get(), iResult ? iResult - 1 : 0);
        return iResult;
    }

    return iResult;
}


template<class _Elem, class _Traits, class _Ax> inline _Success_(return != 0) int GetDateFormatW(_In_ LCID Locale, _In_ DWORD dwFlags, _In_opt_ const SYSTEMTIME *lpDate, _In_opt_z_ LPCWSTR lpFormat, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sDate) noexcept
{
    int iResult = GetDateFormatW(Locale, dwFlags, lpDate, lpFormat, NULL, 0);
    if (iResult) {
        // Allocate buffer on heap and retry.
        std::unique_ptr<_Elem[]> szBuffer(new _Elem[iResult]);
        iResult = GetDateFormatW(Locale, dwFlags, lpDate, lpFormat, szBuffer.get(), iResult);
        sDate.assign(szBuffer.get(), iResult ? iResult - 1 : 0);
        return iResult;
    }

    return iResult;
}


template<class _Elem, class _Traits, class _Ax>
inline _Success_(return != 0) BOOL LookupAccountSidA(_In_opt_z_ LPCSTR lpSystemName, _In_ PSID lpSid, _Out_opt_ std::basic_string<_Elem, _Traits, _Ax> *sName, _Out_opt_ std::basic_string<_Elem, _Traits, _Ax> *sReferencedDomainName, _Out_ PSID_NAME_USE peUse) noexcept
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
        std::unique_ptr<_Elem[]> bufName     (new _Elem[dwNameLen     ]);
        std::unique_ptr<_Elem[]> bufRefDomain(new _Elem[dwRefDomainLen]);
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


template<class _Elem, class _Traits, class _Ax>
inline _Success_(return != 0) BOOL LookupAccountSidW(_In_opt_z_ LPCWSTR lpSystemName, _In_ PSID lpSid, _Out_opt_ std::basic_string<_Elem, _Traits, _Ax> *sName, _Out_opt_ std::basic_string<_Elem, _Traits, _Ax> *sReferencedDomainName, _Out_ PSID_NAME_USE peUse) noexcept
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
        std::unique_ptr<_Elem[]> bufName     (new _Elem[dwNameLen     ]);
        std::unique_ptr<_Elem[]> bufRefDomain(new _Elem[dwRefDomainLen]);
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


template<class _Ty>
inline _Success_(return != 0) BOOL GetTokenInformation(_In_ HANDLE TokenHandle, _In_ TOKEN_INFORMATION_CLASS TokenInformationClass, _Out_ std::unique_ptr<_Ty> &TokenInformation) noexcept
{
    BYTE szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(BYTE)];
    DWORD dwSize;

    if (GetTokenInformation(TokenHandle, TokenInformationClass, szStackBuffer, sizeof(szStackBuffer), &dwSize)) {
        // The stack buffer was big enough to retrieve complete data. Alloc and copy.
        TokenInformation.reset((_Ty*)(new BYTE[dwSize / sizeof(BYTE)]));
        if (!TokenInformation) {
            SetLastError(ERROR_OUTOFMEMORY);
            return FALSE;
        }
        memcpy(TokenInformation.get(), szStackBuffer, dwSize);
        return TRUE;
    } else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // The stack buffer was too small to retrieve complete data. Alloc and retry.
        TokenInformation.reset((_Ty*)(new BYTE[dwSize / sizeof(BYTE)]));
        if (!TokenInformation) {
            SetLastError(ERROR_OUTOFMEMORY);
            return FALSE;
        }
        return GetTokenInformation(TokenHandle, TokenInformationClass, TokenInformation.get(), dwSize, &dwSize);
    } else
        return FALSE;
}


template<class _Elem, class _Traits, class _Ax>
inline _Success_(return != 0) BOOL
QueryFullProcessImageNameA(_In_ HANDLE hProcess, _In_ DWORD dwFlags, _Inout_ std::basic_string<_Elem, _Traits, _Ax>& sExeName)
{
    _Elem szStackBuffer[WINSTD_STACK_BUFFER_BYTES / sizeof(_Elem)];
    DWORD dwSize = _countof(szStackBuffer);

    // Try with stack buffer first.
    if (::QueryFullProcessImageNameA(hProcess, dwFlags, szStackBuffer, &dwSize)) {
        // Copy from stack.
        sExeName.assign(szStackBuffer, dwSize);
        return TRUE;
    }
    for (DWORD dwCapacity = 2 * WINSTD_STACK_BUFFER_BYTES / sizeof(_Elem); GetLastError() == ERROR_INSUFFICIENT_BUFFER; dwCapacity *= 2) {
        // Allocate on heap and retry.
        std::unique_ptr<_Elem[]> szBuffer(new _Elem[dwCapacity]);
        dwSize = dwCapacity;
        if (::QueryFullProcessImageNameA(hProcess, dwFlags, szBuffer.get(), &dwSize)) {
            sExeName.assign(szBuffer.get(), dwSize);
            return TRUE;
        }
    }
    return FALSE;
}


template<class _Elem, class _Traits, class _Ax>
inline _Success_(return != 0) BOOL
QueryFullProcessImageNameW(_In_ HANDLE hProcess, _In_ DWORD dwFlags, _Inout_ std::basic_string<_Elem, _Traits, _Ax>& sExeName)
{
    _Elem szStackBuffer[WINSTD_STACK_BUFFER_BYTES / sizeof(_Elem)];
    DWORD dwSize = _countof(szStackBuffer);

    // Try with stack buffer first.
    if (::QueryFullProcessImageNameW(hProcess, dwFlags, szStackBuffer, &dwSize)) {
        // Copy from stack.
        sExeName.assign(szStackBuffer, dwSize);
        return TRUE;
    }
    for (DWORD dwCapacity = 2 * WINSTD_STACK_BUFFER_BYTES / sizeof(_Elem); GetLastError() == ERROR_INSUFFICIENT_BUFFER; dwCapacity *= 2) {
        // Allocate on heap and retry.
        std::unique_ptr<_Elem[]> szBuffer(new _Elem[dwCapacity]);
        dwSize = dwCapacity;
        if (::QueryFullProcessImageNameW(hProcess, dwFlags, szBuffer.get(), &dwSize)) {
            sExeName.assign(szBuffer.get(), dwSize);
            return TRUE;
        }
    }
    return FALSE;
}
