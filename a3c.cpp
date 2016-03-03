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

#include <unistd.h>

#define SERVER_TCP_PORT		7000	// Default port
#define BUFLEN			80  	// Buffer length


int main (int argc, char **argv){
	time_t t = time(NULL);
	struct tm *tm;
	int n, bytes_to_read;
	int sd, port, file;
	struct hostent	*hp;
	struct sockaddr_in server;
	char  *host, *bp, rbuf[BUFLEN], tempbuf[BUFLEN], sbuf[BUFLEN], **pptr, *name;
	char str[16];


	switch(argc){
		case 3:
			host =	argv[1];	// Host name
			name =  argv[2];
			port =	SERVER_TCP_PORT;
		break;
		case 4:
			host =	argv[1];
			name =  argv[2];
			file = creat(argv[3], O_RDWR);
			port =	SERVER_TCP_PORT;
		break;
		default:
			fprintf(stderr, "Usage: %s host username [file]\n", argv[0]);
			exit(1);
	}

	// Create the socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("Cannot create socket");
		exit(1);
	}
	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if ((hp = gethostbyname(host)) == NULL){
		fprintf(stderr, "Unknown server address\n");
		exit(1);
	}
	bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

	// Connecting to the server
	if (connect (sd, (struct sockaddr *)&server, sizeof(server)) == -1){
		fprintf(stderr, "Can't connect to server\n");
		perror("connect");
		exit(1);
	}
	printf("Connected:    Server Name: %s\n", hp->h_name);
	pptr = hp->h_addr_list;
	printf("\t\tIP Address: %s\n", inet_ntop(hp->h_addrtype, *pptr, str, sizeof(str)));


	if(fork() == 0){
		//Sending process
		while(1){

			fgets (tempbuf, BUFLEN, stdin);
			sprintf(sbuf,"%s: %s",name, tempbuf);
			// Transmit data through the socket
			tm = localtime(&t);
			write(file, asctime(tm), strlen(asctime(tm)));
			write(file, sbuf, BUFLEN);
			send (sd, sbuf, BUFLEN, 0);
		}
	} else {
		//Receiving process
		while(1){
			bp = rbuf;
			bytes_to_read = BUFLEN;

			// client makes repeated calls to recv until no more data is expected to arrive.
			n = 0;
			while ((n = recv (sd, bp, bytes_to_read, 0)) < BUFLEN){
				bp += n;
				bytes_to_read -= n;
			}

			printf ("%s", rbuf);
			tm = localtime(&t);
			write(file, asctime(tm), strlen(asctime(tm)));
			write(file, rbuf, BUFLEN);
			fflush(stdout);
		}
	}
	close (sd);
	return (0);
}
