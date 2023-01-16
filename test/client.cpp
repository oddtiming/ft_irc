#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include <sys/types.h>
#include <stdlib.h>

#include <unistd.h>
#include <netinet/in.h>

#define PORT 6697
 




int main(void)
{
	//Create network socket (endpoint) for TCP/IP communication
	int client_fd;
	client_fd = socket(AF_INET, SOCK_STREAM, 0);

	//Specify address for socket
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;	//Same as trying to connectto 0.0.0.0 

	//Attempt to connect to socket
	int	connect_status;
	if ((connect_status = connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) //Cast addr struct to correct type
		printf("Unable to connect to server\n");
	
	//Read from server
	char	buffer[1024];
	while (1) {
		recv(client_fd, &buffer, sizeof(buffer), 0);
		printf("Received from server: %s\n", buffer);
	}


	close(client_fd);
}