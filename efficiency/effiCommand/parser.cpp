#include "parser.h"
#include "predParser.h"

using namespace std;

const locale Parser::inputFormats[] = {
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

	locale(locale::classic(), new time_input_facet("%b/%d/%Y %H.%M.%S")),
	locale(locale::classic(), new time_input_facet("%B/%d/%Y %H.%M.%S")),
	locale(locale::classic(), new time_input_facet("%d/%b/%Y %H.%M.%S")),
	locale(locale::classic(), new time_input_facet("%d/%B/%Y %H.%M.%S")),

	locale(locale::classic(), new time_input_facet("%b-%d-%Y %H.%M.%S")),
	locale(locale::classic(), new time_input_facet("%B-%d-%Y %H.%M.%S")),
	locale(locale::classic(), new time_input_facet("%d-%b-%Y %H.%M.%S")),
	locale(locale::classic(), new time_input_facet("%d-%B-%Y %H.%M.%S")),

	locale(locale::classic(), new time_input_facet("%Y/%b/%d %H.%M.%S")),
	locale(locale::classic(), new time_input_facet("%Y/%B/%d %H.%M.%S")),
	locale(locale::classic(), new time_input_facet("%Y-%B-%d %H.%M.%S")),
	locale(locale::classic(), new time_input_facet("%Y-%b-%d %H.%M.%S")),

	locale(locale::classic(), new time_input_facet("%d-%m-%Y %H.%M.%S")),
	locale(locale::classic(), new time_input_facet("%Y-%m-%d %H.%M.%S")),
	locale(locale::classic(), new time_input_facet("%d/%m/%Y %H.%M.%S")),
	locale(locale::classic(), new time_input_facet("%Y/%m/%d %H.%M.%S")),

};

// stores all valid command keywords
void Parser::loadValidCommandKeywords(){

	//validCommandKeywords.push_back(make_pair("view",commandTypeEnum::VIEW));
	//validCommandKeywords.push_back(make_pair("/v",commandTypeEnum::VIEW));

	validCommandKeywords.push_back(make_pair("scroll",commandTypeEnum::SCROLL));
	validCommandKeywords.push_back(make_pair("/sc",commandTypeEnum::SCROLL));

	validCommandKeywords.push_back(make_pair("add",commandTypeEnum::ADD_TASK));
	validCommandKeywords.push_back(make_pair("/a",commandTypeEnum::ADD_TASK));

	validCommandKeywords.push_back(make_pair("update",commandTypeEnum::UPDATE_TASK));
	validCommandKeywords.push_back(make_pair("/u",commandTypeEnum::UPDATE_TASK));

	validCommandKeywords.push_back(make_pair("delete",commandTypeEnum::DELETE_TASK));
	validCommandKeywords.push_back(make_pair("/d",commandTypeEnum::DELETE_TASK));

	validCommandKeywords.push_back(make_pair("complete",commandTypeEnum::MARK_COMPLETE));
	validCommandKeywords.push_back(make_pair("completed",commandTypeEnum::MARK_COMPLETE));
	validCommandKeywords.push_back(make_pair("/c",commandTypeEnum::MARK_COMPLETE));

	validCommandKeywords.push_back(make_pair("incomplete",commandTypeEnum::MARK_INCOMPLETE));
	validCommandKeywords.push_back(make_pair("/i",commandTypeEnum::MARK_INCOMPLETE));

	validCommandKeywords.push_back(make_pair("undo",commandTypeEnum::UNDO));
	validCommandKeywords.push_back(make_pair("/z",commandTypeEnum::UNDO));

	validCommandKeywords.push_back(make_pair("setting",commandTypeEnum::SETTINGS));
	validCommandKeywords.push_back(make_pair("settings",commandTypeEnum::SETTINGS));
	validCommandKeywords.push_back(make_pair("/t",commandTypeEnum::SETTINGS));

	validCommandKeywords.push_back(make_pair("minimize",commandTypeEnum::MINIMIZE));
	validCommandKeywords.push_back(make_pair("/m",commandTypeEnum::MINIMIZE));
	
	validCommandKeywords.push_back(make_pair("help",commandTypeEnum::HELP));
	validCommandKeywords.push_back(make_pair("/?",commandTypeEnum::HELP));

	validCommandKeywords.push_back(make_pair("search",commandTypeEnum::SEARCH));
	validCommandKeywords.push_back(make_pair("/s",commandTypeEnum::SEARCH));

	validCommandKeywords.push_back(make_pair("filter",commandTypeEnum::FILTER));
	validCommandKeywords.push_back(make_pair("/f",commandTypeEnum::FILTER));

	validCommandKeywords.push_back(make_pair("logout",commandTypeEnum::LOGOUT));

	validCommandKeywords.push_back(make_pair("exit",commandTypeEnum::EXIT));

}

