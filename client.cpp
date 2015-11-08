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
#include <cstring>

//#define port 8000
//#define SERV_IP "127.0.0.1"
#define port 8000
#define SERV_IP "192.168.43.184"

using namespace std;

void Help()
{
    cout << "reg - Регистрация" << endl;
    cout << "auth - Авторизация" << endl;
    cout << "help - Список команд" << endl;
    cout << "exit - Выход" << endl;
    cout << "Введите команду:" << endl;    
}
void Register(int ServSock)
{
    int bytes_recv;
    char message[100];
	char Login[50], Password[50];
    send(ServSock,"reg", sizeof("reg"), 0);
    cout << "Введите ID из 6 цифр:" << endl;
    cin >> Login;
    cout << "Введите пароль:" << endl;
    cin >> Password;
    send(ServSock, Login, sizeof(Login), 0);
    send(ServSock, Password, sizeof(Password), 0);
    bytes_recv = recv(ServSock, message, sizeof(message), 0);
    cout << message << endl;
    if (strcmp(message,"ERROR") == 0) cout << "Имя пользователя занято!" << endl; else cout << "Успешная регистрация!" << endl << "Введите команду: " << endl;
}
void Auth(int ServSock)
{
    int bytes_recv;
    char message[100];
	char Login[50], Password[50];
    send(ServSock,"2", sizeof("2"), 0);
    cout << "Введите ID:" << endl;
    cin >> Login;
    cout << "Введите пароль:" << endl;
    cin >> Password;
    send(ServSock, Login, sizeof(Login), 0);
    send(ServSock, Password, sizeof(Password), 0);
    bytes_recv = recv(ServSock, message, sizeof(message), 0);
    if (strcmp(message,"ERROR") == 0) cout << "Ошибка авторизации!" << endl; else cout << "Успешная авторизация!" << endl << "Введите команду: " << endl;
}

int main (int argc, char * argv[])
{
    char serv_ip [50];
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    char message[] = "Hello, Hello, Hey!";
    char buff[1024];
    int res, ServSock;
    struct sockaddr_in ServAddr;
    memset(&ServAddr, 0, sizeof(ServAddr));
    /*ServAddr.sin_family = PF_INET;
    ServAddr.sin_port = port;
    res = inet_pton(PF_INET, SERV_IP, &ServAddr.sin_addr);
    ServSock = socket(ServAddr.sin_family, SOCK_STREAM, PF_INET);*/
    strcpy(serv_ip, serv_ip);
    ServAddr.sin_family = AF_INET;
    ServAddr.sin_port = htons(port);
    ServAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    res = inet_pton(AF_INET, SERV_IP, &ServAddr.sin_addr);
    ServSock = socket(ServAddr.sin_family, SOCK_STREAM, 0);
    if (connect(ServSock,  (struct sockaddr *)&ServAddr, sizeof(ServAddr)) < 0) {
        cout << "Ошибка соединения" << endl;
        close(sock);
        return 1;
    }
    while (1)
    {
        cout << "Введите команду(help - список команд):" << endl;
        cin >> message;
        if (strcmp(message, "help") == 0) { Help(); }
        if (strcmp(message, "auth") == 0) { Auth(ServSock); }
        if (strcmp(message, "reg") == 0) { Register(ServSock); }
        if (strcmp(message, "exit") == 0) { break; }
        cin >> message;
        
    }
    return 0;
}

