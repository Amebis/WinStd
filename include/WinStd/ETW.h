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

///
/// \defgroup WinStdETWAPI Event Tracing for Windows API
/// Integrates WinStd classes with Event Tracing for Windows API
///

#include "Common.h"

#include <assert.h>
#include <evntprov.h>
#include <evntcons.h>
#include <stdarg.h>
#include <tdh.h>

#include <memory>
#include <string>
#include <vector>

namespace winstd
{
    class WINSTD_API WINSTD_NOVTABLE event_data;
    class WINSTD_API WINSTD_NOVTABLE event_rec;
    class WINSTD_API event_provider;
    class WINSTD_API event_session;
    class WINSTD_API event_trace;
    class WINSTD_API event_trace_enabler;
    class WINSTD_API event_fn_auto;
    template<class T> class event_fn_auto_ret;
}

/// \addtogroup WinStdCryptoAPI
/// @{

///
/// Retrieves metadata about an event.
///
/// \sa [TdhGetEventInformation function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa964840.aspx)
///
inline ULONG TdhGetEventInformation(_In_ PEVENT_RECORD pEvent, _In_ ULONG TdhContextCount, _In_ PTDH_CONTEXT pTdhContext, _Out_ std::unique_ptr<TRACE_EVENT_INFO> &info);

///
/// Retrieves information about the event map contained in the event.
///
/// \sa [TdhGetEventMapInformation function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa964841.aspx)
///
inline ULONG TdhGetEventMapInformation(_In_ PEVENT_RECORD pEvent, _In_ LPWSTR pMapName, _Out_ std::unique_ptr<EVENT_MAP_INFO> &info);

///
/// Retrieves a property value from the event data.
///
/// \sa [TdhGetProperty function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa964843.aspx)
///
template<class _Ty, class _Ax> inline ULONG TdhGetProperty(_In_ PEVENT_RECORD pEvent, _In_ ULONG TdhContextCount, _In_ PTDH_CONTEXT pTdhContext, _In_ ULONG PropertyDataCount, _In_ PPROPERTY_DATA_DESCRIPTOR pPropertyData, _Out_ std::vector<_Ty, _Ax> &aData);

/// @}

#pragma once


namespace winstd
{
    /// \addtogroup WinStdETWAPI
    /// @{

    ///
    /// EVENT_DATA_DESCRIPTOR wrapper
    ///
    class WINSTD_API WINSTD_NOVTABLE event_data : public EVENT_DATA_DESCRIPTOR
    {
    public:
        ///
        /// Construct empty class.
        ///
        inline event_data()
        {
            Ptr      = 0;
            Size     = 0;
            Reserved = (ULONG)-1;   // Used for varadic argument terminator.
        }


        ///
        /// Construct class pointing to an `int`.
        ///
        /// \param[in] data  Event data
        ///
        /// \note This class saves a reference to the data only. Therefore, data must be kept available.
        ///
        inline event_data(_In_ const int &data)
        {
            EventDataDescCreate(this, &data, (ULONG)(sizeof(data)));
        }


        ///
        /// Construct class pointing to an `unsigned int`.
        ///
        /// \param[in] data  Event data
        ///
        /// \note This class saves a reference to the data only. Therefore, data must be kept available.
        ///
        inline event_data(_In_ const unsigned int &data)
        {
            EventDataDescCreate(this, &data, (ULONG)(sizeof(data)));
        }


        ///
        /// Construct class pointing to a `long`.
        ///
        /// \param[in] data  Event data
        ///
        /// \note This class saves a reference to the data only. Therefore, data must be kept available.
        ///
        inline event_data(_In_ const long &data)
        {
            EventDataDescCreate(this, &data, (ULONG)(sizeof(data)));
        }


        ///
        /// Construct class pointing to an `unsigned long`.
        ///
        /// \param[in] data  Event data
        ///
        /// \note This class saves a reference to the data only. Therefore, data must be kept available.
        ///
        inline event_data(_In_ const unsigned long &data)
        {
            EventDataDescCreate(this, &data, (ULONG)(sizeof(data)));
        }


        ///
        /// Construct class pointing to a `GUID`.
        ///
        /// \param[in] data  Event data
        ///
        /// \note This class saves a reference to the data only. Therefore, data must be kept available.
        ///
        inline event_data(_In_ const GUID &data)
        {
            EventDataDescCreate(this, &data, (ULONG)(sizeof(data)));
        }


