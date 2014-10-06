#include "stdafx.h"

//TODO: test everything.
Event::UUID TaskList::addEvent(std::string name, EventOperator op = [](Event& evt){ }){
	Event evt(name);
	op(evt);
	userTaskList.insert(make_pair(evt.getId(), evt));
	return evt.getId();
}

void TaskList::updateEvent(Event::UUID id, EventOperator op){
	op(userTaskList.at(id));
}

void TaskList::deleteEvent(Event::UUID id){
	userTaskList.erase(id);
}

Event TaskList::getEvent(Event::UUID id){
	return Event(userTaskList.at(id));
}

vector<Event> TaskList::getAllEvents(){
	vector<Event> results;
	for(auto it = userTaskList.begin(); it!= userTaskList.end(); ++it)
	{
		results.push_back(Event(get<1>(*it)));
	}
	return results;
}

//TODO: actually test.
ostream& operator<<(ostream& os, const TaskList t){
	for(auto it = t.userTaskList.begin(); it!= t.userTaskList.end(); ++it)
	{
		auto uuid = get<0>(*it);
		auto event = get<1>(*it);
		os<<uuid<<" = "<<event<<endl;
	}
	return os;
}

