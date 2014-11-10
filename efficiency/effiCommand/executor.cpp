#include "parser.h"
#include "executor.h"
#include "Undo.h"
#include "controller.h"
#include "optionField.h"
#include "ExecutionError.h"
#include <regex>

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
	actions.insert(make_pair(NAME, 
		[](boost::any value, Controller::CEvent& evt){ evt.setName(any_cast<string>(value)); }));
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

Executor::Executor(Controller* ptr): ctrl(ptr), undoStack(), actions(createActions())
{}

Event::UUID Executor::find_task(Executor::Command command){
	string param = get<string>("param",command);
	regex extract_id("#(\\d+)");
	std::smatch results;
	std::regex_search (param, results, extract_id);
	if(results.ready() && results.prefix() == "" && results.suffix() == "") //Assume trimmed by parser.
		return ctrl->getEvent(atoi(string(results[1]).c_str())).getId(); //ensure that it exists.
	else
		return ctrl->getEventByName(param).getId();
}

std::pair<Event::UUID, Executor::InverseFunction> Executor::add_task(Executor::Command command){
	try{
		find_task(command);
	}
	catch(...)
	{
		auto id = ctrl->addEvent(get<string>("param",command)).getId();
		auto inverse = [this, id](){ ctrl->deleteEvent(id); };
		return make_pair(id, inverse);
	}
	throw executionError(NAME_ALREADY_EXISTS);
}

Executor::InverseFunction Executor::makeUpdateInverse(Controller::CEvent &evt){
	stringstream ss;
	ss<<evt;
	Event::UUID taskid = evt.getId();
	auto eventDump = ss.str();
	return [this, taskid, eventDump]() { 
				stringstream ss;
				ss.str(eventDump);
				ss>>ctrl->getEvent(taskid); 
		};
}

Executor::InverseFunction Executor::makeDeleteInverse(Controller::CEvent &evt){
	stringstream ss;
	ss<<evt;
	auto eventDump = ss.str();
	return [this, eventDump](){
			stringstream ss;
			ss.str(eventDump);
			ss>>(ctrl->addEvent("placeholder"));
		};
}


//update_task must be able to work on both add/update commands.
Executor::InverseFunction Executor::update_task(Executor::Command command, Event::UUID taskid = 0){
	try{
		if(!taskid)
			taskid = find_task(command);
	}
	catch(...)
	{
		throw executionError(CANNOT_FIND_TARGET);
	}
	auto inverse = makeUpdateInverse(ctrl->getEvent(taskid));
		
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
	return inverse;
}

Executor::InverseFunction Executor::delete_task(Executor::Command command, Event::UUID taskid = 0)
{
	try{
		if(!taskid)
			taskid = find_task(command);
	}
	catch(...)
	{
		throw executionError(CANNOT_FIND_TARGET);
	}
	auto inverse = makeDeleteInverse(ctrl->getEvent(taskid));
	ctrl->deleteEvent(taskid);
	return inverse;
}

Executor::InverseFunction Executor::mark_complete(Executor::Command command)
{
	Event::UUID taskid = find_task(command);
	bool isRecursive = command.find(RECURSIVE)!=command.end()  && 
				get<COMMAND_TYPE>(RECURSIVE, command);
	bool setting = get<COMMAND_TYPE>("cmd", command) == COMMAND_TYPE::MARK_COMPLETE;
	return mark_complete(taskid, isRecursive, setting);
}
Executor::InverseFunction Executor::mark_complete(Event::UUID taskid, bool recursive, bool setting){
	auto thisInverse = makeUpdateInverse(ctrl->getEvent(taskid));
	vector<InverseFunction> childInverses;
	if(recursive)
	{
		vector<Controller::CEvent> evts = ctrl->getAllEvents();
		for(auto it = evts.begin(); it!=evts.end();++it)
			if(it->getParent() == taskid)
				childInverses.push_back(mark_complete(it->getId() , recursive, setting));
	}
	Controller::CEvent& evt = ctrl->getEvent(taskid);
	evt.setCompleteStatus(setting);
	evt.exec();
	return [childInverses, thisInverse]() { 
		for(InverseFunction child: childInverses)
			child();
		thisInverse();
	};
}

void Executor::executeCommand(Executor::Command command){
	assert(ctrl!=NULL);
	assert(get<bool>("valid", command) == true);
	
	COMMAND_TYPE cmdtype = get<COMMAND_TYPE>("cmd", command);
	Event::UUID taskid = 0; //assume nothing can have taskid of 0.
	function<void()> inverse;
	switch(cmdtype){
	case COMMAND_TYPE::MARK_COMPLETE:
	case COMMAND_TYPE::MARK_INCOMPLETE:
		inverse = mark_complete(command);
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
		{
			auto result = add_task(command);
			taskid = result.first;
			inverse = result.second;
		}
	case COMMAND_TYPE::UPDATE_TASK:
		{
			auto update_inv = update_task(command, taskid);
			inverse = (cmdtype == COMMAND_TYPE::UPDATE_TASK)? 
			update_inv
			:inverse;
			undoStack.push_back(inverse); //It can also be an add task, which takes precedence.
		}
		break;
	case COMMAND_TYPE::DELETE_TASK:
		inverse = delete_task(command);
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