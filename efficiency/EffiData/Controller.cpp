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
		CEvent& setName(string name);
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

Event::UUID Controller::CEvent::getId(){
	return uuid;
}

string Controller::CEvent::getName(){
	return events->getEvent(uuid).getName();
}

int Controller::CEvent::getPriority(){
	return events->getEvent(uuid).getPriority();
}

vector<string> Controller::CEvent::getTags(){
	return events->getEvent(uuid).getTags();
}

bool Controller::CEvent::getCompleteStatus(){
	return events->getEvent(uuid).getCompleteStatus();
}

ptime Controller::CEvent::getStartDate(){
	return events->getEvent(uuid).getStartDate();
}

ptime Controller::CEvent::getEndDate(){
	return events->getEvent(uuid).getEndDate();
}

Event::UUID Controller::CEvent::getParent(){
	return events->getEvent(uuid).getParent();
}

string Controller::CEvent::getContent(){
	return events->getEvent(uuid).getContent();
}


Controller::CEvent& Controller::CEvent::setName(string name){
	cmdlist.push_back([&name](Event& evt) { evt.changeName(name); } );
	return *this;
}

Controller::CEvent& Controller::CEvent::setPriority(int priority){
	cmdlist.push_back([&priority](Event& evt) { evt.setPriority(priority); } );
	return *this;
}

Controller::CEvent& Controller::CEvent::addTags(vector<string> tags){
	cmdlist.push_back([&tags](Event& evt) { evt.addTags(tags); } );
	return *this;
}

Controller::CEvent& Controller::CEvent::addTag(string tag){
	cmdlist.push_back([&tag](Event& evt) { evt.addTag(tag); } );
	return *this;
}

Controller::CEvent& Controller::CEvent::removeTag(string tag){
	cmdlist.push_back([&tag](Event& evt) { evt.removeTag(tag); } );
	return *this;
}

Controller::CEvent& Controller::CEvent::setCompleteStatus(bool status){
	cmdlist.push_back([&status](Event& evt) { evt.setCompleteStatus(status); } );
	return *this;
}

Controller::CEvent& Controller::CEvent::setStartDate(ptime sd){
	cmdlist.push_back([&sd](Event& evt) { evt.setStartDate(sd); } );
	return *this;
}

Controller::CEvent& Controller::CEvent::setEndDate(ptime ed){
	cmdlist.push_back([&ed](Event& evt) { evt.setEndDate(ed); } );
	return *this;
}

Controller::CEvent& Controller::CEvent::setParent(Event::UUID parent){
	cmdlist.push_back([&parent](Event& evt) { evt.setParent(parent); } );
	return *this;
}

Controller::CEvent& Controller::CEvent::setContent(string content){
	cmdlist.push_back([&content](Event& evt) { evt.setContent(content); } );
	return *this;
}

//Actual controller stuff.
Controller::Controller(): events("TESTUSER"), cevents()
{}

Controller::CEvent& Controller::addEvent(string name){
	auto id = events.addEvent(name);
	cevents.insert(make_pair(id, CEvent(id, &events)));
	return cevents.at(id);
}

Controller::CEvent& Controller::getEvent(Event::UUID id){
	return cevents.at(id);
}
void Controller::deleteEvent(Event::UUID id){
	events.deleteEvent(id); //TODO: check if event exists. Actually don't bother, just let it throw out.
	cevents.erase(id);
}