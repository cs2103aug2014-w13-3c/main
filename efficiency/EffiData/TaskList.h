#ifndef TASKLIST_H
#define TASKLIST_H

#include "EventData.h"
#include <unordered_map>

using namespace std;
class TaskList{
public:
	TaskList();
private:
	unordered_map<Event::UUID, Event> userTaskList;
	string name;
public:
	void setName(string name);
	void addEvent(long id, Event e);
	void updateEvent(long id);
	void deleteEvent(long id);
	void readEvent(long id); 
};

#endif