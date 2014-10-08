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
Controller::CEvent& Controller::CEvent::setName(string name){
	cmdlist.push_back([&name](Event& evt) { evt.changeName(name); } );
	return *this;
}

string Controller::CEvent::getName(){
	return events->getEvent(uuid).getName();
}

//Actual controller stuff.
Controller::Controller(): events("TESTUSER"), cevents()
{}

Controller::CEvent& Controller::createEvent(string name){
	auto id = events.addEvent(name);
	cevents.push_back(CEvent(id, &events));
	return cevents.at(cevents.size()-1);
}
Controller::CEvent& Controller::getEventById(Event::UUID id){
	for(auto it = cevents.begin(); it!= cevents.end(); ++it)
	{
		if( (*it).getId() == id)
			return *it;
	}
	throw new exception("element not found");
}
void Controller::deleteEvent(Event::UUID id){
	events.deleteEvent(id); //TODO: check if event exists. Actually don't bother, just let it throw out.
	cevents.erase(
		remove_if(cevents.begin(), cevents.end(), [&id](CEvent& evt) { return evt.getId() == id; }),
		cevents.end());
}