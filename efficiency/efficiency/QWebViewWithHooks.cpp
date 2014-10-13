// This file extends the QWebView class to add additional functionality.

#include "stdafx.h"
using namespace std;
void QWebViewWithHooks::watch(string selector, Getter getter, Callback callback){
	int uuid = watchUuid++;
	watches.push_back(make_tuple(QString::fromStdString(selector), getter, callback, uuid) );
	watchValues[uuid] = "";
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
		if(currentValue != watchValues[uuid] || ev->key() == Qt::Key_Enter || ev->key() == Qt::Key_Return)
		{
			watchValues[uuid] = currentValue;
			callback(currentValue, ev);
		}
	}
}