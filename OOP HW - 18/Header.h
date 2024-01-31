#pragma once

class User
{
	string _username;
	string _password;
	string _name;
	string _surname;

public:

	User()
	{
		_username = "";
		_password = "";
		_name = "";
		_surname = "";
	}
	User(string username, string password, string name, string surname)
	{
		this->Set_Username(username);
		this->Set_Password(password);
		this->Set_Name(name);
		this->Set_Surname(surname);
	}
	User(const User& other)
	{
		this->Set_Username(other._username);
		this->Set_Password(other._password);
		this->Set_Name(other._name);
		this->Set_Surname(other._surname);
	}

	User& operator=(const User& other)
	{
		this->_username = other._username;
		this->_password = other._password;
		this->_name = other._name;
		this->_surname = other._surname;

		return (*this);
	}

	void Set_Username(string username)
	{
		if (username.length() < 6)
			throw InvalidArgumentException("\nUsername Herf Sayi 6-dan Chox Olmalidir.", GetTime(), __FILE__, __LINE__);
		if (!(int(username.front()) >= 65 && int(username.front()) <= 90))
			throw InvalidArgumentException("\nUsername-in Ilk Herfi Boyuk Olmalidir.", GetTime(), __FILE__, __LINE__);

		this->_username = username;
	}
	void Set_Password(string password)
	{
		if (password.length() < 6)
			throw InvalidArgumentException("\nPassword Herf Sayi 6-dan Chox Olmalidir.", GetTime(), __FILE__, __LINE__);

		this->_password = password;
	}
	void Set_Name(string name)
	{
		if (name.length() < 4)
			throw InvalidArgumentException("\nAdin Herf Sayi 4-dan Chox Olmalidir.", GetTime(), __FILE__, __LINE__);

		this->_name = name;
	}
	void Set_Surname(string surname)
	{
		if (surname.length() < 4)
			throw InvalidArgumentException("\nSoyadin Herf Sayi 4-dan Chox Olmalidir.", GetTime(), __FILE__, __LINE__);

		this->_surname = surname;
	}

	string Get_Username() const { return _username; }
	string Get_Password() const { return _password; }
	string Get_Name() const { return _name; }
	string Get_Surname() const { return _surname; }

	void show() const
	{
		cout << endl << ">>>  User Information  <<<\n";
		cout << endl << "Username : " << _username;
		cout << endl << "Password : " << _password;
		cout << endl << "Name : " << _name;
		cout << endl << "Surname : " << _surname;
		cout << "\n--------------------------------------\n";
	}
};

class Database
{
	User** _users = nullptr;
	size_t _user_count = 0;

public:

	Database() { }
	Database(const Database& other)
	{
		_users = new User * [other._user_count];

		for (size_t k = 0; k < other._user_count; k++)
			_users[k] = new User(*(other._users[k]));

		this->_user_count = other._user_count;
	}

	Database& operator=(const Database& other)
	{
		if (_users != nullptr)
		{
			for (size_t i = 0; i < _user_count; i++)
			{
				delete _users[i];
			}
			delete[] _users;
		}

		_users = new User * [other._user_count];

		for (size_t k = 0; k < other._user_count; k++)
			_users[k] = new User(*(other._users[k]));

		this->_user_count = other._user_count;

		return (*this);
	}

	void addUser(const User& user)
	{
		if (_user_count == 0)
		{
			_users = new User * [1];
			_users[0] = new User(user);
			_user_count++;
			return;
		}

		User** Users = new User * [_user_count + 1];

		for (size_t k = 0; k < _user_count; k++)
			Users[k] = _users[k];

		Users[_user_count] = new User(user);
		_user_count++;

		delete[] _users;
		_users = Users;
	}
	User& getUserByUsername(string username)
	{
		if (_user_count == 0) { throw Exception("\nIstifadechi Sayi 0 dir.", GetTime(), __FILE__, __LINE__); }

		for (size_t us = 0; us < _user_count; us++)
		{
			if (_users[us]->Get_Username() == username)
			{
				User* findedUser = new User(*(_users[us]));

				return *findedUser;
			}
		}
		throw Exception("\nIstifadechi Tapilmadi.", GetTime(), __FILE__, __LINE__);
	}
	void updateUser(User& oldUser, const User& newUser)
	{
		for (size_t k = 0; k < _user_count; k++)
		{
			if (_users[k]->Get_Username() == oldUser.Get_Username())
			{
				*(_users[k]) = newUser;
				return;
			}
		}
		throw Exception("\nIstifadechi Tapilmadi.", GetTime(), __FILE__, __LINE__);
	}

	User** Get_Users() const { return _users; }
	size_t Get_User_Count() const { return _user_count; }

	~Database()
	{
		if (_users != nullptr)
		{
			for (size_t k = 0; k < _user_count; k++)
				if (_users[k] != nullptr)
					delete _users[k];

			delete[] _users;
		}
	}
};

class Registration {

	Database _database;

public:

	Registration(const Database& database)
	{
		_database = database;
	}

	bool Check_User(string username, string password)
	{
		User** users = _database.Get_Users();
		size_t count = _database.Get_User_Count();

		for (size_t m = 0; m < count; m++)
		{
			if (users[m]->Get_Username() == username)
			{
				if (users[m]->Get_Password() == password)
					return true;
				else
					throw Exception("\nPassword Yanlish Daxil Edilib.", GetTime(), __FILE__, __LINE__);
			}
		}
		return false;
	}

	void Sign_In(string username, string password)
	{
		if (this->Check_User(username, password))
			cout << "\n>>>>>> Sign In Successfull. <<<<<<\n";
		else 
			throw Exception("\nBu Username-da Istifadechi Tapilmadi.", GetTime(), __FILE__, __LINE__);
	}

	void Sign_Up(string username, string password, string name, string surname)
	{
		if (this->Check_User(username, password))
			throw DatabaseException("\nBu Username-da Isdifadechi Var.", GetTime(), __FILE__, __LINE__);

		User* newUser = new User(username, password, name, surname);

		_database.addUser(*newUser);
		cout << "\n>>>>>> Sign Up Successfull. <<<<<<\n";
	}

	void ShowAllUsers()
	{
		size_t __len = _database.Get_User_Count();

		User** __users = _database.Get_Users();

		for (size_t _u = 0; _u < __len; _u++)
		{
			__users[_u]->show();
		}
	}
};