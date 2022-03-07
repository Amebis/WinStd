/*
    SPDX-License-Identifier: MIT
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
*/

/// \defgroup WinTrustAPI WinTrust API

#pragma once

#include "Common.h"
#include <WinTrust.h>

namespace winstd
{
    /// \addtogroup WinTrustAPI
    /// @{

    ///
    /// WinTrust engine wrapper class
    ///
    class wintrust
    {
        WINSTD_NONCOPYABLE(wintrust)
        WINSTD_NONMOVABLE(wintrust)

    public:
        ///
        /// Initializes a new class instance.
        ///
        wintrust(_In_opt_ HWND hwnd, _In_ const GUID &action, _Inout_ WINTRUST_DATA &wtd) :
            m_hwnd(hwnd),
            m_action(action),
            m_wtd(wtd)
        {
            const LONG lResult = WinVerifyTrust(m_hwnd, &m_action, &m_wtd);
            if (lResult != ERROR_SUCCESS)
                throw win_runtime_error(lResult, "WinVerifyTrust failed.");
        }

        ///
        /// Destroys the WinTrust context.
        ///
        virtual ~wintrust()
        {
            m_wtd.dwStateAction = WTD_STATEACTION_CLOSE;
            WinVerifyTrust(m_hwnd, &m_action, &m_wtd);
        }

    protected:
        /// \cond internal
        HWND          m_hwnd;
        GUID          m_action;
        WINTRUST_DATA &m_wtd;
        /// \endcond
    };

    /// @}
}
