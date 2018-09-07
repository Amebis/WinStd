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

#include "../include/WinStd/Base64.h"
#include "../include/WinStd/COM.h"
#include "../include/WinStd/Cred.h"
#include "../include/WinStd/Crypt.h"
#include "../include/WinStd/EAP.h"
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
#include "../include/WinStd/ETW.h"
#endif
#include "../include/WinStd/Common.h"
#include "../include/WinStd/MSI.h"
#if defined(SECURITY_WIN32) || defined(SECURITY_KERNEL) || defined(SECURITY_MAC)
#include "../include/WinStd/Sec.h"
#endif
#include "../include/WinStd/Shell.h"
#include "../include/WinStd/Win.h"
#include "../include/WinStd/WLAN.h"

#include <tchar.h>
