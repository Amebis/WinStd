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

#include "StdAfx.h"


//////////////////////////////////////////////////////////////////////
// winstd::win_handle
//////////////////////////////////////////////////////////////////////

winstd::win_handle::~win_handle()
{
    if (m_h)
        CloseHandle(m_h);
}


void winstd::win_handle::free_internal()
{
    CloseHandle(m_h);
}


//////////////////////////////////////////////////////////////////////
// winstd::library
//////////////////////////////////////////////////////////////////////

winstd::library::~library()
{
    if (m_h)
        FreeLibrary(m_h);
}


void winstd::library::free_internal()
{
    FreeLibrary(m_h);
}


//////////////////////////////////////////////////////////////////////
// winstd::heap
//////////////////////////////////////////////////////////////////////

winstd::heap::~heap()
{
    if (m_h) {
        enumerate();
        HeapDestroy(m_h);
    }
}


bool winstd::heap::enumerate()
{
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

    DWORD dwResult = GetLastError();
    if (dwResult != ERROR_NO_MORE_ITEMS)
        OutputDebugStr(_T("HeapWalk failed (error %u).\n"), dwResult);

    // Unlock the heap.
    HeapUnlock(m_h);

    return found;
}


void winstd::heap::free_internal()
{
    enumerate();
    HeapDestroy(m_h);
}


//////////////////////////////////////////////////////////////////////
// winstd::actctx_activator
//////////////////////////////////////////////////////////////////////

winstd::actctx_activator::actctx_activator(_In_ HANDLE hActCtx)
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

winstd::user_impersonator::user_impersonator(_In_ HANDLE hToken)
{
    m_cookie = ImpersonateLoggedOnUser(hToken);
}


winstd::user_impersonator::~user_impersonator()
{
    if (m_cookie)
        RevertToSelf();
}


//////////////////////////////////////////////////////////////////////
// winstd::vmemory
//////////////////////////////////////////////////////////////////////

winstd::vmemory::~vmemory()
{
    if (m_h)
        VirtualFreeEx(m_proc, m_h, 0, MEM_RELEASE);
}


void winstd::vmemory::free_internal()
{
    VirtualFreeEx(m_proc, m_h, 0, MEM_RELEASE);
}
