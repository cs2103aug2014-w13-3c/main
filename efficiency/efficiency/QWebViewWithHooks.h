#ifndef QWEBVIEWWITHHOOKS_H
#define QWEBVIEWWITHHOOKS_H

#include <QtWidgets/QWidget>
#include <QtWebKitWidgets/QWebView>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>

class QButton;

class QWebViewWithHooks : public QWebView
{
	Q_OBJECT
	typedef std::function<QString(QWebElement&)> Getter;
	typedef std::function<void(std::string, QKeyEvent *key)> Callback;
	typedef std::function<void()> Action;
	typedef unsigned int UUID;
public:
	QWebViewWithHooks(QWidget *parent);
	//Allows you to register what you want to know.
		//Select using the selector
		//Getter will be passed an element, use it to pull the piece of data you want and return it as a string.
		//you will be notified on changes only via callback(string).
	void watch(std::string selector, Getter getter, Callback callback);
	void watchButtonPress(std::string selector, Action callback);
	void registerPageLoad(Action a); //give me a void function which does stuff.
private:
protected:
	void keyReleaseEvent(QKeyEvent * ev) override;
	std::vector<std::tuple<QString, Getter, Callback, UUID> > watches;
	std::unordered_map<std::string, std::vector<Action>> buttonWatches;
	std::unordered_map<std::string, QButton> buttons;
	std::unordered_map<UUID, std::string> watchValues;
	std::vector<Action> actions;
	UUID watchUuid;


protected slots:
	void onPageLoad(bool loaded);
	void onButtonPress(std::string selector);
};

class QButton: public QObject{
	Q_OBJECT
	protected:
		std::string selector;
		QWebViewWithHooks * webview;
	public:
		QButton(std::string selector, QWebViewWithHooks * webview);
	signals:
		void pressed();
		void onPress(std::string selector);
	protected slots:
		void _pressed();
};

#endif // QWEBVIEWWITHHOOKS_H
