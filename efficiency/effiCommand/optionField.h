#ifndef OPTIONFIELD_H_
#define OPTIONFIELD_H_

#include <string>

using namespace std;

// lists of all the key available for the multimap to be output for executor
// and uiController to be processed are here
namespace cmdOptionField {

	const string VALID = "valid";

	const string COMMAND = "cmd";
	const string PARAMETERS = "param";

	const string FAIL = "fail";

	const string START = "start";
	const string END = "end";
	const string CONTENT = "content";
	const string PRIORITY = "priority";
	const string REPEAT = "repeat";
	const string RECURSIVE = "recursive";
	const string TAGS = "tags";
	const string REMOVETAGS = "removetags";
	const string NAME = "name";
	const string LINK = "link";
	const string HELP = "help";
	const string PREDICATE = "predicate";
	const string PARSE_STRING = "parse_string";

};

#endif