/*
	SPDX-License-Identifier: MIT
	Copyright © 2022 Amebis
*/

#include "pch.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(Win)
	{
	public:
		TEST_METHOD(WideCharToMultiByte)
		{
			string response;
			{
				static const char expected[] = "Copyright \xA9 2017";
				Assert::IsTrue(sizeof(expected) <= WINSTD_STACK_BUFFER_BYTES);
				Assert::AreEqual<int>(_countof(expected), ::WideCharToMultiByte(1252, 0, L"Copyright \u00A9 2017", -1, response, NULL, NULL));
				Assert::AreEqual(expected, response.c_str(), true);
			}

			{
				static const char expected[] = "The WC_COMPOSITECHECK flag causes the WideCharToMultiByte function to test for decomposed Unicode characters and attempts to compose them before converting them to the requested code page. This flag is only available for conversion to single byte (SBCS) or double byte (DBCS) code pages (code pages < 50000, excluding code page 42). If your application needs to convert decomposed Unicode data to single byte or double byte code pages, this flag might be useful. However, not all characters can be converted this way and it is more reliable to save and store such data as Unicode.\r\n\r\nWhen an application is using WC_COMPOSITECHECK, some character combinations might remain incomplete or might have additional nonspacing characters left over.For example, A + \xc2\xa8 + \xc2\xa8 combines to \xc3\x84 + \xc2\xa8.Using the WC_DISCARDNS flag causes the function to discard additional nonspacing characters.Using the WC_DEFAULTCHAR flag causes the function to use the default replacement character(typically \" ? \") instead. Using the WC_SEPCHARS flag causes the function to attempt to convert each additional nonspacing character to the target code page. Usually this flag also causes the use of the replacement character (\" ? \"). However, for code page 1258 (Vietnamese) and 20269, nonspacing characters exist and can be used. The conversions for these code pages are not perfect. Some combinations do not convert correctly to code page 1258, and WC_COMPOSITECHECK corrupts data in code page 20269. As mentioned earlier, it is more reliable to design your application to save and store such data as Unicode.";
				Assert::IsTrue(sizeof(expected) > WINSTD_STACK_BUFFER_BYTES);
				Assert::AreEqual<int>(_countof(expected), ::WideCharToMultiByte(CP_UTF8, 0, L"The WC_COMPOSITECHECK flag causes the WideCharToMultiByte function to test for decomposed Unicode characters and attempts to compose them before converting them to the requested code page. This flag is only available for conversion to single byte (SBCS) or double byte (DBCS) code pages (code pages < 50000, excluding code page 42). If your application needs to convert decomposed Unicode data to single byte or double byte code pages, this flag might be useful. However, not all characters can be converted this way and it is more reliable to save and store such data as Unicode.\r\n\r\nWhen an application is using WC_COMPOSITECHECK, some character combinations might remain incomplete or might have additional nonspacing characters left over.For example, A + \u00a8 + \u00a8 combines to \u00c4 + \u00a8.Using the WC_DISCARDNS flag causes the function to discard additional nonspacing characters.Using the WC_DEFAULTCHAR flag causes the function to use the default replacement character(typically \" ? \") instead. Using the WC_SEPCHARS flag causes the function to attempt to convert each additional nonspacing character to the target code page. Usually this flag also causes the use of the replacement character (\" ? \"). However, for code page 1258 (Vietnamese) and 20269, nonspacing characters exist and can be used. The conversions for these code pages are not perfect. Some combinations do not convert correctly to code page 1258, and WC_COMPOSITECHECK corrupts data in code page 20269. As mentioned earlier, it is more reliable to design your application to save and store such data as Unicode.", -1, response, NULL, NULL));
				Assert::AreEqual(expected, response.c_str(), true);
			}
		}

		TEST_METHOD(CreateWellKnownSid)
		{
			std::unique_ptr<SID> sid;
			Assert::IsTrue(::CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, sid));
		}

		TEST_METHOD(DuplicateTokenEx)
		{
			winstd::win_handle<NULL> processToken;
			Assert::IsTrue(::OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE, processToken));
			winstd::win_handle<NULL> token;
			Assert::IsTrue(::DuplicateTokenEx(processToken, TOKEN_QUERY | TOKEN_IMPERSONATE, NULL, SecurityIdentification, TokenImpersonation, &token));
		}

		TEST_METHOD(CheckTokenMembership)
		{
			std::unique_ptr<SID> sid;
			Assert::IsTrue(::CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, sid));
			winstd::win_handle<NULL> processToken;
			Assert::IsTrue(::OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE, processToken));
			winstd::win_handle<NULL> token;
			Assert::IsTrue(::DuplicateTokenEx(processToken, TOKEN_QUERY | TOKEN_IMPERSONATE, NULL, SecurityIdentification, TokenImpersonation, &token));
			BOOL bIsMember = 0xcdcdcdcd;
			Assert::IsTrue(::CheckTokenMembership(token, sid.get(), &bIsMember));
			Assert::IsTrue(bIsMember == TRUE || bIsMember == FALSE);
		}

		TEST_METHOD(library)
		{
			winstd::library lib_shell32(LoadLibraryEx(_T("shell32.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE));
			if (!lib_shell32)
				Assert::Fail(L"LoadLibraryEx failed");
		}

		TEST_METHOD(system_impersonator)
		{
			winstd::win_handle<NULL> processToken;
			if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE, processToken))
				Assert::Fail(L"OpenProcessToken failed");
			DWORD isElevated, dwLength;
			if (!GetTokenInformation(processToken, TokenElevation, &isElevated, sizeof(isElevated), &dwLength))
				Assert::Fail(L"GetTokenInformation failed");
			winstd::system_impersonator system_impersonator;
			// SYSTEM impersonation works in elevated processes only.
			if (dwLength == sizeof(isElevated) && isElevated)
				Assert::IsTrue(system_impersonator);
			else
				Assert::IsTrue(!system_impersonator && GetLastError() == ERROR_ACCESS_DENIED);
		}

		TEST_METHOD(AllocateAndInitializeSid)
		{
			SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
			winstd::security_id pSIDEveryone;
			Assert::IsTrue(::AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, pSIDEveryone));

			SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
			winstd::security_id pSIDSystem;
			Assert::IsTrue(::AllocateAndInitializeSid(&SIDAuthNT, 1, SECURITY_LOCAL_SYSTEM_RID, 0, 0, 0, 0, 0, 0, 0, pSIDSystem));

			winstd::security_id pSIDAdmin;
			Assert::IsTrue(::AllocateAndInitializeSid(&SIDAuthNT, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, pSIDAdmin));
		}
	};
}
