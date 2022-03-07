/*
	SPDX-License-Identifier: MIT
	Copyright © 2022 Amebis
*/

#pragma once

#define SECURITY_WIN32
#define _WINSOCKAPI_	// Prevent inclusion of winsock.h in windows.h

#include <WinStd/COM.h>
#include <WinStd/Cred.h>
#include <WinStd/Crypt.h>
#include <WinStd/EAP.h>
#include <WinStd/ETW.h>
#include <WinStd/GDI.h>
#include <WinStd/MSI.h>
#include <WinStd/Sec.h>
#include <WinStd/SetupAPI.h>
#include <WinStd/Shell.h>
#include <WinStd/Win.h>
#include <WinStd/WinSock2.h>
#include <WinStd/WinTrust.h>
#include <WinStd/WLAN.h>

#include <CppUnitTest.h>
