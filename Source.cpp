/*
* PHAN MEM MO PHONG MAY ATM
* Do an mon: Cau truc du lieu va giai thuat
* Thuc hien: Nhom 4
*/

#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <stdexcept>
#include <conio.h>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#define tab '\t'
#define space ' '
#define slash '/'
#define dline "_________________________________________________________\n"
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

struct seasion
{
	string time;
	string date;
	string trans_type; // transaction_type
	double amount; // So tien jao zix
	double balance; // So zu hien tai
	string id_passive; // ID cung thuc hien giao jao zix
};

// Main functions
void initial(linklist &l);
node *getNode(BankAcc Account);
void addTail(linklist &l, node *pNew);
void loadData(linklist &l);
void loadDataID(linklist &l);
void writeDataID(linklist l, char *id); 
void writeHistory(linklist l, char *id, double amount, char *id_passive, string transaction_type);
void displayData(linklist l); // for deverloper
char *login(linklist l); // Dang nhap
void checkBalance(linklist l, char *id); // Kiem tra so zu
double withDraw(linklist &l, char *id, double amount, char *id_passive, string transaction_type); // Rut tien
bool isValid(double amount, double balance, char *currency);
bool transfer(linklist &l, char *id, double amount, char *id_passive, string transaction_type); // Chuyen tien va tra ve ketqua
void reviewTransactions(char *id); // Xem lai noi zung jao zich

// Other functions
string getTime();
char *getURL(char *file_name, char *folder); // DIEU CHINH DIA CHI FILES 
char *convertStoC(string s); // Convert string to char*
string convertI2S(int num); //Convert integer to string
double convertCurrency(double Amount, char *from, char *to); // Chuyen doi ty gia tien te
void coloralert(string string); // To mau thong bao

int main()
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	linklist l;
	initial(l);
	loadData(l);
	loadDataID(l);
	char *id = login(l);
	if (id[0] == NULL)
	{
		SetConsoleTextAttribute(out, 12);
		cout << "Login failed. Please take back the card!\n";
		SetConsoleTextAttribute(out, 7);
		system("pause");
	}
	else
	{
		int choice;
		do {
			system("cls");
			cout << tab << "********** USER MANAGEMENT ************" << endl
				<< tab << "1. Balance enquiry" << endl
				<< tab << "2. Withdraw money" << endl
				<< tab << "3. Transfer" << endl
				<< tab << "4. Review transactions" << endl
				<< tab << "0. Get back the card" << endl;
			cout << tab << "Choose the service: ";
			cin >> choice;

			// VARIABLE USING ON SWITCH
			// Case 2
			double amount; // So tien muon rut
			double cash; // So tien rut duoc
			// Case 3
			char *recipient = new char[15]; // ID nhan tien
			bool checkTransaction; // Kiem tra giao dich

			system("cls");
			switch (choice)
			{
			case 1:
				coloralert("BALANCE ENQUIRY");
				cout << tab;
				checkBalance(l, id);
				cout << dline;
				system("pause");
				break;
			case 2:
				coloralert("WITHDRAW MONEY");
				// Nhap
				cout << tab << "Enter Amount: ";
				cin >> amount;
				cout << dline;
				// Thuc thi
				cash = withDraw(l, id, amount, "Not_Applicable", "w");
				// Kiem tra
				if (cash != 0.0)
				{
					cout << "Transaction was successful!" << endl;
					cout << "Do you want to do other transtaction? (y/n)!";
					char choice = getch();
					if (choice == 'n')
						exit(1);
				}
				break;
			case 3:
				coloralert("TRANSFER");
				// Nhap
				cout << tab << "Enter receipt ID: ";
				cin >> recipient;
				cout << tab << "Enter Amount: ";
				cin >> amount;
				cout << dline;
				// Thuc thi
				checkTransaction = transfer(l, id, amount, recipient, "r");
				if (checkTransaction)
				{
					cout << "Transaction was successful!" << endl;
					Sleep(2000);
				}
				else
				{
					cout << "The transaction was failed!\n";
					Sleep(3000);
				}
				break;
			case 4:
				reviewTransactions(id);
				system("pause");
				break;
			case 0:
				exit(1);
			default:
				cout << dline;
				cout << "Please take your card back!";
				Sleep(2000);
				exit(1);
				break;
			}
		} while (choice != 0);
	}

	return 0;
}

