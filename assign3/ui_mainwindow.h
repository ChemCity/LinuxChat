/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QTextEdit *sendWindow;
    QPushButton *sendButton;
    QPushButton *disconnectButton;
    QTextBrowser *chatWindow;
    QGridLayout *loginGrid;
    QPushButton *connectButton;
    QLabel *welcomeLabel;
    QLabel *statusMessage;
    QLineEdit *ipWindow;
    QLineEdit *portWindow;
    QHBoxLayout *horizontalLayout;
    QCheckBox *logChatCheck;
    QLineEdit *logfileWindow;
    QLineEdit *nameWindow;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(710, 674);
        MainWindow->setWindowOpacity(1);
        MainWindow->setDocumentMode(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        sendWindow = new QTextEdit(centralWidget);
        sendWindow->setObjectName(QStringLiteral("sendWindow"));

        gridLayout_2->addWidget(sendWindow, 11, 1, 2, 1);

        sendButton = new QPushButton(centralWidget);
        sendButton->setObjectName(QStringLiteral("sendButton"));
        sendButton->setSizeIncrement(QSize(0, 0));

        gridLayout_2->addWidget(sendButton, 11, 0, 1, 1);

        disconnectButton = new QPushButton(centralWidget);
        disconnectButton->setObjectName(QStringLiteral("disconnectButton"));
        disconnectButton->setFlat(false);

        gridLayout_2->addWidget(disconnectButton, 12, 0, 1, 1);

        chatWindow = new QTextBrowser(centralWidget);
        chatWindow->setObjectName(QStringLiteral("chatWindow"));
        chatWindow->setBaseSize(QSize(0, 0));

        gridLayout_2->addWidget(chatWindow, 10, 0, 1, 2);

        loginGrid = new QGridLayout();
        loginGrid->setSpacing(6);
        loginGrid->setObjectName(QStringLiteral("loginGrid"));
        loginGrid->setSizeConstraint(QLayout::SetDefaultConstraint);
        loginGrid->setContentsMargins(100, 100, 100, 100);
        connectButton = new QPushButton(centralWidget);
        connectButton->setObjectName(QStringLiteral("connectButton"));
        connectButton->setMaximumSize(QSize(16777215, 25));
        connectButton->setCheckable(false);

        loginGrid->addWidget(connectButton, 6, 1, 1, 1);

        welcomeLabel = new QLabel(centralWidget);
        welcomeLabel->setObjectName(QStringLiteral("welcomeLabel"));
        welcomeLabel->setMaximumSize(QSize(16777215, 25));
        welcomeLabel->setLayoutDirection(Qt::LeftToRight);
        welcomeLabel->setAlignment(Qt::AlignCenter);

        loginGrid->addWidget(welcomeLabel, 0, 1, 1, 1);

        statusMessage = new QLabel(centralWidget);
        statusMessage->setObjectName(QStringLiteral("statusMessage"));
        statusMessage->setMaximumSize(QSize(16777215, 25));
        statusMessage->setAlignment(Qt::AlignCenter);

        loginGrid->addWidget(statusMessage, 7, 1, 1, 1);

        ipWindow = new QLineEdit(centralWidget);
        ipWindow->setObjectName(QStringLiteral("ipWindow"));
        ipWindow->setMaxLength(15);

        loginGrid->addWidget(ipWindow, 2, 1, 1, 1);

        portWindow = new QLineEdit(centralWidget);
        portWindow->setObjectName(QStringLiteral("portWindow"));
        portWindow->setMaximumSize(QSize(16777215, 16777215));
        portWindow->setReadOnly(false);

        loginGrid->addWidget(portWindow, 3, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        logChatCheck = new QCheckBox(centralWidget);
        logChatCheck->setObjectName(QStringLiteral("logChatCheck"));

        horizontalLayout->addWidget(logChatCheck);

        logfileWindow = new QLineEdit(centralWidget);
        logfileWindow->setObjectName(QStringLiteral("logfileWindow"));

        horizontalLayout->addWidget(logfileWindow);


        loginGrid->addLayout(horizontalLayout, 4, 1, 1, 1);

        nameWindow = new QLineEdit(centralWidget);
        nameWindow->setObjectName(QStringLiteral("nameWindow"));
        nameWindow->setMaxLength(20);

        loginGrid->addWidget(nameWindow, 1, 1, 1, 1);


        gridLayout_2->addLayout(loginGrid, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 710, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(nameWindow, ipWindow);
        QWidget::setTabOrder(ipWindow, portWindow);
        QWidget::setTabOrder(portWindow, logChatCheck);
        QWidget::setTabOrder(logChatCheck, logfileWindow);
        QWidget::setTabOrder(logfileWindow, connectButton);
        QWidget::setTabOrder(connectButton, sendWindow);
        QWidget::setTabOrder(sendWindow, sendButton);
        QWidget::setTabOrder(sendButton, disconnectButton);
        QWidget::setTabOrder(disconnectButton, chatWindow);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        sendWindow->setPlaceholderText(QApplication::translate("MainWindow", "Enter a Message...", 0));
        sendButton->setText(QApplication::translate("MainWindow", "Send", 0));
        disconnectButton->setText(QApplication::translate("MainWindow", "Disconnect", 0));
        connectButton->setText(QApplication::translate("MainWindow", "Connect", 0));
        welcomeLabel->setText(QApplication::translate("MainWindow", "Welcome to Chat Client", 0));
        statusMessage->setText(QString());
        ipWindow->setPlaceholderText(QApplication::translate("MainWindow", "IP Address", 0));
        portWindow->setPlaceholderText(QApplication::translate("MainWindow", "Port Number", 0));
        logChatCheck->setText(QApplication::translate("MainWindow", "Log Chat?", 0));
        logfileWindow->setPlaceholderText(QApplication::translate("MainWindow", "Chat Dump File", 0));
        nameWindow->setPlaceholderText(QApplication::translate("MainWindow", "Username", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
