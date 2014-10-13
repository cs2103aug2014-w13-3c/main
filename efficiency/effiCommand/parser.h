#ifndef PARSER_H_
#define PARSER_H_

#include <numeric>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>
#include <map>

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/any.hpp"
using namespace boost;
using namespace boost::posix_time;
using namespace std;

class Parser{
public:

	// convert command keyword into enumerator
	// NOTE: External API TO BE CALLED BY UI CONTROLLER!
	static multimap<string, any> parseCommand(const string commandString);

private:

	static vector<pair<string, bool>> optionFieldsChecker;
	static vector<pair<string, CommandTypeEnum::COMMAND_TYPE>> validCommandKeywords;

	static void loadValidCommandKeywords();
	static void loadOptionFieldsChecker();

	static multimap<string, any> checkCommandSyntax(vector<string> commandStringTokens);
	static CommandTypeEnum::COMMAND_TYPE determineCommandType(vector<string> commandStringTokens);
	static multimap<string, any> checkParamAndFields(const CommandTypeEnum::COMMAND_TYPE commandTypeEnum, vector<string> parameters);

	// convert the command string to a string vector
	static vector<string> tokenizeCommandString(string userCommand);

	// check if two strings are equal, ignoring case sensitivity
	static bool areEqualStringsIgnoreCase(const string& s, const string& delimiters = " \f\n\r\t\v" );

	// trim strings
	static string trimRight(const string& s, const string& delimiters = " \f\n\r\t\v" );
	static string trimLeft(const string& s, const string& delimiters = " \f\n\r\t\v" );
	static string trim(const string& s, const string& delimiters = " \f\n\r\t\v" );

};
#endif