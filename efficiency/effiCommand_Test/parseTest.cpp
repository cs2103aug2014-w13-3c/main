#include "stdafx.h"
#include "CppUnitTest.h"

#include "parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace effiCommand_Test
{		
	TEST_CLASS(effiCommand_Test)
	{
	public:

		TEST_METHOD(basicCommands){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			//Add
			string commandString = "add test";
			multimap<string, any> test = parser.parseCommand(commandString);
			bool valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);

			mmSize = test.size();
			expectedMMSize = 3;
			Assert::AreEqual(expectedMMSize, mmSize);

			//tag test
			commandString = "add meeting tomorrow -t meeting,tomorrow UTown, Starbucks";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);

			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

			//start/end time fail test
			commandString = "add Read Tutorial";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);		

			mmSize = test.size();
			expectedMMSize = 3;
			Assert::AreEqual(expectedMMSize, mmSize);

			//update
			commandString = "update meeting tomorrow -s 01-Apr-2014 3:00PM";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);

			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

			//delete
			commandString = "/d meeting tomorrow -s 01-Feb-2014";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(false, valid);

			mmSize = test.size();
			expectedMMSize = 3;
			Assert::AreEqual(expectedMMSize, mmSize);

			//search and filter
			commandString = "search name:G983";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);
			string Param = any_cast<string> ( test.find("parse_string")->second );
			string correctParam = "name:G983 ";
			Assert::AreEqual(correctParam, Param);

			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

			commandString = "filter name:G983";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);
			Param = any_cast<string> ( test.find("parse_string")->second );
			correctParam = "name:G983 ";
			Assert::AreEqual(correctParam, Param);

			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

			//sort
			commandString = "sort all name";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);
			Param = any_cast<string> ( test.find("param")->second );
			correctParam = "all name";
			Assert::AreEqual(correctParam, Param);

			mmSize = test.size();
			expectedMMSize = 3;
			Assert::AreEqual(expectedMMSize, mmSize);

			//complete
			commandString = "/c G983";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);
			Param = any_cast<string> ( test.find("param")->second );
			correctParam = "G983";
			Assert::AreEqual(correctParam, Param);

			mmSize = test.size();
			expectedMMSize = 3;
			Assert::AreEqual(expectedMMSize, mmSize);

			//incomplete
			commandString = "/i G983";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);
			Param = any_cast<string> ( test.find("param")->second );
			correctParam = "G983";
			Assert::AreEqual(correctParam, Param);

			mmSize = test.size();
			expectedMMSize = 3;
			Assert::AreEqual(expectedMMSize, mmSize);

			//scroll
			commandString = "scroll deadlines next";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);
			Param = any_cast<string> ( test.find("param")->second );
			correctParam = "deadlines next";
			Assert::AreEqual(correctParam, Param);

			mmSize = test.size();
			expectedMMSize = 3;
			Assert::AreEqual(expectedMMSize, mmSize);

			//undo
			commandString = "/z";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);

			mmSize = test.size();
			expectedMMSize = 3;
			Assert::AreEqual(expectedMMSize, mmSize);
			
		}

		TEST_METHOD(invalidCommand){

			Parser parser;

			string commandString = "plus meeting tomorrow -p 2 -l G983";
			multimap<string, any> test = parser.parseCommand(commandString);
			bool valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(false, valid);

			commandString = "add";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(false, valid);

			commandString = "delete";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(false, valid);
			
		}

		TEST_METHOD(missingParameters){

			Parser parser;

			string commandString = "add -p 2 -l G983";
			multimap<string, any> test = parser.parseCommand(commandString);
			bool valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(false, valid);
			
		}

		TEST_METHOD(commandWithRedundantField){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			string commandString = "/c yoga lesson -s 12-Dec-2014 -r";
			multimap<string, any> test = parser.parseCommand(commandString);

			bool valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(false, valid);

			int c = test.count("start");
			Assert::AreEqual(0, c);

			mmSize = test.size();
			expectedMMSize = 3; // not 5, -s is redundant
			Assert::AreEqual(expectedMMSize, mmSize);

		}

		TEST_METHOD(invalidScrollParam){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			//test one: invalid task type
			string commandString = "scroll floating next";
			multimap<string, any> test = parser.parseCommand(commandString);
			bool valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(false, valid);
			string Param = any_cast<string> ( test.find("param")->second );
			string correctParam = "floating next";
			Assert::AreEqual(correctParam, Param);

			mmSize = test.size();
			expectedMMSize = 3;
			Assert::AreEqual(expectedMMSize, mmSize);

			//test two: invalid scroll direction
			commandString = "scroll deadlines backwards";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(false, valid);
			Param = any_cast<string> ( test.find("param")->second );
			correctParam = "deadlines backwards";
			Assert::AreEqual(correctParam, Param);

			mmSize = test.size();
			expectedMMSize = 3;
			Assert::AreEqual(expectedMMSize, mmSize);

			//test three: both are invalid
			commandString = "scroll anyhow type";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(false, valid);
			Param = any_cast<string> ( test.find("param")->second );
			correctParam = "anyhow type";
			Assert::AreEqual(correctParam, Param);

			mmSize = test.size();
			expectedMMSize = 3;
			Assert::AreEqual(expectedMMSize, mmSize);

		}

		TEST_METHOD(SensibleDateTimesWithoutPaddingZeroes)
		{
			Parser parser;
			string cmd = "add something -s 4/11/2014 -e 5/11/2014";
			multimap<string, any> test = parser.parseCommand(cmd);
			Assert::IsTrue(any_cast<bool> ( test.find("valid")->second ));
			ptime st = any_cast<ptime> ( test.find("start")->second );
			Assert::IsFalse(st.is_not_a_date_time());
			ptime et = any_cast<ptime> ( test.find("end")->second );
			Assert::IsFalse(et.is_not_a_date_time());

			cmd = "add something -s 4/11/2014 6:30pm -e 5/11/2014 7:30pm";
			test = parser.parseCommand(cmd);
			Assert::IsTrue(any_cast<bool> ( test.find("valid")->second ));
			st = any_cast<ptime> ( test.find("start")->second );
			Assert::IsFalse(st.is_not_a_date_time());
			et = any_cast<ptime> ( test.find("end")->second );
			Assert::IsFalse(et.is_not_a_date_time());

		}

		TEST_METHOD(basicStartEndFieldsTest){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			//start/end time good test
			string commandString0 = "add meeting next week -s 2014-01-01";
			multimap<string, any> test0 = parser.parseCommand(commandString0);
			bool valid0 = any_cast<bool> ( test0.find("valid")->second );
			ptime t = any_cast<ptime> ( test0.find("start")->second );
			Assert::AreEqual(true, valid0);
			Assert::IsFalse(t.is_not_a_date_time());

			//start/end time good test
			string commandString = "add meeting next week -s 2014-01-01 23:59";
			multimap<string, any> test = parser.parseCommand(commandString);
			bool valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);

			ptime expected = ptime(time_from_string("2014-01-01 23:59:00.000"));
			Assert::AreEqual(to_iso_string(any_cast<ptime>(test.find("start")->second)), to_iso_string(expected));

			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

			//start/end time good test
			commandString = "add submit homework -e 2014-10-30 15:00:00";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);

			expected = ptime(time_from_string("2014-10-30 15:00:00.000"));
			Assert::AreEqual(to_iso_string(any_cast<ptime>(test.find("end")->second)), to_iso_string(expected));

			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

			//start/end time good test
			commandString = "add meeting tomorrow -s 01-Feb-2014";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);

			expected = ptime(time_from_string("2014-02-01 00:00:00.000"));
			Assert::AreEqual(to_iso_string(any_cast<ptime>(test.find("start")->second)), to_iso_string(expected));

			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

			//start/end time good test
			commandString = "add meeting tomorrow -s 01-Apr-2014 12:00PM -e 30-Apr-2014 1:00PM";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);

			expected = ptime(time_from_string("2014-04-01 12:00:00.000"));
			Assert::AreEqual(to_iso_string(any_cast<ptime>(test.find("start")->second)), to_iso_string(expected));

			expected = ptime(time_from_string("2014-04-30 13:00:00.000"));
			Assert::AreEqual(to_iso_string(any_cast<ptime>(test.find("end")->second)), to_iso_string(expected));

			mmSize = test.size();
			expectedMMSize = 5;
			Assert::AreEqual(expectedMMSize, mmSize);

			//Add
			commandString = "add stuff -s 2014-09-13 -e 24/Sep/2014";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);

			expected = ptime(time_from_string("2014-09-13 00:00:00.000"));
			Assert::AreEqual(to_iso_string(any_cast<ptime>(test.find("start")->second)), to_iso_string(expected));

			expected = ptime(time_from_string("2014-09-24 00:00:00.000"));
			Assert::AreEqual(to_iso_string(any_cast<ptime>(test.find("end")->second)), to_iso_string(expected));

			mmSize = test.size();
			expectedMMSize = 5;
			Assert::AreEqual(expectedMMSize, mmSize);

		}

		TEST_METHOD(contentField){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			//Add
			string commandString = "add meeting tomorrow -c Meeting is about CS2103T";		
			multimap<string, any> test = parser.parseCommand(commandString);
			string content = any_cast<string> ( test.find("content")->second );
			string expectedContent = "Meeting is about CS2103T";
			Assert::AreEqual(expectedContent, content);

			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

		}

		TEST_METHOD(priorityField){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			//Add
			string commandString = "add meeting tomorrow -p 1";		
			multimap<string, any> test = parser.parseCommand(commandString);
			int priority = any_cast<int> ( test.find("priority")->second );
			int expectedPriority = 1;
			Assert::AreEqual(expectedPriority, priority);

			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

		}

		TEST_METHOD(repeatField){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			//Add
			string commandString = "add yoga lesson -r every Sunday";		
			multimap<string, any> test = parser.parseCommand(commandString);
			string repeat = any_cast<string> ( test.find("repeat")->second );
			string expectedRepeat = "every Sunday";
			Assert::AreEqual(expectedRepeat, repeat);

			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

		}

		TEST_METHOD(incompleteRepeatField){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			//Add
			string commandString = "add yoga lesson -r";		
			multimap<string, any> test = parser.parseCommand(commandString);

			int c = test.count("repeat");
			Assert::AreEqual(0, c);

			c = test.count("recursive");
			Assert::AreEqual(0, c);

			mmSize = test.size();
			expectedMMSize = 3;
			Assert::AreEqual(expectedMMSize, mmSize);

		}

		TEST_METHOD(completeAndIncompleteWithUnusedOption){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			//Complete with unused option
			string commandString = "/c G983 -p 3 -s 2014-01-01";
			multimap<string, any> test = parser.parseCommand(commandString);
			bool valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(false, valid);
			string Param = any_cast<string> ( test.find("param")->second );
			string correctParam = "G983";
			Assert::AreEqual(correctParam, Param);
			
			mmSize = test.size();
			expectedMMSize = 3;
			Assert::AreEqual(expectedMMSize, mmSize);

			//Incomplete with unused option
			commandString = "/i G983 -p 3 -s 2014-01-01";
			test = parser.parseCommand(commandString);
			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(false, valid);
			Param = any_cast<string> ( test.find("param")->second );
			correctParam = "G983";
			Assert::AreEqual(correctParam, Param);

			mmSize = test.size();
			expectedMMSize = 3;
			Assert::AreEqual(expectedMMSize, mmSize);

		}

		TEST_METHOD(completeAndIncompleteWithRecursive){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			//Complete with unused option
			string commandString = "/c G983 -r this is redundant string";
			multimap<string, any> test = parser.parseCommand(commandString);

			bool valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);

			string Param = any_cast<string> ( test.find("param")->second );
			string correctParam = "G983";
			Assert::AreEqual(correctParam, Param);

			bool recursive = any_cast<bool> ( test.find("recursive")->second );
			bool isRecursive = true;
			Assert::AreEqual(isRecursive, recursive);
			
			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

			//Incomplete with unused option
			commandString = "/i G983 -r this is redundant string";
			test = parser.parseCommand(commandString);

			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);

			Param = any_cast<string> ( test.find("param")->second );
			correctParam = "G983";
			Assert::AreEqual(correctParam, Param);

			recursive = any_cast<bool> ( test.find("recursive")->second );
			isRecursive = true;
			Assert::AreEqual(isRecursive, recursive);

			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

		}

		TEST_METHOD(deleteWithUnusedOption){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			//Complete with unused option
			string commandString = "/d Test -p 3 -s 2014-01-01";
			multimap<string, any> test = parser.parseCommand(commandString);
			bool valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(false, valid);
			string Param = any_cast<string> ( test.find("param")->second );
			string correctParam = "Test";
			Assert::AreEqual(correctParam, Param);
			
			mmSize = test.size();
			expectedMMSize = 3;
			Assert::AreEqual(expectedMMSize, mmSize);

		}

		TEST_METHOD(deleteWithRecursive){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			//Complete with unused option
			string commandString = "/d Test -r this is redundant string";
			multimap<string, any> test = parser.parseCommand(commandString);

			bool valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);

			string Param = any_cast<string> ( test.find("param")->second );
			string correctParam = "Test";
			Assert::AreEqual(correctParam, Param);

			bool recursive = any_cast<bool> ( test.find("recursive")->second );
			bool isRecursive = true;
			Assert::AreEqual(isRecursive, recursive);
			
			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

		}

		TEST_METHOD(tagField){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			//Add
			string commandString = "add meeting tomorrow -t starbucks";		
			multimap<string, any> test = parser.parseCommand(commandString);
			vector<string> tags = any_cast< vector<string> > ( test.find("tags")->second );
			size_t expectedSizeOfTag = 1;
			Assert::AreEqual(expectedSizeOfTag, tags.size());

			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

			//Add
			commandString = "add trip to Penang -t Highway, Penang, Malaysia";		
			test = parser.parseCommand(commandString);
			tags = any_cast< vector<string> > ( test.find("tags")->second );
			expectedSizeOfTag = 3;
			Assert::AreEqual(expectedSizeOfTag, tags.size());

			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

		}

		TEST_METHOD(duplicateTags){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			//Add
			string commandString = "add meeting tomorrow -t meeting,meeting,meeting,tomorrow,starbucks";
			multimap<string, any> test = parser.parseCommand(commandString);
			vector<string> tags = any_cast< vector<string> > ( test.find("tags")->second );
			size_t expectedSize = 3;
			Assert::AreEqual(expectedSize, tags.size());

			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

		}

		TEST_METHOD(linkField){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			//Add
			string commandString = "add yoga lesson -l G983";		
			multimap<string, any> test = parser.parseCommand(commandString);
			string link = any_cast<string> ( test.find("link")->second );
			string expectedLink = "G983";
			Assert::AreEqual(expectedLink, link);

			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

		}
		
		TEST_METHOD(extractOptionValueEndWithValue){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			string commandString = "add Meeting tomorrow -p 2 -l G983";
			multimap<string, any> test = parser.parseCommand(commandString);
			int num = any_cast<int> ( test.find("priority")->second );
			string ID = any_cast<string> ( test.find("link")->second );
			int two = 2;
			string correctID = "G983";

			Assert::AreEqual(two, num);
			Assert::AreEqual(correctID, ID);

			bool valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);

			mmSize = test.size();
			expectedMMSize = 5;
			Assert::AreEqual(expectedMMSize, mmSize);
			
		}

		TEST_METHOD(extractOptionValueWithRedundantValues){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			string commandString = "add Meeting tomorrow -p 2 6 -l G983";
			multimap<string, any> test = parser.parseCommand(commandString);
			int num = any_cast<int> ( test.find("priority")->second );
			string ID = any_cast<string> ( test.find("link")->second );
			int two = 2;
			string correctID = "G983";

			Assert::AreEqual(two, num);
			Assert::AreEqual(correctID, ID);

			bool valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);

			mmSize = test.size();
			expectedMMSize = 5;
			Assert::AreEqual(expectedMMSize, mmSize);
			
		}

		TEST_METHOD(extractOptionValueEndWithEmptyOption){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			string commandString = "add Meeting tomorrow -p 2 -l";			
			multimap<string, any> test = parser.parseCommand(commandString);
			int num = any_cast<int> ( test.find("priority")->second );
			int two = 2;
			Assert::AreEqual(two, num);

			int c = test.count("link");
			Assert::AreEqual(0, c);

			bool valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);

			mmSize = test.size();
			expectedMMSize = 4;
			Assert::AreEqual(expectedMMSize, mmSize);

			commandString = "add Meeting tomorrow -p -l";
			test = parser.parseCommand(commandString);
			string param = any_cast<string> ( test.find("param")->second );
			string expect = "Meeting tomorrow";
			Assert::AreEqual(expect, param);

			c = test.count("link");
			Assert::AreEqual(0, c);

			c = test.count("priority");
			Assert::AreEqual(0, c);

			valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);

			mmSize = test.size();
			expectedMMSize = 3;
			Assert::AreEqual(expectedMMSize, mmSize);
			
		}

		TEST_METHOD(inexistentOption){

			Parser parser;
			int mmSize;
			int expectedMMSize;

			string commandString = "add Meeting tomorrow -a 1 -v 2";		
			multimap<string, any> test = parser.parseCommand(commandString);
			string param = any_cast<string> ( test.find("param")->second );
			string expect = "Meeting tomorrow -a 1 -v 2";
			Assert::AreEqual(expect, param);

			bool valid = any_cast<bool> ( test.find("valid")->second );
			Assert::AreEqual(true, valid);
			
			mmSize = test.size();
			expectedMMSize = 3;
			Assert::AreEqual(expectedMMSize, mmSize);		
		}

	};
}