        ///
        /// Construct class pointing to a string.
        ///
        /// \param[in] data  Event data. When `NULL`, the event data will be `"<null>"`.
        ///
        /// \note This class saves a reference to the data only. Therefore, data must be kept available.
        ///
        inline event_data(_In_ const char *data)
        {
            if (data)
                EventDataDescCreate(this, data, (ULONG)((strlen(data) + 1) * sizeof(*data)));
            else {
                // Writing NULL pointer with 0B length causes trouble in Event Viewer: message template string is displayed only, parameters are not rendered.
                static const char null[] = "<null>";
                EventDataDescCreate(this, null, sizeof(null));
            }
        }


        ///
        /// Construct class pointing to a wide string.
        ///
        /// \param[in] data  Event data. When `NULL`, the event data will be `"<null>"`.
        ///
        /// \note This class saves a reference to the data only. Therefore, data must be kept available.
        ///
        inline event_data(_In_ const wchar_t *data)
        {
            if (data)
                EventDataDescCreate(this, data, (ULONG)((wcslen(data) + 1) * sizeof(*data)));
            else {
                // Writing NULL pointer with 0B length causes trouble in Event Viewer: message template string is displayed only, parameters are not rendered.
                static const wchar_t null[] = L"<null>";
                EventDataDescCreate(this, null, sizeof(null));
            }
        }


        ///
        /// Template to construct pointing to a `std::basic_string<>`.
        ///
        /// \param[in] data  Event data
        ///
        /// \note This class saves a reference to the data only. Therefore, data must be kept available.
        ///
        template<class _Elem, class _Traits, class _Ax>
        inline event_data(_In_ const std::basic_string<_Elem, _Traits, _Ax> &data)
        {
            EventDataDescCreate(this, data.c_str(), (ULONG)((data.length() + 1) * sizeof(_Elem)));
        }


        ///
        /// Construct class pointing to binary data.
        ///
        /// \param[in] data  Pointer to event data
        /// \param[in] size  Size of \p data in bytes
        ///
        /// \note This class saves a reference to the data only. Therefore, data must be kept available.
        ///
        inline event_data(_In_bytecount_(size) const void *data, _In_ ULONG size)
        {
            EventDataDescCreate(this, data, size);
        }


        ///
        /// Blank event data used as terminator.
        ///
        static const event_data blank;
    };


    ///
    /// EVENT_RECORD wrapper
    ///
    class WINSTD_API WINSTD_NOVTABLE event_rec : public EVENT_RECORD
    {
    public:
        ///
        /// Constructs a blank event record.
        ///
        inline event_rec()
        {
            memset((EVENT_RECORD*)this, 0, sizeof(EVENT_RECORD));
        }


        ///
        /// Copies an existing event record.
        ///
        /// \param[in] other  Event record to copy from
        ///
        inline event_rec(_In_ const event_rec &other) : EVENT_RECORD(other)
        {
            set_extended_data_internal(other.ExtendedDataCount, other.ExtendedData);
            set_user_data_internal(other.UserDataLength, other.UserData);
        }


        ///
        /// Copies an existing event record.
        ///
        /// \param[in] other  Event record to copy from
        ///
        inline event_rec(_In_ const EVENT_RECORD &other) : EVENT_RECORD(other)
        {
            set_extended_data_internal(other.ExtendedDataCount, other.ExtendedData);
            set_user_data_internal(other.UserDataLength, other.UserData);
        }


        ///
        /// Moves the event record.
        ///
        /// \param[in] other  Event record to move
        ///
        inline event_rec(_Inout_ event_rec&& other) : EVENT_RECORD(other)
        {
            memset((EVENT_RECORD*)&other, 0, sizeof(EVENT_RECORD));
        }


        ///
        /// Destroys event record data and frees the allocated memory.
        ///
        ~event_rec();


        ///
        /// Copies an existing event record.
        ///
        /// \param[in] other  Event record to copy from
        ///
        inline event_rec& operator=(_In_ const event_rec &other)
        {
            if (this != std::addressof(other)) {
                (EVENT_RECORD&)*this = other;
                set_extended_data_internal(other.ExtendedDataCount, other.ExtendedData);
                set_user_data_internal(other.UserDataLength, other.UserData);
            }

            return *this;
        }


