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

	Executor(Controller * ctrl);
	// execute a given command
		// NOTE TO SELF: TO BE CALLED BY UI CONTROLLER
	//Throws: CannotFindTarget('targetstring')
	void executeCommand(Command processedCommand);

protected:
	Controller * ctrl;
	Event::UUID find_task(Executor::Command command);
	Event::UUID add_task(Executor::Command command);
	void delete_task(Executor::Command command, Event::UUID taskid);
	void update_task(Executor::Command command, Event::UUID taskid);
	static std::map<std::string, std::function<void(boost::any, Controller::CEvent& evt)>> actions; 
};
#endif