void coloralert(string string)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(out, 14);
	cout << tab << "*****************************************" << endl << tab << "*";
	SetConsoleTextAttribute(out, 10);
	cout << tab << tab << string;
	SetConsoleTextAttribute(out, 14);
	cout << tab << tab << "*" << endl;
	cout << tab << "*****************************************" << endl << endl;
	SetConsoleTextAttribute(out, 7);
}

string getTime()
{
	string TimeDate;
	time_t theTime = time(NULL);
	struct tm *aTime = localtime(&theTime);

	int day = aTime->tm_mday;
	int month = aTime->tm_mon + 1; // Month is 0 – 11, add 1 to get a jan-dec 1-12 concept
	int year = aTime->tm_year + 1900; // Year is # years since 1900
	int hour = aTime->tm_hour;
	int min = aTime->tm_min;
	int sec = aTime->tm_sec;
	
	TimeDate = "";
	TimeDate += convertI2S(hour) + ":" + convertI2S(min) + ":" + convertI2S(sec);
	TimeDate += tab;
	TimeDate = TimeDate + convertI2S(day) + slash + convertI2S(month) + slash + convertI2S(year);

	return TimeDate;
}

string convertI2S(int num)
{
	string string;
	if (num < 10)
		string += "0" + to_string(num);
	else
		string += to_string(num);
	return string;
}

void reviewTransactions(char *id)
{
	ifstream doc;
	char *url = getURL(id, "GiaoDich");
	doc.open(url);
	if (!doc)
	{
		cout << "Opps! Not found the data file of \"" << url << "\"." << endl;
		return;
	}

	seasion s;
	vector <seasion> seasion_list;
	// Doc file
	int i;
	for (i = 0; !doc.eof(); i++)
	{
		doc >> s.time >> s.date >> s.trans_type;
		doc >> s.amount >> s.balance;
		doc >> s.id_passive;
		seasion_list.push_back(s);
	}
	
	// Xuat len man hinh
	cout << "****************************** TRANSACTION HISTORY ******************************" << endl;
	cout << setw(8) << "Time" << setw(15) << "Date" << setw(15) << "Trans" <<  setw(10) << "Amount" << setw(10) << "Balance" << setw(20) << "Transaction ID"<< endl;
	for (i = 0; i < seasion_list.size(); i++)
	{
		if (seasion_list[i].trans_type == "w")
			seasion_list[i].trans_type = "withdraw";
		if (seasion_list[i].trans_type == "s")
			seasion_list[i].trans_type = "sended";
		if (seasion_list[i].trans_type == "r")
			seasion_list[i].trans_type = "received";
		cout << setw(8) << seasion_list[i].time << setw(15) << seasion_list[i].date << setw(15) << seasion_list[i].trans_type << setw(10) << seasion_list[i].amount << setw(10) << seasion_list[i].balance << setw(20) << seasion_list[i].id_passive  << endl;
	}

}

