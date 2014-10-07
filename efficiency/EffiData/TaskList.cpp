#include "stdafx.h"

TaskList::TaskList(string f = ""): filename(f){
	//Check if primary file exists
	//Does: Read it
		//Check for journal file.
		//If have: read it, then clear it.
		//Else create it.
	//Does not: create it and journal file.
}

TaskList::~TaskList(){
	//Save to primary file
	/*if(filename != "")
	{
		auto tempname = "~"+filename;
		File newFile(tempname);
		newFile.writeLine(toString(*this));
		newFile.forceClose();
		std::remove(filename.c_str());
		rename(tempname.c_str(),filename.c_str());
		//TODO: Delete journal file
	}*/
}
//TODO: test everything.
Event::UUID TaskList::addEvent(std::string name, EventOperator op){
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
		auto event = get<1>(*it);
		os<<event<<endl; //Since JSON escapes newline characters, its safe to add newlines.
	}
	return os;
}

//TODO: test.
istream& operator>>(istream& is, TaskList &t){
	std::stringstream ss;
	string input; 
	std::getline(is, input);
	while(true)
	{
		if(input == "")
		{
			auto eventstring = ss.str();
			if(eventstring == "")
				break;
			else
			{
				Event newevent("");
				ss>>newevent;
				t.userTaskList.insert(make_pair(newevent.getId(), newevent));
				ss.str("");
			}
		}
		ss<<input;
		std::getline(is, input);
	}
	return is;
}

