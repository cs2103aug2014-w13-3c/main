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

		// Tasks
		this->webView->watchButtonPress("task_id", [this](){
			showOnGUISorted(id, task_type);
		});

		this->webView->watchButtonPress("task_name", [this](){
			showOnGUISorted(name, task_type);
		});

		this->webView->watchButtonPress("task_start", [this](){
			showOnGUISorted(start_date, task_type);
		});

		this->webView->watchButtonPress("task_end", [this](){
			showOnGUISorted(end_date, task_type);
		});

		this->webView->watchButtonPress("task_tags", [this](){
			showOnGUISorted(tags, task_type);
		});
		
		this->webView->watchButtonPress("task_description", [this](){
			showOnGUISorted(description, task_type);
		});

		// Deadlines
		this->webView->watchButtonPress("deadline_id", [this](){
			showOnGUISorted(id, deadline_type);
		});

		this->webView->watchButtonPress("deadline_name", [this](){
			showOnGUISorted(name, deadline_type);
		});

		this->webView->watchButtonPress("deadline_start", [this](){
			showOnGUISorted(start_date, deadline_type);
		});

		this->webView->watchButtonPress("deadline_end", [this](){
			showOnGUISorted(end_date, deadline_type);
		});

		this->webView->watchButtonPress("deadline_tags", [this](){
			showOnGUISorted(tags, deadline_type);
		});

		this->webView->watchButtonPress("deadline_description", [this](){
			showOnGUISorted(description, deadline_type);
		});

		// Events
		this->webView->watchButtonPress("event_id", [this](){
			showOnGUISorted(id, event_type);
		});

		this->webView->watchButtonPress("event_name", [this](){
			showOnGUISorted(name, event_type);
		});

		this->webView->watchButtonPress("event_start", [this](){
			showOnGUISorted(start_date, event_type);
		});

		this->webView->watchButtonPress("event_end", [this](){
			showOnGUISorted(end_date, event_type);
		});

		this->webView->watchButtonPress("event_tags", [this](){
			showOnGUISorted(tags, event_type);
		});

		this->webView->watchButtonPress("event_description", [this](){
			showOnGUISorted(description, event_type);
		});
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
			else if(any_cast<COMMAND_TYPE>(parsedCommand.find("cmd")->second) == HELP){
				displayHelp();
			}
			else if(any_cast<COMMAND_TYPE>(parsedCommand.find("cmd")->second) == EXIT){
				qApp->exit(0);
			}
			else if(any_cast<COMMAND_TYPE>(parsedCommand.find("cmd")->second) == SORT){

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
}

