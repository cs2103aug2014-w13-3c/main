#include "stdafx.h"

using namespace std;
using namespace CommandTypeEnum;
using namespace boost::posix_time;
using namespace boost::gregorian;

uiController::uiController(QWebViewWithHooks *webView, unique_ptr<Controller> ctrl):webView(webView),
							controller(std::move(ctrl)), 
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
	});

	// Register the various GUI watches here.
	
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
			dom.findFirst("#message-box").appendInside("Task added.<br>");
		}
		else if(*it == delete_message) {
			dom.findFirst("#message-box").appendInside("Task deleted.<br>");
		}
		else if(*it == invalid_message){
			dom.findFirst("#message-box").appendInside("Error: Enter valid command.<br>");
		}
	}
}

void uiController::showOnGUI(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	vector<Controller::CEvent> events;
	int iterator = 0;

	string name;
	ptime start;
	ptime end;

	events = controller->getAllEvents();
	clearGUI();

	for(auto i = events.begin(); i != events.end(); ++i){
		name = i->getName();
		start = i->getStartDate();
		end = i->getEndDate();
		if(start.is_not_a_date_time() && end.is_not_a_date_time()){
			dom.findFirst("#task-display").appendInside(QString::fromStdString(name)+"<hr><br>");
		}
	}
}

void uiController::clearGUI(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	QWebElement issueDisplay = dom.findFirst("#issue-display");
	issueDisplay.removeAllChildren();
}