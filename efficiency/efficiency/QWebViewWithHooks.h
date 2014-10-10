#ifndef QWEBVIEWWITHHOOKS_H
#define QWEBVIEWWITHHOOKS_H

#include <QtWidgets/QWidget>
#include <QtWebKitWidgets/QWebView>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>

class QWebViewWithHooks : public QWebView
{
	Q_OBJECT
	typedef std::function<QString(QWebElement&)> Getter;
	typedef std::function<void(std::string, QKeyEvent *key)> Callback;
	typedef unsigned int UUID;
public:
	QWebViewWithHooks(QWidget *parent): QWebView(parent), watches(), watchUuid(0), watchValues() {}
	//Allows you to register what you want to know.
		//Select using the selector
		//Getter will be passed an element, use it to pull the piece of data you want and return it as a string.
		//you will be notified on changes only via callback(string).
	void watch(std::string selector, Getter getter, Callback callback);
private:
protected:
	void keyReleaseEvent(QKeyEvent * ev) override;
	std::vector<std::tuple<QString, Getter, Callback, UUID> > watches;
	std::unordered_map<UUID, std::string> watchValues;
	int watchUuid;
};

#endif // QWEBVIEWWITHHOOKS_H
