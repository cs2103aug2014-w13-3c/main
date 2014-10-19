#include "parser.h"

using namespace std;

void Parser::loadValidCommandKeywords(){

	validCommandKeywords.push_back(make_pair("add",CommandTypeEnum::ADD_TASK));
	validCommandKeywords.push_back(make_pair("/a",CommandTypeEnum::ADD_TASK));
	validCommandKeywords.push_back(make_pair("delete",CommandTypeEnum::DELETE_TASK));
	validCommandKeywords.push_back(make_pair("/d",CommandTypeEnum::DELETE_TASK));
	validCommandKeywords.push_back(make_pair("exit",CommandTypeEnum::EXIT));

}

void Parser::loadOptionFieldsChecker(){

	optionFieldsChecker.push_back(make_pair(cmdOptionField::COMMAND,false));
	optionFieldsChecker.push_back(make_pair(cmdOptionField::PARAMETERS,true));
	optionFieldsChecker.push_back(make_pair(cmdOptionField::START_OPTION,true));
	optionFieldsChecker.push_back(make_pair(cmdOptionField::END_OPTION,true));
	optionFieldsChecker.push_back(make_pair(cmdOptionField::PRIORITY_OPTION,true));
	optionFieldsChecker.push_back(make_pair(cmdOptionField::RECURSIVE_OPTION,true));
	optionFieldsChecker.push_back(make_pair(cmdOptionField::TAG_OPTION,true));
	optionFieldsChecker.push_back(make_pair(cmdOptionField::LINK_OPTION,true));
	optionFieldsChecker.push_back(make_pair(cmdOptionField::HELP_OPTION,false));

}

Parser::Parser(const string commandString){

	vector< pair<string, CommandTypeEnum::COMMAND_TYPE> > validCommandKeywords;
	loadValidCommandKeywords();
	vector< pair<string, bool> > optionFieldsChecker;
	loadOptionFieldsChecker();
	cmdResult = parseCommand(commandString);

}

Parser::~Parser(){

}

multimap<string, any> Parser::getCommandContents(){

	return cmdResult;

}

bool Parser::getValid(){

	return any_cast<bool>(cmdResult.find(cmdOptionField::VALID)->second);

}

multimap<string, any> Parser::parseCommand(const string commandString){

	// TODO: refactor
	if( trim(commandString) == "" ){
		throw "No command found.";
	}

	vector<string> commandStringTokens = tokenizeCommandString(commandString);

	return checkCommandSyntax(commandStringTokens);

}

// Check the entire command syntax
multimap<string, any> Parser::checkCommandSyntax(vector<string> commandStringTokens){

	CommandTypeEnum::COMMAND_TYPE cmdType;
	multimap<string,any> cmdParamAndOptMap;

	for(unsigned int i = 0; i <= sizeof(validCommandKeywords); i++){

		if( areEqualStringsIgnoreCase(commandStringTokens[0], validCommandKeywords[i].first) ){
			cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::COMMAND, validCommandKeywords[i].second));
			cmdType = validCommandKeywords[i].second;
		} else if( i == sizeof(Parser::validCommandKeywords)){
			cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false));
			return cmdParamAndOptMap;
		}
		
	}

	bool isFirstOption = true;
	bool hasNoOptions = true;

	switch (cmdType){
	// has mandatory parameter and optional options
	case CommandTypeEnum::ADD_TASK:
	case CommandTypeEnum::DELETE_TASK:

		for(unsigned int i = 1; i <= sizeof(commandStringTokens); i++){

			for(unsigned int j = 0; j <= sizeof(optionFieldsChecker); j++){

				if(i == 1 && areEqualStringsIgnoreCase(commandStringTokens[i], optionFieldsChecker[0].first )){

					cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false));
					return cmdParamAndOptMap;

				} else if( areEqualStringsIgnoreCase(commandStringTokens[i], optionFieldsChecker[j].first ) ){

					if (isFirstOption){

						isFirstOption = false;
						hasNoOptions = false;
						vector<string> extractParam;
						copy(commandStringTokens.begin() + 1, commandStringTokens.begin() + (i-1), back_inserter(extractParam));
						string Param = joinVector(extractParam);
						cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::PARAMETERS, Param));

					}

					//string currentOptionValue = processOption(commandStringTokens, i , optionFieldsChecker[j]);

				}

			}

		}

		if(hasNoOptions){
			 
			string extractParam = accumulate(commandStringTokens.begin() + 1, commandStringTokens.end(), extractParam);
			cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::COMMAND, cmdType) );
			cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::PARAMETERS, extractParam) );
			return cmdParamAndOptMap;

		}

	// has only param (complete)

	// supports logical operations (search and filter)

	// has no param and options
	case CommandTypeEnum::EXIT:

		cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::VALID, true) );
		cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::COMMAND, cmdType) );
		return cmdParamAndOptMap;

	default:

		cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false));
		return cmdParamAndOptMap;

	}

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

string Parser::joinVector(const vector<string>& commandVector){

	return std::accumulate( commandVector.begin(), commandVector.end(), string(""));

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