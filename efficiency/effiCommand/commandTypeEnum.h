#ifndef COMMANDTYPEENUM_H_
#define COMMANDTYPEENUM_H_

namespace commandTypeEnum {

	// these are the possible command types
	enum COMMAND_TYPE {
		VIEW, SCROLL, MARK_COMPLETE,
		ADD_TASK, UPDATE_TASK, DELETE_TASK,  
		SEARCH, FILTER, 
		UNDO, SETTINGS, MINIMIZE, HELP, LOGOUT, EXIT
	};
};
#endif