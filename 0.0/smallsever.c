 
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>




 int main(){
	/*关于socket*/
 	int listen_fd = socket(AF_INET, SOCK_STREAM, 0);//IPV4, TCPint optval = 1;//设置为此套接字结束后，其对应的端口，可以立即被新的套接字绑定
	int optval = 1;//设置为此套接字结束后，其对应的端口，可以立即被新的套接字绑定
	setsockopt(listen_fd, SOL_SOCKET,  SO_REUSEADDR, &optval, sizeof(optval));
	// 设置服务器IP和Port，和监听描述副绑定
    struct sockaddr_in server_addr;
	int port = 8888;
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short)port);
	bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	listen(listen_fd, 19);
	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof(client); 
	int connfd = accept(listen_fd, (struct sockaddr*)&client, &client_addrlength);
	if(connfd > 0){
		char remote[INET_ADDRSTRLEN];
		printf("connected with ip:%s and port:%d\n",
				inet_ntop(AF_INET,&client.sin_addr,remote, INET_ADDRSTRLEN),
				ntohs(client.sin_port));
				
		close(connfd);
	}

	close(listen_fd);
	return 0;

 	
 }
