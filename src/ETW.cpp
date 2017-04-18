/*
    Copyright 1991-2017 Amebis
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
// winstd::event_rec
//////////////////////////////////////////////////////////////////////

winstd::event_rec::~event_rec()
{
    if (ExtendedData)
        delete reinterpret_cast<unsigned char*>(ExtendedData);

    if (UserData)
        delete reinterpret_cast<unsigned char*>(UserData);
}


void winstd::event_rec::set_extended_data(_In_ USHORT count, _In_count_(count) const EVENT_HEADER_EXTENDED_DATA_ITEM *data)
{
    if (ExtendedData)
        delete reinterpret_cast<unsigned char*>(ExtendedData);

    set_extended_data_internal(count, data);
}


void winstd::event_rec::set_user_data(_In_ USHORT size, _In_bytecount_(size) LPCVOID data)
{
    if (UserData)
        delete reinterpret_cast<unsigned char*>(UserData);

    set_user_data_internal(size, data);
}


void winstd::event_rec::set_extended_data_internal(_In_ USHORT count, _In_count_(count) const EVENT_HEADER_EXTENDED_DATA_ITEM *data)
{
    if (count) {
        assert(data);

        // Count the total required memory.
        size_t data_size = 0;
        for (size_t i = 0; i < count; i++)
            data_size += data[i].DataSize;

        // Allocate memory for extended data.
        ExtendedData = reinterpret_cast<EVENT_HEADER_EXTENDED_DATA_ITEM*>(new unsigned char[sizeof(EVENT_HEADER_EXTENDED_DATA_ITEM)*count + data_size]);

        // Bulk-copy extended data descriptors.
        memcpy(ExtendedData, data, sizeof(EVENT_HEADER_EXTENDED_DATA_ITEM) * count);

        // Copy the data.
        unsigned char *ptr = reinterpret_cast<unsigned char*>(ExtendedData + count);
        for (size_t i = 0; i < count; i++) {
            if (data[i].DataSize) {
                memcpy(ptr, (void*)(data[i].DataPtr), data[i].DataSize);
                ExtendedData[i].DataPtr = (ULONGLONG)ptr;
                ptr += data[i].DataSize;
            } else
                ExtendedData[i].DataPtr = NULL;
        }
    } else
        ExtendedData = NULL;

    ExtendedDataCount = count;
}


void winstd::event_rec::set_user_data_internal(_In_ USHORT size, _In_bytecount_(size) LPCVOID data)
{
    if (size) {
        assert(data);

        // Allocate memory for user data.
        UserData = new unsigned char[size];

        // Copy user data.
        memcpy(UserData, data, size);
    } else
        UserData = NULL;

    UserDataLength = size;
}


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
    if (m_h)
        ControlTrace(m_h, name(), m_prop.get(), EVENT_TRACE_CONTROL_STOP);
}


void winstd::event_session::free_internal()
{
    ControlTrace(m_h, name(), m_prop.get(), EVENT_TRACE_CONTROL_STOP);
}


//////////////////////////////////////////////////////////////////////
// winstd::event_trace_enabler
//////////////////////////////////////////////////////////////////////

winstd::event_trace_enabler::~event_trace_enabler()
{
    if (m_status == ERROR_SUCCESS)
        EnableTraceEx(
            m_provider_id,
            m_source_id,
            m_trace_handle,
            EVENT_CONTROL_CODE_DISABLE_PROVIDER,
            m_level,
            m_match_any_keyword,
            m_match_all_keyword,
            m_enable_property,
            m_enable_filter_desc);
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
