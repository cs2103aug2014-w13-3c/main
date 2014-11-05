// This file extends the QWebView class to add additional functionality.

#include "stdafx.h"
using namespace std;

QButton::QButton(std::string id, QWebViewWithHooks * web): selector(id), webview(web), QObject(web)
{
	//find the corresponding object
	QWebFrame *frame = web->page()->mainFrame();
	auto element = frame -> findFirstElement(QString::fromStdString("#"+selector));
	assert(!element.isNull());
	//expose this object
	frame->addToJavaScriptWindowObject(QString::fromStdString(selector), this);
	//connect the javascript hooks so that pressed will run.
	element.evaluateJavaScript(QString::fromStdString("this.onclick = function() { "+selector+".pressed(); };"));
	//call the internal _pressed when its pressed.
	QObject::connect(this, SIGNAL(pressed()),
					this, SLOT(_pressed()));
}

void QButton::_pressed(){
	qDebug()<<QString::fromStdString(selector)<< " pressed!!!!";
	emit onPress(selector);
}

QWebViewWithHooks::QWebViewWithHooks(QWidget *parent): QWebView(parent), watches(), watchUuid(0), watchValues() {
	QObject::connect(this, SIGNAL(loadFinished(bool)),
					this, SLOT(onPageLoad(bool)));
}

void QWebViewWithHooks::watch(string selector, Getter getter, Callback callback){
	int uuid = watchUuid++;
	watches.push_back(make_tuple(QString::fromStdString(selector), getter, callback, uuid) );
	watchValues[uuid] = "";
	//TODO: Change to return unwatch function. However, quite lazy.
}

void QWebViewWithHooks::watchButtonPress(string selector, Action callback){
	if( buttonWatches.find(selector) == buttonWatches.end() )
	{
		buttonWatches[selector] = vector<Action>();
		//Also create a new button and register it.
		auto * button = new QButton(selector, this); //lifetime of this object is managed.
		QObject::connect(button, SIGNAL(onPress(std::string)),
			this, SLOT(onButtonPress(std::string)));
	}
	auto& watches = buttonWatches[selector];
	watches.push_back(callback);
	//TODO: Change to return unwatch function. However, quite lazy.
}



void QWebViewWithHooks::keyReleaseEvent(QKeyEvent * ev)
{
	// Look for all watches and get values that are changed.
	// If there is a change, call the callback.
	for(auto it = watches.begin(); it != watches.end(); ++it) {
		auto selector = get<0>(*it);
		auto getter = get<1>(*it);
		auto callback = get<2>(*it);
		auto uuid = get<3>(*it);
		auto element = this->page()->mainFrame()->findFirstElement(selector);
		auto currentValue = getter(element).toStdString();
		if(currentValue != watchValues[uuid] || (ev->key() == Qt::Key_Enter) || (ev->key() == Qt::Key_Return)
			|| (ev->key() == Qt::Key_Up) 
			|| (ev->key() == Qt::Key_Down) )
		{
			watchValues[uuid] = currentValue;
			callback(currentValue, ev);
		}
	}
}

void QWebViewWithHooks::registerPageLoad(QWebViewWithHooks::Action action)
{
	actions.push_back(action);
}

void QWebViewWithHooks::onPageLoad(bool loaded){
	for(auto it = actions.begin(); it != actions.end(); ++it)
		(*it)();
}



void QWebViewWithHooks::onButtonPress(std::string selector){
	//Look up the list of callbacks tied to that selector
	auto& watches = buttonWatches[selector];
	//Call them all.
	for(auto it = watches.begin(); it!=watches.end();++it)
		(*it)();
}