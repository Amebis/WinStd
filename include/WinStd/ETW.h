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

#include <assert.h>
#include <evntprov.h>
#include <evntcons.h>
#include <stdarg.h>
#include <tdh.h>

#include <memory>
#include <string>
#include <vector>

inline ULONG TdhGetEventInformation(_In_ PEVENT_RECORD pEvent, _In_ ULONG TdhContextCount, _In_ PTDH_CONTEXT pTdhContext, _Out_ std::unique_ptr<TRACE_EVENT_INFO> &info);

namespace winstd
{
    class WINSTD_API event_data;
    class WINSTD_API event_provider;
    class WINSTD_API event_session;
    class WINSTD_API event_trace;
    class WINSTD_API event_trace_enabler;

    class event_fn_auto;
    template<class T> class event_fn_auto_ret;
}

#pragma once


///
/// Retrieves metadata about an event.
///
/// \sa [TdhGetEventInformation function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa964840.aspx)
///
inline ULONG TdhGetEventInformation(_In_ PEVENT_RECORD pEvent, _In_ ULONG TdhContextCount, _In_ PTDH_CONTEXT pTdhContext, _Out_ std::unique_ptr<TRACE_EVENT_INFO> &info)
{
    BYTE szBuffer[WINSTD_STACK_BUFFER_BYTES];
    ULONG ulSize = sizeof(szBuffer), ulResult;

    // Try with stack buffer first.
    ulResult = TdhGetEventInformation(pEvent, TdhContextCount, pTdhContext, (PTRACE_EVENT_INFO)szBuffer, &ulSize);
    if (ulResult == ERROR_SUCCESS) {
        // Copy from stack.
        info.reset((PTRACE_EVENT_INFO)new char[ulSize]);
        memcpy(info.get(), szBuffer, ulSize);
        return ERROR_SUCCESS;
    } else if (ulResult == ERROR_INSUFFICIENT_BUFFER) {
        // Create buffer on heap and retry.
        info.reset((PTRACE_EVENT_INFO)new char[ulSize]);
        return TdhGetEventInformation(pEvent, TdhContextCount, pTdhContext, info.get(), &ulSize);
    }

    return ulResult;
}


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
            if (data)
                EventDataDescCreate(this, data, (ULONG)((strlen(data) + 1) * sizeof(*data)));
            else
                EventDataDescCreate(this, NULL, 0);
        }


        ///
        /// Construct class with wide string.
        ///
        /// \note This class is saves a reference to the data only. Therefore, data must be kept available.
        ///
        inline event_data(_In_ const wchar_t *data)
        {
            if (data)
                EventDataDescCreate(this, data, (ULONG)((wcslen(data) + 1) * sizeof(*data)));
            else
                EventDataDescCreate(this, NULL, 0);
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
        /// Construct class with binary data.
        ///
        /// \note This class is saves a reference to the data only. Therefore, data must be kept available.
        ///
        inline event_data(_In_bytecount_(size) const void *data, ULONG size)
        {
            EventDataDescCreate(this, data, size);
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
            m_prop((EVENT_TRACE_PROPERTIES*)new char[prop->Wnode.BufferSize]),
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
        /// \param[inout] other  A rvalue reference of another session
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
            return (LPCTSTR)((const char*)prop + prop->LoggerNameOffset);
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
            std::unique_ptr<EVENT_TRACE_PROPERTIES> prop((EVENT_TRACE_PROPERTIES*)new char[Properties->Wnode.BufferSize]);
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
    /// ETW trace
    ///
    class WINSTD_API event_trace : public handle<TRACEHANDLE>
    {
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
    /// Helper class to write an event on entry/exit of scope.
    ///
    /// It writes one string event at creation and another at destruction.
    ///
    class event_fn_auto
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
