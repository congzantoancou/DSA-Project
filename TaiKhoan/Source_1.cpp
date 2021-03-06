#include <iostream>
#include <Windows.h>
#include <stdexcept>
#include <conio.h>
#include <fstream>
#include <string>
#define tab '\t'
using namespace std;

string getpassword(const string& prompt = "Enter password> ")
{
	string result;

	// Set the console mode to no-echo, not-line-buffered input
	DWORD mode, count;
	HANDLE ih = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE oh = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!GetConsoleMode(ih, &mode))
		throw runtime_error(
			"getpassword: You must be connected to a console to use this program.\n"
		);
	SetConsoleMode(ih, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

	// Get the password string
	WriteConsoleA(oh, prompt.c_str(), prompt.length(), &count, NULL);
	char c;
	while (ReadConsoleA(ih, &c, 1, &count, NULL) && (c != '\r') && (c != '\n'))
	{
		if (c == '\b')
		{
			if (result.length())
			{
				WriteConsoleA(oh, "\b \b", 3, &count, NULL);
				result.erase(result.end() - 1);
			}
		}
		else
		{
			WriteConsoleA(oh, "*", 1, &count, NULL);
			result.push_back(c);
		}
	}

	// Restore the console mode
	SetConsoleMode(ih, mode);

	return result;
}

struct BankAcc
{
	char id[15];
	char pin[7];
};

struct node
{
	BankAcc data;
	node *pNext;
};

struct linklist
{
	node *pHead;
	node *pTail;
};

void initial(linklist &l);
node *getNode(BankAcc Account);
void addTail(linklist &l, node *pNew);
void loadData(linklist &l);
void displayData(linklist l); // for deverloper
void login(linklist l);
char *convertStoC(string s); // Convert string to char*

int main()
{
	linklist l;
	initial(l);
	loadData(l);
	login(l);

	system("pause");
	return 0;
}

char *convertStoC(string s)
{
	int len = s.length();
	char *sz = new char[len];
	//string.copy(char*, num_of_char, begin);
	size_t length = s.copy(sz, len, 0);
	sz[length] = '\0';
	return sz;
}

void login(linklist l)
{
	char *id = new char[15];
	char *pin = new char[7];
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	do {
		SetConsoleTextAttribute(out, 14);
		cout << tab << "*****************************************" << endl << tab << "*";
		SetConsoleTextAttribute(out, 10);
		cout << tab << tab << "SYSTEM LOGIN";
		SetConsoleTextAttribute(out, 14);
		cout << tab << tab << "*" << endl;
		cout << tab << "*****************************************" << endl;
		SetConsoleTextAttribute(out, 10);
		cout << endl << tab << "ID: ";
		SetConsoleTextAttribute(out, 7);
		fflush(stdin); cin >> id;
	} while (strlen(id) > 14);
	do {
		string temp_pin;
		SetConsoleTextAttribute(out, 10);
		cout << tab << "PIN: ";
		SetConsoleTextAttribute(out, 7);
		temp_pin = getpassword("");
		// Copy (string) temp_pin to (char*) pin;
		pin = convertStoC(temp_pin);
		cout << endl;
	} while (strlen(pin) > 6);

	bool check = false;
	node *p = new node;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		if (strcmpi(id, p->data.id) == 0)
		{
			int e = 0; // (Error) wrong PIN code typed counter
			do {
				if (strcmpi(pin, p->data.pin) == 0)
					check = true;
				else
				{
					char c;
					cout << "Wrong PIN code!\n"
						<< "Press any key to try again. Press ESC to exit . . .\n";
					c = getch();
					if (c != 27)
					{
						do {
							string temp_pin;
							temp_pin = getpassword("PIN: ");
							pin = convertStoC(temp_pin);
							cout << endl;
						} while (strlen(pin) > 6);
						e++;
					}
					else
						exit(1);
				}
			} while (e < 3 && check == false);
		}
	}
	if (check)
		cout << "Login successfully!\n";
	else
		cout << "Fail\n";
}

void displayData(linklist l)
{
	node *p = new node;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		cout << p->data.id << tab << p->data.pin << endl;
	}
}

void loadData(linklist &l)
{
	ifstream fi;
	fi.open("TheTu.dat");
	if (!fi)
	{
		cout << "Opps! Not found the data file \"TheTu.dat\"!" << endl;
		return;
	}

	BankAcc Account;
	node *p;
	while (!fi.eof())
	{
		fi >> Account.id;
		fi >> Account.pin;
		p = getNode(Account);
		if (p->data.id == "" || p->data.pin == "")
		{
			cout << "Error. Database element of \"TheTu.dat\" is not match! Please check the database file\n";
			return;
		}
		addTail(l, p);
	}
	fi.close();
	cout << "(!) Loading data successfully.\n";
}

void addTail(linklist &l, node *pNew)
{
	if (l.pHead == NULL)
	{
		l.pHead = pNew;
		l.pTail = pNew;
	}
	else
	{
		l.pTail->pNext = pNew;
		l.pTail = pNew;
	}
}

node *getNode(BankAcc Account)
{
	node *p = new node;
	if (p == NULL)
		exit(1);
	else
	{
		p->data = Account;
		p->pNext = NULL;
	}
	return p;
}

void initial(linklist &l)
{
	l.pHead = NULL;
	l.pTail = NULL;
}