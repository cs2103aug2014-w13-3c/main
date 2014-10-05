#include "EventData.h"

//TODO: move to EventData.h
// constant key values
const string NAME ="name";
const string ID = "id";
const string PRIORITY = "priority";
const string TAGS = "tags";
const string COMPLETE = "complete";
const string START = "start";
const string END = "end";
const string PARENT = "parent";
const string CONTENT = "content";

//TODO: change to enum.
// constant priority values
const int VERY_LOW = 5;
const int LOW = 4;
const int MODERATE = 3;
const int HIGH = 2;
const int VERY_HIGH = 1;
const int URGENT = 0;

Event::Event(string name = NULL){
	this->eventContent = ptree();
	this->eventContent.put(NAME, name);
	this->eventContent.put(ID, Event::generateID());
	this->eventContent.put(PRIORITY, VERY_LOW);
	this->eventContent.add_child(TAGS , Event::TagArray());
	this->eventContent.put(COMPLETE, false);
	this->eventContent.put(START, NULL);
	this->eventContent.put(END, NULL);
	this->eventContent.put(PARENT, NULL);
	this->eventContent.put(CONTENT, NULL);
}

Event::UUID Event::getId(){
	return this->eventContent.get<Event::UUID>(ID);
}
string Event::getName(){
	return this->eventContent.get<string>(NAME);	
}
int Event::getPriority(){
	return this->eventContent.get<int>(PRIORITY);
}
Event::TagArray Event::_getTags(){
	return this->eventContent.get_child(TAGS); 
}

vector<string> Event::getTags(){
	auto tags = this->_getTags();
	vector<string> result;
	//TODO: implement.
	return result;
}

bool Event::getCompleteStatus(){
	return this->eventContent.get<bool>(COMPLETE);
}
ptime Event::getStartDate(){
	return this->eventContent.get<ptime>(START);	
}
ptime Event::getEndDate(){
	return this->eventContent.get<ptime>(END);
}

//TODO: fix.
ptree Event::getParent(){
	//return this->eventContent.get<ptree>(PARENT);
	return ptree();
}
	
void Event::changeName(string name){
	this->eventContent.put(NAME, name);
}
void Event::setPriority(int priority){
	this->eventContent.put(PRIORITY, priority);
}
void Event::addTags(vector<string> tags){
	for(size_t i =0; i<tags.size(); i++){
		addTag(tags[i]);
	}
}
void Event::addTag(string tag){
	ptree currtags = this->_getTags();
	ptree child;
	child.put("", tag);
	currtags.push_back(std::make_pair("", child));
}
void Event::setCompleteStatus(bool status){
	this->eventContent.put(COMPLETE, status);
}
void Event::setStartDate(ptime sd){
	this->eventContent.put(START, sd);
}
void Event::setEndDate(ptime ed){
	this->eventContent.put(END, ed);
}
void Event::setParent(ptree p){
	//this->eventContent.put(PARENT, p);	
}

Event::UUID Event::generateID(){
	static UUID id = 0; //TODO: initialize properly.
	return id++;
}


int main(int argc, char *argv[])
{
	std::cout<<"test"<<std::endl;
	return 0;
}
