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
		{
			Event e("test event");
			string expected = "{\n"
			"    \"name\": \"test event\",\n"
			"    \"id\": \"0\",\n"
			"    \"priority\": \"5\",\n"
			"    \"tags\": \"\",\n"
			"    \"complete\": \"false\",\n"
			"    \"start\": \"0\",\n"
			"    \"end\": \"0\",\n"
			"    \"parent\": \"0\",\n"
			"    \"content\": \"0\"\n"
			"}\n";
			std::cout<<e;
			string result = consoleDump();
			Logger::WriteMessage(result.c_str());
			Assert::AreEqual(expected, result);

		}

		TEST_METHOD(IDIncrementCheck)
		{
			Event e("test event");
			Event e2("test event");
			Assert::AreEqual(e.getId() +1 ,e2.getId());
		}

		TEST_METHOD(InputTest)
		{
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

	};
}