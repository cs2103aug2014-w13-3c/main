#include "parser.h"
#include "predParser.h"

using namespace std;

//@author A0098802X
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

//@author A0098802X
// stores all valid command keywords
void Parser::loadValidCommandKeywords(){

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

	validCommandKeywords.push_back(make_pair("search",commandTypeEnum::SEARCH));
	validCommandKeywords.push_back(make_pair("/s",commandTypeEnum::SEARCH));

	validCommandKeywords.push_back(make_pair("filter",commandTypeEnum::FILTER));
	validCommandKeywords.push_back(make_pair("/f",commandTypeEnum::FILTER));

	validCommandKeywords.push_back(make_pair("help",commandTypeEnum::HELP));
	validCommandKeywords.push_back(make_pair("/h",commandTypeEnum::HELP));
	validCommandKeywords.push_back(make_pair("/?",commandTypeEnum::HELP));

	validCommandKeywords.push_back(make_pair("sort",commandTypeEnum::SORT));
	validCommandKeywords.push_back(make_pair("/st",commandTypeEnum::SORT));

	validCommandKeywords.push_back(make_pair("exit",commandTypeEnum::EXIT));
	validCommandKeywords.push_back(make_pair("/e",commandTypeEnum::EXIT));

}

//@author A0098802X
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
	optionFieldsChecker.push_back(make_tuple("recursive", cmdOptionField::RECURSIVE,true));
	optionFieldsChecker.push_back(make_tuple("-r", cmdOptionField::REPEAT,true));
	optionFieldsChecker.push_back(make_tuple("-r", cmdOptionField::RECURSIVE,true));

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

//@author A0098802X
void Parser::loadViewToScrollAndSort(){

	viewToScrollAndSort.push_back("all");
	viewToScrollAndSort.push_back("task");
	viewToScrollAndSort.push_back("event");
	viewToScrollAndSort.push_back("deadline");
	viewToScrollAndSort.push_back("tasks");
	viewToScrollAndSort.push_back("events");
	viewToScrollAndSort.push_back("deadlines");

}

//@author A0098802X
void Parser::loadScrollDirection(){

	scrollDirection.push_back("next");
	scrollDirection.push_back("previous");
	scrollDirection.push_back("prev");

}

//@author A0098802X
void Parser::loadSortByType(){

	sortByType.push_back("ID");
	sortByType.push_back("IDs");
	sortByType.push_back("name");
	sortByType.push_back("names");
	sortByType.push_back("start");
	sortByType.push_back("end");
	sortByType.push_back("tag");
	sortByType.push_back("tags");
	sortByType.push_back("content");
	sortByType.push_back("contents");

}

//@author A0098802X
pair<bool,ptime> Parser::checkDateTime(string dtFieldValue, bool firstRun){

	vector<string> dtToken = tokenizeCommandString(dtFieldValue, false);

	pair<bool,ptime> result;
	result.first = true;

	if(isEnteredDateAndTime(dtToken)){

		string time = dtToken[1];

		replace(time.begin(), time.end(), ':', ' ');
		replace(time.begin(), time.end(), '.', ' ');

		vector<string> timeToken = tokenizeCommandString(time, false);

		int hour = 0;
		int minute = 0;
		int second = 0;

		result = standardizeTo24Hour(timeToken, hour, minute, result, second);

		char buff[100];
		sprintf_s(buff, "%02d:%02d:%02d", hour, minute, second);
		time = buff;

		dtToken[1] = time;

	}

	dtToken[0] = addPaddingZeros(dtToken[0]);
	dtFieldValue = joinVector(dtToken, " ");

	try{
		auto date = parseDate(dtFieldValue);
		result.second = date;
		return result;
	}catch(...){
		result.first = false;
		return result;
	}
}

//@author A0098802X
bool Parser::isEnteredDateAndTime(vector<string> &dtToken){
	return dtToken.size() > 1;
}