        ///
        /// Copies an existing event record.
        ///
        /// \param[in] other  Event record to copy from
        ///
        inline event_rec& operator=(_In_ const EVENT_RECORD &other)
        {
            if (this != std::addressof(other)) {
                (EVENT_RECORD&)*this = other;
                set_extended_data_internal(other.ExtendedDataCount, other.ExtendedData);
                set_user_data_internal(other.UserDataLength, other.UserData);
            }

            return *this;
        }


        ///
        /// Moves the event record.
        ///
        /// \param[in] other  Event record to move
        ///
        inline event_rec& operator=(_Inout_ event_rec&& other)
        {
            if (this != std::addressof(other)) {
                (EVENT_RECORD&)*this = other;
                memset((EVENT_RECORD*)&other, 0, sizeof(EVENT_RECORD));
            }

            return *this;
        }


        ///
        /// Sets event record extended data.
        ///
        /// \param[in] count  \p data size (in number of elements)
        /// \param[in] data   Record extended data
        ///
        void set_extended_data(_In_ USHORT count, _In_count_(count) const EVENT_HEADER_EXTENDED_DATA_ITEM *data);


        ///
        /// Sets event record user data.
        ///
        /// \param[in] size  \p data size (in bytes)
        /// \param[in] data  Record user data
        ///
        void set_user_data(_In_ USHORT size, _In_bytecount_(size) LPCVOID data);

    protected:
        ///
        /// Sets event record extended data.
        ///
        /// \param[in] count  \p data size (in number of elements)
        /// \param[in] data   Record extended data
        ///
        void set_extended_data_internal(_In_ USHORT count, _In_count_(count) const EVENT_HEADER_EXTENDED_DATA_ITEM *data);

        ///
        /// Sets event record user data.
        ///
        /// \param[in] size  \p data size (in bytes)
        /// \param[in] data  Record user data
        ///
        void set_user_data_internal(_In_ USHORT size, _In_bytecount_(size) LPCVOID data);
    };


    ///
    /// ETW event provider
    ///
    class WINSTD_API event_provider : public handle<REGHANDLE>
    {
        HANDLE_IMPL(event_provider)

    public:
        ///
        /// Closes the event provider.
        ///
        /// \sa [EventUnregister function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363749.aspx)
        ///
        virtual ~event_provider();


        ///
        /// Registers the event provider.
        ///
        /// \return
        /// - `ERROR_SUCCESS` when creation succeeds;
        /// - error code otherwise.
        ///
        /// \sa [EventRegister function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363744.aspx)
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
        /// \note The list must be terminated with `winstd::event_data::blank`.
        ///
        /// \return
        /// - `ERROR_SUCCESS` when write succeeds;
        /// - error code otherwise.
        ///
        /// \sa [EventWrite function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363752.aspx)
        ///
        inline ULONG write(_In_ PCEVENT_DESCRIPTOR EventDescriptor, _In_ const EVENT_DATA_DESCRIPTOR param1, ...)
        {
            assert(m_h);
            UNREFERENCED_PARAMETER(param1);

            va_list arg;
            va_start(arg, EventDescriptor);
            ULONG ulResult = write(EventDescriptor, arg);
            va_end(arg);

            return ulResult;
        }


