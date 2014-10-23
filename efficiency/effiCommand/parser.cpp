#include "parser.h"

using namespace std;

// stores all valid command keywords
void Parser::loadValidCommandKeywords(){

	validCommandKeywords.push_back(make_pair("add",CommandTypeEnum::ADD_TASK));
	validCommandKeywords.push_back(make_pair("/a",CommandTypeEnum::ADD_TASK));
	validCommandKeywords.push_back(make_pair("delete",CommandTypeEnum::DELETE_TASK));
	validCommandKeywords.push_back(make_pair("/d",CommandTypeEnum::DELETE_TASK));
	validCommandKeywords.push_back(make_pair("exit",CommandTypeEnum::EXIT));

}

// stores all available options and be ready to output to multimap
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

// Constructor
Parser::Parser(){

	loadValidCommandKeywords();
	loadOptionFieldsChecker();

}

// Destructor
Parser::~Parser(){

}

// API to parse command
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

	// Init Enum for identification
	CommandTypeEnum::COMMAND_TYPE cmdType;

	// Final multimap result
	multimap<string,any> cmdParamAndOptMap;

	// REMEMBER: THIS FOR LOOP IS FOR COMMAND TYPE!
	for(unsigned int i = 0; i < validCommandKeywords.size(); i++){

		// FOUND AVAILABLE COMMAND
		if( areEqualStringsIgnoreCase(commandStringTokens[0], validCommandKeywords[i].first) ){
			cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::COMMAND, validCommandKeywords[i].second));
			cmdType = validCommandKeywords[i].second;
			break;

		// REACHED END OF LIST OF VALID COMMANDS, IE NOT FOUND
		} else if( i == validCommandKeywords.size() - 1){
			cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false));
			return cmdParamAndOptMap;
		}
		
	}

	// SET FLAG FOR OPTION WITH PARAM BEFORE IT
	bool hasNoOptions = true;

	switch (cmdType){
	// has mandatory parameter and optional options
	case CommandTypeEnum::ADD_TASK:
	case CommandTypeEnum::DELETE_TASK:		

		// ITERATE THRU ENTIRE ENTERED COMMAND
		for(unsigned int i = 1; i < commandStringTokens.size(); i++){

			// ITERATE THRU AVAILABLE OPTIONS FIELD
			for(unsigned int j = 0; j < optionFieldsChecker.size(); j++){

				// PARAM MISSING
				if(i == 1 && areEqualStringsIgnoreCase(commandStringTokens[i], optionFieldsChecker[j].first )){

					cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false) );
					return cmdParamAndOptMap;

				// OPTIONS MATCH
				} else if( areEqualStringsIgnoreCase(commandStringTokens[i], optionFieldsChecker[j].first ) ){

					// IF IT IS THE FIRST ONE FOUND
					if (hasNoOptions){

						hasNoOptions = false;
						vector<string> extractParam;
						copy(commandStringTokens.begin() + 1, commandStringTokens.begin() + i, back_inserter(extractParam));
						string Param = joinVector(extractParam, " ");
						cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::PARAMETERS, Param));
						cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, true) );

					}

					// EXTRACT OPTION VALUE AND TIE THEM UP FOR CHECK
					cmdParamAndOptMap = extractOptionsAndValues(cmdParamAndOptMap, commandStringTokens, i , optionFieldsChecker[j]);			
					return cmdParamAndOptMap;

				}

			}

		}

		// IF ENTERED COMMAND HAS NO OPTIONS FOUND
		if(hasNoOptions){
			 
			vector<string> extractParam;
			copy(commandStringTokens.begin() + 1, commandStringTokens.end(), back_inserter(extractParam));
			
			// IF PARAM NOT FOUND, COMMAND INVALID
			if(extractParam.empty()){
				cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::VALID, false) );
				return cmdParamAndOptMap;
			}

			// IF THERE IS PARAM, EXTRACT AND INSERT, MARK AS TRUE
			string Param = joinVector(extractParam, " ");
			cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::PARAMETERS, Param) );
			cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::VALID, true) );
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

		// SHOULD NEVER BE RUN
		cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false));
		return cmdParamAndOptMap;

	}

}

