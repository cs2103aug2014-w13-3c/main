#include "parser.h"
#include "executor.h"
#include "Undo.h"
#include "controller.h"
#include <boost\assign.hpp>
#include "optionField.h"

using namespace std;
using CommandTypeEnum::COMMAND_TYPE;
using namespace cmdOptionField;

//Not static in case i need to grab anything off the controller.
std::map<std::string, std::function<void(boost::any, Controller::CEvent&)>> Executor::createActions(){
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


template<typename T>
T get(std::string key, Executor::Command command){
	return any_cast<T>((*command.find(key)).second);
}

Executor::Executor(Controller* ptr): ctrl(ptr), undoStack()
{
	actions = createActions();
}

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
	function<void()> inverse;
	stringstream ss;
	string eventDump;
	switch(cmdtype){
	case COMMAND_TYPE::UNDO:
		undoStack.back()();
		undoStack.pop_back();
		break;
	case COMMAND_TYPE::ADD_TASK:
		//Write in terms of an add and then update.
		taskid = add_task(command);
		inverse = [this, taskid](){ ctrl->deleteEvent(taskid); };
	case COMMAND_TYPE::UPDATE_TASK:
		if(!taskid)
			taskid = find_task(command);
		ss<<ctrl->getEvent(taskid);
		eventDump = ss.str();
		inverse = (cmdtype == COMMAND_TYPE::UPDATE_TASK)? 
			[this, taskid, eventDump]() { 
				stringstream ss;
				ss.str(eventDump);
				ss>>ctrl->getEvent(taskid); 
			}: inverse;
		update_task(command, taskid);
		undoStack.push_back(inverse);
		break;
	case COMMAND_TYPE::DELETE_TASK:
		if(!taskid)
			taskid = find_task(command);
		ss<<ctrl->getEvent(taskid);
		eventDump = ss.str();
		delete_task(command);
		//TODO: implement inverse.
		inverse = [this, eventDump](){
			stringstream ss;
			ss.str(eventDump);
			ss>>(ctrl->addEvent("placeholder"));
		};
		undoStack.push_back(inverse);
		break;
	default:
		assert(false); //Unimplemented.
	}
}