        ///
        /// Writes an event with variable number of parameters.
        ///
        /// \note The list must be terminated with `winstd::event_data::blank`.
        ///
        /// \return
        /// - `ERROR_SUCCESS` when write succeeds;
        /// - error code otherwise.
        ///
        /// \sa [EventWrite function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363752.aspx)
        ///
        inline ULONG write(_In_ PCEVENT_DESCRIPTOR EventDescriptor, _In_ va_list arg)
        {
            assert(m_h);

            va_list arg_start = arg;
            std::vector<EVENT_DATA_DESCRIPTOR> params;
            ULONG param_count;

            // Preallocate array.
            for (param_count = 0;; param_count++) {
                const EVENT_DATA_DESCRIPTOR &p = va_arg(arg, const EVENT_DATA_DESCRIPTOR);
                if (p.Ptr      == winstd::event_data::blank.Ptr      &&
                    p.Size     == winstd::event_data::blank.Size     &&
                    p.Reserved == winstd::event_data::blank.Reserved) break;
            }
            params.reserve(param_count);

            // Copy parameters to array.
            arg = arg_start;
            for (;;) {
                const EVENT_DATA_DESCRIPTOR &p = va_arg(arg, const EVENT_DATA_DESCRIPTOR);
                if (p.Ptr      == winstd::event_data::blank.Ptr      &&
                    p.Size     == winstd::event_data::blank.Size     &&
                    p.Reserved == winstd::event_data::blank.Reserved) break;
                params.push_back(p);
            }

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
        inline ULONG write(_In_ UCHAR Level, _In_ ULONGLONG Keyword, _In_ _Printf_format_string_ PCWSTR String, ...)
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
        virtual void free_internal();


        ///
        /// Receive enable or disable notification requests
        ///
        /// \sa [EnableCallback callback function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363707.aspx)
        ///
        virtual void enable_callback(_In_ LPCGUID SourceId, _In_ ULONG IsEnabled, _In_ UCHAR Level, _In_ ULONGLONG MatchAnyKeyword, _In_ ULONGLONG MatchAllKeyword, _In_opt_ PEVENT_FILTER_DESCRIPTOR FilterData);


        ///
        /// Receive enable or disable notification requests
        ///
        /// \sa [EnableCallback callback function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363707.aspx)
        ///
        static VOID NTAPI enable_callback(_In_ LPCGUID SourceId, _In_ ULONG IsEnabled, _In_ UCHAR Level, _In_ ULONGLONG MatchAnyKeyword, _In_ ULONGLONG MatchAllKeyword, _In_opt_ PEVENT_FILTER_DESCRIPTOR FilterData, _Inout_opt_ PVOID CallbackContext);
    };


    ///
    /// ETW session
    ///
    class WINSTD_API event_session : public handle<TRACEHANDLE>
    {
        WINSTD_NONCOPYABLE(event_session)

    public:
        ///
        /// Initializes a new empty session.
        ///
        inline event_session()
        {
        }


        ///
        /// Initializes a new session with an already available object handle.
        ///
        /// \param[in] h     Initial session handle value
        /// \param[in] prop  Session properties
        ///
        inline event_session(_In_opt_ handle_type h, _In_ const EVENT_TRACE_PROPERTIES *prop) :
            m_prop(reinterpret_cast<EVENT_TRACE_PROPERTIES*>(new char[prop->Wnode.BufferSize])),
            handle(h)
        {
            memcpy(m_prop.get(), prop, prop->Wnode.BufferSize);
        }


        ///
        /// Move constructor
        ///
        /// \param[inout] other  A rvalue reference of another session
        ///
        inline event_session(_Inout_ event_session &&other) :
            m_prop(std::move(other.m_prop)),
            handle(std::move(other))
        {
        }


        ///
        /// Closes the session.
        ///
        /// \sa [ControlTrace function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363696.aspx)
        ///
        virtual ~event_session();


        ///
        /// Move assignment
        ///
        /// \param[inout] other  A rvalue reference of another object
        ///
        inline event_session& operator=(_Inout_ event_session &&other)
        {
            if (this != std::addressof(other)) {
                (handle<handle_type>&&)*this = std::move(other);
                m_prop                       = std::move(other.m_prop);
            }
            return *this;
        }


        ///
        /// Auto-typecasting operator
        ///
        /// \return Session properties
        ///
        inline operator const EVENT_TRACE_PROPERTIES*() const
        {
            return m_prop.get();
        }


        ///
        /// Auto-typecasting operator
        ///
        /// \return Session properties
        ///
        inline LPCTSTR name() const
        {
            const EVENT_TRACE_PROPERTIES *prop = m_prop.get();
            return reinterpret_cast<LPCTSTR>(reinterpret_cast<const char*>(prop) + prop->LoggerNameOffset);
        }


        ///
        /// Sets a new session handle for the class
        ///
        /// When the current session handle of the class is non-NULL, the session is destroyed first.
        ///
        /// \param[in] h     New session handle
        /// \param[in] prop  Session properties
        ///
        inline void attach(_In_opt_ handle_type h, _In_ EVENT_TRACE_PROPERTIES *prop)
        {
            handle<handle_type>::attach(h);
            m_prop.reset(prop);
        }


