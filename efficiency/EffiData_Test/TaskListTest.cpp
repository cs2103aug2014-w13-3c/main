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

	};
}