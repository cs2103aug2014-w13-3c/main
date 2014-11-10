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

	// check if a command is empty
	bool isEmptyCommand(const string commandString);

	// check the date time format if start and end option fields are used
	pair<bool, ptime> checkDateTime(string dtFieldValue, bool firstRun);
	// add padding zeros for day or month entered with a single digit by user
	static string addPaddingZeros(string s);

	// to check the entire user input
	multimap<string, any> checkCommandSyntax(vector<string> commandStringTokens);
	// for commands with task names as parameters, extract it
	multimap<string, any> extractParamOnly(vector<string> commandStringTokens, multimap<string,any> cmdParamAndOptMap);
	// check the command keyword
	pair< multimap<string,any>, commandTypeEnum::COMMAND_TYPE> checkCommandKeyword(vector<string> commandStringTokens, 
		multimap<string,any> cmdParamAndOptMap, 
		commandTypeEnum::COMMAND_TYPE cmdType);

	// process all command types with param and options
	multimap<string, any> processCommandsWithParamAndOptions(multimap<string,any> cmdParamAndOptMap, 
															 vector<string> commandStringTokens, 
															 bool noOptionsInUserInput, 
															 commandTypeEnum::COMMAND_TYPE cmdType);
	// process all command types with param and only supports recursive option field
	multimap<string, any> processCommandsSupportingRecursiveOnly(vector<string> commandStringTokens, 
																multimap<string,any> cmdParamAndOptMap, 
																commandTypeEnum::COMMAND_TYPE cmdType);
	// to process commands using predicates (search and filter)
	multimap<string, any> processCommandsWithPrecidates(vector<string> commandStringTokens, commandTypeEnum::COMMAND_TYPE cmdType);
	// to process commands using 2 string parameters
	multimap<string, any> processCommandsWithTwoStringParam(vector<string> commandStringTokens, 
															multimap<string,any> cmdParamAndOptMap, 
															commandTypeEnum::COMMAND_TYPE cmdType);

	// check the rules of option fields to see if they are restricted to some commands only
	void checkRecursiveOptionConstraints(unsigned int j, 
		multimap<string,any> cmdParamAndOptMap, 
		string Param, 
		commandTypeEnum::COMMAND_TYPE cmdType);	

	// check first param which is event type
	multimap<string, any> checkEventTypeParam(vector<string> commandStringTokens, multimap<string,any> cmdParamAndOptMap);
	// check second param which is after the event type (first param)
	multimap<string, any> checkSpecificCommandParam(commandTypeEnum::COMMAND_TYPE cmdType, 
		vector<string> commandStringTokens, 
		multimap<string,any> cmdParamAndOptMap);

	// process all option fields
	pair<multimap<string,any>, bool> processAllOptionFields(vector<string> commandStringTokens, 
															multimap<string,any> cmdParamAndOptMap, 
															bool noOptionsInUserInput, 
															commandTypeEnum::COMMAND_TYPE cmdType);
	
	// recursively process all the existing option fields in a user input according to command restraints
	// a part of process
	multimap<string, any> extractOptionsAndValues(commandTypeEnum::COMMAND_TYPE cmdType, 
												  multimap<string, any> cmdParamAndOptMap, 
												  vector<string> commandStringTokens, 
												  int fieldPos, 
												  std::tuple<string, string, bool> currentOptionFieldPair,
												  bool validUntilNow);
	bool processOptionWithoutNeededValues(multimap<string, any> &cmdParamAndOptMap, 
		string mmOptionKey, 
		int fieldPos, 
		vector<string> &commandStringTokens, 
		bool validUntilNow, 
		commandTypeEnum::COMMAND_TYPE cmdType, 
		bool processComplete);
	// Identify and process option fields 
	void identifyAndProcessOption(string mmOptionKey, 
								  vector<string> &fieldValueVector, 
								  commandTypeEnum::COMMAND_TYPE cmdType, 
								  multimap<string, any> &cmdParamAndOptMap, 
								  bool &validUntilNow, 
								  any &fieldValue, 
								  vector<string> commandStringTokens, 
								  unsigned int i, 
								  unsigned int j);
	// remove all duplicate tags
	vector<string> checkForDuplicateTags(vector<string> fieldValueVector);

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

	// conditional refactoring

	// for time and date parsing
	bool isEnteredDateAndTime(vector<string> dtToken);
	bool isPM(vector<string> timeToken);
	bool isExistSecond(vector<string> timeToken);
	bool isShortFormTimeFormat(vector<string> timeToken);
	bool isLongTimeFormat(vector<string> timeToken);

	// for the command and option field parsing
	bool isNotFoundOptionField(unsigned int i, unsigned int j, vector<string> commandStringTokens);
	bool isValidRecursiveField(unsigned int j, commandTypeEnum::COMMAND_TYPE cmdType);
	bool isMissingCommandParameters(unsigned int i, unsigned int j, vector<string> commandStringTokens);
	bool isInvalidCommandKeyword(unsigned int i);
	bool isInsufficientParameters(vector<string> commandStringTokens);	
	// terminate parser if valid key is false
	bool isFalseValidKey(multimap<string,any> cmdParamAndOptMap);

};
#endif