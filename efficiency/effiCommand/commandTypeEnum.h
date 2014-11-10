#ifndef COMMANDTYPEENUM_H_
#define COMMANDTYPEENUM_H_

//@author A0098802X
namespace commandTypeEnum {
	// these are the possible command types
	enum COMMAND_TYPE {
		VIEW, SCROLL, MARK_COMPLETE, MARK_INCOMPLETE,
		ADD_TASK, UPDATE_TASK, DELETE_TASK,  
		SEARCH, FILTER, SORT,
		UNDO,
		HELP, EXIT
	};
};
#endif