// stores all available options and be ready to output to multimap
void Parser::loadOptionFieldsChecker(){

	optionFieldsChecker.push_back(make_tuple("start", cmdOptionField::START,true));
	optionFieldsChecker.push_back(make_tuple("-s", cmdOptionField::START,true));

	optionFieldsChecker.push_back(make_tuple("end", cmdOptionField::END,true));
	optionFieldsChecker.push_back(make_tuple("-e", cmdOptionField::END,true));

	optionFieldsChecker.push_back(make_tuple("content", cmdOptionField::CONTENT,true));
	optionFieldsChecker.push_back(make_tuple("-c", cmdOptionField::CONTENT,true));

	optionFieldsChecker.push_back(make_tuple("priority", cmdOptionField::PRIORITY,true));
	optionFieldsChecker.push_back(make_tuple("-p", cmdOptionField::PRIORITY,true));

	optionFieldsChecker.push_back(make_tuple("repeat", cmdOptionField::REPEAT,true));
	optionFieldsChecker.push_back(make_tuple("recursive", cmdOptionField::RECURSIVE,false));
	optionFieldsChecker.push_back(make_tuple("-r", cmdOptionField::REPEAT,true));
	optionFieldsChecker.push_back(make_tuple("-r", cmdOptionField::RECURSIVE,false));

	optionFieldsChecker.push_back(make_tuple("tag", cmdOptionField::TAGS,true));
	optionFieldsChecker.push_back(make_tuple("tags", cmdOptionField::TAGS,true));
	optionFieldsChecker.push_back(make_tuple("-t", cmdOptionField::TAGS,true));

	optionFieldsChecker.push_back(make_tuple("name", cmdOptionField::NAME,true));
	optionFieldsChecker.push_back(make_tuple("-n", cmdOptionField::NAME,true));

	optionFieldsChecker.push_back(make_tuple("removetag", cmdOptionField::REMOVETAGS,true));
	optionFieldsChecker.push_back(make_tuple("removetags", cmdOptionField::REMOVETAGS,true));
	optionFieldsChecker.push_back(make_tuple("-rt", cmdOptionField::REMOVETAGS,true));

	optionFieldsChecker.push_back(make_tuple("link", cmdOptionField::LINK,true));
	optionFieldsChecker.push_back(make_tuple("-l", cmdOptionField::LINK,true));

	optionFieldsChecker.push_back(make_tuple("help", cmdOptionField::HELP,false));
	optionFieldsChecker.push_back(make_tuple("-h", cmdOptionField::HELP,false));

}

void Parser::loadViewToScroll(){

	viewToScroll.push_back("all");
	viewToScroll.push_back("task");
	viewToScroll.push_back("event");
	viewToScroll.push_back("deadline");
	viewToScroll.push_back("tasks");
	viewToScroll.push_back("events");
	viewToScroll.push_back("deadlines");

}

void Parser::loadscrollDirection(){

	scrollDirection.push_back("next");
	scrollDirection.push_back("previous");
	scrollDirection.push_back("prev");

}

