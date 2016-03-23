#include "mainwindow.h"
#include "network.h"


/*------------------------------------------------------------------------------------------------------------------
--  SOURCE FILE:    mainwindow.cpp -   A file which handles the application layer of the client side of the chat
--                                     client program.
--
--  PROGRAM:        ChatClient
--
--  FUNCTIONS:      explicit MainWindow(QWidget *parent = 0);
--                  int getWindowState();
--                  void successfulConnection();
--                  void ShowChatMessage(char *msg, bool local);
--                  void ShowChatMessage(QString text, bool local);
--                  void updateStatusMessage(const QString &msg);
--                  void changeWindowState(int welcomeScreen);
--                  void popup(const char * cc);
--                  void popup(QString cc);
--                  ~MainWindow();
--                  void OnSendReleased();
--                  void OnConnectReleased();
--                  void OnDisconnectReleased();
--                  void onLogChatCheckStateChanged(int state);
--                  Ui::MainWindow *ui;
--                  void SendData();
--
--  DATE:           March 7th, 2016
--
--  REVISIONS:      None
--
--  DESIGNER:       Jaegar Sarauer
--
--  PROGRAMMER:     Jaegar Sarauer
--
--  NOTES:
--  This file contains class functions of the client sided portion of the ChatClient program. It handles the connection
--  and wrapper functions on the application layer, allowing the network layer to easily connect to these functions and
--  allow functionality of the UI to be clear and easily interchangeable if needed.
--
--  This class contains all functions and wrappers in order for the UI of the ChatClient to be able to work as intended.
----------------------------------------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       MainWindow
--
--  DATE:           March 7th, 2016
--
--  REVISIONS:      None
--
--  DESIGNER:       Jaegar Sarauer
--
--  PROGRAMMER:     Jaegar Sarauer
--
--  INTERFACE:      MainWindow(QWidget *parent) :
--                      QMainWindow(parent),
--                      ui(new Ui::MainWindow)
--
--  RETURNS:        nothing, constructor.
--
--  NOTES:
--  The constructor of the MainWindow class. This will instantiate all listeners for buttons within the program, and
--  hide all elements within which should not be shown when the program is first started.
----------------------------------------------------------------------------------------------------------------------*/
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


/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       OnSendReleased
--
--  DATE:           March 7th, 2016
--
--  REVISIONS:      None
--
--  DESIGNER:       Jaegar Sarauer
--
--  PROGRAMMER:     Jaegar Sarauer
--
--  INTERFACE:      void OnSendReleased()
--
--  RETURNS:        void
--
--  NOTES:
--  Button listener for the "Send" button which exists within the WINDOW_CHATSCREEN window. Upon clicking the "Send"
--  button, this function will trigger, calling the SendData() function which will collect data and send it to the
--  server.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::OnSendReleased()
{
    SendData();
}



/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       SendData
--
--  DATE:           March 7th, 2016
--
--  REVISIONS:      March 23rd, 2016 - check message length to make sure it is not too long, truncate message if it is
--                                      (Gabriella Cheung)
--
--  DESIGNER:       Jaegar Sarauer
--
--  PROGRAMMER:     Jaegar Sarauer
--
--  INTERFACE:      void SendData()
--
--  RETURNS:        void
--
--  NOTES:
--  Upon calling this function, the text from the sendWindow element will be processed into plain text to be sent to the
--  server, while also converting it into HTML and sent to the local client, to show colors of the message. The text
--  from this element will be sent to the chat window using the ShowChatMessage function which will process the color
--  of the text and format it to the chatWindow.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::SendData() {
    if (ui->sendWindow->toPlainText().length() <= 0) {
        return;
    }
    QString text = ui->sendWindow->toPlainText();
    std::string msg (text.toStdString());
    std::string name = ui->nameWindow->text().toStdString();
    if ((msg.length() + name.length() + 2) > 1023)
    {
        char message[1024];
        size_t msglen = 1023 - name.length() - 2;

        //truncate message before sending
        strncpy(message, msg.c_str(), msglen);
        message[msglen] = '\0';
        sendToServer(message, false);
    } else
    {
        sendToServer(msg.c_str(), false);
    }

    ui->sendWindow->moveCursor(QTextCursor::Start);
    ui->sendWindow->insertPlainText(ui->nameWindow->text() + ": ");
    ShowChatMessage(ui->sendWindow->toPlainText(), true);
    ui->sendWindow->clear();
}



