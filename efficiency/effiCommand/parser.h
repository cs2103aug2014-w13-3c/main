#ifndef PARSER_H_
#define PARSER_H_

#include <numeric>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>
#include <tuple>
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
	// and UI controller (to check valid and parse)
	multimap<string, any> parseCommand(const string commandString);

	static ptime parseDate(string s);
	static string trim(const string& s, const string& delimiters = " \f\n\r\t\v" );

private:

	// Attributes
	vector< std::tuple<string, string, bool> > optionFieldsChecker;
	vector< pair<string, commandTypeEnum::COMMAND_TYPE> > validCommandKeywords;
	vector< string > viewToScrollAndSort;
	vector< string > scrollDirection;
	vector< string > sortByType;

	// Functions
	void loadValidCommandKeywords();
	void loadOptionFieldsChecker();
	void loadViewToScrollAndSort();
	void loadScrollDirection();
	void loadSortByType();

	multimap<string, any> checkCommandSyntax(vector<string> commandStringTokens);
	multimap<string, any> extractOptionsAndValues(commandTypeEnum::COMMAND_TYPE cmdType, 
												  multimap<string, any> cmdParamAndOptMap, 
												  vector<string> commandStringTokens, 
												  int fieldPos, 
												  std::tuple<string, string, bool> currentOptionFieldPair,
												  bool validUntilNow);
	pair<bool, ptime> checkDateTime(string dtFieldValue, bool firstRun);
	string addSeconds(string time);

	// convert the command string to a string vector
	static vector<string> tokenizeCommandString(string userCommand, bool forTagComma);
	static string joinVector(const vector<string>& commandVector, const string& token);

	// check if two strings are equal, ignoring case sensitivity
	static bool areEqualStringsIgnoreCase(const string& s, const string& delimiters = " \f\n\r\t\v");

	// trim strings
	static string trimRight(const string& s, const string& delimiters = " \f\n\r\t\v" );
	static string trimLeft(const string& s, const string& delimiters = " \f\n\r\t\v" );

	bool hasSuffix(string str, string suffix);

	static const locale inputFormats[];
	static string addPaddingZeros(string s);

};
#endif