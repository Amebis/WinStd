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

#include <Windows.h>

#include <string>
#include <vector>

namespace winstd
{
    ///
    /// \defgroup WinStdWinAPI Windows API
    /// Integrates WinStd classes with Microsoft Windows API
    ///
    /// @{

    ///
    /// Windows HANDLE wrapper class
    ///
    class WINSTD_API win_handle;

    ///
    /// Module handle wrapper
    ///
    class WINSTD_API library;

    ///
    /// Process handle wrapper
    ///
    class WINSTD_API process;

    ///
    /// Heap handle wrapper
    ///
    class WINSTD_API heap;

    ///
    /// Activates given activation context in constructor and deactivates it in destructor
    ///
    class WINSTD_API actctx_activator;

    ///
    /// Lets the calling thread impersonate the security context of a logged-on user
    ///
    class WINSTD_API user_impersonator;

    ///
    /// Memory in virtual address space of a process handle wrapper
    ///
    class WINSTD_API vmemory;

    /// @}
}


/// \addtogroup WinStdWinAPI
/// @{

///
/// Retrieves the fully qualified path for the file that contains the specified module and stores it in a std::string string.
///
/// \sa [GetModuleFileName function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms683197.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline DWORD GetModuleFileNameA(_In_opt_ HMODULE hModule, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue);

///
/// Retrieves the fully qualified path for the file that contains the specified module and stores it in a std::wstring string.
///
/// \sa [GetModuleFileName function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms683197.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline DWORD GetModuleFileNameW(_In_opt_ HMODULE hModule, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue);

///
/// Copies the text of the specified window's title bar (if it has one) into a std::string string.
///
/// \sa [GetWindowText function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms633520.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline int GetWindowTextA(_In_ HWND hWnd, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue);

///
/// Copies the text of the specified window's title bar (if it has one) into a std::wstring string.
///
/// \sa [GetWindowText function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms633520.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline int GetWindowTextW(_In_ HWND hWnd, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue);

///
/// Retrieves version information for the specified file and stores it in a std::vector buffer.
///
/// \sa [GetFileVersionInfo function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms647003.aspx)
///
template<class _Ty, class _Ax> inline BOOL GetFileVersionInfoA(_In_ LPCSTR lptstrFilename, __reserved DWORD dwHandle, _Out_ std::vector<_Ty, _Ax> &aValue);

///
/// Retrieves version information for the specified file and stores it in a std::vector buffer.
///
/// \sa [GetFileVersionInfo function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms647003.aspx)
///
template<class _Ty, class _Ax> inline BOOL GetFileVersionInfoW(_In_ LPCWSTR lptstrFilename, __reserved DWORD dwHandle, _Out_ std::vector<_Ty, _Ax> &aValue);

///
/// Expands environment-variable strings, replaces them with the values defined for the current user, and stores it in a std::string string.
///
/// \sa [ExpandEnvironmentStrings function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724265.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline DWORD ExpandEnvironmentStringsW(_In_ LPCSTR lpSrc, std::basic_string<_Elem, _Traits, _Ax> &sValue);

///
/// Expands environment-variable strings, replaces them with the values defined for the current user, and stores it in a std::wstring string.
///
/// \sa [ExpandEnvironmentStrings function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724265.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline DWORD ExpandEnvironmentStringsW(_In_ LPCWSTR lpSrc, std::basic_string<_Elem, _Traits, _Ax> &sValue);

///
/// Formats GUID and stores it in a std::string string.
///
/// \param[in ] lpGuid  Pointer to GUID
/// \param[out] str     String to store the result to
///
template<class _Elem, class _Traits, class _Ax> inline VOID GuidToStringA(_In_ LPCGUID lpGuid, _Out_ std::basic_string<_Elem, _Traits, _Ax> &str);

///
/// Formats GUID and stores it in a std::wstring string.
///
/// \param[in ] lpGuid  Pointer to GUID
/// \param[out] str     String to store the result to
///
template<class _Elem, class _Traits, class _Ax> inline VOID GuidToStringW(_In_ LPCGUID lpGuid, _Out_ std::basic_string<_Elem, _Traits, _Ax> &str);
#ifdef _UNICODE
#define GuidToString GuidToStringW
#else
#define GuidToString GuidToStringA
#endif

///
/// Parses string with GUID and stores it to GUID
///
/// \param[in ] lpszGuid  String with GUID
/// \param[out] lpGuid    GUID to store the result to
///
BOOL WINSTD_API StringToGuidA(_In_z_ LPCSTR lpszGuid, _Out_ LPGUID lpGuid, _Out_ LPCSTR *lpszGuidEnd = NULL);

