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
		ADD_TASK, DELETE_TASK, EXIT, INVALID
	};

private:													
	
public:

	// Command input prompt
	static void promptCommand();

	/*
	* autocomplete function here soon
	*/

	// convert command keyword into enumerator
	static COMMAND_TYPE parseCommandType(const string commandString);

	// execute a given command string
	static string executeCommand(string userCommand);

	// convert the command string to a string vector
	static vector<string> convertToStringVector(string userCommand);
	
	static string addTask(vector<string> commandStringVector);
	static string deleteTask(vector<string> commandStringVector);



	// extract line of text to be added from command string 
	static string getTaskDetailsToAdd(vector<string> commandString);

	// check if two strings are equal, ignoring case sensitivity
	static bool areEqualStringsIgnoreCase(const string& s, const string& delimiters = " \f\n\r\t\v" );

};
#endif