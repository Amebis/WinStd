﻿/*
    ​SPDX-License-Identifier: MIT
    Copyright © 1991-2022 Amebis
*/

///
/// \defgroup WinStdGdiAPI GDI API
/// Integrates WinStd classes with Microsoft Windows GDI
///

#include "Common.h"

namespace winstd
{
    template<class T> class gdi_handle;
    class dc;
    class window_dc;
    class dc_selector;
}


#pragma once


namespace winstd
{
    /// \addtogroup WinStdGdiAPI
    /// @{

    ///
    /// Windows HGDIOBJ wrapper class
    ///
    template<class T>
    class gdi_handle : public handle<T, NULL>
    {
        WINSTD_HANDLE_IMPL(gdi_handle, NULL)

    public:
        ///
        /// Closes an open object handle.
        ///
        /// \sa [DeleteObject function](https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-deleteobject)
        ///
        virtual ~gdi_handle()
        {
            if (m_h != invalid)
                DeleteObject(m_h);
        }

    protected:
        ///
        /// Closes an open object handle.
        ///
        /// \sa [DeleteObject function](https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-deleteobject)
        ///
        void free_internal() noexcept override
        {
            DeleteObject(m_h);
        }
    };


    ///
    /// Device context wrapper class
    ///
    class dc : public handle<HDC, NULL>
    {
        WINSTD_HANDLE_IMPL(dc, NULL)

    public:
        ///
        /// Deletes the specified device context (DC).
        ///
        /// \sa [DeleteDC function](https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-deletedc)
        ///
        virtual ~dc()
        {
            if (m_h != invalid)
                DeleteDC(m_h);
        }

    protected:
        ///
        /// Deletes the specified device context (DC).
        ///
        /// \sa [DeleteDC function](https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-deletedc)
        ///
        void free_internal() noexcept override
        {
            DeleteDC(m_h);
        }
    };


    ///
    /// Device context wrapper class
    ///
    class window_dc : public handle<HDC, NULL>
    {
    public:
        window_dc() noexcept :
            m_hwnd(NULL)
        {}

        window_dc(_In_opt_ handle_type h, _In_opt_ HWND hwnd) noexcept :
            handle<handle_type, NULL>(h),
            m_hwnd(hwnd)
        {}

        window_dc(_Inout_ window_dc &&h) noexcept :
            handle<handle_type, NULL>(std::move(h)),
            m_hwnd(h.m_hwnd)
        {}

        window_dc& operator=(_Inout_ window_dc &&h) noexcept
        {
            handle<handle_type, NULL>::operator=(std::move(h));
            m_hwnd = h.m_hwnd;
            return *this;
        }

        WINSTD_NONCOPYABLE(window_dc)

    public:
        ///
        /// Releases a device context (DC), freeing it for use by other applications.
        ///
        /// \sa [ReleaseDC function](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-releasedc)
        ///
        virtual ~window_dc()
        {
            if (m_h != invalid)
                ReleaseDC(m_hwnd, m_h);
        }

    protected:
        ///
        /// Releases a device context (DC), freeing it for use by other applications.
        ///
        /// \sa [ReleaseDC function](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-releasedc)
        ///
        void free_internal() noexcept override
        {
            ReleaseDC(m_hwnd, m_h);
        }

    protected:
        HWND m_hwnd;    ///< Window handle
    };


    ///
    /// Context scope DC object restorer
    ///
    class dc_selector
    {
        WINSTD_NONCOPYABLE(dc_selector)
        WINSTD_NONMOVABLE(dc_selector)

    public:
        ///
        /// Selects an object into the specified device context (DC). The new object replaces the previous object of the same type.
        ///
        /// \sa [SelectObject function](https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-selectobject)
        ///
        dc_selector(_In_ HDC hdc, _In_ HGDIOBJ h) noexcept :
            m_hdc(hdc),
            m_orig(SelectObject(hdc, h))
        {
        }


        ///
        /// Restores original object.
        ///
        /// \sa [SelectObject function](https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-selectobject)
        ///
        virtual ~dc_selector()
        {
            if (m_orig)
                SelectObject(m_hdc, m_orig);
        }


        ///
        /// Return result of `SelectObject()` call.
        ///
        /// \sa [SelectObject function](https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-selectobject)
        ///
        HGDIOBJ status() const noexcept
        {
            return m_orig;
        }

    protected:
        HDC m_hdc;      ///< A handle to the device context
        HGDIOBJ m_orig; ///< Original object handle
    };

    /// @}
}