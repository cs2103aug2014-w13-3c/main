#include "EventData.h"

// constant key values
const string NAME ="name";
const string ID = "id";
const string PRIORITY = "priority";
const string TAGS = "tags";
const string COMPLETE = "complete";
const string START = "start";
const string END = "end";
const string PARENT = "parent";

// constant priority values
const int VERY_LOW = 5;
const int LOW = 4;
const int MODERATE = 3;
const int HIGH = 2;
const int VERY_HIGH = 1;
const int URGENT = 0;

Event::Event(){
	this->eventContent = ptree();
	this->eventContent.put(NAME, NULL);
	this->eventContent.put(ID, Event::generateID());
	this->eventContent.put(PRIORITY, VERY_LOW);
	this->eventContent.put(NAME, NULL);
	this->eventContent.put(NAME, NULL);
	this->eventContent.put(NAME, NULL);
}