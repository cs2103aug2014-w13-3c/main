#include "stdafx.h"

using namespace std;
using namespace CommandTypeEnum;
using namespace boost::posix_time;
using namespace boost::gregorian;

uiController::uiController(QWebViewWithHooks *webView, unique_ptr<Controller> ctrl):webView(webView),
							controller(std::move(ctrl)),
							maxIssues(2),
							currentEvents(0),
							currentDeadlines(0),
							currentTasks(0),
							taskPage(1),
							deadlinePage(1),
							eventPage(1),
							parser(){
	executor = unique_ptr<Executor>(new Executor(controller.get()));
	
	// Add the date to the display on load
	webView->registerPageLoad([webView](){
		ptime now = second_clock::local_time();
		date today = now.date();
		string date_string =  to_simple_string(today);
		QWebElement dom = webView->page()->mainFrame()->documentElement();
		dom.findFirst("#date").appendInside("Today's date is "+QString::fromStdString(date_string));
	});

	// Show existing issues on load
	webView->registerPageLoad([this](){
		showOnGUI();
		changeButtonDisplay();

		//()<<QString::fromStdString("button pressed");
		this->webView->watchButtonPress("deadline_forward", [this](){
			deadlinePage++;

			QWebElement dom = this->webView->page()->mainFrame()->documentElement();
			QWebElement deadlinePrevButton = dom.findFirst("#deadline_back");
			deadlinePrevButton.setAttribute("style","display:inline;");

			showOnGUI();
			changeButtonDisplay();
		});

		this->webView->watchButtonPress("deadline_back", [this](){
			deadlinePage--;

			QWebElement dom = this->webView->page()->mainFrame()->documentElement();
			QWebElement deadlineNextButton = dom.findFirst("#deadline_forward");
			deadlineNextButton.setAttribute("style","display:inline;");

			showOnGUI();
			changeButtonDisplay();
		});

		this->webView->watchButtonPress("event_forward", [this](){
			eventPage++;

			QWebElement dom = this->webView->page()->mainFrame()->documentElement();
			QWebElement eventPrevButton = dom.findFirst("#event_back");
			eventPrevButton.setAttribute("style","display:inline;");

			showOnGUI();
			changeButtonDisplay();
		});

		this->webView->watchButtonPress("event_back", [this](){
			eventPage--;

			QWebElement dom = this->webView->page()->mainFrame()->documentElement();
			QWebElement eventNextButton = dom.findFirst("#event_forward");
			eventNextButton.setAttribute("style","display:inline;");

			showOnGUI();
			changeButtonDisplay();
		});

		this->webView->watchButtonPress("task_forward", [this](){
			taskPage++;

			QWebElement dom = this->webView->page()->mainFrame()->documentElement();
			QWebElement taskPrevButton = dom.findFirst("#task_back");
			taskPrevButton.setAttribute("style","display:inline;");

			showOnGUI();
			changeButtonDisplay();
		});

		this->webView->watchButtonPress("task_back", [this](){
			taskPage--;

			QWebElement dom = this->webView->page()->mainFrame()->documentElement();
			QWebElement taskNextButton = dom.findFirst("#task_forward");
			taskNextButton.setAttribute("style","display:inline;");

			showOnGUI();
			changeButtonDisplay();
		});
	});

	webView->watch("#command-box",
		[](QWebElement &element)->QString{ return element.evaluateJavaScript("this.value").toString(); },
		[this](std::string s, QKeyEvent *key) { 
				if((key->key() == Qt::Key_Enter) || (key->key() == Qt::Key_Return)){
					onCommandInput(s); 
				}
			});
}

void uiController::onCommandInput(string input){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	qDebug()<<QString::fromStdString(input); // check output

	multimap<string,any> parsedCommand = parser.parseCommand(input);

	if(any_cast<bool>(parsedCommand.find("valid")->second) == true){
		executor->executeCommand(parsedCommand);

		if(any_cast<COMMAND_TYPE>(parsedCommand.find("cmd")->second) == ADD_TASK){
			displayResultMessage(add_message);
		}
		else if(any_cast<COMMAND_TYPE>(parsedCommand.find("cmd")->second) == DELETE_TASK){
			displayResultMessage(delete_message);
		}
		else if(any_cast<COMMAND_TYPE>(parsedCommand.find("cmd")->second) == UPDATE_TASK){
			displayResultMessage(update_message);
		}

		showOnGUI();
	}
	else {
		displayResultMessage(invalid_message);
	}
}

void uiController::displayResultMessage(result_message_t message){
	QWebElement dom = webView->page()->mainFrame()->documentElement();

	if(resultMessageStore.size() >= 5){
		resultMessageStore.erase(resultMessageStore.begin(),resultMessageStore.begin()+1);
	}
	resultMessageStore.push_back(message);

	QWebElement messageBox = dom.findFirst("#message-box");
	messageBox.removeAllChildren();

	for (auto it = resultMessageStore.begin(); it != resultMessageStore.end(); ++it) {
		if(*it == add_message){
			dom.findFirst("#message-box").appendInside("Issue added.<br>");
		}
		else if(*it == delete_message) {
			dom.findFirst("#message-box").appendInside("Issue deleted.<br>");
		}
		else if(*it == update_message) {
			dom.findFirst("#message-box").appendInside("Issue updated.<br>");
		}
		else if(*it == invalid_message){
			dom.findFirst("#message-box").appendInside("Error: Enter valid command.<br>");
		}
	}
}

