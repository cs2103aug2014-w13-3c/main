#ifndef EFFICIENCY_H
#define EFFICIENCY_H

#include <QtWidgets/QMainWindow>
#include "ui_efficiency.h"

class efficiency : public QMainWindow
{
	Q_OBJECT

public:
	efficiency(QWidget *parent = 0);
	~efficiency();
	QWebFrame* getMainFrame();
		//@honoo: this is what you are looking for http://qt-project.org/doc/qt-5/qwebframe.html#findAllElements
private:
	Ui::efficiencyClass ui;
};

#endif // EFFICIENCY_H
