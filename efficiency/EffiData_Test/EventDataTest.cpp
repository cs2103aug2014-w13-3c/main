#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EffiData_Test
{		
	TEST_CLASS(EffiData_Test)
	{
	protected:
		streambuf * oldStdoutBuf;
		stringstream consoleOutput;
		string consoleDump(){ 
			std::istreambuf_iterator<char> eos;
			std::string sout(std::istreambuf_iterator<char>(consoleOutput), eos);
			return sout;
		}
	public:
		//Redirect cout.
		TEST_METHOD_INITIALIZE(pre) {
			oldStdoutBuf = std::cout.rdbuf();
			std::cout.rdbuf(consoleOutput.rdbuf());
		}

		//Restore redirect cout
		TEST_METHOD_CLEANUP(post) 
		{
			std::cout.rdbuf(oldStdoutBuf);
		}

		TEST_METHOD(BasicPrintCheck)
		{//id = 1
			Event e("test event");
			string expected = "{\n"
			"    \"name\": \"test event\",\n"
			"    \"id\": \"1\",\n"
			"    \"priority\": \"5\",\n"
			"    \"tags\": \"\",\n" //Should be [], but boost property tree doesn't support it.
			"    \"complete\": \"false\",\n"
			"    \"start\": \"not-a-date-time\",\n"
			"    \"end\": \"not-a-date-time\",\n"
			"    \"parent\": \"0\",\n"
			"    \"content\": \"\"\n"
			"}\n";
			std::cout<<e;
			string result = consoleDump();
			Logger::WriteMessage(result.c_str());
			Assert::AreEqual(expected, result);
		}


		//WARNING: DO NOT RE-ORDER TESTS. ADD TO THE END.
		TEST_METHOD(TagCheck)
		{//id = 2
			Event e("test event");
			e.addTag("tag 1");
			e.addTag("nextTag");
			
			auto tags = e.getTags();
			Assert::AreEqual(2, (int)tags.size());
			Assert::IsTrue(std::find(tags.begin(), tags.end(), "tag 1")!=tags.end());
			Assert::IsTrue(std::find(tags.begin(), tags.end(), "nextTag")!=tags.end());
		}

		TEST_METHOD(TagPrintCheck)
		{ //id = 3
			Event e("test event");
			e.addTag("tag 1");
			e.addTag("nextTag");
			std::cout<<e;
			string result = consoleDump();
			Logger::WriteMessage(result.c_str());
			std::string expected = "{\n"
			"    \"name\": \"test event\",\n"
			"    \"id\": \"3\",\n"
			"    \"priority\": \"5\",\n"
			"    \"tags\": [\n"
			"        \"tag 1\",\n"
			"        \"nextTag\"\n"
			"    ],\n"
			"    \"complete\": \"false\",\n"
			"    \"start\": \"not-a-date-time\",\n"
			"    \"end\": \"not-a-date-time\",\n"
			"    \"parent\": \"0\",\n"
			"    \"content\": \"\"\n"
			"}\n";
			Assert::AreEqual(expected, result);
		
		}

		TEST_METHOD(IDIncrementCheck)
		{ //id = 4,5
			Event e("test event");
			Event e2("test event");
			Assert::AreEqual(e.getId() +1 ,e2.getId());
		}

		TEST_METHOD(InputTest)
		{ //id=6.
			string contents = "{\n"
			"    \"name\": \"test event\",\n"
			"    \"id\": \"40\",\n"
			"    \"priority\": \"3\",\n"
			"    \"tags\": \"\",\n"
			"    \"complete\": \"true\",\n"
			"    \"start\": \"0\",\n"
			"    \"end\": \"0\",\n"
			"    \"parent\": \"0\",\n"
			"    \"content\": \"0\"\n"
			"}\n";
			Event e("");
			stringstream ss;
			ss.str(contents);
			ss>>e;
			std::cout<<e;
			string result = consoleDump();
			Logger::WriteMessage(result.c_str());
			Assert::AreEqual(contents, result);
		}

		TEST_METHOD(TagDeleteCheck)
		{//id = 6
			Event e("test event");
			e.addTag("tag 1");
			e.addTag("nextTag");
			e.addTag("final tag");
			e.removeTag("nextTag");
			e.removeTag("noexisttag");
			auto tags = e.getTags();
			Assert::AreEqual(2, (int)tags.size());
			Assert::IsTrue(std::find(tags.begin(), tags.end(), "tag 1")!=tags.end());
			Assert::IsTrue(std::find(tags.begin(), tags.end(), "final tag")!=tags.end());
		}

		// Set the start date for an event.
		TEST_METHOD(SetStartDateTest)
		{
			string ts("2002-01-20 23:59:59.000");
			ptime datetime(time_from_string(ts));
			Event e("test event");
			e.setStartDate(datetime);
			Assert::AreEqual(to_simple_string(datetime), to_simple_string(e.getStartDate()));
		}

		// Set the content of an event.
		TEST_METHOD(SetContentTest)
		{
			Event e("test event");
			string content = "test content";
			e.setContent(content);
			Assert::AreEqual(content, e.getContent());
		}

		// Check that the default content of an event is an empty string.
		TEST_METHOD(ContentDefaultTest){
			Event e("test event");
			string empty = "";
			Assert::AreEqual(empty, e.getContent());
		}
	};
}