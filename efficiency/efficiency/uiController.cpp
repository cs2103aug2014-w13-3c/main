//@author A0101642X
#include "stdafx.h"
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace commandTypeEnum;
using namespace boost::posix_time;
using namespace boost::gregorian;

void drawTable(vector<Controller::CEvent> issues, QWebElement target);

// Constructor. Initialises class variables.
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

		// Register watches on headers for clicks.
		// Tasks
		this->webView->watchButtonPress("task_id", [this](){
			showOnGUISorted("id", "tasks");
		});

		this->webView->watchButtonPress("task_name", [this](){
			showOnGUISorted("name", "tasks");
		});

		this->webView->watchButtonPress("task_start", [this](){
			showOnGUISorted("start", "tasks");
		});

		this->webView->watchButtonPress("task_end", [this](){
			showOnGUISorted("end", "tasks");
		});

		this->webView->watchButtonPress("task_tags", [this](){
			showOnGUISorted("tags", "tasks");
		});
		
		this->webView->watchButtonPress("task_content", [this](){
			showOnGUISorted("content", "tasks");
		});

		// Deadlines
		this->webView->watchButtonPress("deadline_id", [this](){
			showOnGUISorted("id", "deadlines");
		});

		this->webView->watchButtonPress("deadline_name", [this](){
			showOnGUISorted("name", "deadlines");
		});

		this->webView->watchButtonPress("deadline_start", [this](){
			showOnGUISorted("start", "deadlines");
		});

		this->webView->watchButtonPress("deadline_end", [this](){
			showOnGUISorted("end", "deadlines");
		});

		this->webView->watchButtonPress("deadline_tags", [this](){
			showOnGUISorted("tags", "deadlines");
		});

		this->webView->watchButtonPress("deadline_content", [this](){
			showOnGUISorted("content", "deadlines");
		});

		// Events
		this->webView->watchButtonPress("event_id", [this](){
			showOnGUISorted("id", "events");
		});

		this->webView->watchButtonPress("event_name", [this](){
			showOnGUISorted("name", "events");
		});

		this->webView->watchButtonPress("event_start", [this](){
			showOnGUISorted("start", "events");
		});

		this->webView->watchButtonPress("event_end", [this](){
			showOnGUISorted("end", "events");
		});

		this->webView->watchButtonPress("event_tags", [this](){
			showOnGUISorted("tags", "events");
		});

		this->webView->watchButtonPress("event_content", [this](){
			showOnGUISorted("content", "events");
		});
	});

	// Register a watch on the input box for change in input.
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

// Gets input from the command box.
string uiController::getCommandBox(){
	auto element = webView->page()->mainFrame()->documentElement().findFirst("#command-box");
	return element.evaluateJavaScript("this.value").toString().toStdString();
}

// Sets the contents of the command box.
void uiController::setCommandBox(string str){
	auto element = webView->page()->mainFrame()->documentElement().findFirst("#command-box");
	element.evaluateJavaScript(QString::fromStdString("this.value=\'"+str+"\'"));
}

// Empties the command box.
void uiController::clearCommandBox(){
	setCommandBox("");
}

// Gets the next command in the command history.
void uiController::commandHistoryNext(){
	if(commandIndex == commandHistory.size())
		return;
	commandIndex++;
	if(commandIndex == commandHistory.size()) //restore current command
		setCommandBox(commandTmp);
	else
		setCommandBox(commandHistory[commandIndex]);
}

// Gets the previous command in the command history.
void uiController::commandHistoryPrev(){
	if(commandIndex == 0 || commandHistory.size() == 0)
		return;
	commandIndex--;
	if(commandIndex == commandHistory.size()) //temporarily store current command.
		commandTmp = getCommandBox();
	setCommandBox(commandHistory[commandIndex]);
}

// Sends command input to the parser and executor, and outputs the results.
void uiController::onCommandInput(string input){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
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
				string params = any_cast<string>(parsedCommand.find("param")->second);
	
				vector<string> split_params;
				boost::split(split_params, params, boost::is_any_of(" "));

				showOnGUISorted(split_params[1], split_params[0]);
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

// Displays search results in a modal box.
void uiController::displaySearchResults(vector<Controller::CEvent> events){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	QWebElement searchResultsTarget = dom.findFirst("#Search-display-target");
	searchResultsTarget.removeAllChildren();
	drawTable(events, searchResultsTarget);
	QWebElement searchResultsWindow = dom.findFirst("#searchResultsWindow");
	searchResultsWindow.addClass("display");
}

// Displays filtered results in the GUI.
void uiController::displayFilterResults(std::pair<Controller::unregisterAction, string> events){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	QWebElement messageArea = dom.findFirst("#message-area");
	messageArea.appendInside("Filtered issues.<br>");

	clearGUI();
	showOnGUI();
}

// Brings up the Help screen.
void uiController::displayHelp(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	QWebElement searchResultsWindow = dom.findFirst("#helpWindow");
	searchResultsWindow.addClass("display");
}

// Displays result messages in the Message area.
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

// Changes a date into a string for printing.
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

// Formats tags for printing.
string formatTags(vector<string> tags){
	stringstream ss;
	for(auto tag: tags)
		ss<<tag<<" ";
	return ss.str();
}

// Outputs all issues onto the GUI.
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

// Outputs issues of a specific type onto the GUI.
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
		target.appendInside("<tr></tr>");
		auto tr = target.lastChild();
		// Is complete?
		addQfield(tr, issue->getCompleteStatus()?complete: notcomplete); 
		// ID
		addfield(tr, to_string(issue->getId()), "");
		// Start
		// Highlight if date is today's date
		start_date = issue->getStartDate().date();
		if(start_date == today_date){
			attribute = "style='color:red;'";
			addfield(tr, formatDate(issue->getStartDate()), attribute);
		}
		else  {
			addfield(tr, formatDate(issue->getStartDate()), "");
		}
		// End
		// Highlight if date is today's date
		end_date = issue->getEndDate().date();
		if(end_date == today_date){
			attribute = "style='color:red;'";
			addfield(tr, formatDate(issue->getEndDate()), attribute);
		}
		else {
			addfield(tr, formatDate(issue->getEndDate()), "");
		}
		// Name
		addfield(tr, issue->getName(), "");
		// Tags
		addfield(tr, formatTags(issue->getTags()), "");
		// Content
		addfield(tr, issue->getContent(), ""); 
		target.appendInside(tr);
	}
}