///
/// Parses string with GUID and stores it to GUID
///
/// \param[in ] lpszGuid  String with GUID
/// \param[out] lpGuid    GUID to store the result to
///
BOOL WINSTD_API StringToGuidW(_In_z_ LPCWSTR lpszGuid, _Out_ LPGUID lpGuid, _Out_ LPCWSTR *lpszGuidEnd = NULL);

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
template<class _Elem, class _Traits, class _Ax> inline LSTATUS RegQueryStringValue(_In_ HKEY hReg, _In_z_ LPCSTR pszName, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue);

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
template<class _Elem, class _Traits, class _Ax> inline LSTATUS RegQueryStringValue(_In_ HKEY hReg, _In_z_ LPCWSTR pszName, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue);

///
/// Retrieves the type and data for the specified value name associated with an open registry key and stores the data in a std::vector buffer.
///
/// \sa [RegQueryValueEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724911.aspx)
///
template<class _Ty, class _Ax> inline LSTATUS RegQueryValueExA(_In_ HKEY hKey, _In_opt_ LPCSTR lpValueName, __reserved LPDWORD lpReserved, _Out_opt_ LPDWORD lpType, _Out_ std::vector<_Ty, _Ax> &aData);

///
/// Retrieves the type and data for the specified value name associated with an open registry key and stores the data in a std::vector buffer.
///
/// \sa [RegQueryValueEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724911.aspx)
///
template<class _Ty, class _Ax> inline LSTATUS RegQueryValueExW(_In_ HKEY hKey, _In_opt_ LPCWSTR lpValueName, __reserved LPDWORD lpReserved, _Out_opt_ LPDWORD lpType, _Out_ std::vector<_Ty, _Ax> &aData);

#if _WIN32_WINNT >= _WIN32_WINNT_VISTA

///
/// Loads the specified string from the specified key and subkey, and stores it in a std::string string.
///
/// \sa [RegLoadMUIString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724890.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline LSTATUS RegLoadMUIStringA(_In_ HKEY hKey, _In_opt_ LPCSTR pszValue, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sOut, _In_ DWORD Flags, _In_opt_ LPCSTR pszDirectory);

///
/// Loads the specified string from the specified key and subkey, and stores it in a std::wstring string.
///
/// \sa [RegLoadMUIString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724890.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline LSTATUS RegLoadMUIStringW(_In_ HKEY hKey, _In_opt_ LPCWSTR pszValue, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sOut, _In_ DWORD Flags, _In_opt_ LPCWSTR pszDirectory);

#endif

///
/// Maps a UTF-16 (wide character) string to a std::string. The new character string is not necessarily from a multibyte character set.
///
/// \sa [WideCharToMultiByte function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline int WideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cchWideChar) LPCWSTR lpWideCharStr, _In_ int cchWideChar, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sMultiByteStr, _In_opt_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar);

///
/// Maps a character string to a UTF-16 (wide character) std::wstring. The character string is not necessarily from a multibyte character set.
///
/// \sa [MultiByteToWideChar function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd319072.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline int MultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cbMultiByte) LPCSTR lpMultiByteStr, _In_ int cbMultiByte, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sWideCharStr);

///
/// Loads a string resource from the executable file associated with a specified module.
///
/// \sa [LoadString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms647486.aspx)
///
template<class _Traits, class _Ax> inline int WINAPI LoadString(_In_opt_ HINSTANCE hInstance, _In_ UINT uID, _Out_ std::basic_string<char, _Traits, _Ax> &sBuffer);

///
/// Loads a string resource from the executable file associated with a specified module.
///
/// \sa [LoadString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms647486.aspx)
///
template<class _Traits, class _Ax> inline int WINAPI LoadString(_In_opt_ HINSTANCE hInstance, _In_ UINT uID, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sBuffer);

///
/// Formats and sends a string to the debugger for display.
///
/// \sa [OutputDebugString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363362.aspx)
///
inline VOID OutputDebugStrV(_In_ LPCSTR lpOutputString, _In_ va_list arg);

///
/// Formats and sends a string to the debugger for display.
///
/// \sa [OutputDebugString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363362.aspx)
///
inline VOID OutputDebugStrV(_In_ LPCWSTR lpOutputString, _In_ va_list arg);

///
/// Formats and sends a string to the debugger for display.
///
/// \sa [OutputDebugString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363362.aspx)
///
inline VOID OutputDebugStr(_In_ LPCSTR lpOutputString, ...);

///
/// Formats and sends a string to the debugger for display.
///
/// \sa [OutputDebugString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363362.aspx)
///
inline VOID OutputDebugStr(_In_ LPCWSTR lpOutputString, ...);

///
/// Formats a date as a date string for a locale specified by the locale identifier. The function formats either a specified date or the local system date. 
///
/// \sa [GetDateFormat function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd318086.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline int GetDateFormatA(_In_ LCID Locale, _In_ DWORD dwFlags, _In_opt_ const SYSTEMTIME *lpDate, _In_opt_ LPCSTR lpFormat, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sDate);

///
/// Formats a date as a date string for a locale specified by the locale identifier. The function formats either a specified date or the local system date. 
///
/// \sa [GetDateFormat function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd318086.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline int GetDateFormatW(_In_ LCID Locale, _In_ DWORD dwFlags, _In_opt_ const SYSTEMTIME *lpDate, _In_opt_ LPCWSTR lpFormat, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sDate);

