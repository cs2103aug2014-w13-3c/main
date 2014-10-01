#ifndef COMMANDDATA_H
#define COMMANDDATA_H
#include "EventData.h"
#include "EventData.cpp"
#include "TaskList.h"

class CommandData{
public:
	static boolean createTaskList(string taskListName, string userName);
	static boolean addEvent(Event e);
	static boolean deleteEvent(Event e);
	static vector<string> readAll();
	static boolean updateTaskList();
	static string search(string name);
	static string search(string keyWords);
};
#endif