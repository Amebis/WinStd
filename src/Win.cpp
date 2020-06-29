/*
    Copyright 1991-2020 Amebis
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

#include "PCH.h"


//////////////////////////////////////////////////////////////////////
// StringToGuidA
//////////////////////////////////////////////////////////////////////

_Success_(return) BOOL WINSTD_API StringToGuidA(_In_z_ LPCSTR lpszGuid, _Out_ LPGUID lpGuid, _Out_opt_ LPCSTR *lpszGuidEnd) noexcept
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


_Success_(return) BOOL WINSTD_API StringToGuidW(_In_z_ LPCWSTR lpszGuid, _Out_ LPGUID lpGuid, _Out_opt_ LPCWSTR *lpszGuidEnd) noexcept
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


//////////////////////////////////////////////////////////////////////
// winstd::library
//////////////////////////////////////////////////////////////////////

winstd::library::~library()
{
    if (m_h != invalid)
        FreeLibrary(m_h);
}


void winstd::library::free_internal() noexcept
{
    FreeLibrary(m_h);
}


//////////////////////////////////////////////////////////////////////
// winstd::critical_section
//////////////////////////////////////////////////////////////////////

winstd::critical_section::critical_section()
{
    __try {
        InitializeCriticalSection(&m_data);
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        throw std::runtime_error("InitializeCriticalSection failed");
    }
}


winstd::critical_section::~critical_section()
{
    DeleteCriticalSection(&m_data);
}


//////////////////////////////////////////////////////////////////////
// winstd::find_file
//////////////////////////////////////////////////////////////////////

winstd::find_file::~find_file()
{
    if (m_h != invalid) {
        FindClose(m_h);
    }
}


void winstd::find_file::free_internal() noexcept
{
    FindClose(m_h);
}


//////////////////////////////////////////////////////////////////////
// winstd::heap
//////////////////////////////////////////////////////////////////////

winstd::heap::~heap()
{
    if (m_h != invalid) {
        enumerate();
        HeapDestroy(m_h);
    }
}


bool winstd::heap::enumerate() noexcept
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


void winstd::heap::free_internal() noexcept
{
    enumerate();
    HeapDestroy(m_h);
}


//////////////////////////////////////////////////////////////////////
// winstd::actctx_activator
//////////////////////////////////////////////////////////////////////

winstd::actctx_activator::actctx_activator(_In_ HANDLE hActCtx) noexcept
{
    if (!ActivateActCtx(hActCtx, &m_cookie))
        m_cookie = 0;
}


winstd::actctx_activator::~actctx_activator()
{
    if (m_cookie)
        DeactivateActCtx(0, m_cookie);
}


//////////////////////////////////////////////////////////////////////
// winstd::user_impersonator
//////////////////////////////////////////////////////////////////////

winstd::user_impersonator::user_impersonator(_In_opt_ HANDLE hToken) noexcept
{
    m_cookie = hToken && ImpersonateLoggedOnUser(hToken);
}


winstd::user_impersonator::~user_impersonator()
{
    if (m_cookie)
        RevertToSelf();
}


//////////////////////////////////////////////////////////////////////
// winstd::console_ctrl_handler
//////////////////////////////////////////////////////////////////////

winstd::console_ctrl_handler::console_ctrl_handler(_In_opt_ PHANDLER_ROUTINE HandlerRoutine) noexcept :
    m_handler(HandlerRoutine)
{
    m_cookie = SetConsoleCtrlHandler(m_handler, TRUE);
}


winstd::console_ctrl_handler::~console_ctrl_handler()
{
    if (m_cookie)
        SetConsoleCtrlHandler(m_handler, FALSE);
}


//////////////////////////////////////////////////////////////////////
// winstd::vmemory
//////////////////////////////////////////////////////////////////////

winstd::vmemory::~vmemory()
{
    if (m_h != invalid)
        VirtualFreeEx(m_proc, m_h, 0, MEM_RELEASE);
}


void winstd::vmemory::free_internal() noexcept
{
    VirtualFreeEx(m_proc, m_h, 0, MEM_RELEASE);
}


//////////////////////////////////////////////////////////////////////
// winstd::reg_key
//////////////////////////////////////////////////////////////////////

winstd::reg_key::~reg_key()
{
    if (m_h != invalid)
        RegCloseKey(m_h);
}


bool winstd::reg_key::delete_subkey(_In_z_ LPCTSTR szSubkey)
{
    LSTATUS s;

    s = RegDeleteKey(m_h, szSubkey);
    if (s == ERROR_SUCCESS || s == ERROR_FILE_NOT_FOUND)
        return true;

    {
        reg_key k;
        if (!k.open(m_h, szSubkey, 0, KEY_ENUMERATE_SUB_KEYS))
            return false;
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


void winstd::reg_key::free_internal() noexcept
{
    RegCloseKey(m_h);
}


//////////////////////////////////////////////////////////////////////
// winstd::security_id
//////////////////////////////////////////////////////////////////////

winstd::security_id::~security_id()
{
    if (m_h != invalid)
        FreeSid(m_h);
}


void winstd::security_id::free_internal() noexcept
{
    FreeSid(m_h);
}
