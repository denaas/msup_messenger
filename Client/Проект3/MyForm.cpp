#include "MyForm.h"
#include <Windows.h>
#include <iostream>
#include <fstream>

using namespace Проект3;


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	printf("TCP CLIENT DEMO\n");
	WSAData wsaData;
	int res;
	res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0)
	{
		cout << "WSA fail \n";
		system("pause");
	}
	addrinfo* loc_addr;
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	ZeroMemory(&loc_addr, sizeof(loc_addr));
	hints.ai_family = AF_INET;
	hints.ai_protocol = IPPROTO_TCP;
	res = getaddrinfo(SERV_ADDR, "8080", &hints, &loc_addr);
	my_s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	connect(my_s, loc_addr->ai_addr, (int)loc_addr->ai_addrlen);
	if (res) {
		//printf("Connect error %d\n", WSAGetLastError());
		//system("pause");
		exit;
	}
	//s_sock = accept(my_s, NULL, NULL);
	//res = bind(s_sock, loc_addr->ai_addr, (int)loc_addr->ai_addrlen);
	bool l = true;
	ioctlsocket(my_s, FIONBIO, (unsigned long*)&l);
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	MyForm form;
	Application::Run(%form);

	return 0;
}
