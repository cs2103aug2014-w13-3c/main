#include "stdafx.h"

using namespace std;

uiController::uiController(QWebViewWithHooks *webView, Controller * ctrl):webView(webView) , controller(ctrl){
	webView->watch("#command-box",
		[](QWebElement &element)->QString{ return element.evaluateJavaScript("this.value").toString(); },
		[this](std::string s) { onCommandInput(s); });
}

void uiController::onCommandInput(string input){
	qDebug()<<QString::fromStdString(input); // check output

	map<string,function<void(string)>> functionStore;
	functionStore["add"] = [this](string input)->void{ controller->createEvent("testevent"); };
	//functionStore["update"] = [this](string input)->void{};
	functionStore["delete"] = [this](string input)->void{controller->deleteEvent(atoi(input.c_str()));};

	string command = input.substr(0, input.find(" "));
	string content = input.substr(input.find(" ")+1);

	if(functionStore.find(command) == functionStore.end()) {
			qDebug()<< QString::fromStdString("Error: Please enter a valid command.\n");
	}
	else {
		functionStore[command](content);
	}
}