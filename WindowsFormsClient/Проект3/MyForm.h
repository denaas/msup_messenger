#pragma once
#include <iostream>
#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>
#include <list>
#include <WS2tcpip.h>
#include <string.h>
#pragma comment (lib, "ws2_32.lib")
#define SERV_ADDR "127.0.0.1"

using namespace std;

namespace Проект3 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для MyForm
	/// </summary>

	SOCKET my_s, s_sock;

	public ref class MyForm : public System::Windows::Forms::Form
	{
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
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	protected:
	private: System::Windows::Forms::RichTextBox^  richTextBox1;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Button^  button2;


	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
      this->button1 = (gcnew System::Windows::Forms::Button());
      this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
      this->textBox1 = (gcnew System::Windows::Forms::TextBox());
      this->button2 = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // button1
      // 
      this->button1->Location = System::Drawing::Point(328, 236);
      this->button1->Margin = System::Windows::Forms::Padding(2);
      this->button1->Name = L"button1";
      this->button1->Size = System::Drawing::Size(142, 37);
      this->button1->TabIndex = 2;
      this->button1->Text = L"button1";
      this->button1->UseVisualStyleBackColor = true;
      this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
      // 
      // richTextBox1
      // 
      this->richTextBox1->Location = System::Drawing::Point(23, 37);
      this->richTextBox1->Margin = System::Windows::Forms::Padding(2);
      this->richTextBox1->Name = L"richTextBox1";
      this->richTextBox1->ReadOnly = true;
      this->richTextBox1->Size = System::Drawing::Size(293, 163);
      this->richTextBox1->TabIndex = 1;
      this->richTextBox1->Text = L"";
      this->richTextBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::richTextBox1_TextChanged);
      // 
      // textBox1
      // 
      this->textBox1->Location = System::Drawing::Point(23, 236);
      this->textBox1->Margin = System::Windows::Forms::Padding(2);
      this->textBox1->Name = L"textBox1";
      this->textBox1->Size = System::Drawing::Size(293, 20);
      this->textBox1->TabIndex = 0;
      this->textBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox1_TextChanged);
      this->textBox1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::textBox1_KeyPress);
      // 
      // button2
      // 
      this->button2->Location = System::Drawing::Point(328, 37);
      this->button2->Margin = System::Windows::Forms::Padding(2);
      this->button2->Name = L"button2";
      this->button2->Size = System::Drawing::Size(142, 41);
      this->button2->TabIndex = 3;
      this->button2->Text = L"button2";
      this->button2->UseVisualStyleBackColor = true;
      this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
      // 
      // MyForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(479, 364);
      this->Controls->Add(this->button2);
      this->Controls->Add(this->textBox1);
      this->Controls->Add(this->richTextBox1);
      this->Controls->Add(this->button1);
      this->Margin = System::Windows::Forms::Padding(2);
      this->Name = L"MyForm";
      this->Text = L"MyForm";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
	public: void ChangeText(string s)
	{
		this->richTextBox1->Text = L"New Message!";
	}

	void MarshalString(String ^ s, string& os) {
		using namespace Runtime::InteropServices;
		const char* chars =
					(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		os = chars;
		Marshal::FreeHGlobal(IntPtr((void*)chars));
	}

#pragma endregion
	private: System::Void richTextBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		char buff[1024];
		int res;
		char q[1024];
		String ^str;
		str = this->textBox1->Text;
		string NewStr;
		//this->richTextBox1->Text = str;
		MarshalString(str, NewStr);
		strcpy(q, NewStr.c_str());
	
		send(my_s, q, sizeof(q), 0);
		this->richTextBox1->Text += "You: " + str + "\n";
		int b_recv;
		b_recv = recv(my_s, buff, sizeof(buff), 0);
		string s(buff);
		str = gcnew String(s.c_str());
		this->richTextBox1->Text += "Server: " + str + "\n";
		this->textBox1->Text = "";
		//this->button2 = true;
	}
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
	/*while (1)
	{
		char buff[1024];
		String ^str;
		int b_recv;
		b_recv = recv(my_s, buff, sizeof(buff), 0);
		string s(buff);
		str = gcnew String(s.c_str());
		if (b_recv > 0) this->richTextBox1->Text += "Server: " + str + "\n";
		if (this->button1->Enabled) break;
	}*/
}
private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
/*  char buff[1024];
  int res;
  char q[1024];
  String ^str;
  str = this->textBox1->Text;
  string NewStr;
  //this->richTextBox1->Text = str;
  MarshalString(str, NewStr);
  strcpy(q, NewStr.c_str());

  if (strchr(q, '\n') != NULL)
  {
    send(my_s, q, sizeof(q), 0);
    this->richTextBox1->Text += "You: " + str + "\n";
    int b_recv;
    b_recv = recv(my_s, buff, sizeof(buff), 0);
    string s(buff);
    str = gcnew String(s.c_str());
    this->richTextBox1->Text += "Server: " + str + "\n";
    this->textBox1->Text = "";
    //this->button2 = true;
  }
  */
  
}
         
private: System::Void textBox1_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {

  if (e->KeyChar == 13)
  {
    char buff[1024];
    int res;
    char q[1024];
    String ^str;
    str = this->textBox1->Text;
    string NewStr;
    //this->richTextBox1->Text = str;
    MarshalString(str, NewStr);
    strcpy(q, NewStr.c_str());

    send(my_s, q, sizeof(q), 0);
    this->richTextBox1->Text += "You: " + str + "\n";
    int b_recv;
    b_recv = recv(my_s, buff, sizeof(buff), 0);
    string s(buff);
    str = gcnew String(s.c_str());
    this->richTextBox1->Text += "Server: " + str + "\n";
    this->textBox1->Text = "";
    //this->button2 = true;
  }

}

};
}
