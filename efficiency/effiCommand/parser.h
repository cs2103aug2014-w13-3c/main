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

	Parser();
	~Parser();

	// Accessors
	// For executor (to execute) 
	multimap<string, any> getCommandContents();
	// and UI controller (to check valid and parse)
	multimap<string, any> parseCommand(const string commandString);
	bool getValid();

protected:

	multimap<string, any> cmdResult;

public:

	// Attributes
	vector<pair<string, bool>> optionFieldsChecker;
	vector<pair<string, CommandTypeEnum::COMMAND_TYPE>> validCommandKeywords;

	// Functions
	void loadValidCommandKeywords();
	void loadOptionFieldsChecker();

	multimap<string, any> checkCommandSyntax(vector<string> commandStringTokens);
	tuple<string, any, int> extractOptionValue(vector<string> commandStringTokens, int fieldPos, pair<string, bool> currentOptionFieldPair);

	// convert the command string to a string vector
	static vector<string> tokenizeCommandString(string userCommand);
	static string joinVector(const vector<string>& commandVector, const string& token);

	// check if two strings are equal, ignoring case sensitivity
	static bool areEqualStringsIgnoreCase(const string& s, const string& delimiters = " \f\n\r\t\v");

	// trim strings
	static string trimRight(const string& s, const string& delimiters = " \f\n\r\t\v" );
	static string trimLeft(const string& s, const string& delimiters = " \f\n\r\t\v" );
	static string trim(const string& s, const string& delimiters = " \f\n\r\t\v" );

};
#endif