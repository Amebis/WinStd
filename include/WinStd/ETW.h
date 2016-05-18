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
    class event_auto;
    template <class T> class event_auto_res;
}

#define WINSTD_ETW_ERROR(ep, key, f, ...)      (ep).write                               (    TRACE_LEVEL_ERROR      , key, _T("  ") _T(__FUNCTION__) _T(" ") f, ##__VA_ARGS__)
#define WINSTD_ETW_WARNING(ep, key, f, ...)    (ep).write                               (    TRACE_LEVEL_WARNING    , key, _T("  ") _T(__FUNCTION__) _T(" ") f, ##__VA_ARGS__)
#define WINSTD_ETW_INFO(ep, key, f, ...)       (ep).write                               (    TRACE_LEVEL_INFORMATION, key, _T("  ") _T(__FUNCTION__) _T(" ") f, ##__VA_ARGS__)
#define WINSTD_ETW_DEBUG(ep, key, f, ...)      (ep).write                               (    TRACE_LEVEL_VERBOSE    , key, _T("  ") _T(__FUNCTION__) _T(" ") f, ##__VA_ARGS__)
#define WINSTD_ETW_FN_VOID_WARNING(ep, key)    winstd::event_auto            _event_auto(ep, TRACE_LEVEL_WARNING    , key, _T("->") _T(__FUNCTION__), _T("<-") _T(__FUNCTION__))
#define WINSTD_ETW_FN_VOID_INFO(ep, key)       winstd::event_auto            _event_auto(ep, TRACE_LEVEL_INFORMATION, key, _T("->") _T(__FUNCTION__), _T("<-") _T(__FUNCTION__))
#define WINSTD_ETW_FN_VOID_DEBUG(ep, key)      winstd::event_auto            _event_auto(ep, TRACE_LEVEL_VERBOSE    , key, _T("->") _T(__FUNCTION__), _T("<-") _T(__FUNCTION__))
#define WINSTD_ETW_FN_WARNING(ep, key, res)    winstd::event_auto_res<DWORD> _event_auto(ep, TRACE_LEVEL_WARNING    , key, _T("->") _T(__FUNCTION__), _T("<-") _T(__FUNCTION__) _T(" (%ld)"), res)
#define WINSTD_ETW_FN_INFO(ep, key, res)       winstd::event_auto_res<DWORD> _event_auto(ep, TRACE_LEVEL_INFORMATION, key, _T("->") _T(__FUNCTION__), _T("<-") _T(__FUNCTION__) _T(" (%ld)"), res)
#define WINSTD_ETW_FN_DEBUG(ep, key, res)      winstd::event_auto_res<DWORD> _event_auto(ep, TRACE_LEVEL_VERBOSE    , key, _T("->") _T(__FUNCTION__), _T("<-") _T(__FUNCTION__) _T(" (%ld)"), res)

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
        /// Writes a string event.
        ///
        /// \return
        /// - `ERROR_SUCCESS` when creation succeeds;
        /// - error code otherwise.
        ///
        /// \sa [EventWriteString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363750v=vs.85.aspx)
        ///
        inline ULONG write(_In_ UCHAR Level, _In_ ULONGLONG Keyword, _In_ PCWSTR String, ...)
        {
            if (!m_h)
                return ERROR_NOT_SUPPORTED;

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

    // winstd::event_auto actually and winstd::event_auto_res<> do not need an assignment operator actually, so the C4512 warning is safely ignored.
    #pragma warning(push)
    #pragma warning(disable: 4512)

    ///
    /// Helper class to write a string event on entry/exit of scope.
    ///
    /// It writes one string event at creation and another at destruction.
    ///
    class event_auto {
    public:
        inline event_auto(_In_ event_provider &ep, _In_ UCHAR Level, _In_ ULONGLONG Keyword, _In_z_ PCWSTR pszFormatEntry, _In_z_ PCWSTR pszFormatExit) :
            m_ep(ep),
            m_level(Level),
            m_keyword(Keyword),
            m_pszFormatExit(pszFormatExit)
        {
            m_ep.write(m_level, m_keyword, pszFormatEntry);
        }

        inline ~event_auto()
        {
            m_ep.write(m_level, m_keyword, m_pszFormatExit);
        }

    protected:
        event_provider &m_ep;           // Reference to event provider in use
        UCHAR          m_level;         // Event level
        ULONGLONG      m_keyword;       // Event keyword mask
        PCWSTR         m_pszFormatExit; // Pointer to string for string event at destruction
    };


    ///
    /// Helper template to write a string event on entry/exit of scope with one parameter (typically result).
    ///
    /// It writes one string event at creation and another at destruction, with allowing one sprintf type parameter for string event at destruction.
    ///
    template <class T>
    class event_auto_res : public event_auto {
    public:
        inline event_auto_res(_In_ event_provider &ep, _In_ UCHAR Level, _In_ ULONGLONG Keyword, _In_z_ PCWSTR pszFormatEntry, _In_z_ PCWSTR pszFormatExit, _In_ T &tResult) :
            m_tResult(tResult),
            event_auto(ep, Level, Keyword, pszFormatEntry, pszFormatExit)
        {
        }

        inline ~event_auto_res()
        {
            m_ep.write(m_level, m_keyword, m_pszFormatExit, m_tResult);
        }

    protected:
        T &m_tResult;   // Parameter for string event at destruction.
    };

    #pragma warning(pop)

    /// @}
}
