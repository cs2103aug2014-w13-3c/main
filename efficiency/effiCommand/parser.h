#ifndef PARSER_H_
#define PARSER_H_

#include <numeric>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>
#include <map>

#include "commandTypeEnum.h"
#include "optionField.h"

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/any.hpp"
using namespace boost;
using namespace boost::posix_time;
using namespace std;

class Parser{
public:

	Parser(const string commandString);
	~Parser();

	// Accessors
	// For executor (to execute) 
	multimap<string, any> getCommandContents();
	// and UI controller (to check valid) only
	bool getValid();

protected:

	multimap<string, any> cmdResult;

private:

	// Attributes
	vector<pair<string, bool>> optionFieldsChecker;
	vector<pair<string, CommandTypeEnum::COMMAND_TYPE>> validCommandKeywords;

	// Functions

	// parse the Command
	// Called by constructor
	multimap<string, any> parseCommand(const string commandString);

	void loadValidCommandKeywords();
	void loadOptionFieldsChecker();

	multimap<string, any> checkCommandSyntax(vector<string> commandStringTokens);

	// convert the command string to a string vector
	static vector<string> tokenizeCommandString(string userCommand);
	static string joinVector(const vector<string>& commandVector);

	// check if two strings are equal, ignoring case sensitivity
	static bool areEqualStringsIgnoreCase(const string& s, const string& delimiters = " \f\n\r\t\v" );

	// trim strings
	static string trimRight(const string& s, const string& delimiters = " \f\n\r\t\v" );
	static string trimLeft(const string& s, const string& delimiters = " \f\n\r\t\v" );
	static string trim(const string& s, const string& delimiters = " \f\n\r\t\v" );

};
#endif