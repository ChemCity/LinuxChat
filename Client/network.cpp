#include "network.h"
#include "mainwindow.h"

/*------------------------------------------------------------------------------------------------------------------
--  SOURCE FILE:    network.cpp -   A file which handles the network layer of the client side of the chat
--                                     client program.
--
--  PROGRAM:        ChatClient
--
--  FUNCTIONS:      void startConnection(MainWindow *w, const char *username, const char *IP , int port, const char *fileName)
--		    void receiveFromServer()			
--		    void sendToServer(const char *msg, bool sysMsg)
--		    void disconnectClient()	
--
--
--  DATE:           March 14th, 2016
--
--  REVISIONS:      March 23rd, 2016 - connection timeout, joined/left chat messages (Gabriella Cheung)
--
--  DESIGNER:       Tom Tang
--
--  PROGRAMMER:     Tom Tang
--
--  NOTES:
--  This file contains the client side networking code
--
----------------------------------------------------------------------------------------------------------------------*/

int file = 0;
int sd;
char name[BUFLEN];
time_t t;
struct tm *tm;
bool connected;

MainWindow *window;

/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       startConnection
--
--  DATE:           March 14th, 2016
--
--  REVISIONS:      March 23rd, 2016 - added "joined chat" message (Gabriella Cheung)
--		    March 22nd, 2016 - added connection timeout (Gabriella Cheung)
--
--  DESIGNER:       Tom Tang
--
--  PROGRAMMER:     Tom Tang
--
--  INTERFACE:      void startConnection(MainWindow *w, const char *username, const char *IP , int port, const char *fileName)
--		        MainWindow *w         pointer to main ui window                      
--		        const char *username  username
--			const char *IP        IP to connect to
--			int port              port number to bind too
--			const char *fileName  file to dump user data
--
--
--  RETURNS:        void
--
--  NOTES:
--  This method initializes the server connection and sends up the file dump if needed
--
----------------------------------------------------------------------------------------------------------------------*/
void startConnection(MainWindow *w, const char *username, const char *IP , int port, const char *fileName){
  struct hostent	*hp;
  struct sockaddr_in server;
  fd_set fdset;
  struct timeval tv;
  int rv;

  window = w;
  sprintf(name,username);
	connected = true;

  if(fileName != NULL){
    file = creat(fileName, O_RDWR);
  }

  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
      window->updateStatusMessage("Cannot Create Socket");
      return;
  }
  
  int flags = fcntl(file, F_GETFL, 0);
  fcntl(file, F_SETFL, flags|O_NONBLOCK);
  
  bzero((char *)&server, sizeof(struct sockaddr_in));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  if ((hp = gethostbyname(IP)) == NULL){
      window->updateStatusMessage("Unknown Server Address");
      return;
  }
  bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

  // Connecting to the server
    fcntl(sd, F_SETFL, O_NONBLOCK);
    connect (sd, (struct sockaddr *)&server, sizeof(server));

    FD_ZERO(&fdset);
    FD_SET(sd, &fdset);
    tv.tv_sec = 10;             /* 10 second timeout */
    tv.tv_usec = 0;

    if ((rv = select(sd+1, NULL, &fdset, NULL, &tv)) == 1)
    {
	int so_error;
        socklen_t len = sizeof so_error;

        getsockopt(sd, SOL_SOCKET, SO_ERROR, &so_error, &len);

        if (so_error == 0) {
            int flags = fcntl(sd, F_GETFL, 0);
            fcntl(sd, F_SETFL, flags|O_NONBLOCK);
	    sendToServer("joined the chat", true);
            window->successfulConnection();
        }
    } else if (rv == 0)
    {
	window->updateStatusMessage("Cannot Connect to Server");
	return;
    }

  std::thread t1(receiveFromServer);
  t1.detach();

}

/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       receiveFromServer
--
--  DATE:           March 14th, 2016
--
--  REVISIONS:      None
--
--  DESIGNER:       Tom Tang
--
--  PROGRAMMER:     Tom Tang
--
--  INTERFACE:      void receiveFromServer()
--
--
--  RETURNS:        void
--
--  NOTES:
--  This function polls for server messages and sends the messages to be shown on the UI
--
----------------------------------------------------------------------------------------------------------------------*/
void receiveFromServer(){
  char *bp, rbuf[BUFLEN];
  int bytes_to_read, n;

  while(connected){
    bp = rbuf;
    bytes_to_read = BUFLEN;

    // client makes repeated calls to recv until no more data is expected to arrive.
    n = 0;
    while (connected && (n = recv (sd, bp, bytes_to_read, 0)) < BUFLEN){
      if (n == -1){
        n = 0;
      }
      bp += n;
      bytes_to_read -= n;
    }


    window->ShowChatMessage(rbuf, false);

    if(file && n > 0){

      t  = time(NULL);
      tm = localtime(&t);
      write(file, asctime(tm), strlen(asctime(tm)));
      write(file, rbuf, strlen(rbuf));
      write(file,"\n",1);
    }
  }
  close(sd);

}

/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       sendToServer
--
--  DATE:           March 14th, 2016
--
--  REVISIONS:      March 23rd, 2016 - added message formatting flag (Gabriella Cheung)
--
--  DESIGNER:       Tom Tang
--
--  PROGRAMMER:     Tom Tang
--
--  INTERFACE:      void sendToServer(const char *msg, bool sysMsg)
--			const char *msg  message to be sent to the server
--			bool sysMsg  flag for message formatting
--
--
--  RETURNS:        void
--
--  NOTES:
--  This function sends message to the server
--
----------------------------------------------------------------------------------------------------------------------*/
void sendToServer(const char *msg, bool sysMsg){
  char sbuf[BUFLEN];
  if (sysMsg)
  {
    sprintf(sbuf,"%s %s",name, msg);
  } else
  {
    sprintf(sbuf,"%s: %s",name, msg);
  }
  send (sd, sbuf, BUFLEN, 0);


  if(file){
    t = time(NULL);
    tm = localtime(&t);
    write(file, asctime(tm), strlen(asctime(tm)));
    write(file, sbuf, strlen(sbuf));
    write(file,"\n",1);
  }
}

/*------------------------------------------------------------------------------------------------------------------
--  FUNCTION:       disconnectClient
--
--  DATE:           March 14th, 2016
--
--  REVISIONS:      March 23rd, 2016 - added disconnect message (Gabriella Cheung)
--
--  DESIGNER:       Tom Tang
--
--  PROGRAMMER:     Tom Tang
--
--  INTERFACE:      void disconnectClient()
--
--
--  RETURNS:        void
--
--  NOTES:
--  This function signals for the network to disconnect and closes the file handler
--
----------------------------------------------------------------------------------------------------------------------*/
void disconnectClient(){
    sendToServer("left the chat", true);
    if(file){
        close(file);
    }
    connected = false;
}
