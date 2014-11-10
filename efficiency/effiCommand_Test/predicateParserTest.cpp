//@author A0086591U
#include "stdafx.h"
#include "CppUnitTest.h"

#include "predParser.h"
#include "controller.h"
#include "boost/date_time/posix_time/posix_time.hpp"


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

		TEST_METHOD(splitConditional1){
			auto result = splitConditional("field> 5");
			Assert::AreEqual(string("field"),std::get<0>(result));
			Assert::AreEqual(string(">"),std::get<1>(result));
			Assert::AreEqual(string("5"),std::get<2>(result));
		}

		TEST_METHOD(splitConditional2){
			auto result = splitConditional("name: some shit");
			Assert::AreEqual(string("name"),std::get<0>(result));
			Assert::AreEqual(string(":"),std::get<1>(result));
			Assert::AreEqual(string("some shit"),std::get<2>(result));
		}

		TEST_METHOD(splitConditional3){
			auto result = splitConditional("name!: some shit");
			Assert::AreEqual(string("name"),std::get<0>(result));
			Assert::AreEqual(string("!:"),std::get<1>(result));
			Assert::AreEqual(string("some shit"),std::get<2>(result));
		}

		//Removed since dates need to include some of the forbidden symbols.
		/*TEST_METHOD(splitConditionalFail1){
			Assert::ExpectException<expected>([](){splitConditional("name: some ~shit");});
		}*/

		TEST_METHOD(predParseNameSatisfactionTest){
			//Despite appearances, this is not an integration test. This is just the easiest way to do it.
			Controller ctrl(""); 
			auto evt = ctrl.addEvent("meow");
			pred p = parsePredicate("name=meow");
			Assert::AreEqual(true, p(boost::any(evt)));
			pred p2 = parsePredicate("name=meowz");
			Assert::AreEqual(false, p2(boost::any(evt)));
			pred p3 = parsePredicate("name=meowz||name=meowzy");
			Assert::AreEqual(false, p3(boost::any(evt)));
			pred p4 = parsePredicate("name=meowz||name=meowzy||name=meow");
			Assert::AreEqual(true, p4(boost::any(evt)));
			pred p5 = parsePredicate("(name=meowz||name=meowzy)&&name=meow");
			Assert::AreEqual(false, p5(boost::any(evt)));
			pred p6 = parsePredicate("(name=meowz&&name=meowzy)||name=meow");
			Assert::AreEqual(true, p6(boost::any(evt)));
			pred p7 = parsePredicate("((((name=meow))))");
			Assert::AreEqual(true, p7(boost::any(evt)));
		}

		TEST_METHOD(predParseTagSatisfactionTest){
			//Despite appearances, this is not an integration test. This is just the easiest way to do it.
			Controller ctrl(""); 
			auto evt = ctrl.addEvent("meow");
			evt.addTag("tag1");
			evt.addTag("much");
			evt.addTag("frustrate");
			evt.addTag("such");
			evt.addTag("wow");
			evt.exec();
			pred p = parsePredicate("name=meow&&tags:much&&tags:frustrate");
			Assert::AreEqual(true, p(boost::any(evt)));
		}

		//@author A0101642X
		TEST_METHOD(predParseDateSatisfactionTest){
			Controller ctrl("");
			auto evt = ctrl.addEvent("woof");
			string date1 = "2014-10-21 23:59:00.000";
			string date2 = "2014-10-22 23:59:00.000";
			ptime t1 = ptime(time_from_string(date1));
			ptime t2 = ptime(time_from_string(date2));
			evt.setStartDate(t1);
			evt.setEndDate(t2);
			evt.exec();
			pred p = parsePredicate("name=woof&&start="+date1+"&&end="+date2);
			Assert::AreEqual(true, p(boost::any(evt)));
		}

		TEST_METHOD(predParseContentSatisfactionTest){
			Controller ctrl("");
			auto evt = ctrl.addEvent("woof");
			evt.setContent("test");
			evt.exec();
			pred p = parsePredicate("name=woof&&content:test");
			Assert::AreEqual(true, p(boost::any(evt)));
			pred p2 = parsePredicate("name=woof&&content!:stuff");
			Assert::AreEqual(true, p2(boost::any(evt)));
		}

		TEST_METHOD(predParseSpacesTest){
			Controller ctrl("");
			auto evt = ctrl.addEvent("woof");
			evt.setContent("test");
			evt.addTag("tag1");
			evt.addTag("much");
			evt.addTag("frustrate");
			evt.addTag("such");
			evt.addTag("wow");
			evt.exec();
			pred p = parsePredicate("name = woof");
			Assert::AreEqual(true, p(boost::any(evt)));
			pred p2 = parsePredicate("name = woof && tags: much");
			Assert::AreEqual(true, p2(boost::any(evt)));
		}
	};
}