#include "parser.h"
#include "executor.h"
#include "Undo.h"

using namespace std;

multimap<string, any> Executor::getProcessedCommandContents(Parser processedCommand){

	return processedCommand.getCommandContents();

}

CommandTypeEnum::COMMAND_TYPE Executor::getCommandType(multimap<string,any> processedCommand){

	return any_cast<CommandTypeEnum::COMMAND_TYPE>(processedCommand.find(cmdOptionField::COMMAND)->second);

}

string Executor::getParameters(multimap<string,any> processedCommand){

	return any_cast<string>(processedCommand.find(cmdOptionField::PARAMETERS)->second);

}

ptime Executor::getStartOption(multimap<string,any> processedCommand){

	return any_cast<ptime>(processedCommand.find(cmdOptionField::START_OPTION)->second);
}

ptime Executor::getEndOption(multimap<string,any> processedCommand){

	return any_cast<ptime>(processedCommand.find(cmdOptionField::END_OPTION)->second);

}

int Executor::getPriorityOption(multimap<string,any> processedCommand){

	return any_cast<int>(processedCommand.find(cmdOptionField::PRIORITY_OPTION)->second);

}

string Executor::isRecursiveOption(multimap<string,any> processedCommand){

	return any_cast<string>(processedCommand.find(cmdOptionField::RECURSIVE_OPTION)->second);

}

vector<string> Executor::getTagOption(multimap<string,any> processedCommand){

	return any_cast<vector<string>>(processedCommand.find(cmdOptionField::TAG_OPTION)->second);

}

string Executor::getLinkOption(multimap<string,any> processedCommand){

	return any_cast<string>(processedCommand.find(cmdOptionField::LINK_OPTION)->second);

}

bool Executor::getHelpOption(multimap<string,any> processedCommand){

	return any_cast<bool>(processedCommand.find(cmdOptionField::HELP_OPTION)->second);

}

void Executor::executeCommand(Parser processedCommand){

	multimap<string,any> processedCommandContents = getProcessedCommandContents(processedCommand);

}

string Executor::addTask(){

	return "task added";

}

string Executor::deleteTask(){

	return "task deleted";

}