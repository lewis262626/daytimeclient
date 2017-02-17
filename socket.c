#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>
#include <arpa/inet.h>

enum { MAXLINE=1000 };

int main(int argc, char **argv) {
	int sockfd, n;
	char recvline[MAXLINE+1];
	struct sockaddr_in servaddr;
	time_t ticks;

	if (argc != 2) {
		printf("usage: a.out <IPaddr>\n");
		return -1;
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket could not be created!\n");
		return -1;
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		printf("inet_pton error for %s\n", argv[1]);
		return -1;
	}
	if (connect(sockfd, &servaddr, sizeof(servaddr)) < 0) {
		printf("connect error\n");
		return -1;
	}

	while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0; /* null terminate */
		if (fputs(recvline, stdout) == EOF) {
			printf("fputs error");
			return -1;
		}
	}

	if (n < 0) {
		printf("read error");
		return -1;
	}
	exit(0);
}

