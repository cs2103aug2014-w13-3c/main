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
	enum result_message_t {add_message, delete_message, update_message, invalid_message};
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
	std::vector<result_message_t> resultMessageStore;
	
	//  Methods
	// Output to GUI
	void displayResultMessage(result_message_t message);
	void clearGUI();
	void showOnGUI(/* takes a vector of issues and date range */);
	void showOnAgenda(/* takes a vector of issues and date range */);
	void showOnCalendar(/* takes a vector of issues and date range */);
	// Manipulate GUI
	void swapView(view_type_t view);
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