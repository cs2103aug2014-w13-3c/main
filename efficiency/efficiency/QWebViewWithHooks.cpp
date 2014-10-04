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
	for(auto it = watches.begin(); it != watches.end(); ++it) {
		auto selector = get<0>(*it);
		auto getter = get<1>(*it);
		auto callback = get<2>(*it);
		auto uuid = get<3>(*it);
		auto element = this->page()->mainFrame()->findFirstElement(selector);
		auto currentValue = getter(element).toStdString();
		if(currentValue != watchValues[uuid])
		{
			watchValues[uuid] = currentValue;
			callback(currentValue);
		}
	}
}