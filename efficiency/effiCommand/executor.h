//@author A0086591U
#ifndef EXECUTOR_H_
#define EXECUTOR_H_

#include <string>
#include <vector>
#include <map>
#include <functional>

#include "commandTypeEnum.h"
#include "optionField.h"
#include "controller.h"

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/any.hpp"
using namespace boost;
using namespace boost::posix_time;
using namespace std;

class Executor{
public:
	typedef multimap<std::string, boost::any> Command;
	typedef std::function<void()> InverseFunction;

	Executor(Controller * ctrl);
	// execute a given command
		// NOTE TO SELF: TO BE CALLED BY UI CONTROLLER
	//Throws: CannotFindTarget('targetstring')
	void executeCommand(Command processedCommand);
	std::pair<Controller::unregisterAction, string> addFilter(Command processedCommand);
	vector<Controller::CEvent> search(Command processedCommand);

protected:
	Controller * ctrl;
	vector<std::function<void()>> undoStack;
	Event::UUID find_task(Executor::Command command);
	std::pair<Event::UUID, InverseFunction> add_task(Executor::Command command);
	InverseFunction delete_task(Executor::Command command, Event::UUID taskid);
	InverseFunction update_task(Executor::Command command, Event::UUID taskid);
	InverseFunction mark_complete(Executor::Command command);
	InverseFunction mark_complete(Event::UUID taskid, bool recursive, bool setting);
	std::map<std::string, std::function<void(boost::any, Controller::CEvent& evt)>> actions; 
	std::map<std::string, std::function<void(boost::any, Controller::CEvent& evt)>> createActions();
	InverseFunction makeUpdateInverse(Controller::CEvent& evt);
	InverseFunction makeDeleteInverse(Controller::CEvent& evt);
};
#endif