//@author A0101642X
#ifndef UICONTROLLER_H
#define UI_CONTROLLER_H

#include <string>
#include <vector>
#include <QWebViewWithHooks.h>
#include "controller.h"
#include "executor.h"
#include "parser.h"
#include "commandTypeEnum.h"

class uiController
{
public:
	// Properties
	enum result_message_t {add_message, delete_message, update_message, undo_message, 
							search_message, filter_message, mark_complete_message, 
							invalid_message, undo_error_message, duplicate_message, 
							not_found_message};

	// Methods
	// Constructor
	uiController(QWebViewWithHooks *webView, unique_ptr<Controller> mainController);
	// Input from GUI
	void onCommandInput(std::string input);
	
protected:
	// Properties
	QWebViewWithHooks * webView;
	unique_ptr<Controller> controller;
	Parser parser;
	unique_ptr<Executor> executor;
	enum view_type_t {agenda_view, calendar_view};
	enum sort_type_t {id, name, start_date, end_date, tags, content};
	enum issue_type_t {event_type, deadline_type, task_type};
	std::vector<result_message_t> resultMessageStore;
	vector<Controller::CEvent> currentTasksStore;
	vector<Controller::CEvent> currentDeadlinesStore;
	vector<Controller::CEvent> currentEventsStore;
	int maxIssues;
	int currentEvents;
	int currentDeadlines;
	int currentTasks;
	int eventPage;
	int deadlinePage;
	int taskPage;
	
	//  Methods
	// Output to GUI
	void displayResultMessage(result_message_t message);
	void displaySearchResults(vector<Controller::CEvent> events);
	void displayFilterResults(std::pair<Controller::unregisterAction, string> filterResults);
	void displayHelp();
	void clearGUI();
	void clearEvents();
	void clearDeadlines();
	void clearTasks();
	void showOnGUI();
	void showOnGUISorted(string type, string issue);
	void sortByString(string s1, string s2, int j, vector<Controller::CEvent> &issues);
	void sortByTag(vector<string> s1, vector<string> s2, int j, vector<Controller::CEvent> &issues);
	void sortByNum(int n1, int n2, int j, vector<Controller::CEvent> &issues);
	void sortByDate(ptime d1, ptime d2, int j, vector<Controller::CEvent> &issues);
	void swapIssues(int j, vector<Controller::CEvent> &issues);
	// Manipulate GUI
	void swapView(view_type_t view);

	//Command box related
	vector<string> commandHistory;
	int commandIndex;
	string commandTmp;
	
	string getCommandBox();
	void setCommandBox(string s);
	void clearCommandBox();
	void commandHistoryNext();
	void commandHistoryPrev();
};

#endif // UICONTROLLER_H