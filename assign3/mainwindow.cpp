#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->chatWindow->hide();
    ui->sendButton->hide();
    ui->sendWindow->hide();
    ui->disconnectButton->hide();
    connect(ui->sendButton, SIGNAL (released()),this, SLOT (OnSendReleased()));
    connect(ui->connectButton, SIGNAL (released()),this, SLOT (OnConnectReleased()));
    connect(ui->disconnectButton, SIGNAL (released()),this, SLOT (OnDisconnectReleased()));
    ui->portWindow->setValidator( new QIntValidator(1, 100000, this) );
    changeWindowState(WINDOW_WELCOME);
}

void MainWindow::OnSendReleased()
{
    SendData();
}

void MainWindow::SendData() {
    if (ui->sendWindow->toPlainText().length() <= 0) {
        return;
    }
    QString text = ui->sendWindow->toPlainText();
    std::string msg (text.toUtf8().constData());
    sendToServer(msg.c_str());

    ui->sendWindow->moveCursor(QTextCursor::Start);
    ui->sendWindow->insertPlainText(ui->nameWindow->text() + ": ");
    ShowChatMessage(ui->sendWindow->toHtml(), true);
    ui->sendWindow->clear();
}

void MainWindow::ShowChatMessage(char *msg, bool local) {
  QString text = QString(msg);
  ui->chatWindow->insertHtml("<div style='color: " + QString((local) ? "green" : "red") + "'>" + text + "</div>");
  ui->chatWindow->append("");

  ui->chatWindow->ensureCursorVisible();
}

void MainWindow::ShowChatMessage(QString text, bool local) {
    ui->chatWindow->insertHtml("<div style='color: " + QString((local) ? "green" : "red") + "'>" + text + "</div>");
    ui->chatWindow->append("");

    ui->chatWindow->ensureCursorVisible();
}

void MainWindow::OnConnectReleased()
{
    if (ui->nameWindow->text().length() <= 0) {
        QMessageBox::information(this, tr("Chat Client"), tr("Enter a username!"));
        return;
    }
    if (ui->ipWindow->text().length() <= 0) {
        QMessageBox::information(this, tr("Chat Client"), tr("Enter an IP!"));
        return;
    }
    if (ui->portWindow->text().length() <= 0) {
        QMessageBox::information(this, tr("Chat Client"), tr("Enter a port number!"));
        return;
    }
    updateStatusMessage("Attempting to Connect...");
    std::string username(ui->nameWindow->text().toUtf8().constData());
    std::string IP(ui->ipWindow->text().toUtf8().constData());
    int port = atoi(ui->portWindow->text().toUtf8().constData());
    std::string filePath((ui->logChatCheck->isChecked()) ? ui->logfileWindow->text().toUtf8().constData() : "NULL");

    startConnection(this, username.c_str(), IP.c_str(), port, filePath.c_str());

}

void MainWindow::successfulConnection() {
    changeWindowState(WINDOW_CHATSCREEN);
}

void MainWindow::updateStatusMessage(const QString &msg) {
    ui->statusMessage->setText(msg);
}

void MainWindow::OnDisconnectReleased()
{
    disconnectClient();
    if (getWindowState() == WINDOW_CHATSCREEN) {
        changeWindowState(WINDOW_WELCOME);
    }
}

void MainWindow::changeWindowState(int welcomeScreen) {
    switch (welcomeScreen) {
    case WINDOW_WELCOME:
        ui->chatWindow->hide();
        ui->sendButton->hide();
        ui->sendWindow->hide();
        ui->disconnectButton->hide();
        ui->connectButton->show();
        ui->nameWindow->show();
        ui->ipWindow->show();
        ui->portWindow->show();
        ui->welcomeLabel->show();
        ui->logChatCheck->show();
        ui->logfileWindow->show();
        updateStatusMessage("");
        ui->statusMessage->show();
        ui->loginGrid->layout()->setMargin(100);
        ui->logfileWindow->setDisabled(true);
    break;
    case WINDOW_CHATSCREEN:
        ui->chatWindow->show();
        ui->sendButton->show();
        ui->sendWindow->show();
        ui->disconnectButton->show();
        ui->connectButton->hide();
        ui->nameWindow->hide();
        ui->ipWindow->hide();
        ui->portWindow->hide();
        ui->welcomeLabel->hide();
        ui->logChatCheck->hide();
        ui->logfileWindow->hide();
        ui->statusMessage->hide();
        ui->loginGrid->layout()->setMargin(0);
    break;
    }
}

int MainWindow::getWindowState() {
    if (ui->chatWindow->isHidden())
        return WINDOW_WELCOME;
    return WINDOW_CHATSCREEN;
}

void MainWindow::popup(const char * cc) {
        QMessageBox::information(this, tr("Chat Client"), tr(cc));
}


void MainWindow::popup(QString cc) {
        QMessageBox::information(this, tr("Chat Client"), cc);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_logChatCheck_stateChanged(int state)
{
    if (state == 0)
        ui->logfileWindow->setDisabled(true);
    else
        ui->logfileWindow->setDisabled(false);

}
