/********************************************************************************
** Form generated from reading UI file 'testqt.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTQT_H
#define UI_TESTQT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TestQtClass
{
public:
    QAction *newFile;
    QAction *saveFile;
    QAction *drawLine;
    QAction *choose;
    QWidget *centralWidget;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menu_F;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *TestQtClass)
    {
        if (TestQtClass->objectName().isEmpty())
            TestQtClass->setObjectName(QStringLiteral("TestQtClass"));
        TestQtClass->resize(800, 600);
        newFile = new QAction(TestQtClass);
        newFile->setObjectName(QStringLiteral("newFile"));
        saveFile = new QAction(TestQtClass);
        saveFile->setObjectName(QStringLiteral("saveFile"));
        drawLine = new QAction(TestQtClass);
        drawLine->setObjectName(QStringLiteral("drawLine"));
        drawLine->setCheckable(true);
        drawLine->setChecked(false);
        QIcon icon;
        icon.addFile(QStringLiteral(":/TestQt/D:/QtResource/DrawLine.png"), QSize(), QIcon::Normal, QIcon::Off);
        drawLine->setIcon(icon);
        choose = new QAction(TestQtClass);
        choose->setObjectName(QStringLiteral("choose"));
        choose->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/TestQt/D:/QtResource/Choose.png"), QSize(), QIcon::Normal, QIcon::Off);
        choose->setIcon(icon1);
        centralWidget = new QWidget(TestQtClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        TestQtClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(TestQtClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        TestQtClass->setStatusBar(statusBar);
        menuBar = new QMenuBar(TestQtClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 23));
        menu_F = new QMenu(menuBar);
        menu_F->setObjectName(QStringLiteral("menu_F"));
        TestQtClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TestQtClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TestQtClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menu_F->menuAction());
        menu_F->addAction(newFile);
        menu_F->addAction(saveFile);
        mainToolBar->addAction(drawLine);
        mainToolBar->addAction(choose);

        retranslateUi(TestQtClass);

        QMetaObject::connectSlotsByName(TestQtClass);
    } // setupUi

    void retranslateUi(QMainWindow *TestQtClass)
    {
        TestQtClass->setWindowTitle(QApplication::translate("TestQtClass", "TestQt", 0));
        newFile->setText(QApplication::translate("TestQtClass", "\346\226\260\345\273\272\346\226\207\344\273\266", 0));
        saveFile->setText(QApplication::translate("TestQtClass", "\344\277\235\345\255\230\346\226\207\344\273\266", 0));
        drawLine->setText(QApplication::translate("TestQtClass", "\347\224\273\347\272\277", 0));
        choose->setText(QApplication::translate("TestQtClass", "\351\200\211\344\270\255", 0));
        menu_F->setTitle(QApplication::translate("TestQtClass", "\346\226\207\344\273\266(&F)", 0));
    } // retranslateUi

};

namespace Ui {
    class TestQtClass: public Ui_TestQtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTQT_H