bool transfer(linklist &l, char *id, double amount, char *id_passive, string transaction_type)
{
	bool checkTransition = false; // Kiem tra giao dich co thanh cong hay khong
	// Kiem tra trung id
	if (strcmpi(id, id_passive) == 0)
	{
		cout << "Sorry! Can not make this transaction.\n";
		return false;
	}

	// Dinh vi id (initiative) trong list
	node *pi;
	for (pi = l.pHead; pi != NULL; pi = pi->pNext)
		if (strcmpi(pi->data.id, id) == 0)	break;
	// Dinh vi id_passive trong list
	node *pj;
	for (pj = l.pHead; pj != NULL; pj = pj->pNext)
		if (strcmpi(pj->data.id, id_passive) == 0)	break;
	if (pj == NULL) // Khong tim thay id_passive trong list
		cout << "Error! Not found the id \"" << id_passive << "\" in database.\n";
	else // Thuc hien giao dich
	{
		system("cls");
		cout << "******* Confirm *******" << endl
			// Xac nhan ID nhan tien
			<< "* Recipient ID\t: " << id_passive << endl
			<< "* Amount\t: " << amount << space << pi->data.currency << endl
			<< "* Agree? (y/n)>";
		char choice = getch();
		cout << endl;
		if (choice != 'n' && choice != 'N')
		{
			// Xu li tai khoan gui tien
			double cash = withDraw(l, id, amount, id_passive, "s");
			// Xu li tai khoan nhan tien
			if (cash != 0.0) // Kiem tra neu tien da nhan duoc
			{
				// Chuyen doi so tien tuong ung theo don vi cua ID nhan tien
				cash = convertCurrency(cash, pi->data.currency, pj->data.currency); 
				// Xu li co so zu lieu trong list
				pj->data.balance = pj->data.balance + cash;
				// Ghi zu lieu vao file
				writeDataID(l, id_passive);
				writeHistory(l, id_passive, cash, id, transaction_type);
				checkTransition = true;
			}
		}
	}
	return checkTransition;
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
// Kiem tra so tien nhap co hop le hay khong
bool isValid(double amount, double balance, char *currency)
{
	bool result = true;
	double amount_VND = convertCurrency(amount, currency, "VND");
	if (amount_VND < 50000)
	{
		cout << "The amount is too low!" << endl;
		result = false;
	}
	else if (amount > balance)
	{
		cout << "The balance is not enough!" << endl;
		result = false;
	}
	else if (convertCurrency(balance - amount, currency, "VND") < 50000)
	{
		cout << "The balance after withdraw is not allow lower than 50000 VND." << endl;
		result = false;
	}
	return result;
}
// Tru tien 
double withDraw(linklist &l, char *id, double amount, char *id_passive, string transaction_type)
{
	double result = 0.0; // So tien rut duoc
	node *p;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		// Dinh vi id trong list
		if (strcmpi(p->data.id, id) == 0)
		{
			// Kiem tra nhap khoan tien muon giao dich
			bool checkTransaction = true; // Kiem tra giao dich. (true: dong y jao zix)
			bool checkInput = isValid(amount, p->data.balance, p->data.currency); // Kt nhap so tien
			int counter = 0; // Dem so lan nhap khogn hop le
			while (!checkInput)
			{
				cout << "Enter Amount: ";
				cin >> amount;
				checkInput = isValid(amount, p->data.balance, p->data.currency);
				counter++;
				if (counter >= 3 && !checkInput) // Too many time invalid inputed and the invalid input
				{
					cout << "Do you want to do the other transaction? (y/n)>";
					char choice = getch();
					cout << endl;
					if (choice != 'n' && choice != 'N')
					{
						checkTransaction = false;
						break;
					}
					counter = 0;
				}
			}
			// Kiem tra neu khoan tien hop le thi thuc hien jao zix
			if (checkTransaction)
			{
				if (transaction_type == "w")
				{
					// Xac nhan so tien rut
					cout << "******* Confirm *******" << endl
						<< "* Amount: " << amount << space << p->data.currency << endl
						<< "* Agree? (y/n)>";
					char choice = getch();
					cout << endl;
					if (choice == 'n' || choice == 'N')
					{
						cout << "The transaction is canceled!\n";
						checkTransaction = false;
						break;
					}
				}
				// Chap nhan so tien rut
				result = amount;
				// Xu li co so zu lieu trong list
				p->data.balance = p->data.balance - amount;
				// Ghi zu lieu vao file
				writeDataID(l, id);
				writeHistory(l, id, amount, id_passive, transaction_type);
				break;
			}
		}
	}
	return result;
}

