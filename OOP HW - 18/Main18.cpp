#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>

using namespace std;

string GetTime()
{
	time_t now;
	time(&now);

	char* dt = new char[26];

	ctime_s(dt, 26, &now);

	return string(dt);
}

class Exception : public exception
{
	string _text = "";
	string _time = "";
	string _source = "";
	size_t _line = 0;
	string _completed_message = "";

public:

	Exception(string Text, string Time, string Source, size_t Line)
		: exception(Text.c_str())
	{
		this->_text = Text;
		this->_time = Time;
		this->_source = Source;
		this->_line = Line;
		this->_completed_message
			= "\nDescription of Exception : " + _text
			+ "\nException Source : " + _source
			+ "\nException Time : " + _time
			+ "\nException Line : " + to_string(_line)
			+ "\n";
	}

	string Get_Message()const
	{
		return this->_completed_message;
	}

	char const* what() const override
	{
		return this->_completed_message.c_str();
	}

};


class DatabaseException : public Exception
{
	string _text = "\n\t>>>>  Database Exception  <<<<\n";

public:

	DatabaseException(string Text, string Time, string Source, size_t Line)
		: Exception(Text, Time, Source, Line)
	{}

	char const* what() const override
	{
		return (_text + Exception::Get_Message()).c_str();
	}
};


class InvalidArgumentException : public Exception
{
	string _text = "\n\t>>>>  Invalid Argument Exception  <<<<\n";

public:

	InvalidArgumentException(string Text, string Time, string Source, size_t Line)
		: Exception(Text, Time, Source, Line)
	{}

	char const* what() const override
	{
		return (_text + Exception::Get_Message()).c_str();
	}
};

#include "Header.h"

class System {

public:

	static int Key_Check(int key, int choice, int min, int max)
	{
		if (key == 72) {
			choice--;
			if (choice < min) choice = max;
		}
		else if (key == 77) {
			choice--;
			if (choice < min) choice = max;
		}
		else if (key == 75) {
			choice++;
			if (choice > max) choice = min;
		}
		else if (key == 80) {
			choice++;
			if (choice > max) choice = min;
		}
		else if (key == 119) {
			choice--;
			if (choice < min) choice = max;
		}
		else if (key == 97) {
			choice++;
			if (choice > max) choice = min;
		}
		else if (key == 115) {
			choice++;
			if (choice > max) choice = min;
		}
		else if (key == 100) {
			choice--;
			if (choice < min) choice = max;
		}

		return choice;
	}

	static void Control()
	{
		Database db;

		Registration instagram(db);

		short int key, secim = 1;

		while (true)
		{
			system("cls");

			cout << (secim == 1 ? "\t>>>  " : "\t     ") << "[ 1 ] - Show All Users.\n";
			cout << (secim == 2 ? "\t>>>  " : "\t     ") << "[ 2 ] - Sign In.\n";
			cout << (secim == 3 ? "\t>>>  " : "\t     ") << "[ 3 ] - Sign Up.\n";
			cout << (secim == 0 ? "\t>>>  " : "\t     ") << "[ 0 ] - Exit\n";

			key = _getch();

			if (key == 224)
			{
				key = _getch();
				secim = Key_Check(key, secim, 0, 3);
			}
			else if (key == 13)
			{
				if (secim == 1)
				{
					cout << "\n\t\t>>>>>  All Users  <<<<<\n\n";

					instagram.ShowAllUsers();

					system("pause");
				}
				else if (secim == 2)
				{
					string userName, Password;

					cout << "\n\nEnter Username : ";
					cin >> userName;
					cout << "\nEnter Password : ";
					cin >> Password;

					try { instagram.Sign_In(userName, Password); }
					catch (Exception ex) { cout<<ex.what(); }

					system("pause");
				}
				else if (secim == 3)
				{
					string userName, Password, Name, Surname;

					cout << "\n\nEnter Username : ";
					cin >> userName;
					cout << "\nEnter Password : ";
					cin >> Password;
					cout << "\nEnter Name : ";
					cin >> Name;
					cout << "\nEnter Surname : ";
					cin >> Surname;

					try { instagram.Sign_Up(userName, Password, Name, Surname); }
					catch (Exception ex) { cout << ex.what(); }

					system("pause");
				}
				else if (secim == 0) break;
			}
			else { secim = Key_Check(key, secim, 0, 3); }
		}
	}
};


void main()
{
	System::Control();
	system("pause");
}