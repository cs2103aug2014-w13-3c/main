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
			bool invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(false, invalid);

			commandString = "add";
			test = parser.parseCommand(commandString);
			invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(false, invalid);

			commandString = "delete";
			test = parser.parseCommand(commandString);
			invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(false, invalid);
			
		}

		TEST_METHOD(basicCommands){

			//Add
			string commandString = "add meeting tomorrow";
			Parser parser;
			multimap<string, any> test = parser.parseCommand(commandString);
			bool invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);

			//tag test
			commandString = "add meeting tomorrow -t meeting,tomorrow UTown, Starbucks";
			test = parser.parseCommand(commandString);
			invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);

			//start/end time good 1 test
			commandString = "add meeting tomorrow -s 2014-01-01 23:59";
			test = parser.parseCommand(commandString);
			invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);

			ptime expected = ptime(time_from_string("2014-01-01 23:59:00.000"));
			Assert::AreEqual(to_iso_string(any_cast<ptime>(test.find("start")->second)), to_iso_string(expected));


			//start/end time good 2 test
			commandString = "add meeting tomorrow -s 01-Jan-2014";
			test = parser.parseCommand(commandString);
			invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);

			expected = ptime(time_from_string("2014-01-01 00:00:00.000"));
			Assert::AreEqual(to_iso_string(any_cast<ptime>(test.find("start")->second)), to_iso_string(expected));

			//start/end time good 3 test
			commandString = "add meeting tomorrow -s 01-Feb-2014 12:00PM";
			test = parser.parseCommand(commandString);
			invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);

			expected = ptime(time_from_string("2014-02-01 12:00:00.000"));
			Assert::AreEqual(to_iso_string(any_cast<ptime>(test.find("start")->second)), to_iso_string(expected));

			//start/end time fail test
			commandString = "add meeting tomorrow -s pizza";
			test = parser.parseCommand(commandString);
			invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);
			int mmSize = test.size();
			Assert::AreEqual(3, mmSize);

			//update
			commandString = "update meeting tomorrow";
			test = parser.parseCommand(commandString);
			invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);

			//delete
			commandString = "/d meeting tomorrow";
			test = parser.parseCommand(commandString);
			invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);

			//Complete
			commandString = "/c G983";
			test = parser.parseCommand(commandString);
			invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);
			string Param = any_cast<string> ( test.find("param")->second );
			string correctParam = "G983";
			Assert::AreEqual(correctParam, Param);

			//Complete
			commandString = "/c G983";
			test = parser.parseCommand(commandString);
			invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);
			Param = any_cast<string> ( test.find("param")->second );
			correctParam = "G983";
			Assert::AreEqual(correctParam, Param);

			//undo
			commandString = "/z";
			test = parser.parseCommand(commandString);
			invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);

			//settings
			commandString = "/t";
			test = parser.parseCommand(commandString);
			invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);

			//minimize
			commandString = "/m";
			test = parser.parseCommand(commandString);
			invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);

			//help
			commandString = "/?";
			test = parser.parseCommand(commandString);
			invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);

			//logout
			commandString = "logout";
			test = parser.parseCommand(commandString);
			invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);

			//exit
			commandString = "/c G983";
			test = parser.parseCommand(commandString);
			invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);
			Param = any_cast<string> ( test.find("param")->second );
			correctParam = "G983";
			Assert::AreEqual(correctParam, Param);
			
		}

		TEST_METHOD(dateParser){

			string commandString = "add blahblah -s 2002-01-20";
			Parser parser;
			multimap<string, any> test = parser.parseCommand(commandString);
			ptime expected = ptime(time_from_string("2002-01-20 00:00:00.000"));
			Assert::AreEqual(to_iso_string(any_cast<ptime>(test.find("start")->second)), to_iso_string(expected));
			
		}

		TEST_METHOD(missingParameters){

			string commandString = "add -p 2 -l G983";
			Parser parser;
			multimap<string, any> test = parser.parseCommand(commandString);
			bool invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(false, invalid);
			
		}
		
		TEST_METHOD(extractOptionValueEndWithValue){

			string commandString = "add Meeting tomorrow -p 2 -l G983";
			Parser parser;
			multimap<string, any> test = parser.parseCommand(commandString);
			string num = any_cast<string> ( test.find("priority")->second );
			string ID = any_cast<string> ( test.find("link")->second );
			string two = "2";
			string correctID = "G983";

			Assert::AreEqual(two, num);
			Assert::AreEqual(correctID, ID);

			bool invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);
			
		}

		TEST_METHOD(extractOptionValueWithRedundantValues){

			string commandString = "add Meeting tomorrow -p 2 6 -l G983";
			Parser parser;
			multimap<string, any> test = parser.parseCommand(commandString);
			string num = any_cast<string> ( test.find("priority")->second );
			string ID = any_cast<string> ( test.find("link")->second );
			string two = "2 6";
			string correctID = "G983";

			Assert::AreEqual(two, num);
			Assert::AreEqual(correctID, ID);

			bool invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);
			
		}

		TEST_METHOD(extractOptionValueEndWithEmptyOption){

			string commandString = "add Meeting tomorrow -p 2 -l";
			Parser parser;
			multimap<string, any> test = parser.parseCommand(commandString);
			string num = any_cast<string> ( test.find("priority")->second );
			string two = "2";
			Assert::AreEqual(two, num);

			int c = test.count("-l");
			Assert::AreEqual(0, c);

			bool invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);
			
		}

		TEST_METHOD(allOptionMissingValues){

			string commandString = "add Meeting tomorrow -p -l";
			Parser parser;
			multimap<string, any> test = parser.parseCommand(commandString);
			string param = any_cast<string> ( test.find("param")->second );
			string expect = "Meeting tomorrow";
			Assert::AreEqual(expect, param);

			int c = test.count("link");
			Assert::AreEqual(0, c);

			c = test.count("priority");
			Assert::AreEqual(0, c);

			bool invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);
			
		}

		TEST_METHOD(inexistentOption){

			string commandString = "add Meeting tomorrow -a 1 -v 2";
			Parser parser;
			multimap<string, any> test = parser.parseCommand(commandString);
			string param = any_cast<string> ( test.find("param")->second );
			string expect = "Meeting tomorrow -a 1 -v 2";
			int mmSize = test.size();

			Assert::AreEqual(3, mmSize);
			Assert::AreEqual(expect, param);

			bool invalid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, invalid);
			
		}

	};
}