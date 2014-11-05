#include "stdafx.h"

using namespace std;
using namespace commandTypeEnum;
using namespace boost::posix_time;
using namespace boost::gregorian;

void  drawTable(vector<Controller::CEvent> issues, QWebElement target);

uiController::uiController(QWebViewWithHooks *webView, unique_ptr<Controller> ctrl):webView(webView),
							controller(std::move(ctrl)),
							maxIssues(2),
							currentEvents(0),
							currentDeadlines(0),
							currentTasks(0),
							taskPage(1),
							deadlinePage(1),
							eventPage(1),
							parser(),
							commandIndex(0){
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
		/*this->webView->watchButtonPress("deadline_forward", [this](){
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
		});*/
	});

	webView->watch("#command-box",
		[](QWebElement &element)->QString{ return element.evaluateJavaScript("this.value").toString(); },
		[this](std::string s, QKeyEvent *key) { 
				if((key->key() == Qt::Key_Enter) || (key->key() == Qt::Key_Return)){
					onCommandInput(s); 
				}
				else if (key->key() == Qt::Key_Up)
				{
					commandHistoryPrev();
				} else if (key->key() == Qt::Key_Down)
				{
					commandHistoryNext();
				}
			});
}

string uiController::getCommandBox(){
	auto element = webView->page()->mainFrame()->documentElement().findFirst("#command-box");
	return element.evaluateJavaScript("this.value").toString().toStdString();
}


void uiController::setCommandBox(string str){
	auto element = webView->page()->mainFrame()->documentElement().findFirst("#command-box");
	element.evaluateJavaScript(QString::fromStdString("this.value=\'"+str+"\'"));
}
void uiController::commandHistoryNext()
{
	if(commandIndex == commandHistory.size())
		return;
	commandIndex++;
	if(commandIndex == commandHistory.size()) //restore current command
		setCommandBox(commandTmp);
	else
		setCommandBox(commandHistory[commandIndex]);
}

void uiController::commandHistoryPrev()
{
	if(commandIndex == 0 || commandHistory.size() == 0)
		return;
	commandIndex--;
	if(commandIndex == commandHistory.size()) //temporarily store current command.
		commandTmp = getCommandBox();
	setCommandBox(commandHistory[commandIndex]);
}

void uiController::clearCommandBox(){
	setCommandBox("");
}


void uiController::onCommandInput(string input){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	qDebug()<<QString::fromStdString(input); // check output
	std::pair<Controller::unregisterAction, string> filterResults;
	vector<Controller::CEvent> searchResults;
	multimap<string,any> parsedCommand;
	try{
		parsedCommand = parser.parseCommand(input);
	}
	catch(...)
	{
		displayResultMessage(invalid_message);
		return;
	}
	if(any_cast<bool>(parsedCommand.find("valid")->second) == true){
		commandHistory.push_back(input);
		commandIndex = commandHistory.size();
		clearCommandBox();
		try {
			if(any_cast<COMMAND_TYPE>(parsedCommand.find("cmd")->second) == SEARCH){
				searchResults = executor->search(parsedCommand);
				displaySearchResults(searchResults);
			}
			else if(any_cast<COMMAND_TYPE>(parsedCommand.find("cmd")->second) == FILTER){
				filterResults = executor->addFilter(parsedCommand);
				displayFilterResults(filterResults);
			}
			else {
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
				else if(any_cast<COMMAND_TYPE>(parsedCommand.find("cmd")->second) == UNDO){
					displayResultMessage(undo_message);
				}
				else if(any_cast<COMMAND_TYPE>(parsedCommand.find("cmd")->second) == MARK_COMPLETE){
					displayResultMessage(mark_complete_message);
				}

				showOnGUI();
				changeButtonDisplay();
			}
		}
		catch(executionError& e){
			if(e.getErrType() == NOTHING_TO_UNDO){
				displayResultMessage(undo_error_message);
			}
			else if(e.getErrType() == NAME_ALREADY_EXISTS){
				displayResultMessage(duplicate_message);
			}
			else if(e.getErrType() == CANNOT_FIND_TARGET){
				displayResultMessage(not_found_message);
			}
		}
	}
	else {
		displayResultMessage(invalid_message);
	}
}

