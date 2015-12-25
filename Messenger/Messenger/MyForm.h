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
//#define SERV_ADDR "127.0.0.1"

namespace Auth {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// —водка дл€ MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
//	public: SOCKET my_s;
	public:	Messenger::MyForm1^ mfrm1 = gcnew Messenger::MyForm1;
	public:
		MyForm(void)
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
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	//public: SOCKET my_s;
	private: System::Windows::Forms::RichTextBox^  richTextBox1;
	protected:
	private: System::Windows::Forms::RichTextBox^  richTextBox2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::ErrorProvider^  errorProvider1;
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
			this->richTextBox2 = (gcnew System::Windows::Forms::RichTextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->errorProvider1 = (gcnew System::Windows::Forms::ErrorProvider(this->components));
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->errorProvider1))->BeginInit();
			this->SuspendLayout();
			// 
			// richTextBox1
			// 
			this->richTextBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16));
			this->richTextBox1->Location = System::Drawing::Point(101, 62);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(395, 54);
			this->richTextBox1->TabIndex = 0;
			this->richTextBox1->Text = L"";
			// 
			// richTextBox2
			// 
			this->richTextBox2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16));
			this->richTextBox2->Location = System::Drawing::Point(101, 182);
			this->richTextBox2->Name = L"richTextBox2";
			this->richTextBox2->Size = System::Drawing::Size(395, 54);
			this->richTextBox2->TabIndex = 1;
			this->richTextBox2->Text = L"";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16));
			this->label1->Location = System::Drawing::Point(204, 28);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(204, 31);
			this->label1->TabIndex = 2;
			this->label1->Text = L"¬ведите логин:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16));
			this->label2->Location = System::Drawing::Point(194, 148);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(223, 31);
			this->label2->TabIndex = 3;
			this->label2->Text = L"¬ведите пароль:";
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->button1->Location = System::Drawing::Point(101, 272);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(187, 47);
			this->button1->TabIndex = 4;
			this->button1->Text = L"ј¬“ќ–»«ј÷»я";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// button2
			// 
			this->button2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->button2->Location = System::Drawing::Point(320, 272);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(178, 47);
			this->button2->TabIndex = 5;
			this->button2->Text = L"–≈√»—“–ј÷»я";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// errorProvider1
			// 
			this->errorProvider1->ContainerControl = this;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(555, 370);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->richTextBox2);
			this->Controls->Add(this->richTextBox1);
			this->Name = L"MyForm";
			this->Text = L"Messenger";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->errorProvider1))->EndInit();
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
	//Messenger::MyForm1^ mfrm1 = gcnew Messenger::MyForm1;

	//mfrm1->Show();

	char buff[1024] = "";
	String ^str, ^Login, ^Password;
	int b_recv = 0;
	std::string NewStr;
//	char *message = (char*) calloc(1024, sizeof(char));
	char message[1024];

	Login = this->richTextBox1->Text;
	Password = this->richTextBox2->Text;
	str = "/AUTH ";
	str += Login; str += " "; str += Password;// str += "\r"; str += "\n";
	MarshalString(str, NewStr);
	strcpy(message, NewStr.c_str());
	send(mfrm1->my_s, message, strlen(message), 0);
	while ((b_recv == 0) || (b_recv == -1))
	{
		b_recv = recv(mfrm1->my_s, buff, sizeof(buff), 0);
	}
	int check = strcmp(buff, "/AUTH OK\r\n");
	if (check == 0)
	{
		//Messenger::MyForm1^ mfrm1 = gcnew Messenger::MyForm1;
		mfrm1->Show();
	}
	else {
		this->richTextBox1->Text = "";
		this->richTextBox2->Text = "";
		//errorProvider1->SetError(this->richTextBox1, "ERROR");
	}
}
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {

	char buff[1024] = "";
	String ^str, ^Login, ^Password;
	int b_recv = 0;
	std::string NewStr;
	//	char *message = (char*) calloc(1024, sizeof(char));
	char message[1024];

	Login = this->richTextBox1->Text;
	Password = this->richTextBox2->Text;
	str = "/REGISTER ";
	str += Login; str += " "; str += Password; //str += "\r"; str += "\n";
	MarshalString(str, NewStr);
	strcpy(message, NewStr.c_str());
	send(mfrm1->my_s, message, strlen(message), 0);
	while ((b_recv == 0) || (b_recv == -1))
	{
		b_recv = recv(mfrm1->my_s, buff, sizeof(buff), 0);
	}
	int check = strcmp(buff, "/REGISTER OK\r\n");
	if (check == 0)
	{
		this->richTextBox2->Text = "";
	}
	else {

	}
}
};
}
