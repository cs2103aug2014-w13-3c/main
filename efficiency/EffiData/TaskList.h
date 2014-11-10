#ifndef TASKLIST_H
#define TASKLIST_H

#include "EventData.h"
#include <map>
#include <functional>
#include "file.h"

class TaskList{
public:
	typedef std::function<void(Event&)> EventOperator;
	TaskList(std::string filename); //Constructor will read file into TaskList.
	~TaskList(); //Destructor will save TaskList into file.
protected:
	TaskList(const TaskList & other);
	File journal;
	map<Event::UUID, Event> userTaskList;
	string filebase;
public:
	//void setName(string name);
	Event::UUID addEvent(std::string name,bool silent = false, EventOperator op = [](Event& evt){});
	//update using a functional.
		//This enforces that all changes are done at once.
	void updateEvent(Event::UUID id, EventOperator op, bool silent = false); 
	void deleteEvent(Event::UUID id, bool silent = false);
	void changeId(Event::UUID prev, Event::UUID curr);
	Event getEvent(Event::UUID id);  //Note that this returns a copy.
	vector<Event> getAllEvents(); //This returns copies too.
	friend ostream& operator<<(ostream& os, const TaskList &t);
	friend istream& operator>>(istream& is, TaskList &t);
	std::string toString();
	void processJournal(string str);
};

#endif