void uiController::displayHelp(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	QWebElement searchResultsWindow = dom.findFirst("#helpWindow");
	searchResultsWindow.addClass("display");
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
	auto addfield = [](QWebElement &e, string field, string attribute){ 
		e.appendInside(QString::fromStdString("<td "+attribute+">"+field+"</td>"));
	};
	auto addQfield = [](QWebElement &e, QString field){ 
		e.appendInside("<td>"+field+"</td>");
	};

	const QChar notcomplete(0x2610);
	const QChar complete(0x2611);
	ptime now = second_clock::local_time();
	date today_date = now.date();
	date start_date;
	date end_date;
	string attribute;

	for(auto issue = issues.begin(); issue!=issues.end(); ++issue)
	{
		qDebug()<<QString::fromStdString(issue->getName());	
		target.appendInside("<tr></tr>");
		auto tr = target.lastChild();
		//is complete?
		addQfield(tr, issue->getCompleteStatus()?complete: notcomplete); 
		//ID
		addfield(tr, to_string(issue->getId()), "");
		//start
		// highlight if date is today's date
		start_date = issue->getStartDate().date();
		if(start_date == today_date){
			attribute = "style='color:red;'";
			addfield(tr, formatDate(issue->getStartDate()), attribute);
		}
		else  {
			addfield(tr, formatDate(issue->getStartDate()), "");
		}
		//end
		// highlight if date is today's date
		end_date = issue->getEndDate().date();
		if(end_date == today_date){
			attribute = "style='color:red;'";
			addfield(tr, formatDate(issue->getEndDate()), attribute);
		}
		else {
			addfield(tr, formatDate(issue->getEndDate()), "");
		}
		//name
		addfield(tr, issue->getName(), "");
		//tags
		addfield(tr, formatTags(issue->getTags()), "");
		//description
		addfield(tr, issue->getContent(), ""); 
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

void uiController::showOnGUISorted(sort_type_t type, issue_type_t issue_type){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	QWebElement target;
	if(issue_type == event_type){
		target = dom.findFirst("#Events-display-target");
		clearEvents();
	}
	else if(issue_type == deadline_type){
		target = dom.findFirst("#Deadlines-display-target");
		clearDeadlines();
	}
	else {
		target = dom.findFirst("#Tasks-display-target");
		clearTasks();
	}

	vector<Controller::CEvent> allIssues = controller->getAllEvents();
	vector<Controller::CEvent> filteredIssues;
	// Populate the issue list
	for(auto issue: allIssues){
		if(issue_type == event_type){
			if(!(issue.getStartDate().is_not_a_date_time() || issue.getEndDate().is_not_a_date_time()))
				filteredIssues.push_back(issue);
		}
		else if(issue_type == deadline_type){
			if(!issue.getStartDate().is_not_a_date_time() && issue.getEndDate().is_not_a_date_time())
				filteredIssues.push_back(issue);
		}
		else {
			if(issue.getStartDate().is_not_a_date_time() && issue.getEndDate().is_not_a_date_time())
				filteredIssues.push_back(issue);
		}
	}

	// Sort the issue list
	for(int i = 0; i < filteredIssues.size(); i++){
		for(int j = 0; j < (filteredIssues.size()-1); j++){
			if(type == id){
				int id1 = filteredIssues[j].getId();
				int id2 = filteredIssues[j+1].getId();
				sortByNum(id1, id2, j, filteredIssues);
			}
			else if(type == name){
				string name1 = filteredIssues[j].getName();
				string name2 = filteredIssues[j+1].getName();
				sortByString(name1, name2, j, filteredIssues);
			}
			else if(type == start_date){
				ptime date1 = filteredIssues[j].getStartDate();
				ptime date2 = filteredIssues[j+1].getStartDate();
				sortByDate(date1, date2, j, filteredIssues);
			}
			else if(type == end_date){
				ptime date1 = filteredIssues[j].getEndDate();
				ptime date2 = filteredIssues[j+1].getEndDate();
				sortByDate(date1, date2, j, filteredIssues);
			}
			else if(type == tags){
				vector<string> tags1 = filteredIssues[j].getTags();
				vector<string> tags2 = filteredIssues[j+1].getTags();
				sortByTag(tags1, tags2, j, filteredIssues);
			}
			else if(type == description){
				string content1 = filteredIssues[j].getContent();
				string content2 = filteredIssues[j+1].getContent();
				sortByString(content1, content2, j, filteredIssues);
			}
		}
	}

	drawTable(filteredIssues, target);
}

void uiController::sortByString(string s1, string s2, int j, 
								vector<Controller::CEvent> &filteredIssues){
	transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
	transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
	if(s1 > s2){
		swapIssues(j, filteredIssues);
	}
}

void uiController::sortByNum(int n1, int n2, int j,
							 vector<Controller::CEvent> &filteredIssues){
	if(n1 > n2){
		swapIssues(j, filteredIssues);
	}
}

void uiController::sortByDate(ptime d1, ptime d2, int j,
					vector<Controller::CEvent> &filteredIssues){
	if(d1 > d2){
		swapIssues(j, filteredIssues);
	}
}

void uiController::sortByTag(vector<string> tags1, vector<string> tags2, int j,
							 vector<Controller::CEvent> &filteredIssues){
	if(tags1.empty()){
		return;
	}
	else if((!tags1.empty() && tags2.empty())
			|| tags1[0] > tags2[0]){
		swapIssues(j, filteredIssues);
	}
}

void uiController::swapIssues(int j, vector<Controller::CEvent> &issues){
		auto temp = issues[j];
		issues[j] = issues[j+1];
		issues[j+1] = temp;
}

void uiController::clearGUI(){
	clearEvents();
	clearDeadlines();
	clearTasks();
}

void uiController::clearEvents(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	auto issuetarget = dom.findFirst("#Events-display-target");
	issuetarget.removeAllChildren();
}

void uiController::clearDeadlines(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	auto issuetarget = dom.findFirst("#Deadlines-display-target");
	issuetarget.removeAllChildren();
}


void uiController::clearTasks(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	auto issuetarget = dom.findFirst("#Tasks-display-target");
	issuetarget.removeAllChildren();
}