///
/// Retrieves the name of the account for this SID and the name of the first domain on which this SID is found.
///
/// \sa [LookupAccountSid function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379166.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline BOOL LookupAccountSidA(_In_opt_ LPCSTR lpSystemName, _In_ PSID lpSid, _Out_opt_ std::basic_string<_Elem, _Traits, _Ax> *sName, _Out_opt_ std::basic_string<_Elem, _Traits, _Ax> *sReferencedDomainName, _Out_ PSID_NAME_USE peUse);

///
/// Retrieves the name of the account for this SID and the name of the first domain on which this SID is found.
///
/// \sa [LookupAccountSid function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379166.aspx)
///
template<class _Elem, class _Traits, class _Ax> inline BOOL LookupAccountSidW(_In_opt_ LPCWSTR lpSystemName, _In_ PSID lpSid, _Out_opt_ std::basic_string<_Elem, _Traits, _Ax> *sName, _Out_opt_ std::basic_string<_Elem, _Traits, _Ax> *sReferencedDomainName, _Out_ PSID_NAME_USE peUse);

/// @}

#pragma once


namespace winstd
{
    class WINSTD_API win_handle : public handle<HANDLE>
    {
    public:
        ///
        /// Initializes a new class instance with the object handle set to NULL.
        ///
        inline win_handle() : handle<HANDLE>() {}

        ///
        /// Initializes a new class instance with an already available object handle.
        ///
        /// \param[in] h  Initial object handle value
        ///
        inline win_handle(_In_opt_ handle_type h) : handle<HANDLE>(h) {}

        ///
        /// Move constructor
        ///
        /// \param[inout] h  A rvalue reference of another object
        ///
        inline win_handle(_Inout_ win_handle &&h) : handle<HANDLE>(std::move(h)) {}

        ///
        /// Closes an open object handle.
        ///
        /// \sa [CloseHandle function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724211.aspx)
        ///
        virtual ~win_handle();

        ///
        /// Move assignment
        ///
        /// \param[inout] h  A rvalue reference of another object
        ///
        win_handle& operator=(_Inout_ win_handle &&h)
        {
            if (this != std::addressof(h))
                *(handle<handle_type>*)this = std::move(h);
            return *this;
        }

    private:
        // This class is noncopyable.
        win_handle(_In_ const win_handle &h);
        win_handle& operator=(_In_ const win_handle &h);

    protected:
        ///
        /// Closes an open object handle.
        ///
        /// \sa [CloseHandle function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms724211.aspx)
        ///
        virtual void free_internal();
    };


    class WINSTD_API library : public handle<HMODULE>
    {
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
        inline bool load(_In_ LPCTSTR lpFileName, __reserved handle_type hFile, _In_ DWORD dwFlags)
        {
            handle_type h = LoadLibraryEx(lpFileName, hFile, dwFlags);
            if (h) {
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
        virtual void free_internal();
    };


    class WINSTD_API process : public win_handle
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
        inline bool open(_In_ DWORD dwDesiredAccess, _In_ BOOL bInheritHandle, _In_ DWORD dwProcessId)
        {
            handle_type h = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
            if (h) {
                attach(h);
                return true;
            } else
                return false;
        }
    };


