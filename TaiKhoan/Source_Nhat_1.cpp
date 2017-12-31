#include <iostream>
#include <Windows.h>
#include <stdexcept>
#include <conio.h>
#include <fstream>
#include <string>
#include <iomanip>
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
	char name[32];
	double balance;
	char currency[4];
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
void loadDataID(linklist &l);
void writeDataID(linklist l, char *id);
void writeHistory(linklist l, char *id, double soTienGD, char *idNhan);
void displayData(linklist l); // for deverloper
char *login(linklist l); // Dang nhap
char *convertStoC(string s); // Convert string to char*
void checkBalance(linklist l, char *id); // Kiem tra so du
void withDraw(linklist &l, char *id); // Rut tien
bool transfer(linklist &l, char *idGui, char* idNhan); // Chuyen tien
double convertCurrency(double Amount, char *from, char *to); // Chuyen doi ty gia tien te
void reviewTransactions(linklist l, char *id); // Xem lai noi dung giao dich
void select(int &nChon); //Lua chon xem co tiep tuc chuc nang khac khi ket thuc chuc nang da chon

int main()
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	linklist l;
	initial(l);
	loadData(l);
	loadDataID(l);
	int nChoose, nChooseOther;
	char *id = login(l);

	if (id[0] == NULL)
	{
		SetConsoleTextAttribute(out, 12);
		cout << "Login failed! Please take back the card!\n";
		SetConsoleTextAttribute(out, 7);
	}
	else
	{
		Sleep(1000);
		system("cls");

		//Dung MENU de chay cac chuc nang
		do {
			system("cls");
			cout << tab << "\t\t --MENU--" << endl;
			cout << tab << "1 - Withdrawals" << endl; // Rut tien
			cout << tab << "2 - Transfer" << endl; // Chuyen tien
			cout << tab << "3 - Check transaction history" << endl; // Kiem tra lich su giao dich
			cout << tab << "4 - Balance enquiry" << endl; // Kiem tra so du tai khoan
			cout << tab << "5 - Features other" << endl; //Chon chuc nang khac
			cout << tab << "0 - Press other key to exit !" << endl;
			cout << tab << "Please ! Enter to features: ";
			cin >> nChoose;
			char *idNhan = new char[15];
			switch (nChoose)
			{
			case 1:
				withDraw(l, id);
				Sleep(700);
				select(nChoose);
				break;
			case 2:
				transfer(l, id, idNhan);
				Sleep(700);
				select(nChoose);
				break;
			case 3:
				//reviewTransactions(l, id);
				Sleep(700);
				select(nChoose);
				break;
			case 4:

				select(nChoose);
				break;
			case 5:
				do {
					Sleep(1000);
					system("cls");
					cout << tab << "\t\t --Features other: " << endl;
					cout << tab << "1 - Check account information" << endl; // Kiem tra thong tin tai khoan khach hang
					cout << tab << "2 - Change PIN account" << endl; // Thay doi ma PIN tai khoan khach hang
					cout << tab << "0 - Press other key to exit !" << endl; // Nhan 0 de thoat chuc nang khac
					cout << tab << "--> Enter key: "; cin >> nChooseOther;
					switch (nChooseOther)
					{
					case 1:

						break;
					case 2:

						break;
					case 0:
						system("cls");
						cout << "- Exited other features !" << endl;
						Sleep(1000);
						break;
					default:
						cout << "- Enter fail features ! Please press choose again !!" << endl;
						Sleep(900);
					}
				} while (nChooseOther != 0);
				Sleep(700);
				select(nChoose);
			case 0:
				cout << "Exited !" << endl;
				Sleep(900);
				cout << "Thanks for using services and see you later !" << endl;
				break;
			default:
				cout << "- Not features need !" << endl;
				Sleep(800);
				cout << "- Please press choose agian !" << endl;
				Sleep(1000);
				break;
			}
		} while (nChoose != 0);
	}

	cout << endl;
	system("pause");
	return 0;
}

