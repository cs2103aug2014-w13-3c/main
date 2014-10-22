//This unit is responsible for gluing many functionalities together.

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <functional>
#include "TaskList.h"
#include "EventData.h"

#include "boost/date_time/posix_time/posix_time.hpp"
using namespace boost::posix_time;

class Controller{
public:
	class CEvent{
	public:
		typedef std::function<void(Event& evt)> command;
	protected:
		Event::UUID uuid;
		vector<command> cmdlist;
		TaskList * events;
	public:
		void exec(); //commits changes to database.
		CEvent(Event::UUID id, TaskList * tl);
		~CEvent();
		//TODO: All the event related functionalities go here.

		//Literally pasted from eventData.h
			//Please keep in sync.
		//get information
		Event::UUID getId();
		string getName();
		int getPriority();
		vector<string> getTags();
		bool getCompleteStatus();
		ptime getStartDate();
		ptime getEndDate();
		Event::UUID getParent();
		string getContent();
	
		//set properties
			//The only change is the return value - its for chaining.
		CEvent& setName(string name);
		CEvent& setPriority(int priority);
		CEvent& addTags(vector<string>tags);
		CEvent& addTag(string tag);
		CEvent& removeTag(string tag);
		CEvent& setCompleteStatus(bool status);
		CEvent& setStartDate(ptime sd);
		CEvent& setEndDate(ptime ed);
		CEvent& setParent(Event::UUID p);
		CEvent& setContent(string s);
	};
protected:
	TaskList events;
	map<Event::UUID, CEvent> cevents;
public:
	Controller();

	typedef function<void()> unregisterAction;
	typedef function<void(const vector<const CEvent>)> watchRangeCallback;
	typedef function<bool(const CEvent&)> filter;

	//Create, delete events, getById.
	CEvent& addEvent(string name);
	CEvent& getEvent(Event::UUID id);
	CEvent& getEventByName(std::string name);
	void deleteEvent(Event::UUID id);
	vector<CEvent> getAllEvents();

	//Watches
	unregisterAction watchRange(ptime start, ptime end, watchRangeCallback cb);
	unregisterAction watchFrom(ptime start, watchRangeCallback cb);
	unregisterAction watchTill(ptime end, watchRangeCallback cb);
	//filters
	unregisterAction addFilter(filter f);
};

#endif