    class WINSTD_API heap : public handle<HANDLE>
    {
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
        inline bool create(_In_ DWORD flOptions, _In_ SIZE_T dwInitialSize, _In_ SIZE_T dwMaximumSize)
        {
            handle_type h = HeapCreate(flOptions, dwInitialSize, dwMaximumSize);
            if (h) {
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
        bool enumerate();

    protected:
        ///
        /// Destroys the heap.
        ///
        /// \sa [HeapDestroy function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa366700.aspx)
        ///
        virtual void free_internal();
    };


    class WINSTD_API actctx_activator
    {
    public:
        ///
        /// Construct the activator and activates the given activation context
        ///
        /// \param[in] hActCtx  Activation context
        ///
        /// \sa [ActivateActCtx function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa374151.aspx)
        ///
        actctx_activator(_In_ HANDLE hActCtx);

        ///
        /// Deactivates activation context and destructs the activator
        ///
        /// \sa [DeactivateActCtx function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa375140.aspx)
        ///
        virtual ~actctx_activator();

    protected:
        ULONG_PTR m_cookie; ///< Cookie for context deactivation
    };


    class WINSTD_API user_impersonator
    {
    public:
        ///
        /// Construct the impersonator and impersonates the given user
        ///
        /// \param[in] hToken  A handle to a primary or impersonation access token that represents a logged-on user
        ///
        /// \sa [ImpersonateLoggedOnUser function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa378612.aspx)
        ///
        user_impersonator(_In_ HANDLE hToken);

        ///
        /// Reverts to current user and destructs the impersonator
        ///
        /// \sa [RevertToSelf function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa379317.aspx)
        ///
        virtual ~user_impersonator();

    protected:
        BOOL m_cookie; ///< Did impersonation succeed?
    };


    class WINSTD_API vmemory : public handle<LPVOID>
    {
    public:
        ///
        /// Initializes a new class instance with the memory handle set to NULL.
        ///
        inline vmemory() :
            m_proc(NULL),
            handle<LPVOID>()
        {
        }

        ///
        /// Initializes a new class instance with an already available object handle.
        ///
        /// \param[in] proc  Handle of process the memory belongs to
        /// \param[in] h     Initial object handle value
        ///
        inline vmemory(_In_ HANDLE proc, _In_opt_ handle_type h) :
            m_proc(proc),
            handle<LPVOID>(h)
        {
        }

        ///
        /// Move constructor
        ///
        /// \param[inout] h  A rvalue reference of another object
        ///
        inline vmemory(_Inout_ vmemory &&h)
        {
            // Transfer process.
            m_proc = h.m_proc;

            // Transfer handle.
            m_h   = h.m_h;
            h.m_h = NULL;
        }

        ///
        /// Frees the memory.
        ///
        /// \sa [VirtualFreeEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa366894.aspx)
        ///
        virtual ~vmemory();

        ///
        /// Sets a new memory handle for the class
        ///
        /// When the current object handle of the class is non-NULL, the object is destroyed first.
        ///
        /// \param[in] proc  Handle of process the memory belongs to
        /// \param[in] h     Initial object handle value
        ///
        inline void attach(_In_ HANDLE proc, _In_opt_ handle_type h)
        {
            m_proc = proc;
            if (m_h)
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
            _In_     DWORD  flProtect)
        {
            handle_type h = VirtualAllocEx(hProcess, lpAddress, dwSize, flAllocationType, flProtect);
            if (h) {
                attach(hProcess, h);
                return true;
            } else
                return false;
        }

    private:
        // This class is noncopyable.
        vmemory(_In_ const vmemory &h);
        vmemory& operator=(_In_ const vmemory &h);

    protected:
        ///
        /// Frees the memory.
        ///
        /// \sa [VirtualFreeEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa366894.aspx)
        ///
        virtual void free_internal();

    protected:
        HANDLE m_proc;  ///< Handle of memory's process
    };
}


template<class _Elem, class _Traits, class _Ax>
inline DWORD GetModuleFileNameA(_In_opt_ HMODULE hModule, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue)
{
    assert(0); // TODO: Test this code.

    _Elem szBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];

    // Try with stack buffer first.
    DWORD dwResult = ::GetModuleFileNameA(hModule, szBuffer, _countof(szBuffer));
    if (dwResult < _countof(szBuffer)) {
        // Copy from stack.
        sValue.assign(szBuffer, dwResult);
        return dwResult;
    } else {
        for (DWORD dwCapacity = 2*WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem);; dwCapacity *= 2) {
            // Allocate on heap and retry.
            auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[dwCapacity]);
            dwResult = ::GetModuleFileNameA(hModule, szBuffer.get(), dwCapacity);
            if (dwResult < dwCapacity) {
                sValue.assign(szBuffer.get(), dwResult);
                return dwResult;
            }
        }
    }
}


template<class _Elem, class _Traits, class _Ax>
inline DWORD GetModuleFileNameW(_In_opt_ HMODULE hModule, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue)
{
    _Elem szBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];

    // Try with stack buffer first.
    DWORD dwResult = ::GetModuleFileNameW(hModule, szBuffer, _countof(szBuffer));
    if (dwResult < _countof(szBuffer)) {
        // Copy from stack.
        sValue.assign(szBuffer, dwResult);
        return dwResult;
    } else {
        for (DWORD dwCapacity = 2*WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem);; dwCapacity *= 2) {
            // Allocate on heap and retry.
            auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[dwCapacity]);
            dwResult = ::GetModuleFileNameW(hModule, szBuffer.get(), dwCapacity);
            if (dwResult < dwCapacity) {
                sValue.assign(szBuffer.get(), dwResult);
                return dwResult;
            }
        }
    }
}


template<class _Elem, class _Traits, class _Ax>
inline int GetWindowTextA(_In_ HWND hWnd, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue)
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
            auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[++iResult]);
            iResult = ::GetWindowTextA(hWnd, szBuffer.get(), iResult);
            sValue.assign(szBuffer.get(), iResult);
        }
        return iResult;
    }

    sValue.clear();
    return 0;
}


template<class _Elem, class _Traits, class _Ax>
inline int GetWindowTextW(_In_ HWND hWnd, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue)
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
            auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[++iResult]);
            iResult = ::GetWindowTextW(hWnd, szBuffer.get(), iResult);
            sValue.assign(szBuffer.get(), iResult);
        }
        return iResult;
    }

    sValue.clear();
    return 0;
}


