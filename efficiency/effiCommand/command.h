#ifndef COMMAND_H_
#define COMMAND_H_

#include <iostream>
#include <sstream>
#include <functional>
#include <string>
#include <vector>
#include <map>

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/any.hpp"
using namespace boost;
using namespace boost::posix_time;
using namespace std;

class Command{
public:

	// these are the possible command types
	enum COMMAND_TYPE {
		ADD_TASK, DELETE_TASK, EXIT, EMPTY, INVALID
	};

	static const string COMMAND;
	static const string PARAMETERS;
	static const string START_OPTION;
	static const string END_OPTION;
	static const string PRIORITY_OPTION;
	static const string RECURSIVE_OPTION;
	static const string TAG_OPTION;
	static const string LINK_OPTION;
	static const string HELP_OPTION;

	Command();
	Command(multimap<string, any> cmdParamAndOpt);
	~Command();

	//API
	COMMAND_TYPE getCommand();
	string getParameters();
	ptime getStartOption();
	ptime getEndOption();
	int getPriorityOption();
	string isRecursiveOption();
	vector<string> getTagOption();
	string getLinkOption();
	bool getHelpOption();

	static void loadValidCommandKeywords();
	static void loadCategorizeMap();

	// Command input prompt
	static void promptCommand();

	/*
	* autocomplete function here soon
	*/

	// convert command keyword into enumerator
	static Command parseCommand(const string commandString);

	static Command checkCommandSyntax(vector<string> commandStringTokens);
	static COMMAND_TYPE determineCommandType(vector<string> commandStringTokens);
	static Command checkParamAndFields(const COMMAND_TYPE commandTypeEnum, vector<string> parameters);

	static Command isValidParameters(const COMMAND_TYPE commandTypeEnum, vector<string> parameters);

	// execute a given command string
	static string executeCommand(string userCommand);

	// convert the command string to a string vector
	static vector<string> tokenizeCommandString(string userCommand);
	
	static string addTask();
	static string deleteTask();

	// check if two strings are equal, ignoring case sensitivity
	static bool areEqualStringsIgnoreCase(const string& s, const string& delimiters = " \f\n\r\t\v" );

	// trim strings
	static string trimRight(const string& s, const string& delimiters = " \f\n\r\t\v" );
	static string trimLeft(const string& s, const string& delimiters = " \f\n\r\t\v" );
	static string trim(const string& s, const string& delimiters = " \f\n\r\t\v" );

private:

	multimap<string, any> cmdParamAndOpt;
	static pair<string, bool> categorizeMap[];
	static vector<pair<string, COMMAND_TYPE>> validCommandKeywords;

};
#endif