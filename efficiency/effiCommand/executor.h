#ifndef EXECUTOR_H_
#define EXECUTOR_H_

#include <string>
#include <vector>
#include <map>

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/any.hpp"
using namespace boost;
using namespace boost::posix_time;
using namespace std;

class Executor{
public:

	// execute a given command string
	// NOTE TO SELF: TO BE CALLED BY UI CONTROLLER
	static void executeCommand(multimap<string,any> processedCommand);

private:

	static CommandTypeEnum::COMMAND_TYPE getCommandType(multimap<string,any> processedCommand);
	static string getParameters(multimap<string,any> processedCommand);
	static ptime getStartOption(multimap<string,any> processedCommand);
	static ptime getEndOption(multimap<string,any> processedCommand);
	static int getPriorityOption(multimap<string,any> processedCommand);
	static string isRecursiveOption(multimap<string,any> processedCommand);
	static vector<string> getTagOption(multimap<string,any> processedCommand);
	static string getLinkOption(multimap<string,any> processedCommand);
	static bool getHelpOption(multimap<string,any> processedCommand);

	static string addTask();
	static string deleteTask();

};
#endif