#ifndef TASKLIST_H
#define TASKLIST_H

#include "EventData.h"
#include <map>
#include <functional>

using namespace std;
class TaskList{
public:
	typedef std::function<void(Event&)> EventOperator;
	TaskList(std::string filename); //Constructor will read file into TaskList.
	~TaskList(); //Destructor will save TaskList into file.
protected:
	map<Event::UUID, Event> userTaskList;
	string filename;
public:
	//void setName(string name);
	Event::UUID addEvent(std::string name, EventOperator op = [](Event& evt){});
	//update using a functional.
		//This enforces that all changes are done at once.
	void updateEvent(Event::UUID id, EventOperator op); 
	void deleteEvent(Event::UUID id);
	Event getEvent(Event::UUID id);  //Note that this returns a copy.
	vector<Event> getAllEvents(); //This returns copies too.
	friend ostream& operator<<(ostream& os, const TaskList t);
	friend istream& operator>>(istream& is, TaskList &t);
};

#endif