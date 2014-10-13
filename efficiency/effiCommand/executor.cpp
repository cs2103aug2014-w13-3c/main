#include "commandTypeEnum.h"
#include "optionField.h"
#include "parser.h"
#include "executor.h"
#include "Undo.h"

using namespace std;

CommandTypeEnum::COMMAND_TYPE Executor::getCommandType(multimap<string,any> processedCommand){

	return any_cast<CommandTypeEnum::COMMAND_TYPE>(processedCommand.find(optionField::COMMAND)->second);

}

string Executor::getParameters(multimap<string,any> processedCommand){

	return any_cast<string>(processedCommand.find(optionField::PARAMETERS)->second);

}

ptime Executor::getStartOption(multimap<string,any> processedCommand){

	return any_cast<ptime>(processedCommand.find(optionField::START_OPTION)->second);
}

ptime Executor::getEndOption(multimap<string,any> processedCommand){

	return any_cast<ptime>(processedCommand.find(optionField::END_OPTION)->second);

}

int Executor::getPriorityOption(multimap<string,any> processedCommand){

	return any_cast<int>(processedCommand.find(optionField::PRIORITY_OPTION)->second);

}

string Executor::isRecursiveOption(multimap<string,any> processedCommand){

	return any_cast<string>(processedCommand.find(optionField::RECURSIVE_OPTION)->second);

}

vector<string> Executor::getTagOption(multimap<string,any> processedCommand){

	return any_cast<vector<string>>(processedCommand.find(optionField::TAG_OPTION)->second);

}

string Executor::getLinkOption(multimap<string,any> processedCommand){

	return any_cast<string>(processedCommand.find(optionField::LINK_OPTION)->second);

}

bool Executor::getHelpOption(multimap<string,any> processedCommand){

	return any_cast<bool>(processedCommand.find(optionField::HELP_OPTION)->second);

}

void Executor::executeCommand(multimap<string,any> processedCommand){

}

string Executor::addTask(){

	return "task added";

}

string Executor::deleteTask(){

	return "task deleted";

}