template<class _Ty, class _Ax>
inline BOOL GetFileVersionInfoA(_In_ LPCSTR lptstrFilename, __reserved DWORD dwHandle, _Out_ std::vector<_Ty, _Ax> &aValue)
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
inline BOOL GetFileVersionInfoW(_In_ LPCWSTR lptstrFilename, __reserved DWORD dwHandle, _Out_ std::vector<_Ty, _Ax> &aValue)
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
inline DWORD ExpandEnvironmentStringsW(_In_ LPCSTR lpSrc, std::basic_string<_Elem, _Traits, _Ax> &sValue)
{
    assert(0); // TODO: Test this code.

    for (DWORD dwSizeOut = (DWORD)strlen(lpSrc) + 0x100;;) {
        DWORD dwSizeIn = dwSizeOut;
        auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[dwSizeIn + 2]); // Note: ANSI version requires one extra char.
        dwSizeOut = ::ExpandEnvironmentStringsA(lpSrc, szBuffer.get(), dwSizeIn);
        if (dwSizeOut == 0) {
            // Error.
            break;
        } else if (dwSizeOut <= dwSizeIn) {
            // The buffer was sufficient.
            sValue.assign(szBuffer.get(), dwSizeOut);
            return dwSizeOut;
        }
    }

    sValue.clear();
    return 0;
}


template<class _Elem, class _Traits, class _Ax>
inline DWORD ExpandEnvironmentStringsW(_In_ LPCWSTR lpSrc, std::basic_string<_Elem, _Traits, _Ax> &sValue)
{
    assert(0); // TODO: Test this code.

    for (DWORD dwSizeOut = (DWORD)wcslen(lpSrc) + 0x100;;) {
        DWORD dwSizeIn = dwSizeOut;
        auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[dwSizeIn + 1]);
        dwSizeOut = ::ExpandEnvironmentStringsW(lpSrc, szBuffer.get(), dwSizeIn);
        if (dwSizeOut == 0) {
            // Error.
            break;
        } else if (dwSizeOut <= dwSizeIn) {
            // The buffer was sufficient.
            sValue.assign(szBuffer.get(), dwSizeOut);
            return dwSizeOut;
        }
    }

    sValue.clear();
    return 0;
}


template<class _Elem, class _Traits, class _Ax>
inline VOID GuidToStringA(_In_ LPCGUID lpGuid, _Out_ std::basic_string<_Elem, _Traits, _Ax> &str)
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
inline VOID GuidToStringW(_In_ LPCGUID lpGuid, _Out_ std::basic_string<_Elem, _Traits, _Ax> &str)
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
inline LSTATUS RegQueryStringValue(_In_ HKEY hReg, _In_z_ LPCSTR pszName, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue)
{
    assert(0); // TODO: Test this code.

    LSTATUS lResult;
    BYTE aStackBuffer[WINSTD_STACK_BUFFER_BYTES];
    DWORD dwSize = sizeof(aStackBuffer), dwType;

    // Try with stack buffer first.
    lResult = ::RegQueryValueExA(hReg, pszName, NULL, &dwType, aStackBuffer, &dwSize);
    if (lResult == ERROR_SUCCESS) {
        if (dwType == REG_SZ || dwType == REG_MULTI_SZ) {
            // The value is REG_SZ or REG_MULTI_SZ.
            sValue.assign((_Elem*)aStackBuffer, dwSize / sizeof(_Elem));
        } else if (dwType == REG_EXPAND_SZ) {
            // The value is REG_EXPAND_SZ. Expand it from stack buffer.
            if (::ExpandEnvironmentStringsW((const _Elem*)aStackBuffer, sValue) == 0)
                lResult = ::GetLastError();
        } else {
            // The value is not a string type.
            lResult = ERROR_INVALID_DATA;
        }
    } else if (lResult == ERROR_MORE_DATA) {
        if (dwType == REG_SZ || dwType == REG_MULTI_SZ) {
            // The value is REG_SZ or REG_MULTI_SZ. Read it now.
            auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[dwSize / sizeof(_Elem)]);
            if ((lResult = ::RegQueryValueExA(hReg, pszName, NULL, NULL, (LPBYTE)szBuffer.get(), &dwSize)) == ERROR_SUCCESS)
                sValue.assign(szBuffer.get(), dwSize / sizeof(_Elem));
            else
                sValue.clear();
        } else if (dwType == REG_EXPAND_SZ) {
            // The value is REG_EXPAND_SZ. Read it and expand environment variables.
            auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[dwSize / sizeof(_Elem)]);
            if ((lResult = ::RegQueryValueExA(hReg, pszName, NULL, NULL, (LPBYTE)szBuffer.get(), &dwSize)) == ERROR_SUCCESS) {
                if (::ExpandEnvironmentStringsW(szBuffer.get(), sValue) == 0)
                    lResult = ::GetLastError();
            } else
                sValue.clear();
        } else {
            // The value is not a string type.
            lResult = ERROR_INVALID_DATA;
        }
    }

    return lResult;
}


