#include "stdafx.h"
#include <regex>


void TaskList::processJournal(string cmd){
	//Extract the values.
	regex extractor("(\\w+?),\\s(\\d+?),\\s(.*)");
	smatch matches;
	regex_match(cmd, matches, extractor);
	if(!matches.ready())
		throw "Unexpected format";
	string type = matches[1];
	Event::UUID id = atol(string(matches[2]).c_str());
	string content = matches[3];

	//Apply accordingly.
	if(type == "ADD")
	{
		//Grab the dump, restore it to a temp.
		auto tid = addEvent("placeholder", true);
		changeId(tid, id);
		updateEvent(id,[&content](Event& evt) { 
			stringstream ss;
			ss.str(content);
			ss>>evt; 
		}, true);
	}
	else if (type == "UPDATE")
	{
		//Assume ID doesn't change.
		updateEvent(id,[&content](Event& evt) { 
			stringstream ss;
			ss.str(content);
			ss>>evt; 
		}, true);
	}
	else if (type == "DELETE")
	{
		deleteEvent(id, true);
	}
	else
		assert(false);
}

TaskList::TaskList(string f = ""): filebase(f), userTaskList(){
	//Check if primary file exists
	if(filebase!= "")
	{
		string filename = filebase+".json"; 
		File db(filename); //creates if it doesn't exist.
		stringstream ss;
		ss.str(db.toString());
		ss>>*this;

		string journalFilename = filebase+".journal"; 
		File journal(journalFilename);
		string line = journal.readLine();
		//If have: read it, then clear it.
		while(line != "")
		{
			processJournal(line);
			line = journal.readLine();
		}
		this->journal = std::move(journal);
	}
	else
		this->journal = std::move(File("TEST.journal")); //Strictly for test cases only.
}

TaskList::~TaskList(){
	//Save to primary file
	if(filebase != "")
	{
		auto tempname = "~"+filebase+".json";
		File newFile(tempname);
		auto contents = toString(); //This is causing an infinite loop?!?!?!?!?!??
			//Apparently because something to do with destructor.
		newFile.writeLine(contents);
		newFile.forceClose();
		std::remove((filebase+".json").c_str());
		rename(tempname.c_str(),(filebase+".json").c_str());
		//TODO: Delete journal file
		journal.forceClose();
		remove (journal.filename().c_str());
	}
}


string generate_dump(Event & evt){
	stringstream ss;
	ss<<evt;
	string dump = ss.str();
	std::replace( dump.begin(), dump.end(), '\n', ' ');
	return dump;
}

Event::UUID TaskList::addEvent(std::string name, bool silent, EventOperator op){
	Event evt(name);
	op(evt);
	userTaskList.insert(make_pair(evt.getId(), evt));
	if(!silent)
	{
		journal.writeLine("ADD, "+to_string(evt.getId())+", "+generate_dump(evt));
	}
	return evt.getId();
}

void TaskList::updateEvent(Event::UUID id, EventOperator op, bool silent){
	op(userTaskList.at(id));
	if(!silent)
		journal.writeLine("UPDATE, "+to_string(id)+", "+generate_dump(userTaskList.at(id)));
	
}

void TaskList::deleteEvent(Event::UUID id, bool silent){
	userTaskList.erase(id);
	if(!silent)
		journal.writeLine("DELETE, "+to_string(id)+", {}");
}

Event TaskList::getEvent(Event::UUID id){
	return Event(userTaskList.at(id));
}

vector<Event> TaskList::getAllEvents(){
	vector<Event> results;
	for(auto it = userTaskList.begin(); it!= userTaskList.end(); ++it)
	{
		results.push_back(Event(get<1>(*it)));
	}
	return results;
}

string TaskList::toString(){
	stringstream os;
	//i copy pasted, because <<*this causes infinite destructor loops.
	for(auto it = userTaskList.begin(); it!= userTaskList.end(); ++it)
	{
		auto event = get<1>(*it);
		os<<event<<endl; //Since JSON escapes newline characters, its safe to add newlines.
	}
	return os.str();
}

void TaskList::changeId(Event::UUID prev, Event::UUID curr)
{
	Event evt = userTaskList.find(prev)->second;
	userTaskList.erase(prev);
	userTaskList.insert(make_pair(curr, std::move(evt)));
}

ostream& operator<<(ostream& os, const TaskList &t){
	for(auto it = t.userTaskList.begin(); it!= t.userTaskList.end(); ++it)
	{
		auto event = get<1>(*it);
		os<<event<<endl; //Since JSON escapes newline characters, its safe to add newlines.
	}
	return os;
}

//TODO: test.
istream& operator>>(istream& is, TaskList &t){
	std::stringstream ss;
	string input; 
	std::getline(is, input);
	Event::UUID idmax = 0;
	while(true)
	{
		if(input == "")
		{
			auto eventstring = ss.str();
			if(eventstring == "")
				break;
			else
			{
				Event newevent("");
				ss>>newevent;
				t.userTaskList.insert(make_pair(newevent.getId(), newevent));
				ss.str("");
				idmax = max(newevent.getId(), idmax);
			}
		}
		ss<<input;
		std::getline(is, input);
	}
	Event::setGlobalID(idmax);
	return is;
}

