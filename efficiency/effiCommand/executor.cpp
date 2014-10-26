#include "parser.h"
#include "executor.h"
#include "Undo.h"
#include "controller.h"
#include <boost\assign.hpp>
#include "optionField.h"

using namespace std;
using CommandTypeEnum::COMMAND_TYPE;
using namespace cmdOptionField;

std::map<std::string, std::function<void(boost::any, Controller::CEvent&)>> createActions(){
	std::map<std::string, std::function<void(boost::any, Controller::CEvent&)>> actions;
	actions.insert(make_pair(START_OPTION, 
		[](boost::any value, Controller::CEvent& evt){ evt.setStartDate(any_cast<ptime>(value)); }));
	actions.insert(make_pair(END_OPTION, 
		[](boost::any value, Controller::CEvent& evt){ evt.setEndDate(any_cast<ptime>(value)); }));
	actions.insert(make_pair(TAG_OPTION, 
		[](boost::any value, Controller::CEvent& evt){ evt.addTags(any_cast<vector<string>>(value)); }));
	actions.insert(make_pair(PRIORITY_OPTION, 
		[](boost::any value, Controller::CEvent& evt){ evt.setPriority(any_cast<int>(value)); }));
	return actions;
}

std::map<std::string, std::function<void(boost::any, Controller::CEvent&)>> Executor::actions
	= createActions();

template<typename T>
T get(std::string key, Executor::Command command){
	return any_cast<T>((*command.find(key)).second);
}

Executor::Executor(Controller* ptr): ctrl(ptr)
{}

Event::UUID Executor::find_task(Executor::Command command){
	string param = get<string>("param",command);
	//TODO: if its ID?
	return ctrl->getEventByName(param).getId();
}

Event::UUID Executor::add_task(Executor::Command command){
	return ctrl->addEvent(get<string>("param",command)).getId();
}

//update_task must be able to work on both add/update commands.
void Executor::update_task(Executor::Command command, Event::UUID taskid = 0){
	if(!taskid)
		taskid = find_task(command);
	Controller::CEvent& evt = ctrl->getEvent(taskid);
	for(auto it = command.begin(); it!=command.end();++it)
	{
		string field = it->first;
		boost::any value = it->second;
		auto action = actions.find(field);
		if(action != actions.end())
			action->second(value, evt);
	}
	evt.exec();
}

void Executor::delete_task(Executor::Command command, Event::UUID taskid = 0)
{
	if(!taskid)
		taskid = find_task(command);
	ctrl->deleteEvent(taskid);
}

void Executor::executeCommand(Executor::Command command){
	assert(ctrl!=NULL);
	assert(get<bool>("valid", command) == true);
	
	COMMAND_TYPE cmdtype = get<COMMAND_TYPE>("cmd", command);
	Event::UUID taskid = 0; //assume nothing can have taskid of 0.
	switch(cmdtype){
	case COMMAND_TYPE::ADD_TASK:
		//Write in terms of an add and then update.
		taskid = add_task(command);
	case COMMAND_TYPE::UPDATE_TASK:
		update_task(command, taskid);
		break;
	case COMMAND_TYPE::DELETE_TASK:
		delete_task(command);
		break;
	default:
		assert(false); //Unimplemented.
	}
}
