


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
# define BUF_SIZE 1024



int main(){

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(sockfd >= 0);

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, IPADDRESS, &servaddr.sin_addr);
	int ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	assert(ret >= 0);

	char buffer_recv[BUF_SIZE] = {0};
	char buffer_send[BUF_SIZE] = {0};

	while(1){
		memset(buffer_recv, 0, BUF_SIZE);//清理缓冲区
		memset(buffer_send, 0, BUF_SIZE);

		printf("client:");
		fgets(buffer_send, BUF_SIZE, stdin);
		send(sockfd, buffer_send, strlen(buffer_send), 0);
		
		if(strcmp(buffer_send, "quit\n") == 0){
			printf("communication is over");
			break;
		}

		ret = recv(sockfd, buffer_recv, BUF_SIZE-1, 0);
		assert(ret >= 0);

		if(strcmp(buffer_recv, "quit\n") == 0 || ret == 0){
			printf("communication is over");
			break;
		}
		printf("server:%s", buffer_recv);
	}

    close(sockfd);

	return 0;


}
