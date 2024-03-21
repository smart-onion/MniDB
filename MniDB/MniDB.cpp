#include <iostream>
#include <windows.h>
#include <ctime>

using namespace std;

enum SIZES { LENGTH = 3, TXT = 100 };

enum CREDENTIAL {
	LOGIN,
	PASWWORD,
	TIME,
};

enum ACTION {
	ADD_USER,
	DELETE_USER,
	CHANGE_PASSWORD,
	SHOW_DATA
};

int GetStringLength(char* ar)
{
	int length = 0;
	int i = 0;
	while (ar[i] != 0)
	{
		length++;
		i++;
	}
	return length;
}

void CreateDB(char**** db, int* size)
{
	*db = new char** [*size];
	for (int i = 0; i < *size; i++)
	{
		(*db)[i] = new char* [SIZES::LENGTH];
		for (int j = 0; j < SIZES::LENGTH; j++)
		{
			(*db)[i][j] = new char[SIZES::TXT] {};
		}
	}

}


void DeleteDB(char**** db, int* size)
{
	for (int i = 0; i < *size; i++)
	{
		for (int j = 0; j < SIZES::LENGTH; j++)
		{
			delete[](*db)[i][j];
		}
		delete[](*db)[i];
	}
	delete[](*db);
}

int UserExist(char*** db, int size, char* login)
{
	for (int i = 0; i < size; i++)
	{
		if (strcmp(db[i][CREDENTIAL::LOGIN], login) == 0)
		{
			return i;
		}
	}
	return -1;
}

void AddUser(char**** db, int* size, char* login, char* password)
{
	if (UserExist(*db, *size, login) != -1)
	{
		cout << "\nUser already exist!\n\n";
		return;
	}
	char*** temp = nullptr;
	int tempSize = *size + 1;
	CreateDB(&temp, &tempSize);


	time_t now = time(0);
	char buffer[SIZES::TXT];
	ctime_s(buffer, sizeof(buffer), &now);

	int loginSize = GetStringLength(login);
	int passwordSize = GetStringLength(password);

	for (int i = 0; i < *size; i++)
	{
		for (int j = 0; j < SIZES::LENGTH; j++)
		{
			for (int k = 0; k < SIZES::TXT; k++)
			{
				if ((*db)[i][j][k] == 0) break;

				temp[i][j][k] = (*db)[i][j][k];
			}
		}
	}

	for (int i = 0; i < loginSize; i++)
	{
		temp[*size][CREDENTIAL::LOGIN][i] = login[i];
	}
	for (int i = 0; i < passwordSize; i++)
	{
		temp[*size][CREDENTIAL::PASWWORD][i] = password[i];
	}
	for (int i = 0; i < SIZES::TXT; i++)
	{
		temp[*size][CREDENTIAL::TIME][i] = buffer[i];
	}

	DeleteDB(db, size);

	(*size)++;

	CreateDB(db, size);

	for (int i = 0; i < *size; i++)
	{
		for (int j = 0; j < SIZES::LENGTH; j++)
		{
			for (int k = 0; k < SIZES::TXT; k++)
			{
				if (temp[i][j][k] == 0) break;

				(*db)[i][j][k] = temp[i][j][k];
			}
		}
	}

	DeleteDB(&temp, size);

	cout << "\nUser '" << login << "' created!\n";
}



void DeleteUser(char**** db, int* size, int index)
{
	if (index >= *size || index < 0)
	{
		cout << "User not exist!";
		return;
	}
	char*** temp = nullptr;
	(*size)--;
	CreateDB(&temp, size);

	for (int i = 0, i2 = 0; i < *size; i++, i2++)
	{
		if (i2 == index) i2 += 1;

		for (int j = 0; j < SIZES::LENGTH; j++)
		{
			for (int k = 0; k < SIZES::TXT; k++)
			{
				if ((*db)[i2][j][k] == 0) break;

				temp[i][j][k] = (*db)[i2][j][k];
			}
		}
	}
	DeleteDB(db, size);
	CreateDB(db, size);

	for (int i = 0; i < *size; i++)
	{
		for (int j = 0; j < SIZES::LENGTH; j++)
		{
			for (int k = 0; k < SIZES::TXT; k++)
			{
				if (temp[i][j][k] == 0) break;

				(*db)[i][j][k] = temp[i][j][k];
			}
		}
	}

	DeleteDB(&temp, size);

	cout << "\nDeleted Successfully!\n";
}

void ChangePassword(char*** db, int size, char* login, char* password)
{
	int id = UserExist(db, size, login);

	if (id != -1)
	{
		for (int i = 0; i < SIZES::TXT - 1; i++)
		{
			db[id][CREDENTIAL::PASWWORD][i] = password[i];
		}

		cout << "\nChanged successfully!\n";
	}
	else {
		cout << "User not exist\n";
		return;
	}


}

void PrintDB(char*** db, int size)
{
	cout << "---------------------------------------------\n\n";
	for (int i = 0; i < size; i++)
	{
		cout << i << " - ";
		for (int j = 0; j < SIZES::LENGTH; j++)
		{

			cout << db[i][j] << " ";

		}
		cout << '\n';
	}
	cout << "\n---------------------------------------------\n\n";

}


int main()
{
	int size = 0;
	char*** db = nullptr;

	int action;

	char* login = new char[SIZES::TXT];
	char* password = new char[SIZES::TXT];
	int userToDelete;
	CreateDB(&db, &size);

	while (true)
	{
		cout << "\nData Base:\n0 - Add new user;\n1 - Delete user;\n2 - Change password;\n3 - Show Data Base;\n~// ";
		cin >> action;
		cout << '\n';
		switch (action)
		{
		case ADD_USER:
			cout << "Enter login: ";
			cin >> login;
			cout << "Enter password: ";
			cin >> password;
			AddUser(&db, &size, login, password);
			break;
		case DELETE_USER:
			cout << "Enter index of user to delete: ";
			cin >> userToDelete;
			DeleteUser(&db, &size, userToDelete);
			break;
		case CHANGE_PASSWORD:
			cout << "Enter login: ";
			cin >> login;
			cout << "Enter new password: ";
			cin >> password;
			ChangePassword(db, size, login, password);
			break;
		case SHOW_DATA:
			PrintDB(db, size);
			break;
		default:
			cout << "action not exist!";
			break;
		}
	}

}