//@author A0098802X
pair<bool, ptime> Parser::standardizeTo24Hour(vector<string> &timeToken, int &hour, int &minute, pair<bool,ptime> &result, int &second){
	// process 24-hour and 12-hour format which has 3 values
	// e.g 12:00:00pm and 23:59:59
	if(isLongTimeFormat(timeToken)){

		try{
			hour = stoi(timeToken[0]);
			minute = stoi(timeToken[1]);
		} catch(...) {
			result.first = false;
			return result;
		}

		if(isExistSecond(timeToken)){
			second = stoi(timeToken[2]);;
		}

		if ( hasSuffix(timeToken[timeToken.size() - 1],"noon") ||
			hasSuffix(timeToken[timeToken.size() - 1],"PM") || 
			hasSuffix(timeToken[timeToken.size() - 1],"AM") ){

				if(isPM(timeToken)){			
					hour = (hour % 12) + 12;
				}

				timeToken[0] = to_string(hour);

		}

		return result;

		// for 12-hour format only which is short form
		// e.g 4pm, 5am, 14pm, 3pm
	} else if(isShortFormTimeFormat(timeToken)) {

		string hourString = timeToken[0].substr(0, timeToken[0].rfind("AM"));
		hourString = timeToken[0].substr(0, timeToken[0].rfind("PM"));
		hourString = timeToken[0].substr(0, timeToken[0].rfind("noon"));

		try{
			hour = stoi(hourString);
		} catch (...){
			result.first = false;
			return result;
		}

		if(isPM(timeToken)){				
			hour = (hour % 12) + 12;
		}

		timeToken[0] = to_string(hour);

		return result;

	}
}

//@author A0098802X
bool Parser::isLongTimeFormat(vector<string> &timeToken){
	return timeToken.size() >= 2 && timeToken.size() <= 3;
}

//@author A0098802X
bool Parser::isShortFormTimeFormat(vector<string> &timeToken){
	return timeToken.size() == 1;
}

//@author A0098802X
bool Parser::isExistSecond(vector<string> &timeToken){
	return timeToken.size() == 3;
}

//@author A0098802X
bool Parser::isPM(vector<string> &timeToken){
	return hasSuffix(timeToken[timeToken.size() - 1],"PM");
}

//@author A0098802X
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

//@author A0098802X
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

//@author A0098802X
// Constructor
Parser::Parser(){

	loadValidCommandKeywords();
	loadOptionFieldsChecker();
	loadViewToScrollAndSort();
	loadScrollDirection();

}

//@author A0098802X
// Destructor
Parser::~Parser(){

}

//@author A0098802X
// API to parse command
multimap<string, any> Parser::parseCommand(const string commandString){

	if(isEmptyCommand(commandString)){
		throw "No command found.";
	}

	vector<string> commandStringTokens = tokenizeCommandString(commandString, false);
	return checkCommandSyntax(commandStringTokens);

}

//@author A0098802X
bool Parser::isEmptyCommand(const string commandString){
	return trim(commandString) == "";
}

