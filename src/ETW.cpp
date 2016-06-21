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

#if _WIN32_WINNT >= _WIN32_WINNT_VISTA


//////////////////////////////////////////////////////////////////////
// winstd::event_data
//////////////////////////////////////////////////////////////////////

const winstd::event_data winstd::event_data::blank;


//////////////////////////////////////////////////////////////////////
// winstd::event_provider
//////////////////////////////////////////////////////////////////////

winstd::event_provider::~event_provider()
{
    if (m_h)
        EventUnregister(m_h);
}


void winstd::event_provider::free_internal()
{
    EventUnregister(m_h);
}


void winstd::event_provider::enable_callback(_In_ LPCGUID SourceId, _In_ ULONG IsEnabled, _In_ UCHAR Level, _In_ ULONGLONG MatchAnyKeyword, _In_ ULONGLONG MatchAllKeyword, _In_opt_ PEVENT_FILTER_DESCRIPTOR FilterData)
{
    UNREFERENCED_PARAMETER(SourceId);
    UNREFERENCED_PARAMETER(IsEnabled);
    UNREFERENCED_PARAMETER(Level);
    UNREFERENCED_PARAMETER(MatchAnyKeyword);
    UNREFERENCED_PARAMETER(MatchAllKeyword);
    UNREFERENCED_PARAMETER(FilterData);
}


VOID NTAPI winstd::event_provider::enable_callback(_In_ LPCGUID SourceId, _In_ ULONG IsEnabled, _In_ UCHAR Level, _In_ ULONGLONG MatchAnyKeyword, _In_ ULONGLONG MatchAllKeyword, _In_opt_ PEVENT_FILTER_DESCRIPTOR FilterData, _Inout_opt_ PVOID CallbackContext)
{
    if (CallbackContext)
        static_cast<event_provider*>(CallbackContext)->enable_callback(SourceId, IsEnabled, Level, MatchAnyKeyword, MatchAllKeyword, FilterData);
    else
        assert(0); // Where did the "this" pointer get lost?
}


//////////////////////////////////////////////////////////////////////
// winstd::event_session
//////////////////////////////////////////////////////////////////////

winstd::event_session::~event_session()
{
    if (m_h) {
        EVENT_TRACE_PROPERTIES *prop = m_prop.get();
        ControlTrace(m_h, (LPCTSTR)((const char*)prop + prop->LoggerNameOffset), prop, EVENT_TRACE_CONTROL_STOP);
    }
}


void winstd::event_session::free_internal()
{
    EVENT_TRACE_PROPERTIES *prop = m_prop.get();
    ControlTrace(m_h, (LPCTSTR)((const char*)prop + prop->LoggerNameOffset), prop, EVENT_TRACE_CONTROL_STOP);
}


//////////////////////////////////////////////////////////////////////
// winstd::event_trace
//////////////////////////////////////////////////////////////////////

winstd::event_trace::~event_trace()
{
    if (m_h)
        CloseTrace(m_h);
}


void winstd::event_trace::free_internal()
{
    CloseTrace(m_h);
}

#endif
