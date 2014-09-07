#include "stdafx.h"
#include "efficiency.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	efficiency w;
	w.show();
	return a.exec();
}
