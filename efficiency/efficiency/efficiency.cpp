#include "stdafx.h"
#include "efficiency.h"

efficiency::efficiency(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.webView->load(QUrl ("qrc:///efficiency/Resources/index.html"));
	uicontroller = std::move(unique_ptr<uiController>(new uiController(ui.webView, unique_ptr<Controller>(new Controller()))));

	/*	//demo use of watch.
	ui.webView->watch("#command-box", 
		[](QWebElement &element)->QString{ return element.evaluateJavaScript("this.value").toString();  }, 
		[](std::string s) { qDebug()<<QString::fromStdString(s); });
	*/
}

efficiency::~efficiency()
{

}

QWebFrame* efficiency::getMainFrame()
{
	return ui.webView->page()->mainFrame();
}

