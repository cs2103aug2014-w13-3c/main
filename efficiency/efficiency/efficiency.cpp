#include "stdafx.h"
#include "efficiency.h"

efficiency::efficiency(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.webView->load(QUrl ("qrc:///efficiency/Resources/index.html"));
}

efficiency::~efficiency()
{

}
