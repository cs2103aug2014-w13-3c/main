//@author A0086591U
#include "stdafx.h"

//static
Event::UUID Event::id = 0;

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
	this->eventContent.put(START, ptime());
	this->eventContent.put(END, ptime());
	this->eventContent.put(PARENT, 0);
	this->eventContent.put(CONTENT, "");
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

bool Event::getCompleteStatus(){
	return this->eventContent.get<bool>(COMPLETE);
}
ptime Event::getStartDate(){
	return this->eventContent.get<ptime>(START);	
}
ptime Event::getEndDate(){
	return this->eventContent.get<ptime>(END);
}

Event::UUID Event::getParent(){
	return this->eventContent.get<UUID>(PARENT);
}

string Event::getContent(){
	return this->eventContent.get<string>(CONTENT);
}

void Event::setContent(string content){
	this->eventContent.put(CONTENT, content);
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
	ptree& currtags = this->_getTags();
	ptree child;
	child.put("", tag);
	currtags.push_back(std::make_pair("", child));
}

//TODO: maybe notify success/failure?
void Event::removeTag(string tag){
	ptree& currtags = this->_getTags();
	auto& tags = currtags.get_child("");
	for(auto it = tags.begin();it!=tags.end();++it)
	{
		if(it->second.get<std::string>("") == tag)
		{
			tags.erase(it);
			break;
			//return true;
		}
	}
	//return false;
}

vector<string> Event::getTags(){
	auto& tags = this->_getTags();
	vector<string> result;
	BOOST_FOREACH(const ptree::value_type& tag,
                  tags.get_child("")) {
		result.push_back(tag.second.get<std::string>(""));
    }
	return result;
}

Event::TagArray& Event::_getTags(){
	return this->eventContent.get_child(TAGS); 
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
void Event::setParent(Event::UUID p){
	this->eventContent.put(PARENT, p);	
}

Event::UUID Event::generateID(){
	return ++id;
}

void Event::setGlobalID(Event::UUID _id)
{
	id = _id;
}

ostream& operator<<(ostream& os, const Event& evt){
	write_json(os, evt.eventContent);
	return os;
}

istream& operator>>(istream& is, Event& evt){
	read_json(is, evt.eventContent);
	return is;
}
