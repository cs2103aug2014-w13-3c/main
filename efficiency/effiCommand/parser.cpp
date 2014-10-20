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

Parser::Parser(){

	loadValidCommandKeywords();
	loadOptionFieldsChecker();

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

	for(unsigned int i = 0; i < validCommandKeywords.size(); i++){

		if( areEqualStringsIgnoreCase(commandStringTokens[0], validCommandKeywords[i].first) ){
			cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::COMMAND, validCommandKeywords[i].second));
			cmdType = validCommandKeywords[i].second;
			break;
		} else if( i == validCommandKeywords.size() - 1){
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

		for(unsigned int i = 1; i < commandStringTokens.size(); i++){

			for(unsigned int j = 0; j < optionFieldsChecker.size(); j++){

				if(i == 1 && areEqualStringsIgnoreCase(commandStringTokens[i], optionFieldsChecker[j].first )){

					cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false));
					return cmdParamAndOptMap;

				} else if( areEqualStringsIgnoreCase(commandStringTokens[i], optionFieldsChecker[j].first ) ){

					if (isFirstOption){

						isFirstOption = false;
						hasNoOptions = false;
						vector<string> extractParam;
						copy(commandStringTokens.begin() + 1, commandStringTokens.begin() + i, back_inserter(extractParam));
						string Param = joinVector(extractParam, " ");
						cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::PARAMETERS, Param));

					}

					tuple<string, any, int> currentOptionValue = extractOptionValue(commandStringTokens, i , optionFieldsChecker[j]);
					string fieldOptions;
					any fieldValue;
					int k = 0;
					tie(fieldOptions, fieldValue, k) = currentOptionValue;

					if(!areEqualStringsIgnoreCase(fieldOptions, cmdOptionField::EMPTY_FIELD)){
						cmdParamAndOptMap.insert( pair<string,any> (fieldOptions, fieldValue));
					}

					i = k;

				}

			}

		}

		if(hasNoOptions){
			 
			vector<string> extractParam;
			copy(commandStringTokens.begin() + 1, commandStringTokens.end(), back_inserter(extractParam));
			string Param = joinVector(extractParam, " ");
			cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::PARAMETERS, Param) );
			return cmdParamAndOptMap;

		}

		return cmdParamAndOptMap;

	// has only param (complete and undo)

	// supports logical operations (search and filter)

	// has no param and options
	case CommandTypeEnum::SETTINGS:
	case CommandTypeEnum::MINIMIZE:
	case CommandTypeEnum::LOGOUT:
	case CommandTypeEnum::EXIT:

		cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::VALID, true) );
		cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::COMMAND, cmdType) );
		return cmdParamAndOptMap;

	default:

		cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false));
		return cmdParamAndOptMap;

	}

}

tuple<string, any, int> Parser::extractOptionValue(vector<string> commandStringTokens, int fieldPos, pair<string, bool> currentOptionFieldPair){

	string currentOptionField = currentOptionFieldPair.first;

	string fieldValue;
	vector<string> fieldValueVector;

	tuple<string, any, int> result;

	bool hasFieldValueEntered = false;

	bool hasFieldValueFormat = currentOptionFieldPair.second;

	if(!hasFieldValueFormat){

		result = make_tuple(currentOptionField, true, fieldPos);
		return result;

	} else {

		if (areEqualStringsIgnoreCase(currentOptionField, cmdOptionField::REPEAT_OPTION)){

		} else if (areEqualStringsIgnoreCase(currentOptionField, cmdOptionField::TAG_OPTION)){

		} else if(areEqualStringsIgnoreCase(currentOptionField, cmdOptionField::START_OPTION)){

		} else if (areEqualStringsIgnoreCase(currentOptionField, cmdOptionField::END_OPTION)){

		} else if (areEqualStringsIgnoreCase(currentOptionField, cmdOptionField::LINK_OPTION) ||
				   areEqualStringsIgnoreCase(currentOptionField, cmdOptionField::PRIORITY_OPTION)){

			for(unsigned int i = fieldPos; i < commandStringTokens.size() - 1; i++){

				for(unsigned int j = 0; j < optionFieldsChecker.size(); j++){

					if(fieldPos == commandStringTokens.size() - 1){

						result = make_tuple(cmdOptionField::EMPTY_FIELD, "null", fieldPos);
						return result;

					} else {

						if(areEqualStringsIgnoreCase(commandStringTokens[i+1], optionFieldsChecker[j].first)){

							if(!hasFieldValueEntered){

								result = make_tuple(cmdOptionField::EMPTY_FIELD, "null", fieldPos);
								return result;

							} else {

								copy(commandStringTokens.begin() + fieldPos + 1, commandStringTokens.begin() + (i+1), back_inserter(fieldValueVector));
								fieldValue = joinVector(fieldValueVector, " ");
								result = make_tuple(currentOptionField, fieldValue, i);
								return result;

							}

						} else {

							if( (i + 1 == commandStringTokens.size() - 1) && (j == optionFieldsChecker.size() - 1) ){

								copy(commandStringTokens.begin() + fieldPos + 1, commandStringTokens.end(), back_inserter(fieldValueVector));
								fieldValue = joinVector(fieldValueVector, " ");
								result = make_tuple(currentOptionField, fieldValue, i);
								return result;

							}

						}

					}

				}

				hasFieldValueEntered = true;

			}

			result = make_tuple(cmdOptionField::EMPTY_FIELD, "null", fieldPos);
			return result;

		} else {

			result = make_tuple(cmdOptionField::EMPTY_FIELD, "null", fieldPos);
			return result;

		}

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

string Parser::joinVector(const vector<string>& commandVector, const string& token){

  ostringstream result;

  for (vector<string>::const_iterator i = commandVector.begin(); i != commandVector.end(); i++){
    if (i != commandVector.begin()) {
		result << token;
	}
    result << *i;
  }

  return result.str();

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