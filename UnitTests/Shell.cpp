/*
	SPDX-License-Identifier: MIT
	Copyright © 2022 Amebis
*/

#include "pch.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(Shell)
	{
	public:
		TEST_METHOD(PathCanonicalizeA)
		{
			string path;
			Assert::IsTrue(::PathCanonicalizeA(path, "C:\\Windows\\Temp\\test\\.."));
			Assert::AreEqual("C:\\Windows\\Temp", path.c_str(), true);
		}

		TEST_METHOD(PathCanonicalizeW)
		{
			wstring path;
			Assert::IsTrue(::PathCanonicalizeW(path, L"C:\\Windows\\Temp\\test\\.."));
			Assert::AreEqual(L"C:\\Windows\\Temp", path.c_str(), true);
		}
	};
}