// Sorts issues of a certain type by a certain header and outputs them onto the GUI.
void uiController::showOnGUISorted(string type, string issue_type){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	QWebElement target;
	if(issue_type == "events"){
		target = dom.findFirst("#Events-display-target");
	}
	else if(issue_type == "deadlines"){
		target = dom.findFirst("#Deadlines-display-target");
	}
	else if(issue_type == "tasks")
	{
		target = dom.findFirst("#Tasks-display-target");
	}
	else {
		displayResultMessage(invalid_message);
		return;
	}

	vector<Controller::CEvent> allIssues = controller->getAllEvents();
	vector<Controller::CEvent> filteredIssues;
	// Populate the issue list
	for(auto issue: allIssues){
		if(issue_type == "events"){
			if(!(issue.getStartDate().is_not_a_date_time() || issue.getEndDate().is_not_a_date_time()))
				filteredIssues.push_back(issue);
		}
		else if(issue_type == "deadlines"){
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
			if(type == "id"){
				int id1 = filteredIssues[j].getId();
				int id2 = filteredIssues[j+1].getId();
				sortByNum(id1, id2, j, filteredIssues);
			}
			else if(type == "name"){
				string name1 = filteredIssues[j].getName();
				string name2 = filteredIssues[j+1].getName();
				sortByString(name1, name2, j, filteredIssues);
			}
			else if(type == "start"){
				ptime date1 = filteredIssues[j].getStartDate();
				ptime date2 = filteredIssues[j+1].getStartDate();
				sortByDate(date1, date2, j, filteredIssues);
			}
			else if(type == "end"){
				ptime date1 = filteredIssues[j].getEndDate();
				ptime date2 = filteredIssues[j+1].getEndDate();
				sortByDate(date1, date2, j, filteredIssues);
			}
			else if(type == "tags"){
				vector<string> tags1 = filteredIssues[j].getTags();
				vector<string> tags2 = filteredIssues[j+1].getTags();
				sortByTag(tags1, tags2, j, filteredIssues);
			}
			else if(type == "content"){
				string content1 = filteredIssues[j].getContent();
				string content2 = filteredIssues[j+1].getContent();
				sortByString(content1, content2, j, filteredIssues);
			}
			else {
				displayResultMessage(invalid_message);
				return;
			}
		}
	}

	if(issue_type == "events"){
		clearEvents();
	}
	else if(issue_type == "deadlines"){
		clearDeadlines();
	}
	else
	{
		clearTasks();
	}

	drawTable(filteredIssues, target);
}

// Swap issues using a string comparator.
void uiController::sortByString(string s1, string s2, int j, 
								vector<Controller::CEvent> &filteredIssues){
	transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
	transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
	if(s1 > s2){
		swapIssues(j, filteredIssues);
	}
}

// Swap issues using an integer comparator.
void uiController::sortByNum(int n1, int n2, int j,
							 vector<Controller::CEvent> &filteredIssues){
	if(n1 > n2){
		swapIssues(j, filteredIssues);
	}
}

// Swap issues using a date comparator.
void uiController::sortByDate(ptime d1, ptime d2, int j,
					vector<Controller::CEvent> &filteredIssues){
	if(d1 > d2){
		swapIssues(j, filteredIssues);
	}
}

// Swap issues by comparing the first tag.
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

// Generic swap for issues.
void uiController::swapIssues(int j, vector<Controller::CEvent> &issues){
		auto temp = issues[j];
		issues[j] = issues[j+1];
		issues[j+1] = temp;
}

// Clear all issues from the GUI.
void uiController::clearGUI(){
	clearEvents();
	clearDeadlines();
	clearTasks();
}

// Clear Events from the GUI.
void uiController::clearEvents(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	auto issuetarget = dom.findFirst("#Events-display-target");
	issuetarget.removeAllChildren();
}

// Clear Deadlines from the GUI.
void uiController::clearDeadlines(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	auto issuetarget = dom.findFirst("#Deadlines-display-target");
	issuetarget.removeAllChildren();
}

// Clear Tasks from the GUI.
void uiController::clearTasks(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	auto issuetarget = dom.findFirst("#Tasks-display-target");
	issuetarget.removeAllChildren();
}