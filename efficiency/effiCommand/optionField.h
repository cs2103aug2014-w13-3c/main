#ifndef OPTIONFIELD_H_
#define OPTIONFIELD_H_

#include <string>

using namespace std;

namespace cmdOptionField {

	static const string VALID = "valid";
	static const string COMMAND = "cmd";
	static const string PARAMETERS = "param";
	static const string START_OPTION = "-s";
	static const string END_OPTION = "-e";
	static const string PRIORITY_OPTION = "-p";
	static const string REPEAT_OPTION = "-r";
	static const string RECURSIVE_OPTION = "-dr";
	static const string TAG_OPTION = "-t";
	static const string LINK_OPTION = "-l";
	static const string HELP_OPTION = "-h";

	static const string EMPTY_FIELD = "-!";

};

#endif