void uiController::showOnGUI(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	vector<Controller::CEvent> events;

	string name;
	string content;
	ptime start;
	ptime end;

	int eventCount = 0;
	int deadlineCount = 0;
	int taskCount = 0;

	currentTasks = 0;
	currentDeadlines = 0;
	currentEvents = 0;

	events = controller->getAllEvents();
	clearGUI();

	for(auto i = events.begin(); i != events.end(); ++i){
		name = i->getName();
		content = i->getContent();
		start = i->getStartDate();
		end = i->getEndDate();
		if(start.is_not_a_date_time() && end.is_not_a_date_time()){
			currentTasks++;
			qDebug()<<QString::fromStdString(to_string(currentTasks));
			if(taskCount == maxIssues || 
				taskPage*maxIssues < currentTasks || 
				(taskPage*maxIssues-1) > currentTasks){
				continue;
			}
			else {
				taskCount++;
			}

			dom.findFirst("#task-display").appendInside(QString::fromStdString(name)+"<br>");
			dom.findFirst("#task-display").appendInside(QString::fromStdString(content));
			dom.findFirst("#task-display").appendInside("<hr><br>");
		}
		else if(end.is_not_a_date_time()){
			currentDeadlines++;

			if(deadlineCount == maxIssues || 
				deadlinePage*maxIssues < currentDeadlines || 
				(deadlinePage*maxIssues-1) > currentDeadlines){
				continue;
			}
			else {
				deadlineCount++;
			}

			dom.findFirst("#deadline-display").appendInside(QString::fromStdString(to_simple_string(start))+" ");
			dom.findFirst("#deadline-display").appendInside(QString::fromStdString(name)+"<br>");
			dom.findFirst("#deadline-display").appendInside(QString::fromStdString(content));
			dom.findFirst("#deadline-display").appendInside("<hr><br>");

			if(deadlineCount == maxIssues){
				continue;
			}
			else {

				deadlineCount++;
			}
		}
		else {
			currentEvents++;

			qDebug()<<QString::fromStdString("currentEvents:"+to_string(currentEvents));
			qDebug()<<QString::fromStdString("eventCount:"+to_string(eventCount));
			qDebug()<<QString::fromStdString("eventPage:"+to_string(eventPage));
			if(eventCount == maxIssues || 
				eventPage*maxIssues < currentEvents || 
				(eventPage*maxIssues-1) > currentEvents){
				continue;
			}
			else {
				eventCount++;
			}

			dom.findFirst("#event-display").appendInside(QString::fromStdString(to_simple_string(start))+" to ");
			dom.findFirst("#event-display").appendInside(QString::fromStdString(to_simple_string(end))+" ");
			dom.findFirst("#event-display").appendInside(QString::fromStdString(name)+"<br>");
			dom.findFirst("#event-display").appendInside(QString::fromStdString(content));
			dom.findFirst("#event-display").appendInside("<hr><br>");
		}
	}

	/*currentTasks = 0;
	currentDeadlines = 0;
	currentEvents = 0;*/
}

void uiController::clearGUI(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	QWebElement issueDisplay = dom.findFirst("#event-display");
	issueDisplay.removeAllChildren();
	issueDisplay = dom.findFirst("#deadline-display");
	issueDisplay.removeAllChildren();
	issueDisplay = dom.findFirst("#task-display");
	issueDisplay.removeAllChildren();
}

void uiController::changeButtonDisplay(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();

	if(taskPage == 1){
		QWebElement taskPrevButton = dom.findFirst("#task_back");
		taskPrevButton.setAttribute("style","display:none;");
	}

	if(taskPage*maxIssues > currentTasks && currentTasks >= (taskPage*(maxIssues-1) + 1)){
		QWebElement taskNextButton = dom.findFirst("#task_forward");
		taskNextButton.setAttribute("style","display:none;");
	}

	if(deadlinePage == 1){
		QWebElement deadlinePrevButton = dom.findFirst("#deadline_back");
		deadlinePrevButton.setAttribute("style","display:none;");
	}

	if(deadlinePage*maxIssues > currentDeadlines && currentDeadlines >= (deadlinePage*(maxIssues-1) + 1)){
		QWebElement deadlineNextButton = dom.findFirst("#deadline_forward");
		deadlineNextButton.setAttribute("style","display:none;");
	}

	if(eventPage == 1){
		QWebElement eventPrevButton = dom.findFirst("#event_back");
		eventPrevButton.setAttribute("style","display:none;");
	}

	if(eventPage*maxIssues > currentEvents && currentEvents >= (eventPage*(maxIssues-1) + 1)){
		QWebElement eventNextButton = dom.findFirst("#event_forward");
		eventNextButton.setAttribute("style","display:none;");
	}
}