void uiController::displaySearchResults(vector<Controller::CEvent> events){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	QWebElement searchResultsTarget = dom.findFirst("#Search-display-target");
	searchResultsTarget.removeAllChildren();
	drawTable(events, searchResultsTarget);
	QWebElement searchResultsWindow = dom.findFirst("#searchResultsWindow");
	searchResultsWindow.addClass("display");
}

void uiController::displayFilterResults(std::pair<Controller::unregisterAction, string> events){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	QWebElement messageArea = dom.findFirst("#message-area");
	messageArea.appendInside("Filtered issues.<br>");

	clearGUI();
	showOnGUI();
	changeButtonDisplay();
}

void uiController::displayResultMessage(result_message_t message){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	QWebElement messageArea = dom.findFirst("#message-area");
	messageArea.removeAllChildren();

	if(resultMessageStore.size() >= 5){
		resultMessageStore.erase(resultMessageStore.begin(),resultMessageStore.begin()+1);
	}
	resultMessageStore.push_back(message);

	for (auto it = resultMessageStore.begin(); it != resultMessageStore.end(); ++it) {
		if(*it == add_message){
			messageArea.appendInside("Issue added.<br>");
		}
		else if(*it == delete_message) {
			messageArea.appendInside("Issue deleted.<br>");
		}
		else if(*it == update_message) {
			messageArea.appendInside("Issue updated.<br>");
		}
		else if(*it == undo_message) {
			messageArea.appendInside("Undid action.<br>");
		}
		else if(*it == mark_complete_message) {
			messageArea.appendInside("Marked issue as completed.<br>");
		}
		else if(*it == invalid_message){
			messageArea.appendInside("Error: Enter valid command.<br>");
		}
		else if(*it == undo_error_message){
			messageArea.appendInside("Error: Cannot undo.<br>");
		}
		else if(*it == duplicate_message){
			messageArea.
				appendInside("Error: Another issue with the same name already exists.<br>");
		}
		else if(*it == not_found_message){
			messageArea.appendInside("Error: Issue not found.<br>");
		}
	}
}

string formatDate(ptime t){
	if(t.is_not_a_date_time())
		return "-";
	stringstream ss;
	boost::posix_time::time_facet* facet = new boost::posix_time::time_facet();
	facet->format("%d-%b %H:%M");
	ss.imbue(std::locale(std::locale::classic(), facet));
	ss << t;
	return ss.str();
}

string formatTags(vector<string> tags){
	stringstream ss;
	for(auto tag: tags)
		ss<<tag<<" ";
	return ss.str();
}

void drawTable(vector<Controller::CEvent> issues, QWebElement target){
	auto addfield = [](QWebElement &e, string field){ 
		e.appendInside(QString::fromStdString("<td>"+field+"</td>"));
	};
	auto addQfield = [](QWebElement &e, QString field){ 
		e.appendInside("<td>"+field+"</td>");
	};
	const QChar notcomplete(0x2610);
	const QChar complete(0x2611);
	for(auto issue = issues.begin(); issue!=issues.end(); ++issue)
	{
		qDebug()<<QString::fromStdString(issue->getName());	
		target.appendInside("<tr></tr>");
		auto tr = target.lastChild();
		//is complete?
		addQfield(tr, issue->getCompleteStatus()?complete: notcomplete); 
		//ID
		addfield(tr, to_string(issue->getId()));
		//start
		addfield(tr, formatDate(issue->getStartDate())); 
		//end
		addfield(tr, formatDate(issue->getEndDate()));
		//name
		addfield(tr, issue->getName());
		//tags
		addfield(tr, formatTags(issue->getTags()));
		//description
		addfield(tr, issue->getContent()); 
		target.appendInside(tr);
	}
}

