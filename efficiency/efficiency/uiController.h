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
	enum button_click_t {add_button, delete_button, search_button, exit_button};

	// Methods
	// Constructor
	uiController(QWebViewWithHooks *webView, unique_ptr<Controller> mainController);
	// Input from GUI
	void onCommandInput(std::string input);
	void onButtonInput(button_click_t button);
	
protected:
	// Properties
	QWebViewWithHooks * webView;
	unique_ptr<Controller> controller;
	Parser parser;
	unique_ptr<Executor> executor;
	enum view_type_t {agenda_view, calendar_view};
	enum button_type_t {task_next, task_prev, deadline_next, deadline_prev, event_next, event_prev};
	enum sort_type_t {id, name, start_date, end_date, tags, description};
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
	void clearGUI();
	void clearEvents();
	void clearDeadlines();
	void clearTasks();
	void showOnGUI();
	void showOnGUISorted(sort_type_t type, issue_type_t issue);
	void sortByString(string s1, string s2, int j, vector<Controller::CEvent> &issues);
	void sortByTag(vector<string> s1, vector<string> s2, int j, vector<Controller::CEvent> &issues);
	void sortByNum(int n1, int n2, int j, vector<Controller::CEvent> &issues);
	void sortByDate(ptime d1, ptime d2, int j, vector<Controller::CEvent> &issues);
	void swapIssues(int j, vector<Controller::CEvent> &issues);
	void showOnAgenda(/* takes a vector of issues and date range */);
	void showOnCalendar(/* takes a vector of issues and date range */);
	// Manipulate GUI
	void swapView(view_type_t view);
	void changeButtonDisplay();

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

/*
Output to GUI (private methods)
- outputTask
	- outputToAgenda
	- outputToCalendar
- clearEvent
	- clearEventFromAgenda
	- clearEventFromCalendar
- clearAllEvents
	- clearAllFromAgenda
	- clearAllFromCalendar
- outputMessage
- swapView(targetView)

Input from GUI (public methods)
- onCommandInput
- onButtonClick
(use these to send to parser and main controller)

Input from main controller (public methods)
- watchMainController
*/

#endif // UICONTROLLER_H