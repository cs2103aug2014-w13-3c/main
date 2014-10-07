#ifndef COMMAND_H_
#define COMMAND_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <QtWidgets/QMainWindow>


using namespace std;

class Command{
public:

	// these are the possible command types
	enum COMMAND_TYPE {
		ADD_TASK, DELETE_TASK, EXIT, EMPTY, INVALID
	};

	static string validCommandKeywords[];
	static COMMAND_TYPE matchingCommandEnumerators[];
	static string validFields[];
	static bool hasFieldValues[];

	static int fieldsToIdxMapping[];
	static const int NUM_OF_PARAM_OPTION_FIELDS = 8;
	static string paramOptionFields[NUM_OF_PARAM_OPTION_FIELDS];

	Command();
	Command(COMMAND_TYPE, vector<string>);
	~Command();

	COMMAND_TYPE getCommandType();
	vector<string> getParameters();

	// Command input prompt
	static void promptCommand();

	/*
	* autocomplete function here soon
	*/

	// convert command keyword into enumerator
	static Command parseCommand(const string commandString);

	static COMMAND_TYPE determineCommandType(const string commandKeyword);

	static Command isValidParameters(const COMMAND_TYPE commandTypeEnum, vector<string> parameters);

	// execute a given command string
	static string executeCommand(string userCommand);

	// convert the command string to a string vector
	static vector<string> tokenizeCommandString(string userCommand);
	
	static string addTask(vector<string> commandStringVector);
	static string deleteTask(vector<string> commandStringVector);

	// extract line of text to be added from command string 
	static string getTaskDetailsToAdd(vector<string> commandString);

	// check if two strings are equal, ignoring case sensitivity
	static bool areEqualStringsIgnoreCase(const string& s, const string& delimiters = " \f\n\r\t\v" );

	// trim strings
	static string trimRight(const string& s, const string& delimiters = " \f\n\r\t\v" );
	static string trimLeft(const string& s, const string& delimiters = " \f\n\r\t\v" );
	static string trim(const string& s, const string& delimiters = " \f\n\r\t\v" );

private:

	COMMAND_TYPE executingCommand;
	vector<string> parameters;

};
#endif