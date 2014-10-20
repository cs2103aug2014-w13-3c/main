#include "stdafx.h"

using namespace std;

uiController::uiController(QWebViewWithHooks *webView, unique_ptr<Controller> ctrl):webView(webView),
							controller(std::move(ctrl)){
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
	qDebug()<<QString::fromStdString(input); // check output

	map<string,function<void(string)>> functionStore;
	functionStore["add"] = [this](string input)->
							void{ 
								controller->addEvent(input);
								displayResultMessage(add_message);
								showOnGUI();
							};
	//functionStore["update"] = [this](string input)->void{};
	functionStore["delete"] = [this](string input)->
								void{
									controller->deleteEvent(atoi(input.c_str()));
									displayResultMessage(delete_message);
									showOnGUI();
								};

	string command = input.substr(0, input.find(" "));
	string content = input.substr(input.find(" ")+1);

	if(functionStore.find(command) == functionStore.end()) {
			qDebug()<< QString::fromStdString("Error: Please enter a valid command.\n");
			LOG(INFO) << "Error: Please enter a valid command.\n";
	}
	else {
		functionStore[command](content);
	}
}

void uiController::displayResultMessage(result_message_t message){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	if(message == add_message){
		dom.findFirst("#message-box").appendInside("Task added.<br>");
	}
	else if(message == delete_message){
		dom.findFirst("#message-box").appendInside("Task deleted.<br>");
	}
}

void uiController::showOnGUI(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	vector<Controller::CEvent> events;
	events = controller->getAllEvents();
	string name;

	// TODO: clear the display first
	clearGUI();

	for(auto i = events.begin(); i != events.end(); ++i){
		name = i->getName();
		dom.findFirst("#issue-display").appendInside(QString::fromStdString(name)+"<br>");
	}
}

void uiController::clearGUI(){
	QWebElement dom = webView->page()->mainFrame()->documentElement();
	QWebElement issueDisplay = dom.findFirst("#issue-display");
	issueDisplay.removeAllChildren();
}