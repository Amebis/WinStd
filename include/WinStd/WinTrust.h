/*
    Copyright 1991-2018 Amebis
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

#pragma once

///
/// \defgroup WinTrustAPI WinTrust API
/// Integrates WinStd classes with Microsoft WinTrust API
///

#include "Common.h"

namespace winstd
{
    class WINSTD_API wintrust;
}

#pragma once

#include <WinTrust.h>


namespace winstd
{
    /// \addtogroup WinTrustAPI
    /// @{

    ///
    /// WinTrust engine wrapper class
    ///
    class WINSTD_API wintrust
    {
    public:
        ///
        /// Initializes a new class instance.
        ///
        inline wintrust(_In_opt_ HWND hwnd, _In_ const GUID &action, _Inout_ WINTRUST_DATA &wtd) :
            m_hwnd(hwnd),
            m_action(action),
            m_wtd(wtd)
        {
            LONG lResult = WinVerifyTrust(m_hwnd, &m_action, &m_wtd);
            if (lResult != ERROR_SUCCESS)
                throw win_runtime_error(lResult, "WinVerifyTrust failed.");
        }

        ///
        /// Destroys the WinTrust context.
        ///
        virtual ~wintrust();

    protected:
        HWND          m_hwnd;
        GUID          m_action;
        WINTRUST_DATA &m_wtd;
    };

    /// @}
}