void select(int &nChon)
{
	char szSelect;
	cout << "- You continue features: " << endl;
	Sleep(700);
	cout << "\t-->Enter to continue or ESC to exit " << endl;
	szSelect = getch();

	if (szSelect == 27)
	{
		system("cls");
		cout << "Thanks for using service !!" << endl;
		nChon = 0;
	}
	else
		system("cls");
}

void reviewTransactions(linklist l, char *id) //Chua hoan thien
{
	ifstream read;
	char *file_id_url = new char[128]; // Bien tam luu tru duong dan file id
	// ATTENTION! CHANGE file_id_url TO YOUR CURRENT URL OF DIRECTORY
	strcpy(file_id_url, "D:\\BT CTDL&GT\\11\\11\\GiaoDich\\");
	strcat(file_id_url, id);
	strcat(file_id_url, ".dat");
	read.open(file_id_url);
	if (!read)
	{
		cout << "Opps! Not found the data file of \"" << id << ".dat\"" << endl;
		return;
	}
	node *p;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		if (strcmpi(p->data.id, id) == 0)
		{
			/*
			- Lam sao de tai du lieu lich su giao dich tu file len day
			- In ra mang hinh chinh cho khach hang xem !!!!
			*/
		}
	}
	read.close();
}

double convertCurrency(double Amount, char *from, char *to)
{
	double result = 0.0;
	/*
	* 1. VND
	* 2. USD
	* 3. EUR
	* 4. JPY
	*/
	int currency = 0;
	if (strcmpi(from, to) == 0)
		currency = 1;
	if (strcmpi(from, "VND") == 0 && strcmpi(to, "USD") == 0)
		currency = 12;
	if (strcmpi(from, "VND") == 0 && strcmpi(to, "EUR") == 0)
		currency = 13;
	if (strcmpi(from, "VND") == 0 && strcmpi(to, "JPY") == 0)
		currency = 14;
	if (strcmpi(from, "USD") == 0 && strcmpi(to, "VND") == 0)
		currency = 21;
	if (strcmpi(from, "USD") == 0 && strcmpi(to, "EUR") == 0)
		currency = 23;
	if (strcmpi(from, "USD") == 0 && strcmpi(to, "JPY") == 0)
		currency = 24;
	if (strcmpi(from, "EUR") == 0 && strcmpi(to, "VND") == 0)
		currency = 31;
	if (strcmpi(from, "EUR") == 0 && strcmpi(to, "USD") == 0)
		currency = 32;
	if (strcmpi(from, "EUR") == 0 && strcmpi(to, "JPY") == 0)
		currency = 34;
	if (strcmpi(from, "JPY") == 0 && strcmpi(to, "VND") == 0)
		currency = 41;
	if (strcmpi(from, "JPY") == 0 && strcmpi(to, "USD") == 0)
		currency = 42;
	if (strcmpi(from, "JPY") == 0 && strcmpi(to, "EUR") == 0)
		currency = 43;


	switch (currency)
	{
	case 1:	 result = Amount;	break;
	case 12:	result = Amount / 22700;	break;
	case 21:	result = Amount * 22700;	break;
	case 13:	result = Amount / 26700;	break;
	case 31:	result = Amount * 26700;	break;
	case 14:	result = Amount / 200;		break;
	case 41:	result = Amount * 200;		break;
	case 23:	result = Amount / 1.17;		break;
	case 32:	result = Amount * 1.17;		break;
	case 24:	result = Amount * 113;		break;
	case 42:	result = Amount / 113;		break;
	case 34:	result = Amount * 133;		break;
	case 43:	result = Amount / 133;		break;
	}

	return result;
}