        ///
        /// Registers and starts an event tracing session.
        ///
        /// \return
        /// - `ERROR_SUCCESS` when creation succeeds;
        /// - error code otherwise.
        ///
        /// \sa [StartTrace function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa364117.aspx)
        ///
        inline ULONG create(_In_ LPCTSTR SessionName, _In_ const EVENT_TRACE_PROPERTIES *Properties)
        {
            handle_type h;
            std::unique_ptr<EVENT_TRACE_PROPERTIES> prop(reinterpret_cast<EVENT_TRACE_PROPERTIES*>(new char[Properties->Wnode.BufferSize]));
            memcpy(prop.get(), Properties, Properties->Wnode.BufferSize);
            ULONG ulRes = StartTrace(&h, SessionName, prop.get());
            if (ulRes == ERROR_SUCCESS)
                attach(h, prop.release());
            return ulRes;
        }


        ///
        /// Enables the specified event trace provider.
        ///
        /// \return
        /// - `ERROR_SUCCESS` when succeeds;
        /// - error code otherwise.
        ///
        /// \sa [EnableTraceEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363711.aspx)
        ///
        inline ULONG enable_trace(_In_ LPCGUID ProviderId, _In_ UCHAR Level, _In_opt_ ULONGLONG MatchAnyKeyword = 0, _In_opt_ ULONGLONG MatchAllKeyword = 0, _In_opt_ ULONG EnableProperty = 0, _In_opt_ PEVENT_FILTER_DESCRIPTOR EnableFilterDesc = NULL)
        {
            assert(m_h);
            return EnableTraceEx(
                ProviderId,
                &m_prop->Wnode.Guid,
                m_h,
                EVENT_CONTROL_CODE_ENABLE_PROVIDER,
                Level,
                MatchAnyKeyword,
                MatchAllKeyword,
                EnableProperty,
                EnableFilterDesc);
        }


        ///
        /// Disables the specified event trace provider.
        ///
        /// \return
        /// - `ERROR_SUCCESS` when succeeds;
        /// - error code otherwise.
        ///
        /// \sa [EnableTraceEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363711.aspx)
        ///
        inline ULONG disable_trace(_In_ LPCGUID ProviderId, _In_ UCHAR Level, _In_opt_ ULONGLONG MatchAnyKeyword = 0, _In_opt_ ULONGLONG MatchAllKeyword = 0, _In_opt_ ULONG EnableProperty = 0, _In_opt_ PEVENT_FILTER_DESCRIPTOR EnableFilterDesc = NULL)
        {
            assert(m_h);
            return EnableTraceEx(
                ProviderId,
                &m_prop->Wnode.Guid,
                m_h,
                EVENT_CONTROL_CODE_DISABLE_PROVIDER,
                Level,
                MatchAnyKeyword,
                MatchAllKeyword,
                EnableProperty,
                EnableFilterDesc);
        }


    protected:
        ///
        /// Releases the session.
        ///
        /// \sa [ControlTrace function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363696.aspx)
        ///
        virtual void free_internal();

    protected:
        std::unique_ptr<EVENT_TRACE_PROPERTIES> m_prop; ///< Session properties
    };


    ///
    /// ETW trace
    ///
    class WINSTD_API event_trace : public handle<TRACEHANDLE>
    {
        HANDLE_IMPL(event_trace)

    public:
        ///
        /// Closes the trace.
        ///
        /// \sa [CloseTrace function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363686.aspx)
        ///
        virtual ~event_trace();


        ///
        /// Opens a real-time trace session or log file for consuming.
        ///
        /// \return
        /// - `ERROR_SUCCESS` when creation succeeds;
        /// - error code otherwise.
        ///
        /// \sa [OpenTrace function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa364089.aspx)
        ///
        inline bool create(_Inout_ PEVENT_TRACE_LOGFILE Logfile)
        {
            handle_type h = OpenTrace(Logfile);
            if (h != (TRACEHANDLE)INVALID_HANDLE_VALUE) {
                attach(h);
                return true;
            } else
                return false;
        }

    protected:
        ///
        /// Closes the trace.
        ///
        /// \sa [CloseTrace function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363686.aspx)
        ///
        virtual void free_internal();
    };


