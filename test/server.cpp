// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>


#define PORT 6667


int main(void)
{
	//Create network socket (endpoint) for TCP/IP communication
	int serv_fd;
	serv_fd = socket(AF_INET, SOCK_STREAM, 0);

	//Specify address for socket
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;	//Will resolve to any IP on the local machine

	//Bind socket to IP/Port
	int bind_status;

	if ((bind_status = bind(serv_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
		printf("Unable to bind to socket\n");

	//Listen for incoming connections
	listen(serv_fd, 3);

	//Accept connection
	int	client_socket;

	client_socket = accept(serv_fd, NULL, NULL);

	//Send message to client
	char	serv_msg[1024] = "Connection to server successful";
	send(client_socket, serv_msg, sizeof(serv_msg), 0);

	//Close server socket
	close(serv_fd);

}