#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include <cmath>
#include "../Main_Code/Header.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLab2
{
	TEST_CLASS(UnitTestLab2)
	{
	public:

		TEST_METHOD(TestMethod_Min_Max)
		{
			std::vector<double> Array = {1, 1, 3, 4, 5, 5};
			std::pair<double, double> minmax;
			std::pair<double, double> minmax_exp(1, 5);
			Laboratory_2::Min_Max(Array, minmax);

			Assert::AreEqual(minmax.first, minmax_exp.first);
			Assert::AreEqual(minmax.second, minmax_exp.second);
		}
	};
}
