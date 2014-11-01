#include "stdafx.h"
#include "CppUnitTest.h"

#include "predParser.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace predParserTest
{		
	TEST_CLASS(predParserTest){
	public:
		TEST_METHOD(splitPredicate1){
			auto result = splitPredicate("A||B");
			Assert::AreEqual(string("A"),std::get<0>(result));
			Assert::AreEqual(string("||"),std::get<1>(result));
			Assert::AreEqual(string("B"),std::get<2>(result));
		}

		TEST_METHOD(splitPredicate2){
			auto result = splitPredicate("(A||B)||C");
			Assert::AreEqual(string("A||B"),std::get<0>(result));
			Assert::AreEqual(string("||"),std::get<1>(result));
			Assert::AreEqual(string("C"),std::get<2>(result));
		}

		TEST_METHOD(splitPredicate3){
			auto result = splitPredicate("A||(B||C)");
			Assert::AreEqual(string("A"),std::get<0>(result));
			Assert::AreEqual(string("||"),std::get<1>(result));
			Assert::AreEqual(string("(B||C)"),std::get<2>(result));
		}

		TEST_METHOD(splitPredicate4){
			auto result = splitPredicate("A||(B)||C");
			Assert::AreEqual(string("A"),std::get<0>(result));
			Assert::AreEqual(string("||"),std::get<1>(result));
			Assert::AreEqual(string("(B)||C"),std::get<2>(result));
		}
	};
}