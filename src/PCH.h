/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
*/

#pragma once

#include "../include/WinStd/Base64.h"
#include "../include/WinStd/COM.h"
#include "../include/WinStd/Cred.h"
#include "../include/WinStd/Crypt.h"
#include "../include/WinStd/EAP.h"
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
#include "../include/WinStd/ETW.h"
#endif
#include "../include/WinStd/GDI.h"
#include "../include/WinStd/Hex.h"
#include "../include/WinStd/MSI.h"
#if defined(SECURITY_WIN32) || defined(SECURITY_KERNEL) || defined(SECURITY_MAC)
#include "../include/WinStd/Sec.h"
#endif
#include "../include/WinStd/SetupAPI.h"
#include "../include/WinStd/Shell.h"
#include "../include/WinStd/Win.h"
#include "../include/WinStd/WinSock2.h"
#include "../include/WinStd/WinTrust.h"
#include "../include/WinStd/WLAN.h"
#include "../include/WinStd/Common.h"

#include <tchar.h>
