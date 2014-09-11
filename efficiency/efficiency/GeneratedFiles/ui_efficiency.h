/********************************************************************************
** Form generated from reading UI file 'efficiency.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EFFICIENCY_H
#define UI_EFFICIENCY_H

#include <QtCore/QVariant>
#include <QtWebKitWidgets/QWebView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_efficiencyClass
{
public:
    QWidget *centralWidget;
    QWebView *webView;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *efficiencyClass)
    {
        if (efficiencyClass->objectName().isEmpty())
            efficiencyClass->setObjectName(QStringLiteral("efficiencyClass"));
        efficiencyClass->resize(692, 544);
        centralWidget = new QWidget(efficiencyClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        webView = new QWebView(centralWidget);
        webView->setObjectName(QStringLiteral("webView"));
        webView->setGeometry(QRect(-1, -1, 961, 511));
        webView->setUrl(QUrl(QStringLiteral("about:blank")));
        efficiencyClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(efficiencyClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 692, 21));
        efficiencyClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(efficiencyClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        efficiencyClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(efficiencyClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        efficiencyClass->setStatusBar(statusBar);

        retranslateUi(efficiencyClass);

        QMetaObject::connectSlotsByName(efficiencyClass);
    } // setupUi

    void retranslateUi(QMainWindow *efficiencyClass)
    {
        efficiencyClass->setWindowTitle(QApplication::translate("efficiencyClass", "efficiency", 0));
    } // retranslateUi

};

namespace Ui {
    class efficiencyClass: public Ui_efficiencyClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EFFICIENCY_H
