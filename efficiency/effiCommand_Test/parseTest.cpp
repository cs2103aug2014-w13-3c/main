#include "stdafx.h"
#include "CppUnitTest.h"

#include "parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace effiCommand_Test
{		
	TEST_CLASS(effiCommand_Test)
	{
	public:

		TEST_METHOD(invalidCommand){

			string commandString = "plus meeting tomorrow -p 2 -l G983";

			Parser parser;

			multimap<string, any> test = parser.parseCommand(commandString);

			bool invalid = any_cast<bool> ( test.find("-valid")->second );

			Assert::AreEqual(false, invalid);

			commandString = "add";

			test = parser.parseCommand(commandString);

			invalid = any_cast<bool> ( test.find("-valid")->second );

			Assert::AreEqual(false, invalid);

			commandString = "delete";

			test = parser.parseCommand(commandString);

			invalid = any_cast<bool> ( test.find("-valid")->second );

			Assert::AreEqual(false, invalid);
			
		}

		TEST_METHOD(basicCommand){

			string commandString = "add meeting tomorrow";

			Parser parser;

			multimap<string, any> test = parser.parseCommand(commandString);

			bool invalid = any_cast<bool> ( test.find("-valid")->second );

			Assert::AreEqual(true, invalid);
			
		}

		TEST_METHOD(missingParameters){

			string commandString = "add -p 2 -l G983";

			Parser parser;

			multimap<string, any> test = parser.parseCommand(commandString);

			bool invalid = any_cast<bool> ( test.find("-valid")->second );

			Assert::AreEqual(false, invalid);
			
		}
		
		TEST_METHOD(extractOptionValueEndWithValue){

			string commandString = "add Meeting tomorrow -p 2 -l G983";

			Parser parser;

			multimap<string, any> test = parser.parseCommand(commandString);

			string num = any_cast<string> ( test.find("-p")->second );
			string ID = any_cast<string> ( test.find("-l")->second );

			string two = "2";
			string correctID = "G983";

			Assert::AreEqual(two, num);
			Assert::AreEqual(correctID, ID);
			bool invalid = any_cast<bool> ( test.find("-valid")->second );

			Assert::AreEqual(true, invalid);
			
		}

		TEST_METHOD(extractOptionValueWithRedundantValues){

			string commandString = "add Meeting tomorrow -p 2 6 -l G983";

			Parser parser;

			multimap<string, any> test = parser.parseCommand(commandString);

			string num = any_cast<string> ( test.find("-p")->second );
			string ID = any_cast<string> ( test.find("-l")->second );

			string two = "2 6";
			string correctID = "G983";

			Assert::AreEqual(two, num);
			Assert::AreEqual(correctID, ID);

			bool invalid = any_cast<bool> ( test.find("-valid")->second );

			Assert::AreEqual(true, invalid);
			
		}

		TEST_METHOD(extractOptionValueEndWithEmptyOption){

			string commandString = "add Meeting tomorrow -p 2 -l";

			Parser parser;

			multimap<string, any> test = parser.parseCommand(commandString);

			string num = any_cast<string> ( test.find("-p")->second );

			string two = "2";

			Assert::AreEqual(two, num);

			int c = test.count("-l");

			Assert::AreEqual(0, c);

			bool invalid = any_cast<bool> ( test.find("-valid")->second );

			Assert::AreEqual(true, invalid);
			
		}

		TEST_METHOD(allOptionMissingValues){

			string commandString = "add Meeting tomorrow -p -l";

			Parser parser;

			multimap<string, any> test = parser.parseCommand(commandString);

			string param = any_cast<string> ( test.find("-param")->second );

			string expect = "Meeting tomorrow";

			Assert::AreEqual(expect, param);

			int c = test.count("-l");

			Assert::AreEqual(0, c);

			c = test.count("-p");

			Assert::AreEqual(0, c);

			bool invalid = any_cast<bool> ( test.find("-valid")->second );

			Assert::AreEqual(true, invalid);
			
		}

		TEST_METHOD(inexistentOption){

			string commandString = "add Meeting tomorrow -a 1 -v 2";

			Parser parser;

			multimap<string, any> test = parser.parseCommand(commandString);

			string param = any_cast<string> ( test.find("-param")->second );

			string expect = "Meeting tomorrow -a 1 -v 2";

			int mmSize = test.size();

			Assert::AreEqual(3, mmSize);

			Assert::AreEqual(expect, param);

			bool invalid = any_cast<bool> ( test.find("-valid")->second );

			Assert::AreEqual(true, invalid);
			
		}

	};
}