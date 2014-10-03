#include "command.h"
//#include "Undo.h"

using namespace std;

void Command::promptCommand(){

	while(true){

		string userCommand;
		getline(cin, userCommand);

		string feedback = executeCommand(userCommand);

		// parse to controller?

	}

}

Command::COMMAND_TYPE Command::parseCommandType(const string command){

	if(areEqualStringsIgnoreCase(command, "add")){
		return ADD_TASK;
	} else if(areEqualStringsIgnoreCase(command, "delete")) {
		return DELETE_TASK;
	} else if(areEqualStringsIgnoreCase(command, "exit")) {
		return EXIT;
	} else {
		return INVALID;
	}

}

string Command::executeCommand(string userCommand){

	if( userCommand == "") {
		return "no command entered";
	}

	COMMAND_TYPE processedCommandType = parseCommandType(userCommand);

	vector<string> commandStringVector = convertToStringVector(userCommand);

	switch(processedCommandType){
		case ADD_TASK:
			return addTask(commandStringVector);
		case DELETE_TASK:
			return deleteTask(commandStringVector);
		case INVALID:
			return "Invalid command!";
		case EXIT:
			exit(0);
		default:
			throw new exception("Command execution error!");
	}

}

// This function converts user command strings into a string vector
vector<string> Command::convertToStringVector(string userCommand){

	vector<string> stringTokens;
	istringstream iss(userCommand);
    copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter< vector<string> > (stringTokens) );

	return stringTokens;

}

string Command::addTask(vector<string> commandStringVector){

	string taskAdded = getTaskDetailsToAdd(commandStringVector);
	
	// Call Sherry's methods

	string inverseCommand = Undo::matchInverseFunction(commandStringVector);

	return "task added";

}

string Command::getTaskDetailsToAdd(vector<string> commandString){

	string text = "";

	unsigned int lastIndex = commandString.size() - 1;

	for(unsigned int i = 1; i < commandString.size(); i++){

		text = text + commandString[i];	

		if (i != lastIndex){
			text = text + " ";
		}

	}

	return text;

}

string Command::deleteTask(vector<string> commandStringVector){

	// call Sherry's API

	return "task deleted";

}

bool Command::areEqualStringsIgnoreCase(const string& stringOne, const string& stringTwo) {

    if (stringOne.size() != stringTwo.size()) {
        return false;
    }
	
	for (string::const_iterator charOne = stringOne.begin(), charTwo = stringTwo.begin(); 
		charOne != stringOne.end(); 
		++charOne, ++charTwo) {

        if (tolower(*charOne) != tolower(*charTwo)) {
            return false;
        }

    }

    return true;

}