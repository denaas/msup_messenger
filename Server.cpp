#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define SERV_IP "127.0.0.1"
#define port 8080

using namespace std;

int main() 
{
	char buff[1024];
	struct sockaddr_in SockAddr;
	int ServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (ServSock == -1) {
		perror("ошибка при создании сокета");
		exit(EXIT_FAILURE);
	}

	memset(&SockAddr, 0, sizeof (SockAddr));

	SockAddr.sin_family = PF_INET;
	SockAddr.sin_port = htons(port);
	SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(ServSock, (struct sockaddr*) &SockAddr, sizeof (SockAddr)) == -1) {
		perror("Ошибка: связывания");

		close(ServSock);
		exit(EXIT_FAILURE);
	}

	if (listen(ServSock, 10) == -1) {
		cout << "Ошибка: прослушивания";

		close(ServSock);
		return 0;
	}
	/*listen(ServSock, 1);
	int Client;
	cout << "New Connection" << endl;
	int bytes_recv, res;
	 while(1)
	{
        sock = accept(listener, NULL, NULL);
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }

        while(1)
        {
            bytes_read = recv(sock, buf, 1024, 0);
            if(bytes_read <= 0) break;
            send(sock, buf, bytes_read, 0);
        }
    
        close(sock);
	}*/

	int Client = accept(ServSock, 0, 0);
	cout << "New Connection" << endl;
	int bytes_recv, res;
	bytes_recv = recv(Client, buff, sizeof(buff), 0);
	res = send(Client, buff, sizeof(buff), 0);
	cout << buff;
	return 0;
}
