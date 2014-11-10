//This unit is responsible for gluing many functionalities together.

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <functional>
#include "TaskList.h"
#include "EventData.h"

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

using namespace boost::posix_time;

class Controller{
public:
	class CEvent;
	typedef std::function<void()> unregisterAction;
	typedef std::function<void(const vector<const CEvent>)> watchRangeCallback;
	typedef std::function<bool(const CEvent&)> filter;
	class CEvent{
	public:
		typedef std::function<void(Event& evt)> command;
	protected:
		Event::UUID uuid;
		vector<command> cmdlist;
		TaskList * events;
		Controller * controller;
	public:
		void exec(); //commits changes to database.
		CEvent(Event::UUID id, TaskList * tl, Controller * ctrl);
		~CEvent();

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
		//[string] operator -> tuple(string, integer, type(string))
		std::tuple<string, long, string> operator[](string field);
	
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
		
		//output overload
		friend ostream& operator<<(ostream& os, const CEvent& evt);
		//input overload
		friend istream& operator>>(istream& is, CEvent& evt);
	};
protected:
	TaskList events;
	map<Event::UUID, CEvent> cevents;
	map<int, filter> filters;
	int filterid;
public:
	Controller();
	Controller(std::string user);

	//Create, delete events, getById.
	CEvent& addEvent(string name);
	CEvent& getEvent(Event::UUID id);
	CEvent& getEventByName(std::string name);
	void deleteEvent(Event::UUID id);
	vector<CEvent> getAllEvents();
	void changeId(Event::UUID prev, Event::UUID curr);

	//Watches
	unregisterAction watchRange(ptime start, ptime end, watchRangeCallback cb);
	unregisterAction watchFrom(ptime start, watchRangeCallback cb);
	unregisterAction watchTill(ptime end, watchRangeCallback cb);
	//filters
	unregisterAction addFilter(filter f);
};

#endif