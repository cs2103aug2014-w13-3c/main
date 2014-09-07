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

private:
	Ui::efficiencyClass ui;
};

#endif // EFFICIENCY_H
