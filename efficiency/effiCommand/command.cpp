#include "command.h"
#include "Undo.h"

using namespace std;

string Command::validCommandKeywords[] = {	"/a", "add",
											"/d", "delete"
											"exit"};

Command::COMMAND_TYPE Command::matchingCommandEnumerators[] = {	Command::ADD_TASK, Command::ADD_TASK,
																Command::DELETE_TASK, Command::DELETE_TASK,
																Command::EXIT};

string Command::validFields[] = { "-s", "-start",
								  "-e", "-end",
								  "-p", "-priority",
								  "-r", "-repeat", "-recursive", // -r can be used as delete recursive or repeat
								  "-t", "-tag",
								  "-l", "-link",
								  "-h", "-help"};

bool Command::hasFieldValues[] = { true, true,
								  true, true,
								  true, true,
								  true, true, false, // -r can be used as delete recursive or repeat
								  true, true,
								  true, true,
								  false, false};

int Command::fieldsToIdxMapping[] = { 1, 1,
								  2, 2,
								  3, 3,
								  4, 4, 4, // -r can be used as delete recursive or repeat
								  5, 5,
								  6, 6,
								  7, 7};

string Command::paramOptionFields[];

Command::Command(){

	this->executingCommand = INVALID;

}

Command::Command(COMMAND_TYPE executedCommand, vector<string> parameters){

	this->executingCommand = executedCommand;
	this->parameters = parameters;

}

Command::COMMAND_TYPE Command::getCommandType(){

	return Command::executingCommand;

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

string Command::executeCommand(string userCommand){

	Command processedCommand = parseCommand(userCommand);

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

Command Command::parseCommand(const string commandString){

	string Command::paramOptionFields[Command::NUM_OF_PARAM_OPTION_FIELDS];

	vector<string> parameters;

	if( trim(commandString) == "" ){
		return Command(EMPTY, parameters);
	}

	vector<string> commandStringTokens = tokenizeCommandString(commandString);

	string commandKeyword = commandStringTokens.front;

	parameters = copy(commandStringTokens.begin() + 1, commandStringTokens.end(), parameters);

	COMMAND_TYPE commandTypeEnum = determineCommandType(commandKeyword);

	if(commandTypeEnum != INVALID){
		return isValidParameters(commandTypeEnum, parameters);
	}

	return Command(commandTypeEnum, parameters);

}

// This function converts user command strings into a string vector
vector<string> Command::tokenizeCommandString(string userCommand){

	vector<string> stringTokens;
	istringstream iss(userCommand);
    copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter< vector<string> > (stringTokens) );

	return stringTokens;

}

Command::COMMAND_TYPE Command::determineCommandType(const string commandKeyword){

	for(unsigned int i = 0; i <= sizeof(Command::validCommandKeywords); i++){

		if( areEqualStringsIgnoreCase(commandKeyword, Command::validCommandKeywords[i]) ){

			return Command::matchingCommandEnumerators[i];

		}

	}

	return INVALID;

}

Command Command::isValidParameters(const COMMAND_TYPE commandTypeEnum, vector<string> parameters){

	switch (commandTypeEnum){

	// has mandatory parameter and optional options
	case ADD_TASK:
	case DELETE_TASK:

		if(parameters.size <= 0){
			return Command(INVALID, parameters);
		}

		bool noFields = false;

		bool firstField = false;

		for(unsigned int i = 0; i <= sizeof(parameters); i++){

			for(unsigned int j = 0; j <= sizeof(validFields); j++){

				if( areEqualStringsIgnoreCase(parameters[i], validFields[j]) ){

					// if mandatory param does not exist
					if(i = 0){
						return Command(INVALID, parameters);
					} else {
						
						if(!firstField){

							firstField = true;

							string paramToAdd = ""; 

							for(unsigned int k = 0; k <= i; k++){

								paramToAdd = paramToAdd + parameters[k];

							}

							paramOptionFields[0] = paramToAdd;

						}

					}

					// check if the option has values
					if(hasFieldValues[j]){
						paramOptionFields[fieldsToIdxMapping[j]] = parameters[i+1];
					}

				}

				if(!firstField && i == sizeof(parameters) && j == sizeof(validFields)){

					noFields = true;

				}

			}

		}

		if(noFields){

			string paramToAdd = ""; 

			for(unsigned int i = 0; i <= sizeof(parameters); i++){

				paramToAdd = paramToAdd + parameters[i];

			}

		}

	// has only param (filter and search)

	// has no param and options
	case EXIT:
		return Command(commandTypeEnum, parameters);
	}

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

inline string Command::trimRight(const string& s, const string& delimiters)
{
	return s.substr( 0, s.find_last_not_of( delimiters ) + 1 );
}

inline string Command::trimLeft(const string& s, const string& delimiters)
{
	return s.substr( s.find_first_not_of( delimiters ) );
}

inline string Command::trim(const string& s, const string& delimiters)
{

	if (!s.empty()){
		return trimLeft(trimRight(s, delimiters), delimiters);		
	}

	return s;

}