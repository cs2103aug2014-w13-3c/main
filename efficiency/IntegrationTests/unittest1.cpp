//@author A0086591U
#include "stdafx.h"
#include "CppUnitTest.h"
#include "parser.h"
#include "executor.h"
#include "controller.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTests
{		
	TEST_CLASS(IntegrationTest)
	{
	public:
		
		TEST_METHOD(Addxecution)
		{
			Parser parser;
			Controller ctrl("");
			Executor executor(&ctrl);
			auto cmd = parser.parseCommand("add something");
			executor.executeCommand(cmd);
			Assert::AreEqual(int(ctrl.getAllEvents().size()),1);
		}

		TEST_METHOD(testAddDate)
		{
			Parser parser;
			Controller ctrl("");
			Executor executor(&ctrl);
			auto cmd = parser.parseCommand("add something -s 2014-10-30 15:30");
			//Logger::WriteMessage(to_string(any_cast<bool>(cmd.find("valid")->second)).c_str());
			Logger::WriteMessage(to_iso_string(any_cast<ptime>(cmd.find("start")->second)).c_str());

			executor.executeCommand(cmd);
			Assert::AreEqual(int(ctrl.getAllEvents().size()),1);
		}


	};
}