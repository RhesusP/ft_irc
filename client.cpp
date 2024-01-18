# include <iostream>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <string>

int main(void)
{
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(6697);
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	std::string str = "Hello World!";
	send(clientSocket, str.c_str(), str.size(), 0);
	send(clientSocket, str.c_str(), str.size(), 0);
	return (0);
}
