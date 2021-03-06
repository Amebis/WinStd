﻿/*
    Copyright © 1991-2021 Amebis

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
/// \defgroup WinStdGdiAPI GDI API
/// Integrates WinStd classes with Microsoft Windows GDI
///

#include "Common.h"

#include <Windows.h>

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
        HANDLE_IMPL(gdi_handle, NULL)

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
        HANDLE_IMPL(dc, NULL)

    public:
        ///
        /// Deletes the specified device context (DC).
        ///
        /// \sa [DeleteDC function](https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-deletedc)
        ///
        virtual ~dc();

    protected:
        ///
        /// Deletes the specified device context (DC).
        ///
        /// \sa [DeleteDC function](https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-deletedc)
        ///
        void free_internal() noexcept override;
    };


    ///
    /// Device context wrapper class
    ///
    class window_dc : public handle<HDC, NULL>
    {
    public:
        inline window_dc() noexcept :
            m_hwnd(NULL)
        {}

        inline window_dc(_In_opt_ handle_type h, _In_opt_ HWND hwnd) noexcept :
            handle<handle_type, NULL>(h),
            m_hwnd(hwnd)
        {}

        inline window_dc(_Inout_ window_dc &&h) noexcept :
            handle<handle_type, NULL>(std::move(h)),
            m_hwnd(h.m_hwnd)
        {}

        inline window_dc& operator=(_Inout_ window_dc &&h) noexcept
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
        virtual ~window_dc();

    protected:
        ///
        /// Releases a device context (DC), freeing it for use by other applications.
        ///
        /// \sa [ReleaseDC function](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-releasedc)
        ///
        void free_internal() noexcept override;

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
        inline dc_selector(_In_ HDC hdc, _In_ HGDIOBJ h) noexcept :
            m_hdc(hdc),
            m_orig(SelectObject(hdc, h))
        {
        }


        ///
        /// Restores original object.
        ///
        /// \sa [SelectObject function](https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-selectobject)
        ///
        virtual ~dc_selector();


        ///
        /// Return result of `SelectObject()` call.
        ///
        /// \sa [SelectObject function](https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-selectobject)
        ///
        inline HGDIOBJ status() const noexcept
        {
            return m_orig;
        }

    protected:
        HDC m_hdc;      ///< A handle to the device context
        HGDIOBJ m_orig; ///< Original object handle
    };

    /// @}
}
