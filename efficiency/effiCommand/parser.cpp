#include "parser.h"

using namespace std;

// stores all valid command keywords
void Parser::loadValidCommandKeywords(){

	validCommandKeywords.push_back(make_pair("add",CommandTypeEnum::ADD_TASK));
	validCommandKeywords.push_back(make_pair("/a",CommandTypeEnum::ADD_TASK));

	validCommandKeywords.push_back(make_pair("update",CommandTypeEnum::UPDATE_TASK));
	validCommandKeywords.push_back(make_pair("/u",CommandTypeEnum::UPDATE_TASK));

	validCommandKeywords.push_back(make_pair("delete",CommandTypeEnum::DELETE_TASK));
	validCommandKeywords.push_back(make_pair("/d",CommandTypeEnum::DELETE_TASK));

	validCommandKeywords.push_back(make_pair("complete",CommandTypeEnum::MARK_COMPLETE));
	validCommandKeywords.push_back(make_pair("completed",CommandTypeEnum::MARK_COMPLETE));
	validCommandKeywords.push_back(make_pair("/c",CommandTypeEnum::MARK_COMPLETE));

	validCommandKeywords.push_back(make_pair("undo",CommandTypeEnum::UNDO));
	validCommandKeywords.push_back(make_pair("/z",CommandTypeEnum::UNDO));

	validCommandKeywords.push_back(make_pair("setting",CommandTypeEnum::SETTINGS));
	validCommandKeywords.push_back(make_pair("settings",CommandTypeEnum::SETTINGS));
	validCommandKeywords.push_back(make_pair("/t",CommandTypeEnum::SETTINGS));

	validCommandKeywords.push_back(make_pair("minimize",CommandTypeEnum::MINIMIZE));
	validCommandKeywords.push_back(make_pair("/m",CommandTypeEnum::MINIMIZE));
	
	validCommandKeywords.push_back(make_pair("help",CommandTypeEnum::HELP));
	validCommandKeywords.push_back(make_pair("/?",CommandTypeEnum::HELP));

	validCommandKeywords.push_back(make_pair("logout",CommandTypeEnum::LOGOUT));

	validCommandKeywords.push_back(make_pair("exit",CommandTypeEnum::EXIT));

}

// stores all available options and be ready to output to multimap
void Parser::loadOptionFieldsChecker(){

	optionFieldsChecker.push_back(make_tuple("start", cmdOptionField::START,true));
	optionFieldsChecker.push_back(make_tuple("-s", cmdOptionField::START,true));

	optionFieldsChecker.push_back(make_tuple("end", cmdOptionField::END,true));
	optionFieldsChecker.push_back(make_tuple("-e", cmdOptionField::END,true));

	optionFieldsChecker.push_back(make_tuple("priority", cmdOptionField::PRIORITY,true));
	optionFieldsChecker.push_back(make_tuple("-p", cmdOptionField::PRIORITY,true));

	optionFieldsChecker.push_back(make_tuple("repeat", cmdOptionField::REPEAT_RECURSIVE,true));
	optionFieldsChecker.push_back(make_tuple("recursive", cmdOptionField::REPEAT_RECURSIVE,true));
	optionFieldsChecker.push_back(make_tuple("-r", cmdOptionField::REPEAT_RECURSIVE,true));

	optionFieldsChecker.push_back(make_tuple("tag", cmdOptionField::TAG,true));
	optionFieldsChecker.push_back(make_tuple("-t", cmdOptionField::TAG,true));

	optionFieldsChecker.push_back(make_tuple("link", cmdOptionField::LINK,true));
	optionFieldsChecker.push_back(make_tuple("-l", cmdOptionField::LINK,true));

	optionFieldsChecker.push_back(make_tuple("help", cmdOptionField::HELP,false));
	optionFieldsChecker.push_back(make_tuple("-h", cmdOptionField::HELP,false));

}