/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       ShowChatMessage
--
--  DATE:           March 7th, 2016
--
--  REVISIONS:      None
--
--  DESIGNER:       Jaegar Sarauer
--
--  PROGRAMMER:     Jaegar Sarauer
--
--  INTERFACE:      void ShowChatMessage(char *msg, bool local)
--                      char *msg
--                          = The message which is intended to be written to the chatScreen.
--                      bool local
--                          = if local is true, the message should show a local color.
--
--  RETURNS:        void
--
--  NOTES:
--  This function takes in a char * and processes the text into html, which allows the text to be represented with
--  color within the chatWindow. If the function is called with the bool local as true, the color of the text will
--  appear to be a color representing your own chat message.
--  This function is intended to be used in the network layer, as it deals with char *'s rather than QStrings.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::ShowChatMessage(char *msg, bool local) {
  QString text = QString(msg);
  QTime time(QTime::currentTime());
  ui->chatWindow->insertHtml("<div style='color: " + QString((local) ? "green" : "red") + "'>" + "[" + time.toString() + "] " + text.remove(QRegExp("<[^>]*>")) + "</div>");
  ui->chatWindow->append("");
  ui->chatWindow->ensureCursorVisible();
}



/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       ShowChatMessage
--
--  DATE:           March 7th, 2016
--
--  REVISIONS:      None
--
--  DESIGNER:       Jaegar Sarauer
--
--  PROGRAMMER:     Jaegar Sarauer
--
--  INTERFACE:      void ShowChatMessage(QString text, bool local)
--                      QString text
--                          = The message which is intended to be written to the chatScreen.
--                      bool local
--                          = if local is true, the message should show a local color.
--
--  RETURNS:        void
--
--  NOTES:
--  This function takes in a QString and processes the text into html, which allows the text to be represented with
--  color within the chatWindow. If the function is called with the bool local as true, the color of the text will
--  appear to be a color representing your own chat message.
--  This function is intended to be used in the application layer, as it uses QStrings more often that char *'s. This
--  avoids the uneccessary conversion from QString to char * to QString again.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::ShowChatMessage(QString text, bool local) {
    QTime time(QTime::currentTime());
    ui->chatWindow->insertHtml("<div style='color: " + QString((local) ? "green" : "red") + "'>" + "[" + time.toString() + "] " + text.remove(QRegExp("<[^>]*>")) + "</div>");
    ui->chatWindow->append("");
    ui->chatWindow->ensureCursorVisible();
}



/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       OnConnectReleased
--
--  DATE:           March 7th, 2016
--
--  REVISIONS:      None
--
--  DESIGNER:       Jaegar Sarauer
--
--  PROGRAMMER:     Jaegar Sarauer
--
--  INTERFACE:      void OnConnectReleased()
--
--  RETURNS:        void
--
--  NOTES:
--  This function is a button listener for the "Connect" button. It will check to ensure all necessary fields are
--  filled and correctly, and processes the filled information and passed to the network layer of the client.
----------------------------------------------------------------------------------------------------------------------*/
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



/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       successfulConnection
--
--  DATE:           March 7th, 2016
--
--  REVISIONS:      None
--
--  DESIGNER:       Jaegar Sarauer
--
--  PROGRAMMER:     Jaegar Sarauer
--
--  INTERFACE:      void successfulConnection()
--
--  RETURNS:        void
--
--  NOTES:
--  This function is intended to be a success call on the attempt to connect to the server, which is started by the
--  OnConnectRelease function. When the network layer has completed the connection, it will call this function to notify
--  the UI to update itself and display the chat messaging lobby to allow chat messages to be sent and recieved.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::successfulConnection() {
    changeWindowState(WINDOW_CHATSCREEN);
}



/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       updateStatusMessage
--
--  DATE:           March 7th, 2016
--
--  REVISIONS:      None
--
--  DESIGNER:       Jaegar Sarauer
--
--  PROGRAMMER:     Jaegar Sarauer
--
--  INTERFACE:      void updateStatusMessage(const QString &msg)
--
--  RETURNS:        void
--
--  NOTES:
--  This function is intended to show and updated message on an attempt to connect to the server. When the user
--  activates the OnConnectReleased function by clicking the "Connect" button found within the WINDOW_WELCOME window,
--  the client will show a message below this button to tell the user what the client is currently attempting to do
--  (or any error messages when attempting to connect).
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::updateStatusMessage(const QString &msg) {
    ui->statusMessage->setText(msg);
}



/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       OnDisconnectReleased
--
--  DATE:           March 7th, 2016
--
--  REVISIONS:      None
--
--  DESIGNER:       Jaegar Sarauer
--
--  PROGRAMMER:     Jaegar Sarauer
--
--  INTERFACE:      void OnDisconnectReleased()
--
--  RETURNS:        void
--
--  NOTES:
--  Button listener for the "Disconnect" button from within the "WINDOW_CHATSCREEN" window. Upon callign this function,
--  the network layer will be notified that the user is attempting to disconnect from the server.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::OnDisconnectReleased()
{
    disconnectClient();

    if (getWindowState() == WINDOW_CHATSCREEN) {
        changeWindowState(WINDOW_WELCOME);
    }
    ui->sendWindow->clear();
}



