#ifndef UICONTROLLER_H
#define UI_CONTROLLER_H

#include <string>
#include <QWebViewWithHooks.h>
#include "controller.h"

class uiController
{
public:
	uiController(QWebViewWithHooks *webView, Controller * mainController);
	void onCommandInput(std::string input);

protected:
	QWebViewWithHooks * webView;
	Controller * controller;
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