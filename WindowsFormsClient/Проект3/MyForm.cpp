#include "MyForm.h"
#include <Windows.h>
#include <iostream>
#include <fstream>

using namespace Проект3;

void DealWithIt(int n)
{
	ofstream OUTP;
	OUTP.open("output.txt");
	if (n == 1)
	{
		Application::EnableVisualStyles();
		Application::SetCompatibleTextRenderingDefault(false);
		MyForm form;
		Application::Run(%form);
	}
	if (n == 2)
	{
		while (1)
		{
			char buff[1024];
			String ^str;
			int b_recv;
			b_recv = recv(my_s, buff, sizeof(buff), 0);
			string s(buff);
			str = gcnew String(s.c_str());
			//if (b_recv > 0) this->richTextBox1->Text += "Server: " + str + "\n";
			if (b_recv > 0) OUTP << buff << endl;
			//if (this->button1->Enabled) break;
		}
	}
}

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
/*
#pragma omp parallel num_threads(2)

	{
#pragma omp for

		for (int n = 0; n < 3; ++n) DealWithIt(n);

	}*/
	

	return 0;
}
