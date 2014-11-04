#include "parser.h"
#include "executor.h"
#include "Undo.h"
#include "controller.h"
#include <boost\assign.hpp>
#include "optionField.h"
#include "ExecutionError.h"

using namespace std;
using commandTypeEnum::COMMAND_TYPE;
using namespace cmdOptionField;

//Not static in case i need to grab anything off the controller.
std::map<std::string, std::function<void(boost::any, Controller::CEvent&)>> Executor::createActions(){
	std::map<std::string, std::function<void(boost::any, Controller::CEvent&)>> actions;
	actions.insert(make_pair(START, 
		[](boost::any value, Controller::CEvent& evt){ evt.setStartDate(any_cast<ptime>(value)); }));
	actions.insert(make_pair(END, 
		[](boost::any value, Controller::CEvent& evt){ evt.setEndDate(any_cast<ptime>(value)); }));
	actions.insert(make_pair(TAGS, 
		[](boost::any value, Controller::CEvent& evt){ evt.addTags(any_cast<vector<string>>(value)); }));
	actions.insert(make_pair(PRIORITY, 
		[](boost::any value, Controller::CEvent& evt){ evt.setPriority(any_cast<int>(value)); }));
	actions.insert(make_pair(CONTENT, 
		[](boost::any value, Controller::CEvent& evt){ evt.setContent(any_cast<string>(value)); }));
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
	try{
		find_task(command);
	}
	catch(...)
	{
		return ctrl->addEvent(get<string>("param",command)).getId();
	}
	throw executionError(NAME_ALREADY_EXISTS);
}

//update_task must be able to work on both add/update commands.
void Executor::update_task(Executor::Command command, Event::UUID taskid = 0){
	try{
		if(!taskid)
			taskid = find_task(command);
	}
	catch(...)
	{
		throw executionError(CANNOT_FIND_TARGET);
	}
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
	try{
		if(!taskid)
			taskid = find_task(command);
	}
	catch(...)
	{
		throw executionError(CANNOT_FIND_TARGET);
	}
	ctrl->deleteEvent(taskid);
}

void Executor::mark_complete(Event::UUID taskid, bool recursive){
	if(recursive)
	{
		vector<Controller::CEvent> evts = ctrl->getAllEvents();
		for(auto it = evts.begin(); it!=evts.end();++it)
			if(it->getParent() == taskid)
				mark_complete(it->getId() , recursive);
	}
	Controller::CEvent& evt = ctrl->getEvent(taskid);
	evt.setCompleteStatus(true);
	evt.exec();
}

void Executor::executeCommand(Executor::Command command){
	assert(ctrl!=NULL);
	assert(get<bool>("valid", command) == true);
	
	COMMAND_TYPE cmdtype = get<COMMAND_TYPE>("cmd", command);
	Event::UUID taskid = 0; //assume nothing can have taskid of 0.
	function<void()> inverse;
	stringstream ss;
	string eventDump;
	bool isRecursive;
	switch(cmdtype){
	case COMMAND_TYPE::MARK_COMPLETE:
		try{ //TODO: clean copy paste coding.
			taskid = find_task(command);
		}
		catch(...)
		{
			throw executionError(CANNOT_FIND_TARGET);
		}
		ss<<ctrl->getEvent(taskid);
		eventDump = ss.str();
		inverse = [this, taskid, eventDump]() { 
			stringstream ss;
			ss.str(eventDump);
			ss>>ctrl->getEvent(taskid); 
		};
		isRecursive = command.find(RECURSIVE)!=command.end()  && 
			get<COMMAND_TYPE>(RECURSIVE, command);
		mark_complete(taskid, isRecursive);
		undoStack.push_back(inverse);
		break;
	case COMMAND_TYPE::UNDO:
		if(undoStack.size() == 0)
			throw executionError(NOTHING_TO_UNDO);
		undoStack.back()();
		undoStack.pop_back();
		break;
	case COMMAND_TYPE::ADD_TASK:
		//Write in terms of an add and then update.
		taskid = add_task(command);
		inverse = [this, taskid](){ ctrl->deleteEvent(taskid); };
	case COMMAND_TYPE::UPDATE_TASK:
		try{ //TODO: clean copy paste coding.
			if(!taskid)
				taskid = find_task(command);
		}
		catch(...)
		{
			throw executionError(CANNOT_FIND_TARGET);
		}
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
		try{ //TODO: clean copy paste coding.
			if(!taskid)
				taskid = find_task(command);
		}
		catch(...)
		{
			throw executionError(CANNOT_FIND_TARGET);
		}
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


std::pair<Controller::unregisterAction, string> Executor::addFilter(Command cmd){
	auto pred = get<std::function<bool(boost::any& e)>>(PREDICATE, cmd);
	string filterstring = get<string>(PARSE_STRING, cmd);
	auto undo = ctrl->addFilter([pred](Controller::CEvent e)->bool{
		return pred(boost::any(e));
	});
	undoStack.push_back(undo);
	return make_pair(undo, filterstring);
}
vector<Controller::CEvent> Executor::search(Command cmd){
	auto pred = get<std::function<bool(boost::any& e)>>(PREDICATE, cmd);
	auto evts = ctrl->getAllEvents();
	vector<Controller::CEvent> newevts;
	for(auto it = evts.begin(); it!=evts.end();++it)
	{
		if(pred(boost::any(*it)))
			newevts.push_back(*it);
	}
	return newevts;
}