template<class _Elem, class _Traits, class _Ax>
inline LSTATUS RegQueryStringValue(_In_ HKEY hReg, _In_z_ LPCWSTR pszName, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sValue)
{
    assert(0); // TODO: Test this code.

    LSTATUS lResult;
    BYTE aStackBuffer[WINSTD_STACK_BUFFER_BYTES];
    DWORD dwSize = sizeof(aStackBuffer), dwType;

    // Try with stack buffer first.
    lResult = ::RegQueryValueExW(hReg, pszName, NULL, &dwType, aStackBuffer, &dwSize);
    if (lResult == ERROR_SUCCESS) {
        if (dwType == REG_SZ || dwType == REG_MULTI_SZ) {
            // The value is REG_SZ or REG_MULTI_SZ.
            sValue.assign((_Elem*)aStackBuffer, dwSize / sizeof(_Elem));
        } else if (dwType == REG_EXPAND_SZ) {
            // The value is REG_EXPAND_SZ. Expand it from stack buffer.
            if (::ExpandEnvironmentStringsW((const _Elem*)aStackBuffer, sValue) == 0)
                lResult = ::GetLastError();
        } else {
            // The value is not a string type.
            lResult = ERROR_INVALID_DATA;
        }
    } else if (lResult == ERROR_MORE_DATA) {
        if (dwType == REG_SZ || dwType == REG_MULTI_SZ) {
            // The value is REG_SZ or REG_MULTI_SZ. Read it now.
            auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[dwSize / sizeof(_Elem)]);
            if ((lResult = ::RegQueryValueExW(hReg, pszName, NULL, NULL, (LPBYTE)szBuffer.get(), &dwSize)) == ERROR_SUCCESS)
                sValue.assign(szBuffer.get(), dwSize / sizeof(_Elem));
            else
                sValue.clear();
        } else if (dwType == REG_EXPAND_SZ) {
            // The value is REG_EXPAND_SZ. Read it and expand environment variables.
            auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[dwSize / sizeof(_Elem)]);
            if ((lResult = ::RegQueryValueExW(hReg, pszName, NULL, NULL, (LPBYTE)szBuffer.get(), &dwSize)) == ERROR_SUCCESS) {
                if (::ExpandEnvironmentStringsW(szBuffer.get(), sValue) == 0)
                    lResult = ::GetLastError();
            } else
                sValue.clear();
        } else {
            // The value is not a string type.
            lResult = ERROR_INVALID_DATA;
        }
    }

    return lResult;
}