pair<bool,ptime> Parser::checkDateTime(string dtFieldValue, bool firstRun){

	vector<string> dtToken = tokenizeCommandString(dtFieldValue, false);

	if(dtToken.size() > 1){

		string time = dtToken[1];

		replace(time.begin(), time.end(), ':', ' ');
		replace(time.begin(), time.end(), '.', ' ');

		vector<string> timeToken = tokenizeCommandString(time, false);

		int hour = 0;
		int minute = 0;
		int second = 0;

		if(timeToken.size() >= 2 && timeToken.size() <= 3){

			hour = stoi(timeToken[0]);

			minute = stoi(timeToken[1]);

			if(timeToken.size() == 3){
				second = stoi(timeToken[2]);;
			}
				
			if ( hasSuffix(timeToken[timeToken.size() - 1],"PM") || 
				 hasSuffix(timeToken[timeToken.size() - 1],"AM") ){

				if( hasSuffix(timeToken[timeToken.size() - 1],"PM") ){
				
					hour = (hour % 12) + 12;
					timeToken[0] = to_string(hour);
				}

			}

		} else if(timeToken.size() == 1) {

			string hourString = timeToken[0].substr(0, timeToken[0].rfind("AM"));
			hourString = timeToken[0].substr(0, timeToken[0].rfind("PM"));

			hour = stoi(hourString);

			if( hasSuffix(timeToken[timeToken.size() - 1],"PM") ){				
				hour = (hour % 12) + 12;
			}

			timeToken[0] = to_string(hour);

		}

		char buff[100];
		sprintf_s(buff, "%02d:%02d:%02d", hour, minute, second);
		time = buff;

		dtToken[1] = time;

	}

	dtToken[0] = addPaddingZeros(dtToken[0]);

	dtFieldValue = joinVector(dtToken, " ");
	try{
		auto date = parseDate(dtFieldValue);
		return make_pair(true, date);
	}catch(...){
		pair<bool,ptime> result;
		result.first = false;
		return result;
	}
}

ptime Parser::parseDate(string s){

	s = addPaddingZeros(s);

	const size_t dtFormats = sizeof(inputFormats)/sizeof(inputFormats[0]);
	for(size_t i=0; i < dtFormats; i++){
		istringstream ss(s);
		ss.imbue(inputFormats[i]);
		ptime dateTime;
		ss >> dateTime;

		if(dateTime != not_a_date_time)
			return dateTime;
	}
	throw "Cannot parse";

}

string Parser::addPaddingZeros(string s){

	replace(s.begin(), s.end(), '/', ' ');
	replace(s.begin(), s.end(), '-', ' ');

	vector<string> dateToken = tokenizeCommandString(s, false);

	if(dateToken.size() == 3){
		for(int i = 0; i < dateToken.size(); i++){
			if( dateToken[i].length() == 1 ){
				dateToken[i] = "0" + dateToken[i];
			}
		}
	}

	s = joinVector(dateToken, "-");

	return s;

}

