#pragma once
#include <iostream>
#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>
#include <list>
#include <WS2tcpip.h>
#include <string.h>

#include "MyForm1.h"

#pragma comment (lib, "ws2_32.lib")
#define SERV_ADDR "192.168.43.184"


namespace Messenger {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// —водка дл€ MyForm1
	/// </summary>
	public ref class MyForm1 : public System::Windows::Forms::Form
	{
	public:
		MyForm1(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~MyForm1()
		{
			if (components)
			{
				delete components;
			}
		}
	public: SOCKET my_s;
	private: System::Windows::Forms::RichTextBox^  richTextBox1;
	protected:
	private: System::Windows::Forms::Button^  button1;

	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Timer^  Recieve;

	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->Recieve = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// richTextBox1
			// 
			this->richTextBox1->Location = System::Drawing::Point(59, 39);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->ReadOnly = true;
			this->richTextBox1->Size = System::Drawing::Size(526, 268);
			this->richTextBox1->TabIndex = 5;
			this->richTextBox1->Text = L"";
			this->richTextBox1->TextChanged += gcnew System::EventHandler(this, &MyForm1::richTextBox1_TextChanged);
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->button1->Location = System::Drawing::Point(620, 346);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(148, 41);
			this->button1->TabIndex = 2;
			this->button1->Text = L"ќтправить";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm1::button1_Click);
			// 
			// button2
			// 
			this->button2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->button2->Location = System::Drawing::Point(620, 39);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(148, 41);
			this->button2->TabIndex = 4;
			this->button2->Text = L"ќбновить";
			this->button2->UseVisualStyleBackColor = true;
			// 
			// textBox1
			// 
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->textBox1->Location = System::Drawing::Point(59, 358);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(526, 26);
			this->textBox1->TabIndex = 0;
			this->textBox1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::textBox1_KeyPress);
			// 
			// Recieve
			// 
			this->Recieve->Enabled = true;
			this->Recieve->Interval = 500;
			this->Recieve->Tick += gcnew System::EventHandler(this, &MyForm1::timer1_Tick);
			// 
			// MyForm1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(796, 438);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->richTextBox1);
			this->Name = L"MyForm1";
			this->Text = L"Messenger";
			this->ResumeLayout(false);
			this->PerformLayout();

		}

		void MarshalString(String ^ s, std::string& os) {
			using namespace Runtime::InteropServices;
			const char* chars =
				(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
			os = chars;
			Marshal::FreeHGlobal(IntPtr((void*)chars));
		}

#pragma endregion

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

		char buff[1024];
		int res;
		char message[1024] = "";
		String ^str, ^Message;
		str = this->textBox1->Text;
		std::string NewStr;
		Message = "/MESSAGE ";
		Message += str;
		MarshalString(Message, NewStr);
		strcpy(message, NewStr.c_str());

		send(my_s, message, strlen(message), 0);
		this->richTextBox1->Text += "You: " + str + "\n";

		this->textBox1->Text = "";
	}
private: System::Void textBox1_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {


	if (e->KeyChar == 13)
	{
		char buff[1024];
		int res;
		char message[1024] = "";
		String ^str, ^Message;
		str = this->textBox1->Text;
		std::string NewStr;
		Message = "/MESSAGE ";
		Message += str;
		MarshalString(Message, NewStr);
		strcpy(message, NewStr.c_str());

		send(my_s, message, strlen(message), 0);
		this->richTextBox1->Text += "You: " + str + "\n";

		this->textBox1->Text = "";
		//this->button2 = true;
	}
}
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {

	Recieve->Stop();
	char buff[1024]="";
	String ^str;
	int b_recv;
	while ((b_recv = recv(my_s, buff, sizeof(buff), 0)) != 0)
	{
		std::string s(buff);
		str = gcnew String(s.c_str());
		if (b_recv > 0)
		{
			s.erase(b_recv - 1, 1);
			str = gcnew String(s.c_str());
			int check = strcmp(buff, "/ERROR\r\n");
			if (check == 0) MessageBox::Show("¬ведите им€ получател€!");
				else this->richTextBox1->Text += str;
		}
		if (this->button1->Enabled) break;
	}
	Recieve->Start();
}
private: System::Void richTextBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {

	richTextBox1->SelectionStart += richTextBox1->TextLength;
	richTextBox1->ScrollToCaret();
}
};
}