template<class _Ty, class _Ax>
inline LSTATUS RegQueryValueExA(_In_ HKEY hKey, _In_opt_ LPCSTR lpValueName, __reserved LPDWORD lpReserved, _Out_opt_ LPDWORD lpType, _Out_ std::vector<_Ty, _Ax> &aData)
{
    assert(0); // TODO: Test this code.

    LSTATUS lResult;
    BYTE aStackBuffer[WINSTD_STACK_BUFFER_BYTES];
    DWORD dwSize = sizeof(aStackBuffer);

    // Try with stack buffer first.
    lResult = RegQueryValueExA(hKey, lpValueName, lpReserved, NULL, aStackBuffer, &dwSize);
    if (lResult == ERROR_SUCCESS) {
        // Copy from stack buffer.
        aData.resize((dwSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        memcpy(aData.data(), aStackBuffer, dwSize);
    } else if (lResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap and retry.
        aData.resize((dwSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        if ((lResult = RegQueryValueExA(hKey, lpValueName, lpReserved, lpType, aData.data(), &dwSize)) != ERROR_SUCCESS)
            aData.clear();
    }

    return lResult;
}


template<class _Ty, class _Ax>
inline LSTATUS RegQueryValueExW(_In_ HKEY hKey, _In_opt_ LPCWSTR lpValueName, __reserved LPDWORD lpReserved, _Out_opt_ LPDWORD lpType, _Out_ std::vector<_Ty, _Ax> &aData)
{
    assert(0); // TODO: Test this code.

    LSTATUS lResult;
    BYTE aStackBuffer[WINSTD_STACK_BUFFER_BYTES];
    DWORD dwSize = sizeof(aStackBuffer);

    // Try with stack buffer first.
    lResult = RegQueryValueExW(hKey, lpValueName, lpReserved, NULL, aStackBuffer, &dwSize);
    if (lResult == ERROR_SUCCESS) {
        // Copy from stack buffer.
        aData.resize((dwSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        memcpy(aData.data(), aStackBuffer, dwSize);
    } else if (lResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap and retry.
        aData.resize((dwSize + sizeof(_Ty) - 1) / sizeof(_Ty));
        if ((lResult = RegQueryValueExW(hKey, lpValueName, lpReserved, lpType, aData.data(), &dwSize)) != ERROR_SUCCESS)
            aData.clear();
    }

    return lResult;
}


#if _WIN32_WINNT >= _WIN32_WINNT_VISTA

template<class _Elem, class _Traits, class _Ax>
inline LSTATUS RegLoadMUIStringA(_In_ HKEY hKey, _In_opt_ LPCSTR pszValue, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sOut, _In_ DWORD Flags, _In_opt_ LPCSTR pszDirectory)
{
    assert(0); // TODO: Test this code.

    LSTATUS lResult;
    _Elem szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];
    DWORD dwSize;

    Flags &= ~REG_MUI_STRING_TRUNCATE;

    // Try with stack buffer first.
    lResult = RegLoadMUIStringA(hKey, pszValue, szStackBuffer, _countof(szStackBuffer), &dwSize, Flags, pszDirectory);
    if (lResult == ERROR_SUCCESS) {
        // Copy from stack buffer.
        sOut.assign(szStackBuffer, dwSize);
    } else if (lResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap and retry.
        auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[dwSize + 1]);
        sOut.assign(szBuffer.get(), (lResult = RegLoadMUIStringA(hKey, pszValue, szBuffer.get(), dwSize, &dwSize, Flags, pszDirectory)) == ERROR_SUCCESS ? dwSize : 0);
    }

    return lResult;
}


template<class _Elem, class _Traits, class _Ax>
inline LSTATUS RegLoadMUIStringW(_In_ HKEY hKey, _In_opt_ LPCWSTR pszValue, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sOut, _In_ DWORD Flags, _In_opt_ LPCWSTR pszDirectory)
{
    assert(0); // TODO: Test this code.

    LSTATUS lResult;
    _Elem szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];
    DWORD dwSize;

    Flags &= ~REG_MUI_STRING_TRUNCATE;

    // Try with stack buffer first.
    lResult = RegLoadMUIStringW(hKey, pszValue, szStackBuffer, _countof(szStackBuffer), &dwSize, Flags, pszDirectory);
    if (lResult == ERROR_SUCCESS) {
        // Copy from stack buffer.
        sOut.assign(szStackBuffer, dwSize);
    } else if (lResult == ERROR_MORE_DATA) {
        // Allocate buffer on heap and retry.
        auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[dwSize + 1]);
        sOut.assign(szBuffer.get(), (lResult = RegLoadMUIStringW(hKey, pszValue, szBuffer.get(), dwSize, &dwSize, Flags, pszDirectory)) == ERROR_SUCCESS ? dwSize : 0);
    }

    return lResult;
}

#endif


template<class _Elem, class _Traits, class _Ax>
inline int WideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cchWideChar) LPCWSTR lpWideCharStr, _In_ int cchWideChar, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sMultiByteStr, _In_opt_ LPCSTR lpDefaultChar, _Out_opt_ LPBOOL lpUsedDefaultChar)
{
    _Elem szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];

    // Try to convert to stack buffer first.
    int cch = ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, szStackBuffer, _countof(szStackBuffer), lpDefaultChar, lpUsedDefaultChar);
    if (cch) {
        // Copy from stack. Be careful not to include zero terminator.
        sMultiByteStr.assign(szStackBuffer, cchWideChar != -1 ? strnlen(szStackBuffer, cch) : cch - 1);
    } else if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Query the required output size. Allocate buffer. Then convert again.
        cch = ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, NULL, 0, lpDefaultChar, lpUsedDefaultChar);
        auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[cch]);
        cch = ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, szBuffer.get(), cch, lpDefaultChar, lpUsedDefaultChar);
        sMultiByteStr.assign(szBuffer.get(), cchWideChar != -1 ? strnlen(szBuffer.get(), cch) : cch - 1);
    }

    return cch;
}


template<class _Elem, class _Traits, class _Ax>
inline int MultiByteToWideChar(_In_ UINT CodePage, _In_ DWORD dwFlags, _In_z_count_(cbMultiByte) LPCSTR lpMultiByteStr, _In_ int cbMultiByte, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sWideCharStr)
{
    _Elem szStackBuffer[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];

    // Try to convert to stack buffer first.
    int cch = ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, szStackBuffer, _countof(szStackBuffer));
    if (cch) {
        // Copy from stack.
        sWideCharStr.assign(szStackBuffer, cch);
    } else if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // Query the required output size. Allocate buffer. Then convert again.
        cch = ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, NULL, 0);
        auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[cch]);
        cch = ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, szBuffer.get(), cch);
        sWideCharStr.assign(szBuffer.get(), cch);
    }

    return cch;
}


template<class _Traits, class _Ax>
inline int WINAPI LoadString(_In_opt_ HINSTANCE hInstance, _In_ UINT uID, _Out_ std::basic_string<char, _Traits, _Ax> &sBuffer)
{
    // Get read-only pointer to string resource.
    LPCSTR pszStr;
    int i = LoadStringA(hInstance, uID, (LPSTR)&pszStr, 0);
    if (i) {
        sBuffer.assign(pszStr, i);
        return i;
    } else
        return 0;
}


