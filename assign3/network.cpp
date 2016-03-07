#include "network.h"
#include "mainwindow.h"

int file = 0;
int sd, connected;
char name[BUFLEN];
time_t t;
struct tm *tm;

MainWindow *window;

void startConnection(MainWindow *w, const char *username, const char *IP , int port, const char *fileName){
  struct hostent	*hp;
  struct sockaddr_in server;

  window = w;
  sprintf(name,username);
  connected = 1;
  
  if(fileName != NULL){
    file = creat(fileName, O_RDWR);
  }

  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    perror("Cannot create socket");
    exit(1);
  }
  
  int flags = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, flags|O_NONBLOCK);
  
  bzero((char *)&server, sizeof(struct sockaddr_in));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  if ((hp = gethostbyname(IP)) == NULL){
    fprintf(stderr, "Unknown server address\n");
    exit(1);
  }
  bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

  // Connecting to the server
  if (connect (sd, (struct sockaddr *)&server, sizeof(server)) == -1){
    fprintf(stderr, "Can't connect to server\n");
    perror("connect");
    exit(1);
  }else{
    window->successfulConnection();
  }

  std::thread t1(receiveFromServer);
  t1.detach();

}

void receiveFromServer(){
  char *bp, rbuf[BUFLEN];
  int bytes_to_read, n;

  while(connected){
    bp = rbuf;
    bytes_to_read = BUFLEN;

    // client makes repeated calls to recv until no more data is expected to arrive.
    n = 0;
    while ((n = recv (sd, bp, bytes_to_read, 0)) < BUFLEN){
	  if(n == -1){
		if(bytes_to_read == BUFLEN){
			break;
		}
		n = 0;
	  }
      bp += n;
      bytes_to_read -= n;
    }

    window->Print(rbuf);

    if(file){
      t  = time(NULL);
      tm = localtime(&t);
      write(file, asctime(tm), strlen(asctime(tm)));
      write(file, rbuf, strlen(rbuf));
    }
  }
  close(sd);
}

void sendToServer(const char *msg){
  char sbuf[BUFLEN];
  sprintf(sbuf,"%s: %s",name, msg);
  send (sd, sbuf, BUFLEN, 0);
  window->Print(rbuf);	
	
  if(file){
    t = time(NULL);
    tm = localtime(&t);
    write(file, asctime(tm), strlen(asctime(tm)));
    write(file, sbuf, strlen(sbuf));
    write(file,"\n",1);
  }
}

void disconnectClient(){
    if(file){
        close(file);
    }
	connected = 0;
}
