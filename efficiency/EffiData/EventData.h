#ifndef EVENTDATA_H
#define EVENTDATA_H

#include "boost/property_tree/json_parser.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <boost/foreach.hpp>

using namespace std;
using boost::property_tree::ptree;
using namespace boost::posix_time;

class Event{
	friend class TaskList;
	typedef ptree TagArray;
public:
	typedef unsigned long UUID;
	Event(string name);
protected:
	ptree eventContent;
	TagArray& _getTags();
	//TODO: implement for syndication.
	vector<string> changes; //unimplemented.
	vector<string> getAndClearChanges(); //unimplemented.
public:
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
	void changeName(string name);
	void setPriority(int priority);
	void addTags(vector<string>tags);
	void addTag(string tag);
	void setCompleteStatus(bool status);
	void setStartDate(ptime sd);
	void setEndDate(ptime ed);
	void setParent(UUID p);
	void setContent(string s);

	//output overload
	friend ostream& operator<<(ostream& os, const Event& evt);
	//input overload
	friend istream& operator>>(istream& is, Event& evt);

protected:
	//static methods
	static UUID generateID();

};

#endif