bool transfer(linklist &l, char *idGui, char* idNhan)
{
	double cash;
	cout << "Enter id recieve: ";
	fflush(stdin);
	cin.getline(idNhan, 15);
	double cashVND; // Chuyen so tien gui sang VND
	double cashTemp; // So tien sau khi chuyen tu don vi cua idGui sang idNhan
	node *p;
	node *q;
	bool bCheckGD = false; // Kiem tra xem giao dich co thanh cong hay khong
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		if (strcmpi(p->data.id, idGui) == 0)
		{
			for (q = l.pHead; q != NULL; q = q->pNext)
			{
				if (strcmpi(q->data.id, idNhan) == 0)
				{
					do {
						cout << "Enter money transfer: ";
						cin >> cash;
						cashVND = convertCurrency(cash, p->data.currency, "VND");
						if (cashVND < 50000)
							cout << "So tien chuyen phai tu 50000 tro len !" << endl;
					} while (cashVND < 50000);			
					cashTemp = convertCurrency(cash, p->data.currency, q->data.currency);
					p->data.balance = p->data.balance - cash;
					q->data.balance = q->data.balance + cashTemp;
					writeDataID(l, p->data.id);
					writeDataID(l, q->data.id);
					writeHistory(l, p->data.id, cash, q->data.id);
					writeHistory(l, q->data.id, cashTemp, p->data.id);
					Sleep(1000);
					cout << "Transaction was made !" << endl;
					bCheckGD = true;
					break;
				}
			}
		}
	}
	if (bCheckGD == false)
		cout << "Fail transaction !" << endl;
	return bCheckGD;
}

void withDraw(linklist &l, char *id)
{
	double cash;
	
	bool checkCash = true; //Kiem tra so tien nhap vao co hop le hay khong (true = hop le)
	node *p;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		if (strcmpi(p->data.id, id) == 0)
		{
			break;
		}
	}
	do {
		
		cout << "Enter Amount: ";
		cin >> cash;
		if (cash == (cash = convertCurrency(cash, p->data.currency, "VND")))
		{
			do {
				 //Chuyen doi tien te theo Balance cua tai khoan khach hang
				if (cash < 50000)
					cout << "So tien rut chi dc rut tu 50000 tro len !" << endl;
			} while (cash < 50000);
		}
		else if (p->data.balance < 50000)
		{
			cout << "Not withdraw !" << endl;
			p->data.balance = p->data.balance + cash;
			checkCash = false;
		}
		cash = convertCurrency(cash, "VND", p->data.currency);
		
		if (p->data.balance < cash)
		{
			cout << "So tien vuot qua so du. Nhap lai " << endl;
			checkCash = false;
		}
		else
		{
			cout << "Withdrwals was made !" << endl;

			p->data.balance = p->data.balance - cash;
			checkCash = true;
			writeDataID(l, id);
			writeHistory(l, id, cash, "0");
		}
	} while (checkCash == false);
}

