#include "stdafx.h"

Controller::CEvent::CEvent(Event::UUID id, TaskList * tl) : uuid(id), cmdlist(), events(tl)
{}

Controller::CEvent::~CEvent()
{
	assert(cmdlist.size() == 0); //if you got this error, you forgot to call exec after making changes.
}
void Controller::CEvent::exec(){
	events->updateEvent(uuid, [this](Event & evt){
		for(auto cmd = cmdlist.begin(); cmd!=cmdlist.end(); ++cmd)
			(*cmd)(evt);
	});
	cmdlist.clear();
}

//One ton of wrapper functions over here.
/*
		//get information
		UUID getId();
		string getName();
		int getPriority();
		vector<string> getTags();
		bool getCompleteStatus();
		ptime getStartDate();
		ptime getEndDate();
		UUID getParent();
		string getContent();
	
		//set properties
		CEvent& changeName(string name);
		CEvent& setPriority(int priority);
		CEvent& addTags(vector<string>tags);
		CEvent& addTag(string tag);
		CEvent& removeTag(string tag);
		CEvent& setCompleteStatus(bool status);
		CEvent& setStartDate(ptime sd);
		CEvent& setEndDate(ptime ed);
		CEvent& setParent(UUID p);
		CEvent& setContent(string s);

*/
Controller::CEvent& Controller::CEvent::changeName(string name){
	cmdlist.push_back([&name](Event& evt) { evt.changeName(name); } );
	return *this;
}

string Controller::CEvent::getName(){
	return events->getEvent(uuid).getName();
}

Event::UUID Controller::CEvent::getId(){
	return uuid;
}

//Actual controller stuff.
Controller::Controller(): events("TESTUSER"), cevents()
{}

Controller::CEvent& Controller::addEvent(string name){
	auto id = events.addEvent(name);
	cevents.insert(make_pair(id, CEvent(id, &events));
	return cevents.at(id);
}

Controller::CEvent& Controller::getEvent(Event::UUID id){
	return cevents.at(id);
}
void Controller::deleteEvent(Event::UUID id){
	events.deleteEvent(id); //TODO: check if event exists. Actually don't bother, just let it throw out.
	cevents.erase(id);
}