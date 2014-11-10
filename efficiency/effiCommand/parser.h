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

//@author A0098802X
class Parser{
public:

	// Constructors
	Parser();
	~Parser();

	// Accessors
	// and UI controller (to check valid and parse)
	multimap<string, any> parseCommand(const string commandString);
	// check if a command is empty
	bool isEmptyCommand(const string commandString);
	// parse a date
	static ptime parseDate(string s);
	// trim strings
	static string trim(const string& s, const string& delimiters = " \f\n\r\t\v" );

private:

	// Attributes

	// list of available option fields
	vector< std::tuple<string, string, bool> > optionFieldsChecker;
	// list of available command keywords
	vector< pair<string, commandTypeEnum::COMMAND_TYPE> > validCommandKeywords;
	// list of view lists available to scroll and sort (deadline, event, task)
	vector< string > viewToScrollAndSort;
	// scrolling direction of list
	vector< string > scrollDirection;
	// types available for sorting
	vector< string > sortByType;
	// lists of all supported time and date formats for start and end option fields
	static const locale inputFormats[];

	// initiators for the lists, used in constructor
	void loadValidCommandKeywords();
	void loadOptionFieldsChecker();
	void loadViewToScrollAndSort();
	void loadScrollDirection();
	void loadSortByType();

	// to check the entire user input
	multimap<string, any> checkCommandSyntax(vector<string> commandStringTokens);
	// to process all the existing option fields in a user input
	multimap<string, any> extractOptionsAndValues(commandTypeEnum::COMMAND_TYPE cmdType, 
												  multimap<string, any> cmdParamAndOptMap, 
												  vector<string> commandStringTokens, 
												  int fieldPos, 
												  std::tuple<string, string, bool> currentOptionFieldPair,
												  bool validUntilNow);

	// remove all duplicate tags
	void checkForDuplicateTags(vector<string> &fieldValueVector);
	// check the date time format if start and end option fields are used
	pair<bool, ptime> checkDateTime(string dtFieldValue, bool firstRun);
	// standardize all time format to have :00 seconds
	string addSeconds(string time);

	// convert the command string to a string vector
	static vector<string> tokenizeCommandString(string userCommand, bool forTagComma);
	// join tokenized command string to a string
	static string joinVector(const vector<string>& commandVector, const string& token);

	// check if two strings are equal, ignoring case sensitivity
	static bool areEqualStringsIgnoreCase(const string& s, const string& delimiters = " \f\n\r\t\v");

	// trim strings
	static string trimRight(const string& s, const string& delimiters = " \f\n\r\t\v" );
	static string trimLeft(const string& s, const string& delimiters = " \f\n\r\t\v" );

	// check for a suffix
	bool hasSuffix(string str, string suffix);

	// add padding zeros for day or month entered with a single digit by user
	static string addPaddingZeros(string s);

	// conditional refactoring

	// for time and date parsing
	bool isEnteredDateAndTime(vector<string> &dtToken);
	bool isPM(vector<string> &timeToken);
	bool isExistSecond(vector<string> &timeToken);
	bool isShortFormTimeFormat(vector<string> &timeToken);
	bool isLongTimeFormat(vector<string> &timeToken);

	// for the command and option field parsing
	bool isNotFoundOptionField(unsigned int i, unsigned int j, vector<string> &commandStringTokens);
	bool isRecursiveField(unsigned int j);
	bool isMissingCommandParameters(unsigned int i, unsigned int j, vector<string> commandStringTokens);
	bool isInvalidCommandKeyword(unsigned int i);
	bool isInsufficientParameters(vector<string> &commandStringTokens);	

};
#endif