pair<bool,ptime> Parser::checkDateTime(string dtFieldValue, bool firstRun){

	vector<string> dtToken = tokenizeCommandString(dtFieldValue, false);

	if(dtToken.size() > 1){

		string time = dtToken[1];

		replace(time.begin(), time.end(), ':', ' ');

		vector<string> timeToken = tokenizeCommandString(time, false);

		int hour = stoi(timeToken[0]);
		int minute = stoi(timeToken[1]);
				
		if ( hasSuffix(timeToken[timeToken.size() - 1],"PM") || 
			 hasSuffix(timeToken[timeToken.size() - 1],"AM") ){

			if( hasSuffix(timeToken[timeToken.size() - 1],"PM") ){
				
				hour = (hour % 12) + 12;
				timeToken[0] = to_string(hour);
			}

		}

		char buff[100];
		sprintf_s(buff, "%02d:%02d:00", hour, minute);
		time = buff;

		dtToken[1] = time;

	}

	dtFieldValue = joinVector(dtToken, " ");

	const locale inputFormats[] = {

		locale(locale::classic(), new time_input_facet("%b/%d/%Y %H:%M:%S")),
		locale(locale::classic(), new time_input_facet("%B/%d/%Y %H:%M:%S")),
		locale(locale::classic(), new time_input_facet("%d/%b/%Y %H:%M:%S")),
		locale(locale::classic(), new time_input_facet("%d/%B/%Y %H:%M:%S")),

		locale(locale::classic(), new time_input_facet("%b-%d-%Y %H:%M:%S")),
		locale(locale::classic(), new time_input_facet("%B-%d-%Y %H:%M:%S")),
		locale(locale::classic(), new time_input_facet("%d-%b-%Y %H:%M:%S")),
		locale(locale::classic(), new time_input_facet("%d-%B-%Y %H:%M:%S")),

		locale(locale::classic(), new time_input_facet("%Y/%b/%d %H:%M:%S")),
		locale(locale::classic(), new time_input_facet("%Y/%B/%d %H:%M:%S")),
		locale(locale::classic(), new time_input_facet("%Y-%B-%d %H:%M:%S")),
		locale(locale::classic(), new time_input_facet("%Y-%b-%d %H:%M:%S")),

		locale(locale::classic(), new time_input_facet("%d-%m-%Y %H:%M:%S")),
		locale(locale::classic(), new time_input_facet("%Y-%m-%d %H:%M:%S")),
		locale(locale::classic(), new time_input_facet("%d/%m/%Y %H:%M:%S")),
		locale(locale::classic(), new time_input_facet("%Y/%m/%d %H:%M:%S")),

	};

	const size_t dtFormats = sizeof(inputFormats)/sizeof(inputFormats[0]);

	pair<bool,ptime> result;

	for(size_t i=0; i < dtFormats; ++i){

		istringstream ss(dtFieldValue);
		ss.imbue(inputFormats[i]);
		ptime dateTime;
		ss >> dateTime;

		if(dateTime != not_a_date_time){

			result.first = true;
			result.second = dateTime;
			return result;

		}

	}

	result.first = false;

	return result;

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

	vector<string> commandStringTokens = tokenizeCommandString(commandString, false);

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
	case CommandTypeEnum::UPDATE_TASK:		

		// ITERATE THRU ENTIRE ENTERED COMMAND
		for(unsigned int i = 1; i < commandStringTokens.size(); i++){

			// ITERATE THRU AVAILABLE OPTIONS FIELD
			for(unsigned int j = 0; j < optionFieldsChecker.size(); j++){

				// PARAM MISSING
				if(i == 1 && areEqualStringsIgnoreCase(commandStringTokens[i], get<0>(optionFieldsChecker[j]) )){

					cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false) );
					return cmdParamAndOptMap;

				// OPTIONS MATCH
				} else if( areEqualStringsIgnoreCase(commandStringTokens[i], get<0>(optionFieldsChecker[j]) ) ){

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
					cmdParamAndOptMap = extractOptionsAndValues(cmdParamAndOptMap, commandStringTokens, i , optionFieldsChecker[j] );			
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

	// has only param
	// case CommandTypeEnum::VIEW:
	case CommandTypeEnum::MARK_COMPLETE:	

		if(commandStringTokens.size() <= 1){

			cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false) );
			return cmdParamAndOptMap;

		} else {

			vector<string> extractParam;
			copy(commandStringTokens.begin() + 1, commandStringTokens.end(), back_inserter(extractParam));
			string Param = joinVector(extractParam, " ");
			cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::PARAMETERS, Param));
			cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, true) );
			return cmdParamAndOptMap;

		}

	// supports logical operations (search and filter)
	//case CommandTypeEnum::SEARCH:
	//case CommandTypeEnum::FILTER:

	// has no param and options
	case CommandTypeEnum::UNDO:
	case CommandTypeEnum::SETTINGS:
	case CommandTypeEnum::MINIMIZE:
	case CommandTypeEnum::HELP:
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
									 tuple<string, string, bool> currentOptionFieldTuple){

	// TAKE CURRENT OPTION
	string currentOptionField = get<0> (currentOptionFieldTuple);

	string mmOptionKey = get<1> (currentOptionFieldTuple);

	bool hasFieldValueFormat = get<2> (currentOptionFieldTuple);

	any fieldValue;
	vector<string> fieldValueVector;

	bool hasFieldValueEntered = false;

	bool processComplete = false;

	// IF THE FOUND OPTION DOESN'T NEED VALUE, SIMPLY RETURN BOOL AS TRUE
	if(!hasFieldValueFormat){

		// CONTINUE HERE: EXCLUDE OPTIONS NOT IN SOME COMMANDS
		if(1){
			cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, true) );
		}

		// READ VALUE FROM THE POSITION AT FIELD INDEX
		for(unsigned int i = fieldPos; i < commandStringTokens.size() - 1; i++){

			// IF POSITION OF OPTION NEEDING VALUE IS AT THE END, NO MORE FUTURE OPTIONS
			if(fieldPos != commandStringTokens.size() - 1){

				// ITERATE THROUGH AVAILABLE OPTIONS UNTIL WE HIT NEXT AVAILABLE OPTIONS FIELD
				for(unsigned int j = 0; j < optionFieldsChecker.size(); j++){

					// IF FOUND NEXT OPTION, RECURSIVELY CALL FOR NEXT PROCESS
					if(areEqualStringsIgnoreCase(commandStringTokens[i+1], get<0>(optionFieldsChecker[j]))){

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

		// READ VALUE FROM THE POSITION AT FIELD INDEX
		for(unsigned int i = fieldPos; i < commandStringTokens.size() - 1; i++){

			// IF POSITION OF OPTION NEEDING VALUE IS AT THE END, NO WAY IT IS VALID
			if(fieldPos != commandStringTokens.size() - 1){

				// ITERATE THROUGH AVAILABLE OPTIONS UNTIL WE HIT NEXT AVAILABLE OPTIONS FIELD
				for(unsigned int j = 0; j < optionFieldsChecker.size(); j++){

					// IF FOUND NEXT OPTION, PREPROCESS
					if(areEqualStringsIgnoreCase(commandStringTokens[i+1], get<0>(optionFieldsChecker[j]))){

						if(hasFieldValueEntered){

							copy(commandStringTokens.begin() + fieldPos + 1, commandStringTokens.begin() + (i+1), back_inserter(fieldValueVector));
							
							if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::TAG)){

								string extractedTagValues = joinVector(fieldValueVector, " ");
								fieldValueVector = tokenizeCommandString(extractedTagValues, true);
								// CONTINUE HERE: EXCLUDE OPTIONS NOT IN SOME COMMANDS
								if(1){
									cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, fieldValueVector) );
								}

							} else {

								if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::START) ||
										areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::END)){

									fieldValue = joinVector(fieldValueVector, " ");

									string dtFieldValue = any_cast<string> (fieldValue);

									pair<bool,ptime> isValidDateTime = checkDateTime(dtFieldValue, true);

									if(isValidDateTime.first){
										// CONTINUE HERE: EXCLUDE OPTIONS NOT IN SOME COMMANDS
										if(1){
											cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, isValidDateTime.second) );
										}
										break;
									}

								} else {

									fieldValue = joinVector(fieldValueVector, " ");
									// CONTINUE HERE: EXCLUDE OPTIONS NOT IN SOME COMMANDS
									if(1){
										cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, fieldValue) );
									}

								}

							}							
							
						}

							cmdParamAndOptMap = extractOptionsAndValues(cmdParamAndOptMap, commandStringTokens, i + 1, optionFieldsChecker[j]);
							processComplete = true;
							break;

					} else {

						// IF VALUE EXTENDS TO END OF ENTERED COMMAND AND NO NEXT OPTION IS FOUND
						if( (i + 1 == commandStringTokens.size() - 1) && (j == optionFieldsChecker.size() - 1) ){

							copy(commandStringTokens.begin() + fieldPos + 1, commandStringTokens.end(), back_inserter(fieldValueVector));
							
							if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::TAG)){

								string extractedTagValues = joinVector(fieldValueVector, " ");
								fieldValueVector = tokenizeCommandString(extractedTagValues, true);
								// CONTINUE HERE: EXCLUDE OPTIONS NOT IN SOME COMMANDS
								if(1){
									cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, fieldValueVector) );
								}

							} else {

								if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::START) ||
										areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::END)){

									fieldValue = joinVector(fieldValueVector, " ");

									string dtFieldValue = any_cast<string> (fieldValue);

									pair<bool, ptime> isValidDateTime = checkDateTime(dtFieldValue, true);

									if(isValidDateTime.first){
										// CONTINUE HERE: EXCLUDE OPTIONS NOT IN SOME COMMANDS
										if(1){
											cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, isValidDateTime.second) );
										}
										break;
									}

								} else {

									fieldValue = joinVector(fieldValueVector, " ");
									// CONTINUE HERE: EXCLUDE OPTIONS NOT IN SOME COMMANDS
									if(1){
										cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, fieldValue) );
									}

								}

							}	

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

	return cmdParamAndOptMap;

}

// This function converts user command strings into a string vector
vector<string> Parser::tokenizeCommandString(string userCommand, bool forTagComma){

	vector<string> stringTokens;
	istringstream iss(userCommand);

	if(!forTagComma){

		copy(istream_iterator<string>(iss),
				 istream_iterator<string>(),
				 back_inserter< vector<string> > (stringTokens) );

	} else {

		string tagTokens;
		string result;

		while(getline(iss, tagTokens, ',')) {
			result = result + tagTokens + " ";
		}

		return tokenizeCommandString(result, false);

	}

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

inline string Parser::trimRight(const string& s, const string& delimiters){
	return s.substr( 0, s.find_last_not_of( delimiters ) + 1 );
}

inline string Parser::trimLeft(const string& s, const string& delimiters){
	return s.substr( s.find_first_not_of( delimiters ) );
}

inline string Parser::trim(const string& s, const string& delimiters){

	if (!s.empty()){
		return trimLeft(trimRight(s, delimiters), delimiters);		
	}

	return s;

}

bool Parser::hasSuffix(const std::string &str, const std::string &suffix){
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}