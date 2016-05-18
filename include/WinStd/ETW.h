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

#include <evntprov.h>
#include <evntcons.h>
#include <stdarg.h>

#include <string>

namespace winstd
{
    class event_provider;
}

#pragma once


namespace winstd
{
    ///
    /// \defgroup WinStdETWAPI Event Tracing for Windows API
    /// Integrates WinStd classes with Event Tracing for Windows API
    ///
    /// @{


    ///
    /// ETW event provider
    ///
    class event_provider : public handle<REGHANDLE>
    {
    public:
        ///
        /// Closes the event provider.
        ///
        /// \sa [EventUnregister function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363749.aspx)
        ///
        virtual ~event_provider()
        {
            if (m_h)
                 EventUnregister(m_h);
        }


        ///
        /// Registers the event provider.
        ///
        /// \return
        /// - `ERROR_SUCCESS` when creation succeeds;
        /// - error code otherwise.
        ///
        /// \sa [CertOpenStore function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa376559.aspx)
        ///
        inline ULONG create(_In_ LPCGUID ProviderId)
        {
            handle_type h;
            ULONG ulRes = EventRegister(ProviderId, enable_callback, this, &h);
            if (ulRes == ERROR_SUCCESS)
                attach(h);
            return ulRes;
        }


        ///
        /// Writes an event.
        ///
        /// \return
        /// - `ERROR_SUCCESS` when write succeeds;
        /// - error code otherwise.
        ///
        /// \sa [EventWrite function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363752.aspx)
        ///
        inline ULONG write(_In_ PCEVENT_DESCRIPTOR EventDescriptor, _In_ ULONG UserDataCount = 0, _In_opt_count_(UserDataCount) PEVENT_DATA_DESCRIPTOR UserData = NULL)
        {
            assert(m_h);
            return EventWrite(m_h, EventDescriptor, UserDataCount, UserData);
        }


        ///
        /// Writes an event with no parameters.
        ///
        /// \return
        /// - `ERROR_SUCCESS` when write succeeds;
        /// - error code otherwise.
        ///
        /// \sa [EventWrite function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363752.aspx)
        ///
        inline ULONG write(_In_ PCEVENT_DESCRIPTOR EventDescriptor)
        {
            assert(m_h);
            return EventWrite(m_h, EventDescriptor, 0, NULL);
        }


        ///
        /// Writes an event with one parameter.
        ///
        /// \return
        /// - `ERROR_SUCCESS` when write succeeds;
        /// - error code otherwise.
        ///
        /// \sa [EventWrite function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363752.aspx)
        ///
        template <class T1>
        inline ULONG write(_In_ PCEVENT_DESCRIPTOR EventDescriptor, _In_ const T1 param1)
        {
            assert(m_h);
            EVENT_DATA_DESCRIPTOR edd;
            EventDataDescCreate(&edd, &param1, (ULONG)(sizeof(T1)));
            return EventWrite(m_h, EventDescriptor, 1, &edd);
        }


        ///
        /// Writes an event with one string as parameter.
        ///
        /// \return
        /// - `ERROR_SUCCESS` when write succeeds;
        /// - error code otherwise.
        ///
        /// \sa [EventWrite function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363752.aspx)
        ///
        inline ULONG write(_In_ PCEVENT_DESCRIPTOR EventDescriptor, _In_ const char *param1)
        {
            assert(m_h);
            EVENT_DATA_DESCRIPTOR edd;
            EventDataDescCreate(&edd, param1, (ULONG)(strlen(param1) + 1)*sizeof(*param1));
            return EventWrite(m_h, EventDescriptor, 1, &edd);
        }


        ///
        /// Writes an event with one wide string as parameter.
        ///
        /// \return
        /// - `ERROR_SUCCESS` when write succeeds;
        /// - error code otherwise.
        ///
        /// \sa [EventWrite function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363752.aspx)
        ///
        inline ULONG write(_In_ PCEVENT_DESCRIPTOR EventDescriptor, _In_ const wchar_t *param1)
        {
            assert(m_h);
            EVENT_DATA_DESCRIPTOR edd;
            EventDataDescCreate(&edd, param1, (ULONG)(wcslen(param1) + 1)*sizeof(*param1));
            return EventWrite(m_h, EventDescriptor, 1, &edd);
        }


        ///
        /// Writes a string event.
        ///
        /// \return
        /// - `ERROR_SUCCESS` when write succeeds;
        /// - error code otherwise.
        ///
        /// \sa [EventWriteString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363750v=vs.85.aspx)
        ///
        inline ULONG write(_In_ UCHAR Level, _In_ ULONGLONG Keyword, _In_ PCWSTR String, ...)
        {
            assert(m_h);

            std::wstring msg;
            va_list arg;

            // Format message.
            va_start(arg, String);
            vsprintf(msg, String, arg);
            va_end(arg);

            // Write string event.
            return EventWriteString(m_h, Level, Keyword, msg.c_str());
        }

    protected:
        ///
        /// Releases the event provider.
        ///
        /// \sa [EventUnregister function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363749.aspx)
        ///
        virtual void free_internal()
        {
            EventUnregister(m_h);
        }


        ///
        /// Receive enable or disable notification requests
        ///
        /// \sa [EnableCallback callback function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363707.aspx)
        ///
        virtual void enable_callback(_In_ LPCGUID SourceId, _In_ ULONG IsEnabled, _In_ UCHAR Level, _In_ ULONGLONG MatchAnyKeyword, _In_ ULONGLONG MatchAllKeyword, _In_opt_ PEVENT_FILTER_DESCRIPTOR FilterData)
        {
            UNREFERENCED_PARAMETER(SourceId);
            UNREFERENCED_PARAMETER(IsEnabled);
            UNREFERENCED_PARAMETER(Level);
            UNREFERENCED_PARAMETER(MatchAnyKeyword);
            UNREFERENCED_PARAMETER(MatchAllKeyword);
            UNREFERENCED_PARAMETER(FilterData);
        }


        ///
        /// Receive enable or disable notification requests
        ///
        /// \sa [EnableCallback callback function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363707.aspx)
        ///
        static VOID NTAPI enable_callback(_In_ LPCGUID SourceId, _In_ ULONG IsEnabled, _In_ UCHAR Level, _In_ ULONGLONG MatchAnyKeyword, _In_ ULONGLONG MatchAllKeyword, _In_opt_ PEVENT_FILTER_DESCRIPTOR FilterData, _Inout_opt_ PVOID CallbackContext)
        {
            if (CallbackContext)
                static_cast<event_provider*>(CallbackContext)->enable_callback(SourceId, IsEnabled, Level, MatchAnyKeyword, MatchAllKeyword, FilterData);
            else
                assert(0); // Where did the "this" pointer get lost?
        }
    };


    /// @}
}
