#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define WINDOW_WELCOME      1
#define WINDOW_CHATSCREEN   2

#include <QMainWindow>
#include <QMessageBox>
#include <QKeyEvent>
#include <QDateTime>
#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    int getWindowState();
    void successfulConnection();
    void ShowChatMessage(char *msg, bool local);
    void ShowChatMessage(QString text, bool local);
    void updateStatusMessage(const QString &msg);
    void changeWindowState(int welcomeScreen);
    void popup(const char * cc);
    void popup(QString cc);
    ~MainWindow();


private slots:
    void OnSendReleased();
    void OnConnectReleased();
    void OnDisconnectReleased();

    void on_logChatCheck_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    void SendData();
};

#endif // MAINWINDOW_H
