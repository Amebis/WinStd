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
#include <vector>

namespace winstd
{
    class WINSTD_API event_data;
    class WINSTD_API event_provider;
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
    /// EVENT_DATA_DESCRIPTOR wrapper
    ///
    class WINSTD_API event_data : public EVENT_DATA_DESCRIPTOR
    {
    public:
        ///
        /// Construct empty class.
        ///
        inline event_data()
        {
            Ptr      = 0;
            Size     = 0;
            Reserved = 0;
        }


        ///
        /// Template to construct class with generic data.
        ///
        /// \note This class is saves a reference to the data only. Therefore, data must be kept available.
        ///
        template<class T>
        inline event_data(_In_ const T &data)
        {
            EventDataDescCreate(this, &data, (ULONG)(sizeof(T)));
        }


        ///
        /// Construct class with string.
        ///
        /// \note This class is saves a reference to the data only. Therefore, data must be kept available.
        ///
        inline event_data(_In_ const char *data)
        {
            EventDataDescCreate(this, data, (ULONG)((strlen(data) + 1) * sizeof(*data)));
        }


        ///
        /// Construct class with wide string.
        ///
        /// \note This class is saves a reference to the data only. Therefore, data must be kept available.
        ///
        inline event_data(_In_ const wchar_t *data)
        {
            EventDataDescCreate(this, data, (ULONG)((wcslen(data) + 1) * sizeof(*data)));
        }


        ///
        /// Template to construct class with string.
        ///
        /// \note This class is saves a reference to the data only. Therefore, data must be kept available.
        ///
        template<class _Elem, class _Traits, class _Ax>
        inline event_data(_In_ const std::basic_string<_Elem, _Traits, _Ax> &data)
        {
            EventDataDescCreate(this, data.c_str(), (ULONG)((data.length() + 1) * sizeof(_Elem)));
        }


        ///
        /// Blank event data used as terminator.
        ///
        static const event_data blank;
    };


    ///
    /// ETW event provider
    ///
    class WINSTD_API event_provider : public handle<REGHANDLE>
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
        /// Writes an event with parameters stored in array.
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
        /// Writes an event with one or more parameter.
        ///
        /// \return
        /// - `ERROR_SUCCESS` when write succeeds;
        /// - error code otherwise.
        ///
        /// \sa [EventWrite function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363752.aspx)
        ///
        inline ULONG write(_In_ PCEVENT_DESCRIPTOR EventDescriptor, _In_ const EVENT_DATA_DESCRIPTOR &param1, ...)
        {
            assert(m_h);

            va_list arg;
            std::vector<EVENT_DATA_DESCRIPTOR> params;
            ULONG param_count;

            // Preallocate array.
            va_start(arg, param1);
            for (param_count = 1;; param_count++) {
                EVENT_DATA_DESCRIPTOR &p = va_arg(arg, EVENT_DATA_DESCRIPTOR);
                if (!p.Ptr) break;
            }
            va_end(arg);
            params.reserve(param_count);

            // Copy parameters to array.
            va_start(arg, param1);
            for (params.push_back(param1);;) {
                EVENT_DATA_DESCRIPTOR &p = va_arg(arg, EVENT_DATA_DESCRIPTOR);
                if (!p.Ptr) break;
                params.push_back(p);
            }
            va_end(arg);

            return EventWrite(m_h, EventDescriptor, param_count, params.data());
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