//@author A0098802X
// Check the entire command syntax
multimap<string, any> Parser::checkCommandSyntax(vector<string> commandStringTokens){

	// Init Enum for identification
	commandTypeEnum::COMMAND_TYPE cmdType;
	// Final multimap result
	multimap<string,any> cmdParamAndOptMap;
	cmdParamAndOptMap = checkCommandKeyword(commandStringTokens, cmdParamAndOptMap, cmdType);

	if( isFalseValidKey(cmdParamAndOptMap)){
		return cmdParamAndOptMap;
	}

	// Assume user has not entered any options field for commands requiring it
	bool noOptionsInUserInput = true;

	switch (cmdType){
	// has mandatory parameter and optional option fields
	case commandTypeEnum::ADD_TASK:
	case commandTypeEnum::DELETE_TASK:	
	case commandTypeEnum::UPDATE_TASK:		

		// Iterate through entire user command input except processed command keyword
		for(unsigned int i = 1; i < commandStringTokens.size(); i++){
			// iterate through lists of available option fields
			for(unsigned int j = 0; j < optionFieldsChecker.size(); j++){
				if(isMissingCommandParameters(i, j, commandStringTokens)){
					cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false) );
					return cmdParamAndOptMap;
				// OPTIONS MATCH
				} else if(areEqualStringsIgnoreCase(commandStringTokens[i], get<0>(optionFieldsChecker[j]) )){

					// IF command has no options
					if (noOptionsInUserInput){
						noOptionsInUserInput = false;
						vector<string> extractParam;
						copy(commandStringTokens.begin() + 1, commandStringTokens.begin() + i, back_inserter(extractParam));
						string Param = joinVector(extractParam, " ");
						cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::PARAMETERS, Param));
					}

					// check if the option fields and their values are valid, and if valid, map into final multimap
					cmdParamAndOptMap = extractOptionsAndValues(cmdType, cmdParamAndOptMap, commandStringTokens, i , optionFieldsChecker[j], true);	
					
					// if no option field is invalid, mark as valid
					if(cmdParamAndOptMap.count("valid") == 0){
						cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::VALID, true) );
					}

					return cmdParamAndOptMap;

				}
			}
		}

		// If the given user input has no option field, process parameters only
		if(noOptionsInUserInput){
			 
			vector<string> extractParam;
			copy(commandStringTokens.begin() + 1, commandStringTokens.end(), back_inserter(extractParam));
			
			// If parameter is not found, user input is invalid
			if(extractParam.empty()){
				cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::VALID, false) );
				return cmdParamAndOptMap;
			}

			// otherwise, extract parameter and mark map as true
			string Param = joinVector(extractParam, " ");
			cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::PARAMETERS, Param) );
			cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::VALID, true) );
			return cmdParamAndOptMap;

		}

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
					if(isMissingCommandParameters(i, j, commandStringTokens)){

						cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false) );
						return cmdParamAndOptMap;

					} else if (areEqualStringsIgnoreCase(commandStringTokens[i], get<0>(optionFieldsChecker[j]) )) {

						vector<string> extractParam;
						copy(commandStringTokens.begin() + 1, commandStringTokens.begin() + i, back_inserter(extractParam));
						string Param = joinVector(extractParam, " ");

						if(isRecursiveField(j)) {

							cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::PARAMETERS, Param));
							cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, true) );	
							cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::RECURSIVE, true) );

						} else {

							cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::PARAMETERS, Param));
							cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false) );	

						}

						return cmdParamAndOptMap;

					} else if(isNotFoundOptionField(i, j, commandStringTokens)) {

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
	case commandTypeEnum::SCROLL:

		if(isInsufficientParameters(commandStringTokens)){

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

			for(unsigned int i = 0; i < viewToScrollAndSort.size(); i++) {

				if( areEqualStringsIgnoreCase(commandStringTokens[1], viewToScrollAndSort[i]) ) {
					break;
				} else if ( i == viewToScrollAndSort.size() - 1 ) {
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

	case commandTypeEnum::SORT:

		if(isInsufficientParameters(commandStringTokens)){

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

			for(unsigned int i = 0; i < viewToScrollAndSort.size(); i++) {

				if( areEqualStringsIgnoreCase(commandStringTokens[1], viewToScrollAndSort[i]) ) {
					break;
				} else if ( i == viewToScrollAndSort.size() - 1 ) {
					cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false) );
					return cmdParamAndOptMap;
				}

			}

			for(unsigned int i = 0; i < sortByType.size(); i++) {

				if( areEqualStringsIgnoreCase(commandStringTokens[2], sortByType[i]) ) {
					break;
				} else if ( i == sortByType.size() - 1 ) {
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
	case commandTypeEnum::HELP:
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

//@author A0098802X
multimap<string, any> Parser::checkCommandKeyword(vector<string> commandStringTokens, multimap<string,any> &cmdParamAndOptMap, commandTypeEnum::COMMAND_TYPE &cmdType)
{
	// Iterate through the list of all valid command keywords
	for(unsigned int i = 0; i < validCommandKeywords.size(); i++){

		// found available command
		if( areEqualStringsIgnoreCase(commandStringTokens[0], validCommandKeywords[i].first) ){
			cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::COMMAND, validCommandKeywords[i].second));
			cmdType = validCommandKeywords[i].second;
			return cmdParamAndOptMap;

			// reached end of list and no valid command keyword is found
		} else if(isInvalidCommandKeyword(i)){
			cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::VALID, false));
			return cmdParamAndOptMap;
		}

	}
}

//@author A0098802X
bool Parser::isFalseValidKey(multimap<string,any> &cmdParamAndOptMap){
	return cmdParamAndOptMap.count("valid") != 0 &&
		any_cast<bool> ( cmdParamAndOptMap.find("valid")->second ) == false;
}

//@author A0098802X
bool Parser::isInvalidCommandKeyword(unsigned int i){
	return i == validCommandKeywords.size() - 1;
}

//@author A0098802X
bool Parser::isMissingCommandParameters(unsigned int i, unsigned int j, vector<string> commandStringTokens){
	return i == 1 && areEqualStringsIgnoreCase(commandStringTokens[i], get<0>(optionFieldsChecker[j]) );
}

//@author A0098802X
bool Parser::isRecursiveField(unsigned int j){
	return (areEqualStringsIgnoreCase("recursive", get<0>(optionFieldsChecker[j]) ) || 
		( areEqualStringsIgnoreCase("-r", get<0>(optionFieldsChecker[j])) ) );
}

//@author A0098802X
bool Parser::isNotFoundOptionField(unsigned int i, unsigned int j, vector<string> &commandStringTokens){
	return (i == commandStringTokens.size() - 1) && (j == optionFieldsChecker.size() - 1);
}

//@author A0098802X
bool Parser::isInsufficientParameters(vector<string> &commandStringTokens)
{
	return commandStringTokens.size() != 3;
}

//@author A0098802X
multimap<string, any> Parser::extractOptionsAndValues(commandTypeEnum::COMMAND_TYPE cmdType,
									 multimap<string, any> cmdParamAndOptMap, 
									 vector<string> commandStringTokens, 
									 int fieldPos, 
									 tuple<string, string, bool> currentOptionFieldTuple,
									 bool validUntilNow){

	// take current option field
	string currentOptionField = get<0> (currentOptionFieldTuple);

	// obtain option key for multimap for the option field
	string mmOptionKey = get<1> (currentOptionFieldTuple);

	// obtain to see if such option field has field value
	bool hasFieldValueFormat = get<2> (currentOptionFieldTuple);

	any fieldValue;
	vector<string> fieldValueVector; // to tokenize field values

	bool hasFieldValueEntered = false; //flag to check if field value is enterd

	bool processComplete = false; //terminate this iteration of function if current field is processed

	// if the found option doesn't need any values, simply return as true
	if(!hasFieldValueFormat){

		cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, true) );

		// from the option field's index, iterate to find next option field
		for(unsigned int i = fieldPos; i < commandStringTokens.size() - 1; i++){

			// if the current option field index is the end of command, no more option fields
			if(fieldPos != commandStringTokens.size() - 1){

				// check if there is next option field
				for(unsigned int j = 0; j < optionFieldsChecker.size(); j++){

					// if found next option, recursively invoke the function itself to process it
					if(areEqualStringsIgnoreCase(commandStringTokens[i+1], get<0>(optionFieldsChecker[j]))){
						if(validUntilNow){
							cmdParamAndOptMap = extractOptionsAndValues(cmdType, cmdParamAndOptMap, commandStringTokens, i + 1, optionFieldsChecker[j], true);
						}
						processComplete = true;
						break;
					}

				}

			}

			if(processComplete){
				break;
			}

		}	

	// if the option field has value, process it according to the current command keyword and 
	// the option field's format
	} else {

		// obtain option field's value from the field's position
		for(unsigned int i = fieldPos; i < commandStringTokens.size() - 1; i++){

			// if the current option field is at end of command, it is invalid
			if(fieldPos != commandStringTokens.size() - 1){

				// iterate to see if there is next option field
				for(unsigned int j = 0; j < optionFieldsChecker.size(); j++){

					// if next option found, pre-process
					if(areEqualStringsIgnoreCase(commandStringTokens[i+1], get<0>(optionFieldsChecker[j]))){

						if(hasFieldValueEntered){

							copy(commandStringTokens.begin() + fieldPos + 1, commandStringTokens.begin() + (i+1), back_inserter(fieldValueVector));
							
							// if option field is either tags or remove tags
							if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::TAGS) ||
								areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::REMOVETAGS)){

								string extractedTagValues = joinVector(fieldValueVector, " ");
								fieldValueVector = tokenizeCommandString(extractedTagValues, true);

								checkForDuplicateTags(fieldValueVector);

								// These fields cannot run on delete command
								if(cmdType != commandTypeEnum::DELETE_TASK){
									cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, fieldValueVector) );
								} else {
									cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::VALID, false) );
									validUntilNow = false;
								}

							// option field is start date/time or end date/time
							} else if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::START) ||
										areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::END)){

								fieldValue = joinVector(fieldValueVector, " ");

								string dtFieldValue = any_cast<string> (fieldValue);

								pair<bool,ptime> isValidDateTime = checkDateTime(dtFieldValue, true);

								// option fields must not run in delete command
								if(isValidDateTime.first && cmdType != commandTypeEnum::DELETE_TASK){
									cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, isValidDateTime.second) );
								} else {
									cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::VALID, false) );
									validUntilNow = false;
								}

							// option field is priority
							} else if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::PRIORITY)){

								int fieldValue = stoi(fieldValueVector[0]);

								// option fields must not run in delete command
								if(cmdType != commandTypeEnum::DELETE_TASK){
									cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, fieldValue) );
								} else {
									cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::VALID, false) );
									validUntilNow = false;
								}
							
							// update task name
							} else if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::NAME)) {

								// option fields can only be run on update command
								if(cmdType == commandTypeEnum::UPDATE_TASK){
									fieldValue = joinVector(fieldValueVector, " ");
									cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::NAME, fieldValue) );
								} else {
									cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::VALID, false) );
									validUntilNow = false;
								}

							} else {

								// option field is repeat or recursive
								if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::REPEAT) ||
									areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::RECURSIVE)) {

									// must only be run on delete command
									if((cmdType == commandTypeEnum::DELETE_TASK)){
										cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::RECURSIVE, true) );
									} else {
										fieldValue = joinVector(fieldValueVector, " ");
										cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::REPEAT, fieldValue) );
									}

								} else {

									fieldValue = joinVector(fieldValueVector, " ");

									// must not run on delete command
									if(cmdType != commandTypeEnum::DELETE_TASK){
										cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, fieldValue) );
									} else {
										cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::VALID, false) );
										if(validUntilNow){
											cmdParamAndOptMap = extractOptionsAndValues(cmdType, cmdParamAndOptMap, commandStringTokens, i + 1, optionFieldsChecker[j], true);
										}
									}								
								}
							}												
						}

						//continue extracting next option field if the option fields processed are valid until now
						if(validUntilNow){
							cmdParamAndOptMap = extractOptionsAndValues(cmdType, cmdParamAndOptMap, commandStringTokens, i + 1, optionFieldsChecker[j], true);
						}
						processComplete = true;
						break;

					// no matching option field
					} else {

						// if value extends to the end of the user input and no further option fields are found
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
									} else {
										cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::VALID, false) );
										validUntilNow = false;
									}

							} else if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::START) ||
								areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::END)){

									fieldValue = joinVector(fieldValueVector, " ");

									string dtFieldValue = any_cast<string> (fieldValue);

									pair<bool,ptime> isValidDateTime = checkDateTime(dtFieldValue, true);

									if(isValidDateTime.first && cmdType != commandTypeEnum::DELETE_TASK){
										cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, isValidDateTime.second) );
									} else {
										cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::VALID, false) );
										validUntilNow = false;
									}

							} else if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::PRIORITY)){

								int fieldValue = stoi(fieldValueVector[0]);

								if(cmdType != commandTypeEnum::DELETE_TASK){
									cmdParamAndOptMap.insert( pair<string,any> (mmOptionKey, fieldValue) );
								} else {
									cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::VALID, false) );
									validUntilNow = false;
								}

							} else if (areEqualStringsIgnoreCase(mmOptionKey, cmdOptionField::NAME)) {

								if(cmdType == commandTypeEnum::UPDATE_TASK){
									fieldValue = joinVector(fieldValueVector, " ");
									cmdParamAndOptMap.insert( pair<string,any> (cmdOptionField::NAME, fieldValue) );
								} else {
									cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::VALID, false) );
									validUntilNow = false;
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
									} else {
										cmdParamAndOptMap.insert( pair<string, any>(cmdOptionField::VALID, false) );
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

//@author A0098802X
void Parser::checkForDuplicateTags(vector<string> &fieldValueVector){
	for(unsigned int m = 0; m < fieldValueVector.size() - 1; m++){
		for(unsigned int n = m + 1; n <= fieldValueVector.size() - 1; n++){
			if(areEqualStringsIgnoreCase(fieldValueVector[m], fieldValueVector[n])){
				fieldValueVector.erase( fieldValueVector.begin() + n);
				n = n - 1;
			}
		}
	}
}

//@author A0098802X
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

//@author A0098802X
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

//@author A0098802X
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

//@author A0098802X
inline string Parser::trimRight(const string& s, const string& delimiters){
	return s.substr( 0, s.find_last_not_of( delimiters ) + 1 );
}

//@author A0098802X
inline string Parser::trimLeft(const string& s, const string& delimiters){
	return s.substr( s.find_first_not_of( delimiters ) );
}

//@author A0098802X
inline string Parser::trim(const string& s, const string& delimiters){

	if (!s.empty()){
		return trimLeft(trimRight(s, delimiters), delimiters);		
	}

	return s;

}

//@author A0098802X
bool Parser::hasSuffix(string str, string suffix){

	transform(str.begin(), str.end(), str.begin(), ::tolower);
	transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);

    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}
