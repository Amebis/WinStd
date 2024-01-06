/*
	SPDX-License-Identifier: MIT
	Copyright © 2022-2024 Amebis
*/

#include "pch.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(SDDL)
	{
	public:
		TEST_METHOD(security_attributes)
		{
			winstd::security_attributes sa;
			Assert::IsTrue(ConvertStringSecurityDescriptorToSecurityDescriptorW(L"O:BAD:PAI(A;;FA;;;BA)", SDDL_REVISION_1, sa, NULL));
			Assert::IsNotNull(sa.lpSecurityDescriptor);
			winstd::security_attributes sa2(move(sa));
#pragma warning(suppress: 26800) // That's exactly what we are testing here: if the object is in a sane state after being moved from.
			Assert::IsNull(sa.lpSecurityDescriptor);
			Assert::IsNotNull(sa2.lpSecurityDescriptor);
		}
	};
}