void uiController::showOnGUI(){
	clearGUI();
	//draw table with Issues.
	auto allIssues = controller->getAllEvents();
	vector<Controller::CEvent> events;
	vector<Controller::CEvent> tasks;
	vector<Controller::CEvent> deadlines;
	for(auto issue: allIssues)
	{
		if(!(issue.getStartDate().is_not_a_date_time() || issue.getEndDate().is_not_a_date_time()))
			events.push_back(issue);
		if(issue.getStartDate().is_not_a_date_time() && issue.getEndDate().is_not_a_date_time())
			tasks.push_back(issue);
		if(!issue.getStartDate().is_not_a_date_time() && issue.getEndDate().is_not_a_date_time())
			deadlines.push_back(issue);
	}
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	auto eventstarget = dom.findFirst("#Events-display-target");
	drawTable(events, eventstarget);
	auto taskstarget = dom.findFirst("#Tasks-display-target");
	drawTable(tasks, taskstarget);
	auto deadlinestarget = dom.findFirst("#Deadlines-display-target");
	drawTable(deadlines, deadlinestarget);
}

void uiController::clearGUI(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	auto issuetarget = dom.findFirst("#Events-display-target");
	issuetarget.removeAllChildren();
	issuetarget = dom.findFirst("#Deadlines-display-target");
	issuetarget.removeAllChildren();
	issuetarget = dom.findFirst("#Tasks-display-target");
	issuetarget.removeAllChildren();
}

/*
void uiController::showOnGUI(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	QWebElement taskDisplay = dom.findFirst("#task-display");
	QWebElement deadlineDisplay = dom.findFirst("#deadline-display");
	QWebElement eventDisplay = dom.findFirst("#event-display");
	vector<Controller::CEvent> events;

	string name;
	string content;
	vector<string> tags;
	ptime start;
	ptime end;
	bool isComplete;
	string isCompleteString;

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
		tags = i->getTags();

		isComplete = i->getCompleteStatus();
		if(isComplete == true){
			isCompleteString = "Yes";
		}
		else {
			isCompleteString = "No";
		}

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

			taskDisplay.appendInside("Name: "+QString::fromStdString(name)+"<br>");
			taskDisplay.appendInside("Complete: "+QString::fromStdString(isCompleteString)+"<br>");
			taskDisplay.appendInside("Description: "+QString::fromStdString(content)+"<br>");
			taskDisplay.appendInside("Tags: ");

			for(auto j = tags.begin(); j != tags.end(); ++j){
				taskDisplay.appendInside(QString::fromStdString(*j)+", ");
			}

			taskDisplay.appendInside("<hr><br>");
		}
		else if(end.is_not_a_date_time()){
			qDebug()<<QString::fromStdString("a deadline!!");
			currentDeadlines++;

			qDebug()<<QString::fromStdString("deadlinecount "+to_string(deadlineCount));
			qDebug()<<QString::fromStdString("maxIssues "+to_string(maxIssues));
			qDebug()<<QString::fromStdString("deadlinePage "+to_string(deadlinePage));
			qDebug()<<QString::fromStdString("currentDeadlines "+to_string(currentDeadlines));

			if(deadlineCount == maxIssues || 
				deadlinePage*maxIssues < currentDeadlines || 
				(deadlinePage*maxIssues-1) > currentDeadlines){
				continue;
			}
			else {
				deadlineCount++;
			}
			qDebug()<<QString::fromStdString("a deadline2!!");

			deadlineDisplay.appendInside(QString::fromStdString(to_simple_string(start))+" <br>");
			deadlineDisplay.appendInside("Name: "+QString::fromStdString(name)+"<br>");
			deadlineDisplay.appendInside("Complete: "+QString::fromStdString(isCompleteString)+"<br>");
			deadlineDisplay.appendInside("Description: "+QString::fromStdString(content)+"<br>");
			deadlineDisplay.appendInside("Tags: ");

			for(auto j = tags.begin(); j != tags.end(); ++j){
				deadlineDisplay.appendInside(QString::fromStdString(*j)+", ");
			}

			deadlineDisplay.appendInside("<hr><br>");
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

			eventDisplay.appendInside(QString::fromStdString(to_simple_string(start))+" to ");
			eventDisplay.appendInside(QString::fromStdString(to_simple_string(end))+" <br>");
			eventDisplay.appendInside("Name: "+QString::fromStdString(name)+"<br>");
			eventDisplay.appendInside("Complete: "+QString::fromStdString(isCompleteString)+"<br>");
			eventDisplay.appendInside("Description: "+QString::fromStdString(content)+"<br>");
			eventDisplay.appendInside("Tags: ");

			for(auto j = tags.begin(); j != tags.end(); ++j){
				eventDisplay.appendInside(QString::fromStdString(*j)+", ");
			}

			eventDisplay.appendInside("<hr><br>");
		}
	}

}*/