    ///
    /// Helper class to enable event provider in constructor and disables it in destructor
    ///
    class WINSTD_API event_trace_enabler
    {
    public:
        ///
        /// Enables event trace
        ///
        /// \sa [EnableTraceEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363711.aspx)
        ///
        inline event_trace_enabler(
            _In_opt_ LPCGUID                  SourceId,
            _In_     TRACEHANDLE              TraceHandle,
            _In_     LPCGUID                  ProviderId,
            _In_     UCHAR                    Level,
            _In_opt_ ULONGLONG                MatchAnyKeyword  = 0,
            _In_opt_ ULONGLONG                MatchAllKeyword  = 0,
            _In_opt_ ULONG                    EnableProperty   = 0,
            _In_opt_ PEVENT_FILTER_DESCRIPTOR EnableFilterDesc = NULL) :
            m_provider_id(ProviderId),
            m_source_id(SourceId),
            m_trace_handle(TraceHandle),
            m_level(Level),
            m_match_any_keyword(MatchAnyKeyword),
            m_match_all_keyword(MatchAllKeyword),
            m_enable_property(EnableProperty),
            m_enable_filter_desc(EnableFilterDesc)
        {
            m_status = EnableTraceEx(
                m_provider_id,
                m_source_id,
                m_trace_handle,
                EVENT_CONTROL_CODE_ENABLE_PROVIDER,
                m_level,
                m_match_any_keyword,
                m_match_all_keyword,
                m_enable_property,
                m_enable_filter_desc);
        }


        ///
        /// Enables event trace
        ///
        /// \sa [EnableTraceEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363711.aspx)
        ///
        inline event_trace_enabler(
            _In_     const event_session            &session,
            _In_           LPCGUID                  ProviderId,
            _In_           UCHAR                    Level,
            _In_opt_       ULONGLONG                MatchAnyKeyword  = 0,
            _In_opt_       ULONGLONG                MatchAllKeyword  = 0,
            _In_opt_       ULONG                    EnableProperty   = 0,
            _In_opt_       PEVENT_FILTER_DESCRIPTOR EnableFilterDesc = NULL) :
            m_provider_id(ProviderId),
            m_source_id(&((const EVENT_TRACE_PROPERTIES*)session)->Wnode.Guid),
            m_trace_handle(session),
            m_level(Level),
            m_match_any_keyword(MatchAnyKeyword),
            m_match_all_keyword(MatchAllKeyword),
            m_enable_property(EnableProperty),
            m_enable_filter_desc(EnableFilterDesc)
        {
            m_status = EnableTraceEx(
                m_provider_id,
                m_source_id,
                m_trace_handle,
                EVENT_CONTROL_CODE_ENABLE_PROVIDER,
                m_level,
                m_match_any_keyword,
                m_match_all_keyword,
                m_enable_property,
                m_enable_filter_desc);
        }


        ///
        /// Return result of `EnableTraceEx()` call.
        ///
        /// \sa [EnableTraceEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363711.aspx)
        ///
        inline ULONG status() const
        {
            return m_status;
        }


        ///
        /// Disables event trace.
        ///
        /// \sa [EnableTraceEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363711.aspx)
        ///
        virtual ~event_trace_enabler();

    protected:
        ULONG m_status;                                 ///< Result of EnableTraceEx call
        LPCGUID m_provider_id;                          ///< Provider ID
        LPCGUID m_source_id;                            ///< Session ID
        TRACEHANDLE m_trace_handle;                     ///< Trace handle
        UCHAR m_level;                                  ///< Logging level
        ULONGLONG m_match_any_keyword;                  ///< Keyword match mask (any)
        ULONGLONG m_match_all_keyword;                  ///< Keyword match mask (all)
        ULONG m_enable_property;                        ///< Enable property
        PEVENT_FILTER_DESCRIPTOR m_enable_filter_desc;  ///< Event filter descriptor
    };


    ///
    /// Helper class to write an event on entry/exit of scope.
    ///
    /// It writes one string event at creation and another at destruction.
    ///
    class WINSTD_API event_fn_auto
    {
    public:
        ///
        /// Writes the `event_cons` event
        ///
        inline event_fn_auto(_In_ event_provider &ep, _In_ const EVENT_DESCRIPTOR *event_cons, _In_ const EVENT_DESCRIPTOR *event_dest, _In_z_ LPCSTR pszFnName) :
            m_ep(ep),
            m_event_dest(event_dest)
        {
            EventDataDescCreate(&m_fn_name, pszFnName, (ULONG)(strlen(pszFnName) + 1)*sizeof(*pszFnName));
            m_ep.write(event_cons, 1, &m_fn_name);
        }


