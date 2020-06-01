 
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

# define BUF_SIZE 1024


 int main(){
	/*关于socket*/
 	int listen_fd = socket(AF_INET, SOCK_STREAM, 0);//IPV4, TCPint optval = 1;//设置为此套接字结束后，其对应的端口，可以立即被新的套接字绑定
	assert(listen_fd >= 0);
	int optval = 1;//设置为此套接字结束后，其对应的端口，可以立即被新的套接字绑定
	setsockopt(listen_fd, SOL_SOCKET,  SO_REUSEADDR, &optval, sizeof(optval));
	// 设置服务器IP和Port，和监听描述副绑定
    struct sockaddr_in server_addr;
	int port = 8888;
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short)port);
	int ret = bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	assert(ret != -1);
	ret = listen(listen_fd, 19);
	assert(ret != -1);

	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof(client);
	int connfd = accept(listen_fd, (struct sockaddr*)&client, &client_addrlength);
	assert(connfd > 0);

	char buffer_recv[BUF_SIZE] = {0};
	char buffer_send[BUF_SIZE] = {0};
	
	while(1){
		memset(buffer_recv, 0, BUF_SIZE);//清理缓冲区
		memset(buffer_send, 0, BUF_SIZE);

		ret = recv(connfd, buffer_recv, BUF_SIZE-1, 0);
		assert(ret >= 0);

		if(strcmp(buffer_recv, "quit\n") == 0 || ret == 0){
			printf("communication is over");
			break;
		}

		printf("client:%s", buffer_recv);
		printf("server:");
		fgets(buffer_send, BUF_SIZE, stdin);
		send(connfd, buffer_send, strlen(buffer_send), 0);
		if(strcmp(buffer_send, "quit\n") == 0){
			printf("communication is over");
			break;
		}
		
	}
	close(listen_fd);
	close(connfd);
	return 0;

 	
 }
