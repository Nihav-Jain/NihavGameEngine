#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_Library_UWP
{
    TEST_CLASS(UnitTest1)
    {
    public:
        TEST_METHOD(TestMethod1)
        {
			Assert::IsTrue(true);
        }

		TEST_METHOD(TestMethod2)
		{
			Assert::Fail();
		}
    };
}