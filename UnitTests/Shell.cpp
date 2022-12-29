/*
	SPDX-License-Identifier: MIT
	Copyright © 2022-2023 Amebis
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

		TEST_METHOD(PathRemoveBackslashA)
		{
			string path;

			static const char normal_path[] = "C:\\Windows\\Temp\\test";
			path = normal_path; path += '\\'; ::PathRemoveBackslashA(path);
			Assert::AreEqual(normal_path, path.c_str());
			path = normal_path; ::PathRemoveBackslashA(path);
			Assert::AreEqual(normal_path, path.c_str());

			static const char very_long_path[] = "C:\\Windows\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\test";
			path = very_long_path; path += '\\'; ::PathRemoveBackslashA(path);
			Assert::AreEqual(very_long_path, path.c_str());
			path = very_long_path; ::PathRemoveBackslashA(path);
			Assert::AreEqual(very_long_path, path.c_str());
		}

		TEST_METHOD(PathRemoveBackslashW)
		{
			wstring path;

			static const wchar_t normal_path[] = L"C:\\Windows\\Temp\\test";
			path = normal_path; path += L'\\'; ::PathRemoveBackslashW(path);
			Assert::AreEqual(normal_path, path.c_str());
			path = normal_path; ::PathRemoveBackslashW(path);
			Assert::AreEqual(normal_path, path.c_str());

			static const wchar_t very_long_path[] = L"C:\\Windows\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\Temp\\test";
			path = very_long_path; path += L'\\'; ::PathRemoveBackslashW(path);
			Assert::AreEqual(very_long_path, path.c_str());
			path = very_long_path; ::PathRemoveBackslashW(path);
			Assert::AreEqual(very_long_path, path.c_str());
		}
	};
}