        ///
        /// Copies the object
        ///
        inline event_fn_auto(_In_ const event_fn_auto &other) :
            m_ep(other.m_ep),
            m_event_dest(other.m_event_dest),
            m_fn_name(other.m_fn_name)
        {
        }


        ///
        /// Moves the object
        ///
        inline event_fn_auto(_Inout_ event_fn_auto &&other) :
            m_ep(other.m_ep),
            m_event_dest(other.m_event_dest),
            m_fn_name(std::move(other.m_fn_name))
        {
            other.m_event_dest = NULL;
        }


        ///
        /// Writes the `event_dest` event
        ///
        inline ~event_fn_auto()
        {
            if (m_event_dest)
                m_ep.write(m_event_dest, 1, &m_fn_name);
        }


        ///
        /// Copies the object
        ///
        inline event_fn_auto& operator=(_In_ const event_fn_auto &other)
        {
            if (this != &other) {
                assert(&m_ep == &other.m_ep);
                m_event_dest = other.m_event_dest;
                m_fn_name    = other.m_fn_name;
            }

            return *this;
        }


        ///
        /// Moves the object
        ///
        inline event_fn_auto& operator=(_Inout_ event_fn_auto &&other)
        {
            if (this != &other) {
                assert(&m_ep == &other.m_ep);
                m_event_dest = other.m_event_dest;
                m_fn_name    = std::move(other.m_fn_name);
                other.m_event_dest = NULL;
            }

            return *this;
        }


    protected:
        event_provider &m_ep;                   ///< Reference to event provider in use
        const EVENT_DESCRIPTOR *m_event_dest;   ///< Event descriptor at destruction
        EVENT_DATA_DESCRIPTOR m_fn_name;        ///< Function name
    };


    ///
    /// Helper template to write an event on entry/exit of scope with one parameter (typically result).
    ///
    /// It writes one string event at creation and another at destruction, with allowing one sprintf type parameter for string event at destruction.
    ///
    template<class T>
    class event_fn_auto_ret
    {
    public:
        ///
        /// Writes the `event_cons` event
        ///
        inline event_fn_auto_ret(_In_ event_provider &ep, _In_ const EVENT_DESCRIPTOR *event_cons, _In_ const EVENT_DESCRIPTOR *event_dest, _In_z_ LPCSTR pszFnName, T &result) :
            m_ep(ep),
            m_event_dest(event_dest),
            m_result(result)
        {
            EventDataDescCreate(m_desc + 0, pszFnName, (ULONG)(strlen(pszFnName) + 1)*sizeof(*pszFnName));
            m_ep.write(event_cons, 1, m_desc);
        }


        ///
        /// Copies the object
        ///
        inline event_fn_auto_ret(_In_ const event_fn_auto_ret<T> &other) :
            m_ep(other.m_ep),
            m_event_dest(other.m_event_dest),
            m_result(other.m_result)
        {
            m_desc[0] = other.m_desc[0];
        }


        ///
        /// Moves the object
        ///
        inline event_fn_auto_ret(_Inout_ event_fn_auto_ret<T> &&other) :
            m_ep(other.m_ep),
            m_event_dest(other.m_event_dest),
            m_result(other.m_result)
        {
            m_desc[0] = std::move(other.m_desc[0]);
            other.m_event_dest = NULL;
        }


        ///
        /// Writes the `event_dest` event
        ///
        inline ~event_fn_auto_ret()
        {
            if (m_event_dest) {
                EventDataDescCreate(m_desc + 1, &m_result, sizeof(T));
                m_ep.write(m_event_dest, 2, m_desc);
            }
        }


        ///
        /// Copies the object
        ///
        inline event_fn_auto_ret& operator=(_In_ const event_fn_auto_ret<T> &other)
        {
            if (this != &other) {
                assert(&m_ep == &other.m_ep);
                m_event_dest = other.m_event_dest;
                m_desc[0] = other.m_desc[0];
                assert(&m_result == &other.m_result);
            }

            return *this;
        }


