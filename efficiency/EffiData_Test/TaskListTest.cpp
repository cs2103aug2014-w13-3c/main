#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TaskListTest
{		
	TEST_CLASS(TaskListTest)
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

		TEST_METHOD(TaskInputTest){
			string contents = "{\n"
			"    \"name\": \"test event\",\n"
			"    \"id\": \"2\",\n"
			"    \"priority\": \"5\",\n"
			"    \"tags\": [\n"
			"        \"tag 1\",\n"
			"        \"nextTag\"\n"
			"    ],\n"
			"    \"complete\": \"false\",\n"
			"    \"start\": \"0\",\n"
			"    \"end\": \"0\",\n"
			"    \"parent\": \"0\",\n"
			"    \"content\": \"0\"\n"
			"}\n"
			"\n"
			"{\n"
			"    \"name\": \"test event2\",\n"
			"    \"id\": \"40\",\n"
			"    \"priority\": \"3\",\n"
			"    \"tags\": \"\",\n"
			"    \"complete\": \"true\",\n"
			"    \"start\": \"0\",\n"
			"    \"end\": \"0\",\n"
			"    \"parent\": \"0\",\n"
			"    \"content\": \"0\"\n"
			"}\n"
			"\n";
			TaskList t("");
			stringstream ss;
			ss.str(contents);
			ss>>t;
			std::cout<<t;
			string result = consoleDump();
			Logger::WriteMessage(result.c_str());
			Assert::AreEqual(contents, result);
		}

	};
}