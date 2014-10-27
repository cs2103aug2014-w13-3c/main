#include "stdafx.h"

TaskList::TaskList(string f = ""): filebase(f), userTaskList(){
	//Check if primary file exists
	if(filebase!= "")
	{
		string filename = filebase+".json"; //TODO: remove const.
		File db(filename); //creates if it doesn't exist.
		stringstream ss;
		ss.str(db.toString());
		ss>>*this;
		//TODO: Check for journal file.
		//If have: read it, then clear it.
	}
}

TaskList::~TaskList(){
	//Save to primary file
	if(filebase != "")
	{
		auto tempname = "~"+filebase+".json";
		File newFile(tempname);
		auto contents = toString(); //This is causing an infinite loop?!?!?!?!?!??
			//Apparently because something to do with destructor.
		newFile.writeLine(contents);
		newFile.forceClose();
		std::remove((filebase+".json").c_str());
		rename(tempname.c_str(),(filebase+".json").c_str());
		//TODO: Delete journal file
	}
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

string TaskList::toString(){
	stringstream os;
	//i copy pasted, because <<*this causes infinite destructor loops.
	for(auto it = userTaskList.begin(); it!= userTaskList.end(); ++it)
	{
		auto event = get<1>(*it);
		os<<event<<endl; //Since JSON escapes newline characters, its safe to add newlines.
	}
	return os.str();
}

void TaskList::changeId(Event::UUID prev, Event::UUID curr)
{
	Event evt = userTaskList.find(prev)->second;
	userTaskList.erase(prev);
	userTaskList.insert(make_pair(curr, std::move(evt)));
}

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
	Event::UUID idmax = 0;
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
				idmax = max(newevent.getId(), idmax);
			}
		}
		ss<<input;
		std::getline(is, input);
	}
	Event::setGlobalID(idmax);
	return is;
}