void checkBalance(linklist l, char *id)
{
	node *p;
	for (p = l.pHead; p != NULL; p = p->pNext)
		if (strcmpi(p->data.id, id) == 0)
			cout << "Your current balance: " << p->data.balance << space << p->data.currency << endl;
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
	coloralert("SYSTEM LOGIN");
	// Enter the ID
	SetConsoleTextAttribute(out, 10);
	do {
		cout << tab << "Card ID\t: ";
		SetConsoleTextAttribute(out, 7);
		fflush(stdin); cin >> id;
	} while (strlen(id) > 14);
	// Enter the PIN
	SetConsoleTextAttribute(out, 10);
	do {
		cout << tab << "PIN code: ";
		SetConsoleTextAttribute(out, 7);
		string temp_pin = getpassword("");
		pin = convertStoC(temp_pin); cout << endl;
	} while (strlen(pin) > 6);

	bool checkID = false;
	bool checkPass = false;
	node *p = new node;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		if (strcmpi(id, p->data.id) == 0)
		{
			checkID = true;
			int e = 0; // (Error) wrong PIN code typed counter
			do {
				if (strcmpi(pin, p->data.pin) == 0)
					checkPass = true;
				else
				{
					SetConsoleTextAttribute(out, 12);
					cout << "Incorrect PIN code!\n";
					SetConsoleTextAttribute(out, 7);
					cout << "Press any key to try again. Press ESC to quit . . .\n";
					char c = getch();
					if (c != 27)
					{
						do {
							system("cls");
							SetConsoleTextAttribute(out, 10);
							cout << endl << tab << "Card ID\t: ";
							SetConsoleTextAttribute(out, 7);
							cout << id << endl;
							SetConsoleTextAttribute(out, 10);
							cout << tab << "PIN code: ";
							SetConsoleTextAttribute(out, 7);
							string temp_pin = getpassword(""); cout << endl;
							pin = convertStoC(temp_pin); 
						} while (strlen(pin) > 6);
						e++;
					}
					else
						exit(1);
				}
			} while (e < 3 && checkPass == false);
		}
		
	}
	if (checkID == false)
	{
		cout << "Invalid card. ";
	}
	if (checkPass == false)
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

char *getURL(char *id, char* folder)
{
	char *url = new char[128];
	// /* ATTENTION! CHANGE URL TO YOUR CURRENT URL OF DIRECTORY OF MACHINE
	// * (Trong truong hop may khong nhan dien duoc dia chi tuong doi):
	// * strcpy(url, "D:\\Source\\DSA\\Team-Project\\Team-Project\\"); */
	strcpy(url, folder);
	strcat(url, "\\");
	strcat(url, id);
	strcat(url, ".dat");
	return url;
}
// Ghi zu lieu vao file "\GiaoDich\[id].dat";
void writeHistory(linklist l, char *id, double amount, char *id_passive, string transaction_type)
{
	// Transaction type: w: withdraw, s: send, r: receive
	ofstream gi;
	char *url = getURL(id, "GiaoDich");
	gi.open(url, ios::app);
	if (!gi) // Kiem tra tac vu doc file id (initiative)
	{
		cout << "Opps! Not found the data file of \"GiaoDich\\\"" << id << ".dat\"" << endl;
	}
	else
	{
		// Zuyet list de lay data
		node *pi;
		for (pi = l.pHead; pi != NULL; pi = pi->pNext)
		{
			if (strcmpi(pi->data.id, id) == 0)
			{
				string time = getTime();
				gi << time << tab << transaction_type << tab << amount << tab << pi->data.balance << tab << id_passive << endl;
				break;
			}
		}
	}
	gi.close();
}
// Ghi zu lieu vao file "\TaiKhoan\[id].dat";
void writeDataID(linklist l, char *id)
{
	ofstream gi;
	char *url = getURL(id, "TaiKhoan");
	gi.open(url);
	if (!gi) // Kiem tra tac vu doc file
	{
		cout << "Opps! Not found the data file of \"TaiKhoan\\\"" << id << ".dat\"" << endl;
	}
	else
	{
		// Zuyet list de lay data
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
}
// Tai du lieu trong file "\TaiKhoan\[id].dat";
void loadDataID(linklist &l)
{
	ifstream fi;
	node *p;
	char *url; // Bien tam luu tru duong dan file id
	bool checkingError = false; // Kiem tra doc file co bi loi hay khong
	for (p = l.pHead; p != NULL; p = p->pNext)
	{	
		url = getURL(p->data.id, "TaiKhoan");
		fi.open(url); // Mo^ file moi'
		if (!fi && p != NULL)
		{
			checkingError = true;
			cout << "Opps! Not found the data file of \"" << url << "\"." << endl;
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