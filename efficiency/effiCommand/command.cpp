#include "command.h"
#include "Undo.h"

using namespace std;

Command::Command(){

	this->executedCommand = INVALID;

}

Command::Command(COMMAND_TYPE executedCommand, vector<string> parameters){

	this->executedCommand = executedCommand;
	this->parameters = parameters;

}

Command::COMMAND_TYPE Command::getCommandType(){

	return Command::executedCommand;

}

vector<string> Command::getParameters(){

	return Command::parameters;

}

void Command::promptCommand(){

	while(true){

		string userCommand;
		getline(cin, userCommand);

		string feedback = executeCommand(userCommand);

		// parse to controller?

	}

}

void Command::promptCommand(){

	while(true){

		string userCommand;
		getline(cin, userCommand);

		string feedback = executeCommand(userCommand);

		// parse to controller?

	}

}

Command Command::parseCommandType(const string command){

	vector<string> parameters = convertToStringVector(command);
	Command parsedCommand;

	if(areEqualStringsIgnoreCase(command, "add")){
		parsedCommand = Command(ADD_TASK, parameters);
	} else if(areEqualStringsIgnoreCase(command, "delete")) {
		parsedCommand = Command(ADD_TASK, parameters);
	} else if(areEqualStringsIgnoreCase(command, "exit")) {		
		parsedCommand = Command(EXIT, parameters);
	} else if(command.empty) {
		parsedCommand = Command(EMPTY, parameters);
	} else {
		parsedCommand = Command(INVALID, parameters);
	}

	return parsedCommand;

}

string Command::executeCommand(string userCommand){

	Command processedCommand = parseCommandType(userCommand);

	switch(processedCommand.getCommandType()){
		case ADD_TASK:
			return addTask(processedCommand.getParameters());
		case DELETE_TASK:
			return deleteTask(processedCommand.getParameters());
		case EMPTY:
			return "Nothing is entered!";
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