/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       changeWindowState
--
--  DATE:           March 7th, 2016
--
--  REVISIONS:      None
--
--  DESIGNER:       Jaegar Sarauer
--
--  PROGRAMMER:     Jaegar Sarauer
--
--  INTERFACE:      void changeWindowState(int welcomeScreen)
--                      int welcomeScreen
--                          The screen which the UI should be changed to, use WINDOW_ defines. See mainwindow.h.
--
--  RETURNS:        void
--
--  NOTES:
--  This function handles switching the UI elements to show a new window, depending on which type of window is
--  requested. Use WINDOW_ defines found within mainwindow.h file to call this function.
--
--  The function will show those elements which belong to the screen, and reset fields if needed.
----------------------------------------------------------------------------------------------------------------------*/
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
    default:
    break;
    }
}



/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       getWindowState
--
--  DATE:           March 7th, 2016
--
--  REVISIONS:      None
--
--  DESIGNER:       Jaegar Sarauer
--
--  PROGRAMMER:     Jaegar Sarauer
--
--  INTERFACE:      int getWindowState()
--
--  RETURNS:        int (WINDOW_ define)
--                      = The defined ID (see WINDOW_ in mainwindow.h) to which window is currently being shown.
--
--  NOTES:
--  This function will return the defined ID (see WINDOW_ in mainwindow.h) to which window is being shown.
--  WINDOW_WELCOME indicates that the welcome screen of the UI is the current window.
--  WINDOW_CHATSCREEN indicats that the lobby of the ChatClient is currently being shown.
----------------------------------------------------------------------------------------------------------------------*/
int MainWindow::getWindowState() {
    if (ui->chatWindow->isHidden())
        return WINDOW_WELCOME;
    return WINDOW_CHATSCREEN;
}



/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       popup
--
--  DATE:           March 7th, 2016
--
--  REVISIONS:      None
--
--  DESIGNER:       Jaegar Sarauer
--
--  PROGRAMMER:     Jaegar Sarauer
--
--  INTERFACE:      void popup(const char * cc)
--
--  RETURNS:        void
--
--  NOTES:
--  Wrapper function to show a popup message with a message passed in as a char *. Intended to be used as a notification
--  from the network layer, as it does not deal with QStrings, so must use char *'s.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::popup(const char * cc) {
        QMessageBox::information(this, tr("ChatClient"), tr(cc));
}



/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       popup
--
--  DATE:           March 7th, 2016
--
--  REVISIONS:      None
--
--  DESIGNER:       Jaegar Sarauer
--
--  PROGRAMMER:     Jaegar Sarauer
--
--  INTERFACE:      void popup(const char * cc)
--
--  RETURNS:        void
--
--  NOTES:
--  Wrapper function to show a popup message with a message passed in as a QString. Intended to be used as a
--  notification from the UI layer, as it mainly uses QStrings. This avoids the pointless conversion from char * and
--  back to QStrings such as in the other popup function.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::popup(QString cc) {
        QMessageBox::information(this, tr("ChatClient"), cc);
}



/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       ~MainWindow
--
--  DATE:           March 7th, 2016
--
--  REVISIONS:      None
--
--  DESIGNER:       Jaegar Sarauer
--
--  PROGRAMMER:     Jaegar Sarauer
--
--  INTERFACE:      ~MainWindow()
--
--  RETURNS:        nothing, destructor.
--
--  NOTES:
--  This function is the destructor of the MainWindow class. It disposes of the UI.
----------------------------------------------------------------------------------------------------------------------*/
MainWindow::~MainWindow()
{
    delete ui;
}



/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       on_logChatCheck_clicked
--
--  DATE:           March 7th, 2016
--
--  REVISIONS:      None
--
--  DESIGNER:       Jaegar Sarauer
--
--  PROGRAMMER:     Jaegar Sarauer
--
--  INTERFACE:      void onLogChatCheckStateChanged(int state)
--
--  RETURNS:        void
--
--  NOTES:
--  This function is a listener for the check box on the WINDOW_WELCOME screen.
--  It will auto update the field to enter in the log path you choose to create, allowing the log file option to be
--  disabled if the user chooses.
----------------------------------------------------------------------------------------------------------------------*/


void MainWindow::on_logChatCheck_clicked(bool checked)
{
    if (checked == 0)
        ui->logfileWindow->setDisabled(true);
    else
        ui->logfileWindow->setDisabled(false);
}
