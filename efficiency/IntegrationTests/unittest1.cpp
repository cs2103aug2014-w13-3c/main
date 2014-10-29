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
			Controller ctrl;
			Executor executor(&ctrl);
			auto cmd = parser.parseCommand("add something");
			executor.executeCommand(cmd);
			Assert::AreEqual(int(ctrl.getAllEvents().size()),1);
		}

	};
}