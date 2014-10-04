#ifndef TASKLIST_H
#define TASKLIST_H

#include "EventData.h"
#include "EventData.cpp"
#include <unordered_map>

using namespace std;
class TaskList{
public:
	TaskList();
	TaskList(string name);
private:
	unordered_map<long, Event> userTaskList;
	string name;
public:
	void setName(string name);
	void addEvent(long id, Event e);
	void updateEvent(long id);
	void deleteEvent(long id);
	void readEvent(long id); 
};

#endif