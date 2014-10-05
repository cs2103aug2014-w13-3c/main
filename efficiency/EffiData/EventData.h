#ifndef EVENTDATA_H
#define EVENTDATA_H

#include "boost/property_tree/json_parser.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using boost::property_tree::ptree;
using namespace boost::posix_time;
class Event{
	typedef ptree TagArray;
public:
	Event();
	Event(string name);
protected:
	ptree eventContent;
	TagArray _getTags();
public:
	//get information
	long getId();
	string getName();
	int getPriority();
	vector<string> getTags();
	bool getCompleteStatus();
	ptime getStartDate();
	ptime getEndDate();
	ptree getParent();
	string getContent();
	
	//set properties
	void changeName(string name);
	void setPriority(int priority);
	void addTags(vector<string>tags);
	void addTag(string tag);
	void setCompleteStatus(bool status);
	void setStartDate(ptime sd);
	void setEndDate(ptime ed);
	void setParent(ptree p);
	void addContent(string s);

	//static methods
	static long generateID();

};
#endif