template<class _Traits, class _Ax>
inline int WINAPI LoadString(_In_opt_ HINSTANCE hInstance, _In_ UINT uID, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &sBuffer)
{
    // Get read-only pointer to string resource.
    LPCWSTR pszStr;
    int i = LoadStringW(hInstance, uID, (LPWSTR)&pszStr, 0);
    if (i) {
        sBuffer.assign(pszStr, i);
        return i;
    } else
        return 0;
}


inline VOID OutputDebugStrV(_In_ LPCSTR lpOutputString, _In_ va_list arg)
{
    std::string str;
    vsprintf(str, lpOutputString, arg);
    OutputDebugStringA(str.c_str());
}


inline VOID OutputDebugStrV(_In_ LPCWSTR lpOutputString, _In_ va_list arg)
{
    std::wstring str;
    vsprintf(str, lpOutputString, arg);
    OutputDebugStringW(str.c_str());
}


inline VOID OutputDebugStr(_In_ LPCSTR lpOutputString, ...)
{
    va_list arg;
    va_start(arg, lpOutputString);
    OutputDebugStrV(lpOutputString, arg);
    va_end(arg);
}


inline VOID OutputDebugStr(_In_ LPCWSTR lpOutputString, ...)
{
    va_list arg;
    va_start(arg, lpOutputString);
    OutputDebugStrV(lpOutputString, arg);
    va_end(arg);
}


template<class _Elem, class _Traits, class _Ax> inline int GetDateFormatA(_In_ LCID Locale, _In_ DWORD dwFlags, _In_opt_ const SYSTEMTIME *lpDate, _In_opt_ LPCSTR lpFormat, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sDate)
{
    int iResult = GetDateFormatA(Locale, dwFlags, lpDate, lpFormat, NULL, 0);
    if (iResult) {
        // Allocate buffer on heap and retry.
        auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[iResult]);
        iResult = GetDateFormatA(Locale, dwFlags, lpDate, lpFormat, szBuffer.get(), iResult);
        sDate.assign(szBuffer.get(), iResult ? iResult - 1 : 0);
        return iResult;
    }

    return iResult;
}


template<class _Elem, class _Traits, class _Ax> inline int GetDateFormatW(_In_ LCID Locale, _In_ DWORD dwFlags, _In_opt_ const SYSTEMTIME *lpDate, _In_opt_ LPCWSTR lpFormat, _Out_ std::basic_string<_Elem, _Traits, _Ax> &sDate)
{
    int iResult = GetDateFormatW(Locale, dwFlags, lpDate, lpFormat, NULL, 0);
    if (iResult) {
        // Allocate buffer on heap and retry.
        auto szBuffer = std::unique_ptr<_Elem[]>(new _Elem[iResult]);
        iResult = GetDateFormatW(Locale, dwFlags, lpDate, lpFormat, szBuffer.get(), iResult);
        sDate.assign(szBuffer.get(), iResult ? iResult - 1 : 0);
        return iResult;
    }

    return iResult;
}


template<class _Elem, class _Traits, class _Ax>
inline BOOL LookupAccountSidA(_In_opt_ LPCSTR lpSystemName, _In_ PSID lpSid, _Out_opt_ std::basic_string<_Elem, _Traits, _Ax> *sName, _Out_opt_ std::basic_string<_Elem, _Traits, _Ax> *sReferencedDomainName, _Out_ PSID_NAME_USE peUse)
{
    assert(0); // TODO: Test this code.

    DWORD dwNameLen = 0, dwRefDomainLen = 0;

    if (LookupAccountSidA(lpSystemName, lpSid,
        NULL, sName                 ? &dwNameLen      : NULL,
        NULL, sReferencedDomainName ? &dwRefDomainLen : NULL,
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
            bufName     .get(), sName                 ? &dwNameLen      : NULL,
            bufRefDomain.get(), sReferencedDomainName ? &dwRefDomainLen : NULL,
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
inline BOOL LookupAccountSidW(_In_opt_ LPCWSTR lpSystemName, _In_ PSID lpSid, _Out_opt_ std::basic_string<_Elem, _Traits, _Ax> *sName, _Out_opt_ std::basic_string<_Elem, _Traits, _Ax> *sReferencedDomainName, _Out_ PSID_NAME_USE peUse)
{
    assert(0); // TODO: Test this code.

    DWORD dwNameLen = 0, dwRefDomainLen = 0;

    if (LookupAccountSidW(lpSystemName, lpSid,
        NULL, sName                 ? &dwNameLen      : NULL,
        NULL, sReferencedDomainName ? &dwRefDomainLen : NULL,
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
            bufName     .get(), sName                 ? &dwNameLen      : NULL,
            bufRefDomain.get(), sReferencedDomainName ? &dwRefDomainLen : NULL,
            peUse))
        {
            if (sName                ) sName                ->assign(bufName     .get(), dwNameLen      - 1);
            if (sReferencedDomainName) sReferencedDomainName->assign(bufRefDomain.get(), dwRefDomainLen - 1);
            return TRUE;
        }
    }

    return FALSE;
}
