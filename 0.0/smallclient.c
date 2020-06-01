


#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>



#define IPADDRESS "127.0.0.1"
#define SERV_PORT 8888



int main(){

	int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, IPADDRESS, &servaddr.sin_addr);

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == 0){
		printf("connet successed");
	}
	else{
		printf("connet fail");
	}

    close(sockfd);


}