        ///
        /// Moves the object
        ///
        inline event_fn_auto_ret& operator=(_Inout_ event_fn_auto_ret<T> &&other)
        {
            if (this != &other) {
                assert(&m_ep == &other.m_ep);
                m_event_dest = other.m_event_dest;
                m_desc[0] = std::move(other.m_desc[0]);
                assert(&m_result == &other.m_result);
                other.m_event_dest = NULL;
            }

            return *this;
        }


    protected:
        event_provider &m_ep;                   ///< Reference to event provider in use
        const EVENT_DESCRIPTOR *m_event_dest;   ///< Event descriptor at destruction
        EVENT_DATA_DESCRIPTOR m_desc[2];        ///< Function name and return value
        T &m_result;                            ///< Function result
    };

    /// @}
}


inline ULONG TdhGetEventInformation(_In_ PEVENT_RECORD pEvent, _In_ ULONG TdhContextCount, _In_ PTDH_CONTEXT pTdhContext, _Out_ std::unique_ptr<TRACE_EVENT_INFO> &info)
{
    BYTE szBuffer[WINSTD_STACK_BUFFER_BYTES];
    ULONG ulSize = sizeof(szBuffer), ulResult;

    // Try with stack buffer first.
    ulResult = TdhGetEventInformation(pEvent, TdhContextCount, pTdhContext, (PTRACE_EVENT_INFO)szBuffer, &ulSize);
    if (ulResult == ERROR_SUCCESS) {
        // Copy from stack.
        info.reset(reinterpret_cast<PTRACE_EVENT_INFO>(new char[ulSize]));
        memcpy(info.get(), szBuffer, ulSize);
        return ERROR_SUCCESS;
    } else if (ulResult == ERROR_INSUFFICIENT_BUFFER) {
        // Create buffer on heap and retry.
        info.reset(reinterpret_cast<PTRACE_EVENT_INFO>(new char[ulSize]));
        return TdhGetEventInformation(pEvent, TdhContextCount, pTdhContext, info.get(), &ulSize);
    }

    return ulResult;
}


inline ULONG TdhGetEventMapInformation(_In_ PEVENT_RECORD pEvent, _In_ LPWSTR pMapName, _Out_ std::unique_ptr<EVENT_MAP_INFO> &info)
{
    BYTE szBuffer[WINSTD_STACK_BUFFER_BYTES];
    ULONG ulSize = sizeof(szBuffer), ulResult;

    // Try with stack buffer first.
    ulResult = TdhGetEventMapInformation(pEvent, pMapName, (PEVENT_MAP_INFO)szBuffer, &ulSize);
    if (ulResult == ERROR_SUCCESS) {
        // Copy from stack.
        info.reset(reinterpret_cast<PEVENT_MAP_INFO>(new char[ulSize]));
        memcpy(info.get(), szBuffer, ulSize);
        return ERROR_SUCCESS;
    } else if (ulResult == ERROR_INSUFFICIENT_BUFFER) {
        // Create buffer on heap and retry.
        info.reset(reinterpret_cast<PEVENT_MAP_INFO>(new char[ulSize]));
        return TdhGetEventMapInformation(pEvent, pMapName, info.get(), &ulSize);
    }

    return ulResult;
}


template<class _Ty, class _Ax>
inline ULONG TdhGetProperty(_In_ PEVENT_RECORD pEvent, _In_ ULONG TdhContextCount, _In_ PTDH_CONTEXT pTdhContext, _In_ ULONG PropertyDataCount, _In_ PPROPERTY_DATA_DESCRIPTOR pPropertyData, _Out_ std::vector<_Ty, _Ax> &aData)
{
    ULONG ulSize, ulResult;

    // Query property size.
    ulResult = TdhGetPropertySize(pEvent, TdhContextCount, pTdhContext, PropertyDataCount, pPropertyData, &ulSize);
    if (ulResult == ERROR_SUCCESS) {
        if (ulSize) {
            // Query property value.
            aData.resize((ulSize + sizeof(_Ty) - 1) / sizeof(_Ty));
            ulResult = TdhGetProperty(pEvent, TdhContextCount, pTdhContext, PropertyDataCount, pPropertyData, ulSize, reinterpret_cast<LPBYTE>(aData.data()));
        } else {
            // Property value size is zero.
            aData.clear();
        }
    }

    return ulResult;
}
