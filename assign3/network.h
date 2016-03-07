#ifndef NETWORK_H
#define NETWORK_H

#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <time.h>
#include <thread>
#include <unistd.h>
#include <atomic>
#include "mainwindow.h"


#define SERVER_TCP_PORT		7000	// Default port
#define BUFLEN			80  	// Buffer length

void sendToServer(const char *msg);
void receiveFromServer();
void startConnection(MainWindow *w, const char *username, const char *IP , int port, const char *fileName);
void disconnectClient();

#endif //NETWORK_H