// Constructor
Parser::Parser(){

	loadValidCommandKeywords();
	loadOptionFieldsChecker();
	loadViewToScroll();
	loadscrollDirection();

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
	commandTypeEnum::COMMAND_TYPE cmdType;

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
	case commandTypeEnum::ADD_TASK:
	case commandTypeEnum::DELETE_TASK:	
	case commandTypeEnum::UPDATE_TASK:		

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
					cmdParamAndOptMap = extractOptionsAndValues(cmdType, cmdParamAndOptMap, commandStringTokens, i , optionFieldsChecker[j] );			
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

	// has only param and a recursive option
	case commandTypeEnum::MARK_COMPLETE:	
	case commandTypeEnum::MARK_INCOMPLETE:	

		if(commandStringTokens.size() <= 1){

			cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false) );
			return cmdParamAndOptMap;

		} else {

			// ITERATE THRU ENTIRE ENTERED COMMAND
			for(unsigned int i = 1; i < commandStringTokens.size(); i++){

				// ITERATE THRU AVAILABLE OPTIONS FIELD
				for(unsigned int j = 0; j < optionFieldsChecker.size(); j++){

					// PARAM MISSING
					if(i == 1 && areEqualStringsIgnoreCase(commandStringTokens[i], get<0>(optionFieldsChecker[j]) )){

						cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false) );
						return cmdParamAndOptMap;

					} else if ( areEqualStringsIgnoreCase(commandStringTokens[i], get<0>(optionFieldsChecker[j]) ) ) {

						vector<string> extractParam;
						copy(commandStringTokens.begin() + 1, commandStringTokens.begin() + i, back_inserter(extractParam));
						string Param = joinVector(extractParam, " ");

						if( (areEqualStringsIgnoreCase("recursive", get<0>(optionFieldsChecker[j]) ) || 
							  ( areEqualStringsIgnoreCase("-r", get<0>(optionFieldsChecker[j])) ) ) ) {

							cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::PARAMETERS, Param));
							cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, true) );	
							cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::RECURSIVE, true) );

						} else {

							cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::PARAMETERS, Param));
							cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false) );	

						}

						return cmdParamAndOptMap;

					} else if( (i == commandStringTokens.size() - 1) && (j == optionFieldsChecker.size() - 1) ) {

						vector<string> extractParam;
						copy(commandStringTokens.begin() + 1, commandStringTokens.end(), back_inserter(extractParam));
						string Param = joinVector(extractParam, " ");
						cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::PARAMETERS, Param));
						cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, true) );
						return cmdParamAndOptMap;

					}

				}

			}

		}

	// has only param
	//case commandTypeEnum::VIEW:

	//	if(commandStringTokens.size() > 2){

	//		vector<string> extractParam;
	//		copy(commandStringTokens.begin() + 1, commandStringTokens.end(), back_inserter(extractParam));
	//		string Param = joinVector(extractParam, " ");
	//		cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::PARAMETERS, Param));
	//		cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false) );
	//		return cmdParamAndOptMap;

	//	} else {

	//		// ITERATE THRU ENTIRE ENTERED COMMAND
	//		for(unsigned int i = 1; i < commandStringTokens.size(); i++){

	//			// ITERATE THRU AVAILABLE OPTIONS FIELD
	//			for(unsigned int j = 0; j < optionFieldsChecker.size(); j++){

	//				// PARAM MISSING
	//				if(i == 1 && areEqualStringsIgnoreCase(commandStringTokens[i], get<0>(optionFieldsChecker[j]) )){

	//					cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false) );
	//					return cmdParamAndOptMap;

	//				// Skip all the option fields
	//				} else if( areEqualStringsIgnoreCase(commandStringTokens[i], get<0>(optionFieldsChecker[j]) ) ){

	//					vector<string> extractParam;
	//					copy(commandStringTokens.begin() + 1, commandStringTokens.begin() + i, back_inserter(extractParam));
	//					string Param = joinVector(extractParam, " ");
	//					cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::PARAMETERS, Param));
	//					cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, true) );
	//					return cmdParamAndOptMap;
	//			
	//				// reach end of command, no options field found, the entire command is a param
	//				} else if( (i == commandStringTokens.size() - 1) && (j == optionFieldsChecker.size() - 1) ) {

	//					vector<string> extractParam;
	//					copy(commandStringTokens.begin() + 1, commandStringTokens.end(), back_inserter(extractParam));
	//					string Param = joinVector(extractParam, " ");
	//					cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::PARAMETERS, Param));
	//					cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, true) );
	//					return cmdParamAndOptMap;

	//				}
	//			}
	//		}
	//	}

	// has only param
	case commandTypeEnum::SCROLL:

		if(commandStringTokens.size() != 3){

			vector<string> extractParam;
			copy(commandStringTokens.begin() + 1, commandStringTokens.end(), back_inserter(extractParam));
			string Param = joinVector(extractParam, " ");
			cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::PARAMETERS, Param));
			cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false) );
			return cmdParamAndOptMap;

		} else {

			vector<string> extractParam;
			copy(commandStringTokens.begin() + 1, commandStringTokens.end(), back_inserter(extractParam));
			string Param = joinVector(extractParam, " ");
			cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::PARAMETERS, Param));

			for(unsigned int i = 0; i < viewToScroll.size(); i++) {

				if( areEqualStringsIgnoreCase(commandStringTokens[1], viewToScroll[i]) ) {
					break;
				} else if ( i == viewToScroll.size() - 1 ) {
					cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false) );
					return cmdParamAndOptMap;
				}

			}

			for(unsigned int i = 0; i < scrollDirection.size(); i++) {

				if( areEqualStringsIgnoreCase(commandStringTokens[2], scrollDirection[i]) ) {
					break;
				} else if ( i == scrollDirection.size() - 1 ) {
					cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false) );
					return cmdParamAndOptMap;
				}

			}

			cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, true) );
			return cmdParamAndOptMap;

		}

	// supports logical operations (search and filter)
	case commandTypeEnum::SEARCH:
	case commandTypeEnum::FILTER:
		{
			try{
				stringstream joined;
				for(int i = 1; i<commandStringTokens.size();i++)
					joined<<commandStringTokens[i]<<" ";
				multimap<string,any> cmdmap;
				cmdmap.insert(make_pair(cmdOptionField::VALID, true) );
				cmdmap.insert(make_pair(cmdOptionField::COMMAND, cmdType) );
				cmdmap.insert(make_pair(cmdOptionField::PREDICATE, parsePredicate(joined.str())));
				cmdmap.insert(make_pair(cmdOptionField::PARSE_STRING, joined.str()));
				return cmdmap;
			}catch(expected& e)
			{
				multimap<string,any> cmdmap;
				cmdmap.insert( pair<string, any>(cmdOptionField::VALID, false) );
				cmdmap.insert( pair<string, any>(cmdOptionField::COMMAND, cmdType) );
				return cmdmap;
			}
		}

	// has no param and options
	case commandTypeEnum::UNDO:
	case commandTypeEnum::SETTINGS:
	case commandTypeEnum::MINIMIZE:
	case commandTypeEnum::HELP:
	case commandTypeEnum::LOGOUT:
	case commandTypeEnum::EXIT:

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
multimap<string, any> Parser::extractOptionsAndValues(commandTypeEnum::COMMAND_TYPE cmdType,
									 multimap<string, any> cmdParamAndOptMap, 
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

		cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, true) );

		// READ VALUE FROM THE POSITION AT FIELD INDEX
		for(unsigned int i = fieldPos; i < commandStringTokens.size() - 1; i++){

			// IF POSITION OF OPTION NEEDING VALUE IS AT THE END, NO MORE FUTURE OPTIONS
			if(fieldPos != commandStringTokens.size() - 1){

				// ITERATE THROUGH AVAILABLE OPTIONS UNTIL WE HIT NEXT AVAILABLE OPTIONS FIELD
				for(unsigned int j = 0; j < optionFieldsChecker.size(); j++){

					// IF FOUND NEXT OPTION, RECURSIVELY CALL FOR NEXT PROCESS
					if(areEqualStringsIgnoreCase(commandStringTokens[i+1], get<0>(optionFieldsChecker[j]))){

						cmdParamAndOptMap = extractOptionsAndValues(cmdType, cmdParamAndOptMap, commandStringTokens, i + 1, optionFieldsChecker[j]);
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
							
							if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::TAGS) ||
								areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::REMOVETAGS)){

								string extractedTagValues = joinVector(fieldValueVector, " ");
								fieldValueVector = tokenizeCommandString(extractedTagValues, true);

								// reminder: Create a method to check for tag duplicate
								for(unsigned int m = 0; m < fieldValueVector.size() - 1; m++){

									for(unsigned int n = m + 1; n <= fieldValueVector.size() - 1; n++){

										if(areEqualStringsIgnoreCase(fieldValueVector[m], fieldValueVector[n])){

											fieldValueVector.erase( fieldValueVector.begin() + n);
											n = n - 1;

										}

									}

								}

								if(cmdType != commandTypeEnum::DELETE_TASK){
									cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, fieldValueVector) );
								}

							} else if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::START) ||
										areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::END)){

								fieldValue = joinVector(fieldValueVector, " ");

								string dtFieldValue = any_cast<string> (fieldValue);

								pair<bool,ptime> isValidDateTime = checkDateTime(dtFieldValue, true);

								if(isValidDateTime.first && cmdType != commandTypeEnum::DELETE_TASK){
									cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, isValidDateTime.second) );
								}

							} else if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::PRIORITY)){

								int fieldValue = stoi(fieldValueVector[0]);

								if(cmdType != commandTypeEnum::DELETE_TASK){
									cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, fieldValue) );
								}

							} else if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::NAME)) {

								if(cmdType == commandTypeEnum::UPDATE_TASK){
									fieldValue = joinVector(fieldValueVector, " ");
									cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::NAME, fieldValue) );
								}

							} else {

								if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::REPEAT) ||
									areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::RECURSIVE)) {

									if((cmdType == commandTypeEnum::DELETE_TASK)){
										cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::RECURSIVE, true) );
									} else {
										fieldValue = joinVector(fieldValueVector, " ");
										cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::REPEAT, fieldValue) );
									}

								} else {

									fieldValue = joinVector(fieldValueVector, " ");

									if(cmdType != commandTypeEnum::DELETE_TASK){
										cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, fieldValue) );
									}
									
								}

							}						
							
						}

						cmdParamAndOptMap = extractOptionsAndValues(cmdType, cmdParamAndOptMap, commandStringTokens, i + 1, optionFieldsChecker[j]);
						processComplete = true;
						break;

					} else {

						// IF VALUE EXTENDS TO END OF ENTERED COMMAND AND NO NEXT OPTION IS FOUND
						if( (i + 1 == commandStringTokens.size() - 1) && (j == optionFieldsChecker.size() - 1) ){

							copy(commandStringTokens.begin() + fieldPos + 1, commandStringTokens.end(), back_inserter(fieldValueVector));
							
							if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::TAGS) ||
								areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::REMOVETAGS)){

								string extractedTagValues = joinVector(fieldValueVector, " ");
								fieldValueVector = tokenizeCommandString(extractedTagValues, true);

								// reminder: Create a method to check for tag duplicate
								for(unsigned int m = 0; m < fieldValueVector.size() - 1; m++){

									for(unsigned int n = m + 1; n <= fieldValueVector.size() - 1; n++){

										if(areEqualStringsIgnoreCase(fieldValueVector[m], fieldValueVector[n])){

											fieldValueVector.erase( fieldValueVector.begin() + n);
											n = n - 1;

										}

									}

								}

								if(cmdType != commandTypeEnum::DELETE_TASK){
									cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, fieldValueVector) );
								}

							} else if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::START) ||
										areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::END)){

								fieldValue = joinVector(fieldValueVector, " ");

								string dtFieldValue = any_cast<string> (fieldValue);

								pair<bool,ptime> isValidDateTime = checkDateTime(dtFieldValue, true);

								if(isValidDateTime.first && cmdType != commandTypeEnum::DELETE_TASK){
									cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, isValidDateTime.second) );
								}

							} else if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::PRIORITY)){

								int fieldValue = stoi(fieldValueVector[0]);

								if(cmdType != commandTypeEnum::DELETE_TASK){
									cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, fieldValue) );
								}

							} else if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::NAME)) {

								if(cmdType == commandTypeEnum::UPDATE_TASK){
									fieldValue = joinVector(fieldValueVector, " ");
									cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::NAME, fieldValue) );
								}

							} else {


								if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::REPEAT) ||
									areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::RECURSIVE)) {

									if((cmdType == commandTypeEnum::DELETE_TASK)){
										cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::RECURSIVE, true) );
									} else {
										fieldValue = joinVector(fieldValueVector, " ");
										cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::REPEAT, fieldValue) );
									}

								} else {

									fieldValue = joinVector(fieldValueVector, " ");

									if(cmdType != commandTypeEnum::DELETE_TASK){
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

bool Parser::hasSuffix(string str, string suffix){

	transform(str.begin(), str.end(), str.begin(), ::tolower);
	transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);

    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}