void checkBalance(linklist l, char *id)
{
	node *p;
	for (p = l.pHead; p != NULL; p = p->pNext)
		if (strcmpi(p->data.id, id) == 0)
			cout << "Your amount: " << p->data.balance << endl;
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

char *login(linklist l)
{
	char *id = new char[15];
	char *pin = new char[7];
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(out, 14);
	cout << tab << "*****************************************" << endl << tab << "*";
	SetConsoleTextAttribute(out, 10);
	cout << tab << tab << "SYSTEM LOGIN";
	SetConsoleTextAttribute(out, 14);
	cout << tab << tab << "*" << endl;
	cout << tab << "*****************************************" << endl;
	// Enter the ID
	SetConsoleTextAttribute(out, 10);
	do {
		cout << endl << tab << "Card ID\t: ";
		SetConsoleTextAttribute(out, 7);
		fflush(stdin); cin >> id;
	} while (strlen(id) > 14);
	// Enter the PIN
	SetConsoleTextAttribute(out, 10);
	do {
		string temp_pin;
		cout << tab << "PIN code: ";
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
					cout << "Incorrect PIN code!\n"
						<< "Press any key to try again. Press ESC to quit . . .\n";
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
	if (check == false)
	{
		id[0] = NULL;
	}
	return id;
}

void displayData(linklist l)
{
	node *p = new node;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		cout << p->data.id << tab << p->data.pin << endl;
	}
}

void writeHistory(linklist l, char *idTruoc, double soTienGD, char *idSau)
{
	ofstream gi;
	char *file_id_url = new char[128]; // Bien tam luu tru duong dan file id
	// ATTENTION! CHANGE file_id_url TO YOUR CURRENT URL OF DIRECTORY
	strcpy(file_id_url, "D:\\BT CTDL&GT\\11\\11\\GiaoDich\\");
	strcat(file_id_url, idTruoc);
	strcat(file_id_url, ".dat");
	gi.open(file_id_url, ios::app);
	if (!gi)
	{
		cout << "Opps! Not found the data file of \"" << idTruoc << ".dat\"" << endl;
		return;
	}
	node *p;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		if (strcmpi(p->data.id, idTruoc) == 0)
		{
			gi << soTienGD << tab << p->data.balance << tab << idSau << endl;
			break;
		}
	}

	gi.close();
}
// Ghi zu lieu vao file "[id].dat";
void writeDataID(linklist l, char *id)
{
	ofstream gi;
	char *file_id_url = new char[128]; // Bien tam luu tru duong dan file id
	// ATTENTION! CHANGE file_id_url TO YOUR CURRENT URL OF DIRECTORY
	strcpy(file_id_url, "D:\\BT CTDL&GT\\11\\11\\TaiKhoan\\");
	strcat(file_id_url, id);
	strcat(file_id_url, ".dat");
	gi.open(file_id_url);
	if (!gi)
	{
		cout << "Opps! Not found the data file of \"" << id << ".dat\"" << endl;
		return;
	}
	node *p;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		if (strcmpi(p->data.id, id) == 0)
		{
			gi << p->data.name << endl;
			gi << p->data.balance << endl;
			gi << p->data.currency << endl;
			break;
		}
	}

	gi.close();
}

// Tai du lieu trong file "[id].dat";
void loadDataID(linklist &l)
{
	ifstream fi;
	node *p;
	char *file_id_url = new char[128]; // Bien tam luu tru duong dan file id
	// ATTENTION! CHANGE file_id_url TO YOUR CURRENT URL OF DIRECTORY
	bool checkingError = false; // Kiem tra doc file co bi loi hay khong
	for (p = l.pHead; p != NULL; p = p->pNext)
	{	
		strcpy(file_id_url, "D:\\BT CTDL&GT\\11\\11\\TaiKhoan\\");
		strcat(file_id_url, p->data.id);
		strcat(file_id_url, ".dat");
		//cout << file_id_url << endl;
		//system("pause");
		fi.open(file_id_url); // Mo^ file moi'
		if (!fi && p != NULL)
		{
			checkingError = true;
			cout << "Opps! Not found the data file of \"" << file_id_url << "\"." << endl;
		}
		else
		{
			fflush(stdin); fi.getline(p->data.name, 32);
			fi >> p->data.balance;
			fi >> p->data.currency;
			fi.close(); // Dong' file
		}
	}
	if (checkingError == false)
		cout << "(+) Loading data file of \"[id].dat\" successfully.\n";
}

// Tai du lieu trong file "TheTu.dat";
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
		// Dua du lieu vao node
		p = getNode(Account);
		if (strlen(p->data.id) < 1 || strlen(p->data.pin) < 1)
		{
			cout << "Warning! Database element of \"TheTu.dat\" is not match. Please check the database file!\n";
			return;
		}
		// Dua node vao list
		addTail(l, p);
	}
	fi.close();
	cout << "(+) Loading data file of \"TheTu.dat\" successfully.\n";
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