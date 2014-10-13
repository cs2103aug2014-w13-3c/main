#ifndef UICONTROLLER_H
#define UI_CONTROLLER_H

#include <string>
#include <QWebViewWithHooks.h>
#include "controller.h"

class uiController
{
public:
	// Properties
	enum result_message_t {add_message, delete_message};
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
	enum view_type_t {agenda_view, calendar_view};
	
	//  Methods
	// Output to GUI
	void displayResultMessage(result_message_t message);
	void showOnGUI(/* takes a vector of issues and date range */);
	void showOnAgenda(/* takes a vector of issues and date range */);
	void showOnCalendar(/* takes a vector of issues and date range */);
	void clearIssue(/* takes a vector of issues and date range */);
	void clearFromAgenda(/* takes a vector of issues and date range */);
	void clearFromCalendar(/* takes a vector of issues and date range */);
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