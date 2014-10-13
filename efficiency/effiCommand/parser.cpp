#include "commandTypeEnum.h"
#include "optionField.h"
#include "parser.h"
#include "executor.h"
#include "Undo.h"

using namespace std;

vector< pair<string, CommandTypeEnum::COMMAND_TYPE> > Parser::validCommandKeywords;
vector< pair<string, bool> > Parser::optionFieldsChecker;

void Parser::loadValidCommandKeywords(){

	Parser::validCommandKeywords.push_back(make_pair("add",CommandTypeEnum::ADD_TASK));
	Parser::validCommandKeywords.push_back(make_pair("/a",CommandTypeEnum::ADD_TASK));
	Parser::validCommandKeywords.push_back(make_pair("delete",CommandTypeEnum::DELETE_TASK));
	Parser::validCommandKeywords.push_back(make_pair("/d",CommandTypeEnum::DELETE_TASK));
	Parser::validCommandKeywords.push_back(make_pair("exit",CommandTypeEnum::EXIT));

}

void Parser::loadOptionFieldsChecker(){

	Parser::optionFieldsChecker.push_back(make_pair(optionField::COMMAND,false));
	Parser::optionFieldsChecker.push_back(make_pair(optionField::PARAMETERS,true));
	Parser::optionFieldsChecker.push_back(make_pair(optionField::START_OPTION,true));
	Parser::optionFieldsChecker.push_back(make_pair(optionField::END_OPTION,true));
	Parser::optionFieldsChecker.push_back(make_pair(optionField::PRIORITY_OPTION,true));
	Parser::optionFieldsChecker.push_back(make_pair(optionField::RECURSIVE_OPTION,true));
	Parser::optionFieldsChecker.push_back(make_pair(optionField::TAG_OPTION,true));
	Parser::optionFieldsChecker.push_back(make_pair(optionField::LINK_OPTION,true));
	Parser::optionFieldsChecker.push_back(make_pair(optionField::HELP_OPTION,false));

}

multimap<string, any> Parser::parseCommand(const string commandString){

	loadValidCommandKeywords();

	// TODO: refactor
	if( trim(commandString) == "" ){
		throw "No command found.";
	}

	vector<string> commandStringTokens = tokenizeCommandString(commandString);

	return checkCommandSyntax(commandStringTokens);

}

// This function converts user command strings into a string vector
vector<string> Parser::tokenizeCommandString(string userCommand){

	vector<string> stringTokens;
	istringstream iss(userCommand);
    copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter< vector<string> > (stringTokens) );

	return stringTokens;

}

// Check the entire command syntax
multimap<string, any> Parser::checkCommandSyntax(vector<string> commandStringTokens){

	CommandTypeEnum::COMMAND_TYPE commandEnum = determineCommandType(commandStringTokens);

	commandStringTokens.erase( commandStringTokens.begin() );

	return checkParamAndFields(commandEnum, commandStringTokens);

}

// Check if a command exist, and if it has mandatory param (where applicable)
CommandTypeEnum::COMMAND_TYPE Parser::determineCommandType(vector<string> commandStringTokens){

	CommandTypeEnum::COMMAND_TYPE determinedCommandEnum;

	for(unsigned int i = 0; i <= sizeof(Parser::validCommandKeywords); i++){

		if( areEqualStringsIgnoreCase(commandStringTokens[0], validCommandKeywords[i].first) ){
			determinedCommandEnum = Parser::validCommandKeywords[i].second;
		}

	}

	switch(determinedCommandEnum){
	case CommandTypeEnum::ADD_TASK:
	case CommandTypeEnum::DELETE_TASK:
		break;
	case CommandTypeEnum::EXIT:
		break;
	default:
		throw "no such command found.";
	}

	return determinedCommandEnum;

}

multimap<string, any> Parser::checkParamAndFields(const CommandTypeEnum::COMMAND_TYPE commandTypeEnum, vector<string> parameters){

	loadOptionFieldsChecker();

	multimap<string,any> cmdParamAndOptMap;

	switch (commandTypeEnum){

	// has mandatory parameter and optional options
	case CommandTypeEnum::ADD_TASK:
	case CommandTypeEnum::DELETE_TASK:

		if( areEqualStringsIgnoreCase(parameters[0], optionFieldsChecker[0].first ) ) {

			throw "No comand parameter found";

		} else {

			bool isFirstOption = true;
			bool hasNoOptions = true;

			for(unsigned int i = 0; i <= sizeof(parameters); i++){

				for(unsigned int j = 0; j <= sizeof(optionFieldsChecker); j++){

					if( areEqualStringsIgnoreCase(parameters[i], optionFieldsChecker[j].first ) ){

						if (isFirstOption){

							isFirstOption = false;
							hasNoOptions = false;
							vector<string> extractParam;
							copy(parameters.begin(), parameters.begin() + j, back_inserter(extractParam));

						}

						// continue!

					}

				}

			}

			if(hasNoOptions){
			 
				string extractParam = accumulate(parameters.begin(), parameters.end(), extractParam);
				cmdParamAndOptMap.insert( pair<string, any>(optionField::COMMAND, commandTypeEnum) );
				cmdParamAndOptMap.insert( pair<string, any>(optionField::PARAMETERS, extractParam) );
				return cmdParamAndOptMap;

			}

		}

	// has only param (complete)

	// supports multiple commands (search and filter)

	// has no param and options
	case CommandTypeEnum::EXIT:
		cmdParamAndOptMap.insert( pair<string, any>(optionField::COMMAND, commandTypeEnum) );
		return cmdParamAndOptMap;
	}

	return cmdParamAndOptMap;

}

bool Parser::areEqualStringsIgnoreCase(const string& stringOne, const string& stringTwo) {

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

inline string Parser::trimRight(const string& s, const string& delimiters)
{
	return s.substr( 0, s.find_last_not_of( delimiters ) + 1 );
}

inline string Parser::trimLeft(const string& s, const string& delimiters)
{
	return s.substr( s.find_first_not_of( delimiters ) );
}

inline string Parser::trim(const string& s, const string& delimiters)
{

	if (!s.empty()){
		return trimLeft(trimRight(s, delimiters), delimiters);		
	}

	return s;

}