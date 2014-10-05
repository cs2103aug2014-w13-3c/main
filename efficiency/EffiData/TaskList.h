#ifndef TASKLIST_H
#define TASKLIST_H

#include "EventData.h"
#include <map>
#include <functional>

using namespace std;
class TaskList{
public:
	typedef std::function<void(Event&)> EventOperator;
	TaskList();
protected:
	map<Event::UUID, Event> userTaskList;
	//string name;
public:
	//void setName(string name);
	Event::UUID addEvent(std::string name);
	//update using a functional.
		//This enforces that all changes are done at once.
	void updateEvent(Event::UUID id, EventOperator); 
	void deleteEvent(Event::UUID id);
	Event& getEvent(Event::UUID id); 
	vector<Event> getAllEvents();
};

#endif