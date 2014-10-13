#include "command.h"
#include "Undo.h"

using namespace std;

const string Command::COMMAND = "cmdType";
const string Command::PARAMETERS = "param";
const string Command::START_OPTION = "-s";
const string Command::END_OPTION = "-e";
const string Command::PRIORITY_OPTION = "-p";
const string Command::RECURSIVE_OPTION = "-r";
const string Command::TAG_OPTION = "-t";
const string Command::LINK_OPTION = "-l";
const string Command::HELP_OPTION = "-h";

vector< pair<string, Command::COMMAND_TYPE> > Command::validCommandKeywords;
pair<string, bool> Command::categorizeMap[9];

void Command::loadValidCommandKeywords(){

	Command::validCommandKeywords.push_back(make_pair("add",Command::ADD_TASK));
	Command::validCommandKeywords.push_back(make_pair("/a",Command::ADD_TASK));
	Command::validCommandKeywords.push_back(make_pair("delete",Command::DELETE_TASK));
	Command::validCommandKeywords.push_back(make_pair("/d",Command::DELETE_TASK));
	Command::validCommandKeywords.push_back(make_pair("exit",Command::EXIT));

}

void Command::loadCategorizeMap(){

	Command::categorizeMap[0] = make_pair(COMMAND,false);
	Command::categorizeMap[1] = make_pair(PARAMETERS,true);
	Command::categorizeMap[2] = make_pair(START_OPTION,true);
	Command::categorizeMap[3] = make_pair(END_OPTION,true);
	Command::categorizeMap[4] = make_pair(PRIORITY_OPTION,true);
	Command::categorizeMap[5] = make_pair(RECURSIVE_OPTION,true);
	Command::categorizeMap[6] = make_pair(TAG_OPTION,true);
	Command::categorizeMap[7] = make_pair(LINK_OPTION,true);
	Command::categorizeMap[8] = make_pair(HELP_OPTION,false);

}

Command::Command(){

	this->cmdParamAndOpt.insert( pair<string, any>(COMMAND, INVALID) );

}

Command::Command( multimap<string, any> cmdParamAndOpt){

	multimap<string, any>::iterator it = this->cmdParamAndOpt.begin();
	while(it != cmdParamAndOpt.end()) {
		this->cmdParamAndOpt.insert( pair<string,any>(it->first, it->second) );
		it++;
	}

}

Command::COMMAND_TYPE Command::getCommand(){

	return any_cast<Command::COMMAND_TYPE>(Command::cmdParamAndOpt.find(COMMAND)->second);

}

string Command::getParameters(){

	return any_cast<string>(Command::cmdParamAndOpt.find(PARAMETERS)->second);

}

ptime Command::getStartOption(){

	return any_cast<ptime>(Command::cmdParamAndOpt.find(START_OPTION)->second);
}

ptime Command::getEndOption(){

	return any_cast<ptime>(Command::cmdParamAndOpt.find(END_OPTION)->second);

}

int Command::getPriorityOption(){

	return any_cast<int>(Command::cmdParamAndOpt.find(PRIORITY_OPTION)->second);

}

string Command::isRecursiveOption(){

	return any_cast<string>(Command::cmdParamAndOpt.find(RECURSIVE_OPTION)->second);

}

vector<string> Command::getTagOption(){

	return any_cast<vector<string>>(Command::cmdParamAndOpt.find(TAG_OPTION)->second);

}

string Command::getLinkOption(){

	return any_cast<string>(Command::cmdParamAndOpt.find(LINK_OPTION)->second);

}

bool Command::getHelpOption(){

	return any_cast<bool>(Command::cmdParamAndOpt.find(HELP_OPTION)->second);

}

void Command::promptCommand(){

	loadValidCommandKeywords();

	while(true){

		string userCommand;

		getline(cin, userCommand);

		string feedback = executeCommand(userCommand);

		// parse to controller?

	}

}

string Command::executeCommand(string userCommand){

	Command processedCommand = parseCommand(userCommand);

	switch(processedCommand.getCommand()){
		case ADD_TASK:
			return addTask();
		case DELETE_TASK:
			return deleteTask();
		case EXIT:
			exit(0);
		default:
			throw "Command execution error!";
	}

}

Command Command::parseCommand(const string commandString){

	multimap<string,any> cmdParamAndOpt;

	if( trim(commandString) == "" ){
		throw "No command found.";
	}

	vector<string> commandStringTokens = tokenizeCommandString(commandString);

	return checkCommandSyntax(commandStringTokens);

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

// Check the entire command syntax
Command Command::checkCommandSyntax(vector<string> commandStringTokens){

	COMMAND_TYPE commandEnum = determineCommandType(commandStringTokens);

	vector<string> parameters = copy(commandStringTokens.begin + 1, commandStringTokens.end, parameters);

	return checkParamAndFields(commandEnum, parameters);

}

// Check if a command exist, and if it has mandatory param (where applicable)
Command::COMMAND_TYPE Command::determineCommandType(vector<string> commandStringTokens){

	COMMAND_TYPE determinedCommandEnum;

	for(unsigned int i = 0; i <= sizeof(Command::validCommandKeywords); i++){

		if( areEqualStringsIgnoreCase(commandStringTokens[0], validCommandKeywords[i].first) ){
			determinedCommandEnum = Command::validCommandKeywords[i].second;
		}

	}

	switch(determinedCommandEnum){
	case ADD_TASK:
	case DELETE_TASK:
		break;
	case EXIT:
		break;
	default:
		throw "no such command found.";
	}

	return determinedCommandEnum;

}

// process the param and option fields. 
// Note to self:
// invalid fields are thrown an exception
// option fields with missing values are ignored rather than thrown an exception
// by this stage, tasks that need param will definitely have one
Command Command::checkParamAndFields(const COMMAND_TYPE commandTypeEnum, vector<string> parameters){

	loadCategorizeMap();

	multimap<string,any> cmdParamAndOptMap;

	switch (commandTypeEnum){

	// has mandatory parameter and optional options
	case ADD_TASK:
	case DELETE_TASK:

		if( areEqualStringsIgnoreCase(parameters[0], categorizeMap[0].first ) ) {

			throw "No comand parameter found";

		} else {

			bool isFirstOption = true;
			bool hasNoOptions = true;

			for(unsigned int i = 0; i <= sizeof(parameters); i++){

				for(unsigned int j = 0; j <= sizeof(categorizeMap); j++){

					if( areEqualStringsIgnoreCase(parameters[i], categorizeMap[j].first ) ){

						if (isFirstOption){

							isFirstOption = false;
							hasNoOptions = false;
							vector<string> extractParam = copy(parameters.begin, parameters.begin + j, extractParam);

						}



					}

				}

			}

			if(hasNoOptions){
			 
				string extractParam = copy(parameters.begin, parameters.end, extractParam).str();
				cmdParamAndOptMap.insert( pair<string, any>(COMMAND, commandTypeEnum) );
				cmdParamAndOptMap.insert( pair<string, any>(PARAMETERS, extractParam) );
				return cmdParamAndOptMap;

			}

		}

	// has only param (complete)

	// supports multiple commands (search and filter)

	// has no param and options
	case EXIT:
		cmdParamAndOptMap.insert( pair<string, any>(COMMAND, commandTypeEnum) );
		return Command(cmdParamAndOptMap);
	}

}

string Command::addTask(){

	return "task added";

}

string Command::deleteTask(){

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