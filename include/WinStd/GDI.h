/*
    ​SPDX-License-Identifier: MIT
    Copyright © 1991-2023 Amebis
*/

/// \defgroup WinStdGdiAPI GDI API

#pragma once

#include "Common.h"

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
        WINSTD_HANDLE_IMPL(gdi_handle, T, NULL)

    public:
        ///
        /// Closes an open object handle.
        ///
        /// \sa [DeleteObject function](https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-deleteobject)
        ///
        virtual ~gdi_handle()
        {
            if (m_h != invalid)
                free_internal();
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
    /// Windows HICON wrapper class
    ///
    class icon : public handle<HICON, NULL>
    {
        WINSTD_HANDLE_IMPL(icon, HICON, NULL)

    public:
        ///
        /// Closes an open object handle.
        ///
        /// \sa [DestroyIcon function](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-destroyicon)
        ///
        virtual ~icon()
        {
            if (m_h != invalid)
                free_internal();
        }

    protected:
        ///
        /// Closes an open object handle.
        ///
        /// \sa [DestroyIcon function](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-destroyicon)
        ///
        void free_internal() noexcept override
        {
            DestroyIcon(m_h);
        }
    };

    ///
    /// Device context wrapper class
    ///
    class dc : public handle<HDC, NULL>
    {
        WINSTD_HANDLE_IMPL(dc, HDC, NULL)

    public:
        ///
        /// Deletes the specified device context (DC).
        ///
        /// \sa [DeleteDC function](https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-deletedc)
        ///
        virtual ~dc()
        {
            if (m_h != invalid)
                free_internal();
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
        ///
        /// Initializes an empty device context.
        ///
        window_dc() noexcept :
            m_hwnd(NULL)
        {}

        ///
        /// Initializes a device context from existing data.
        ///
        window_dc(_In_opt_ handle_type h, _In_opt_ HWND hwnd) noexcept :
            handle<handle_type, NULL>(h),
            m_hwnd(hwnd)
        {}

        ///
        /// Move an existing device context.
        ///
        window_dc(_Inout_ window_dc &&h) noexcept :
            handle<handle_type, NULL>(std::move(h)),
            m_hwnd(h.m_hwnd)
        {}

        ///
        /// Copy an existing device context.
        ///
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
                free_internal();
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
        {}

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