/*
void uiController::clearGUI(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	QWebElement issueDisplay = dom.findFirst("#event-display");
	issueDisplay.removeAllChildren();
	issueDisplay = dom.findFirst("#deadline-display");
	issueDisplay.removeAllChildren();
	issueDisplay = dom.findFirst("#task-display");
	issueDisplay.removeAllChildren();
}*/



void uiController::changeButtonDisplay(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();

	if(taskPage == 1){
		QWebElement taskPrevButton = dom.findFirst("#task_back");
		taskPrevButton.setAttribute("style","display:none;");
	}

	if(taskPage > 1){
		QWebElement taskPrevButton = dom.findFirst("#task_back");
		taskPrevButton.setAttribute("style","display:inline;");
	}

	if(currentTasks > maxIssues || taskPage*maxIssues < currentTasks){
		QWebElement taskNextButton = dom.findFirst("#task_forward");
		taskNextButton.setAttribute("style","display:inline;");
	}

	if((taskPage*maxIssues >= currentTasks && currentTasks >= (taskPage*(maxIssues-1) + 1))
		|| currentTasks <= maxIssues){
		QWebElement taskNextButton = dom.findFirst("#task_forward");
		taskNextButton.setAttribute("style","display:none;");
	}

	if(deadlinePage == 1){
		QWebElement deadlinePrevButton = dom.findFirst("#deadline_back");
		deadlinePrevButton.setAttribute("style","display:none;");
	}

	if(deadlinePage > 1){
		QWebElement deadlinePrevButton = dom.findFirst("#deadline_back");
		deadlinePrevButton.setAttribute("style","display:inline;");
	}

	if(currentDeadlines > maxIssues || deadlinePage*maxIssues < currentDeadlines){
		QWebElement deadlineNextButton = dom.findFirst("#deadline_forward");
		deadlineNextButton.setAttribute("style","display:inline;");
	}

	if((deadlinePage*maxIssues >= currentDeadlines && currentDeadlines >= (deadlinePage*(maxIssues-1) + 1))
		|| currentDeadlines <= maxIssues){
		QWebElement deadlineNextButton = dom.findFirst("#deadline_forward");
		deadlineNextButton.setAttribute("style","display:none;");
	}

	if(eventPage == 1){
		QWebElement eventPrevButton = dom.findFirst("#event_back");
		eventPrevButton.setAttribute("style","display:none;");
	}

	if(eventPage > 1){
		QWebElement eventPrevButton = dom.findFirst("#event_back");
		eventPrevButton.setAttribute("style","display:inline;");
	}

	if(currentEvents > maxIssues || eventPage*maxIssues < currentEvents){
		QWebElement eventNextButton = dom.findFirst("#event_forward");
		eventNextButton.setAttribute("style","display:inline;");
	}

	if((eventPage*maxIssues >= currentEvents && currentEvents >= (eventPage*(maxIssues-1) + 1))
		|| currentEvents <= maxIssues){
		QWebElement eventNextButton = dom.findFirst("#event_forward");
		eventNextButton.setAttribute("style","display:none;");
	}
}