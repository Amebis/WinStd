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

namespace winstd
{
    template <class T> class com_obj;
}

#pragma once


namespace winstd
{
    ///
    /// \defgroup WinStdCOM COM object management
    /// Provides helper templates for Windows COM object manipulation
    ///
    /// @{


    ///
    /// COM object wrapper template
    ///
    template <class T>
    class com_obj : public handle<T>
    {
    public:

        ///
        /// Initializes a new class instance with the object set to NULL.
        ///
        inline com_obj() : handle()
        {
        }


        ///
        /// Initializes a new class instance with an available object.
        ///
        /// \param[in] h  Initial object value
        ///
        inline com_obj(_In_opt_ handle_type h) : handle(h)
        {
        }


        ///
        /// Creates a new object and initializes a new class with it.
        ///
        /// \sa [CoCreateInstance function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms686615.aspx)
        ///
        inline com_obj(_In_ REFCLSID rclsid, _In_opt_ LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
        {
            CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (LPVOID*)&m_h):
        }


        ///
        /// Creates a new object.
        ///
        /// \sa [CoCreateInstance function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms686615.aspx)
        ///
        inline HRESULT create(_In_ REFCLSID rclsid, _In_opt_ LPUNKNOWN pUnkOuter = NULL, _In_ DWORD dwClsContext = CLSCTX_ALL)
        {
            handle_type h;
            HRESULT hr = ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&h);
            if (SUCCEEDED(h))
                attach(h);
            return hr;
        }


        ///
        /// Queries the object for another interface.
        ///
        /// \sa [CoCreateInstance function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms686615.aspx)
        ///
        template <class _Other>
        HRESULT QueryInterface(_Out_ _Other **h) const
        {
            assert(h);
            assert(m_h);
            return m_h->QueryInterface(__uuidof(_Other), (void**)h);
        }
    };

    /// @}
}
