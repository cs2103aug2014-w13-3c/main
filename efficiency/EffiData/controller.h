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
		CEvent& setName(string name);

		//Leaving the optional field out clears it.
		CEvent& setStart(ptime datetime /*OPTIONAL*/);
		CEvent& setEnd(ptime datetime/*OPTIONAL*/);
		CEvent& setPriority(string tag /*OPTIONAL*/); //This declaration is wrong.
		CEvent& getParent(CEvent evt/*OPTIONAL*/);
		CEvent& setContent(string content/*OPTIONAL*/);

		CEvent& setCompleteStatus(bool isComplete);
		CEvent& addTag(string tag);
		CEvent& removeTag(string tag);

		//getters
		Event::UUID getId();
		string getName();
		ptime getStart();
		ptime getEnd();
		int getPriority(); //TODO: change to enum.
		CEvent& getParent();
		bool isComplete();
		string getContent();

		vector<string> getTags();
	};
protected:
	TaskList events;
	vector<CEvent> cevents;
public:
	Controller();

	typedef function<void()> unregisterAction;
	typedef function<void(const vector<const CEvent>)> watchRangeCallback;
	typedef function<bool(const CEvent&)> filter;

	//Create, delete events, getById.
	CEvent& createEvent(string name);
	CEvent& getEventById(Event::UUID id);
	void deleteEvent(Event::UUID id);
	/* Change to:
	+addEvent(name): &CEvent
	+deleteEvent(id): void
	+getEvent(id): &CEvent	
	+getAllEvents(): vector<CEvent>
	*/
	//Watches
	unregisterAction watchRange(ptime start, ptime end, watchRangeCallback cb);
	unregisterAction watchFrom(ptime start, watchRangeCallback cb);
	unregisterAction watchTill(ptime end, watchRangeCallback cb);
	//filters
	unregisterAction addFilter(filter f);
};

#endif