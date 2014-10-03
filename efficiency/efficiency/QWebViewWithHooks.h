#ifndef QWEBVIEWWITHHOOKS_H
#define QWEBVIEWWITHHOOKS_H

#include <QtWidgets/QWidget>
#include <QtWebKitWidgets/QWebView>

class QWebViewWithHooks : public QWebView
{
	Q_OBJECT
public:
	QWebViewWithHooks(QWidget *parent): QWebView(parent){}
private:
protected:
	
	
};

#endif // EFFICIENCY_H
