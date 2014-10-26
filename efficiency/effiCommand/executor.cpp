#include "parser.h"
#include "executor.h"
#include "Undo.h"
#include "controller.h"

using namespace std;
using CommandTypeEnum::COMMAND_TYPE;

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
	//TODO.
	//for each possible property
		//check if its available
			//if so, add it.
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