// ONCE A OPTION FIELD IS FOUND, EXTRACT VALUE
// multimap to update, entered command, current option index, extracted option and bool for value needed
multimap<string, any> Parser::extractOptionsAndValues(multimap<string, any> cmdParamAndOptMap, 
									 vector<string> commandStringTokens, 
									 int fieldPos, 
									 pair<string, bool> currentOptionFieldPair){

	// TAKE CURRENT OPTION
	string currentOptionField = currentOptionFieldPair.first;

	bool hasFieldValueFormat = currentOptionFieldPair.second;

	string fieldValue;
	vector<string> fieldValueVector;

	bool hasFieldValueEntered = false;

	bool processComplete = false;

	// IF THE FOUND OPTION DOESN'T NEED VALUE, SIMPLY RETURN BOOL AS TRUE
	if(!hasFieldValueFormat){

		cmdParamAndOptMap.insert( pair<string,any> (currentOptionField, true) );

		// READ VALUE FROM THE POSITION AT FIELD INDEX
		for(unsigned int i = fieldPos; i < commandStringTokens.size() - 1; i++){

			// IF POSITION OF OPTION NEEDING VALUE IS AT THE END, NO MORE FUTURE OPTIONS
			if(fieldPos != commandStringTokens.size() - 1){

				// ITERATE THROUGH AVAILABLE OPTIONS UNTIL WE HIT NEXT AVAILABLE OPTIONS FIELD
				for(unsigned int j = 0; j < optionFieldsChecker.size(); j++){

					// IF FOUND NEXT OPTION, RECURSIVELY CALL FOR NEXT PROCESS
					if(areEqualStringsIgnoreCase(commandStringTokens[i+1], optionFieldsChecker[j].first)){

						cmdParamAndOptMap = extractOptionsAndValues(cmdParamAndOptMap, commandStringTokens, i + 1, optionFieldsChecker[j]);
						processComplete = true;
						break;

					}

				}

			}

			if(processComplete){
				break;
			}

		}	

	// IF THE FOUND OPTION NEEDS VALUE, PROCESS VALUE ACCORDING TO OPTION FOUND
	} else {

		if (areEqualStringsIgnoreCase(currentOptionField, cmdOptionField::REPEAT_OPTION)){

		} else if (areEqualStringsIgnoreCase(currentOptionField, cmdOptionField::TAG_OPTION)){

		} else if (areEqualStringsIgnoreCase(currentOptionField, cmdOptionField::START_OPTION)){

		} else if (areEqualStringsIgnoreCase(currentOptionField, cmdOptionField::END_OPTION)){

		} else if (areEqualStringsIgnoreCase(currentOptionField, cmdOptionField::LINK_OPTION) ||
				   areEqualStringsIgnoreCase(currentOptionField, cmdOptionField::PRIORITY_OPTION)){

			// READ VALUE FROM THE POSITION AT FIELD INDEX
			for(unsigned int i = fieldPos; i < commandStringTokens.size() - 1; i++){

				// IF POSITION OF OPTION NEEDING VALUE IS AT THE END, NO WAY IT IS VALID
				if(fieldPos != commandStringTokens.size() - 1){

					// ITERATE THROUGH AVAILABLE OPTIONS UNTIL WE HIT NEXT AVAILABLE OPTIONS FIELD
					for(unsigned int j = 0; j < optionFieldsChecker.size(); j++){

						// IF FOUND NEXT OPTION, PREPROCESS
						if(areEqualStringsIgnoreCase(commandStringTokens[i+1], optionFieldsChecker[j].first)){

							if(hasFieldValueEntered){

								copy(commandStringTokens.begin() + fieldPos + 1, commandStringTokens.begin() + (i+1), back_inserter(fieldValueVector));
								fieldValue = joinVector(fieldValueVector, " ");
								cmdParamAndOptMap.insert( pair<string,any> (currentOptionField, fieldValue) );
								cmdParamAndOptMap = extractOptionsAndValues(cmdParamAndOptMap, commandStringTokens, i + 1, optionFieldsChecker[j]);
								processComplete = true;
								break;

							} else {

								cmdParamAndOptMap = extractOptionsAndValues(cmdParamAndOptMap, commandStringTokens, i + 1, optionFieldsChecker[j]);
								processComplete = true;
								break;

							}

						} else {

							// IF VALUE EXTENDS TO END OF ENTERED COMMAND AND NO NEXT OPTION IS FOUND
							if( (i + 1 == commandStringTokens.size() - 1) && (j == optionFieldsChecker.size() - 1) ){

								copy(commandStringTokens.begin() + fieldPos + 1, commandStringTokens.end(), back_inserter(fieldValueVector));
								fieldValue = joinVector(fieldValueVector, " ");
								cmdParamAndOptMap.insert( pair<string,any> (currentOptionField, fieldValue) );
								processComplete = true;
								break;

							}

						}

					}

				}

				if(processComplete){
					break;
				}

				hasFieldValueEntered = true;

			}

		}

	}

	return cmdParamAndOptMap;

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