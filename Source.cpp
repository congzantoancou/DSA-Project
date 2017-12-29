
/*
* PHAN MEM MO PHONG MAY ATM
* Do an mon: Cau truc du lieu va giai thuat
* Thuc hien: Nhom 4
*/

#include <iostream>
#include <Windows.h>
#include <stdexcept>
#include <conio.h>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <ctime>
#include <vector>
#define atm_owner_fee 4/100
#define tab '\t'
#define dtab "\t\t"
#define ttab "\t\t\t"
#define colon ':'
#define space ' '
#define slash '/'
#define dendl "\n\n"
#define pause system("pause")
#define dline "_________________________________________________________\n"
using namespace std;
HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
time_t theTime = time(NULL);
struct tm *aTime = localtime(&theTime);

string getpassword(const string& prompt = "Enter password> ")
{
	string result;

	// Set the console mode to no-echo, not-line-confirmed input
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
	string id_passive; // ID doi tac
	string name_passive; // Ten tai khoan doi tac
};

// Main functions
void initial(linklist &l);
node *getNode(BankAcc Account);
void addTail(linklist &l, node *pNew);
void loadData(linklist &l);
void loadDataID(linklist &l);
void writeData(linklist l);
void writeDataID(linklist l, char *id); 
void writeHistory(linklist l, char *id, double amount, char *id_passive, char *name_passive, string transaction_type);
void displayData(linklist l); // for deverloper
char *login(linklist l); // Dang nhap
bool isValid(char *PIN); // Kiem tra pin hop le
void checkBalance(linklist l, char *id); // Kiem tra so zu
double withDraw(linklist &l, char *id, double amount, char *id_passive, char *name_passive, string transaction_type); // Rut tien
bool isValid(double amount, double balance, char *currency);
bool transfer(linklist &l, char *id, double amount, char *id_passive, string transaction_type); // Chuyen tien
void reviewTransactions(char *id); // Xem lai noi zung jao zich
bool changePIN(linklist &l, char *id, char *NewPIN);

// Other functions
string getTime();
string getDate();
char *getURL(char *file_name, char *folder); // DIEU CHINH DIA CHI FILES 
char *convertStoC(string s); // Convert string to char*
string convertI2S(int num); //Convert integer to string
double convertCurrency(double Amount, char *from, char *to); // Chuyen doi ty gia tien te
void alert(char *string); // To mau thong bao
void paintGreen(char *string);
void paintRed(char *string);
void paintYellow(char *string);
void paintWhite(char *string);
void paintWhite();
void eraseColor();
void inputNum(double &num);
void printBankLabel();
void printReceipt(linklist l, char *id, char type, double); // In bien lai
char randomChar();
string randomString(int);
void waiting(int);
void drawFunction(char *function_name, char site);

int main()
{
	linklist l;
	initial(l);
	loadData(l);
	loadDataID(l);
	char *id = login(l);
	if (id[0] == NULL)
	{
		cout << tab << dline << endl << dtab;
		cout << "Login failed. Please take back the card!\n";
		cout << dtab;
		pause;
	}
	else
	{
		int choice;
		do {
			system("cls");
			cout << ttab << "********** USER MANAGEMENT **********" << endl;
			drawFunction("1. BALANCE ENQUIRY", 'L');
			drawFunction("2. WITHDRAW MONEY", 'R');
			drawFunction("3. TRANSFER MONEY", 'L');
			drawFunction("4. VIEW TRANSACTIONS", 'R');
			drawFunction("5. CHANGE PIN CODE", 'L');
			drawFunction("0. GETBACK CARD", 'R');
			cout << tab << dline;
			cout << tab << "Choose the service: ";
			cin >> choice;
			if (choice < 0)
				choice = 0;

			// VARIABLE USING ON SWITCH
			// General
			char selection;
			// Case 2
			double amount; // So tien muon rut
			double cash; // So tien rut duoc
			// Case 3
			char *recipient = new char[15]; // ID nhan tien
			bool checkTransaction; // Kiem tra giao dich
			// Case 5
			char *NewPIN = new char[7]; // Ma PIN moi

			system("cls");
			switch (choice)
			{
			case 1:
				alert("BALANCE ENQUIRY");
				cout << tab;
				checkBalance(l, id);
				cout << endl << tab << dline << dtab;
				cout << "Do you want to print a receipt? (y/n)" << tab;
				selection = getch();
				cout << endl;
				if (selection == 'y' || selection == 'Y')
				{
					cout << dtab << "Printing ";
					printReceipt(l, id, NULL, 0);
					waiting(4);
					cout << endl;
					cout << dtab << "Done" << endl << tab;
					pause;
				}

				break;
			case 2:
				alert("WITHDRAW MONEY");
				// Nhap
				cout << tab << "Enter Amount: ";
				cin.ignore();  inputNum(amount);
				cout << tab << dline << endl << tab;
				// Thuc thi
				cash = withDraw(l, id, amount, "Not_Applicable", "Not_Applicable", "W");
				// Kiem tra
				if (cash != 0.0)
				{
					cout << dtab << "Transaction was successful!" << endl;
					cout << endl << tab << dline << dtab;
					cout << "Do you want to print a receipt? (y/n) >";
					selection = getch();
					cout << endl;
					if (selection == 'y' || selection == 'Y')
					{
						cout << dtab << "Printing ";
						printReceipt(l, id, 'W', cash);
							waiting(4);
						cout << endl;
						cout << dtab <<"Done" << endl << dtab;
						pause;
					}
				}
				break;
			case 3:
				alert("TRANSFER");
				// Nhap
				cout << "Enter receipt ID: ";
				cin >> recipient;
				cout << dtab << "Enter Amount: ";
				cin >> amount;
				cout << tab << dline;
				// Thuc thi
				checkTransaction = transfer(l, id, amount, recipient, "R");
				if (checkTransaction)
				{
					cout << dtab << "Transaction was successful!" << endl;
					cout << endl << tab << dline;
					cout << dtab << "Do you want to do other transtaction? (y/n)!";
					selection = getch();
					if (selection == 'n')
						exit(1);
				}
				else
				{
					cout << dtab << "The transaction was failed!. . .";
					Sleep(4500);
				}
				break;
			case 4:
				reviewTransactions(id);
				cout << tab << dline << tab;
				pause;
				break;
			case 5:
				do { // Kiem tra hop le
					cout << "New pin\t: ";
					cin >> NewPIN;
				} while (!isValid(NewPIN));
				if (changePIN(l, id, NewPIN))
					cout << "Change PIN succesfully!\n";
				Sleep(2000);
				break;
			case 0:
				exit(1);
			default:
				cout << tab << dline;
				cout << dtab << (char)3 << "Thank you for use our service!" << (char)3 << endl;
				paintWhite();
				cout << dtab << "Please take your card back!";
				eraseColor();
				Sleep(4000);
				exit(1);
				break;
			}
		} while (choice != 0);
	}

	return 0;
}

void eraseColor()
{
	SetConsoleTextAttribute(out, 7);
}

void drawFunction(char *function_name, char site)
{
	if (site == 'L')
	{
		cout << setw(3) << "   _______________________" << endl;
		cout << setw(3) << "|" << setw(24) << "\\" << endl;
		cout << setw(3) << "|" << setw(20) << function_name << setw(5) << ">" << endl;
		cout << setw(4) << "  |_______________________/" << endl;
	}
	if (site == 'R')
	{
		cout << setw(76) << "_______________________" << endl;
		cout << setw(53) << "/" << setw(24) << "|" << endl;
		cout << setw(52) << "<" << setw(20) << function_name << setw(5) << "|" << endl;
		cout << setw(77) << "\\_______________________|" << endl;
	}
}

void waiting(int second)
{
	int ms = 250;
	for (int i = 0; i < second; i++)
	{
		cout << '-';
		Sleep(ms);
		cout << '\b';
		cout << '\\';
		Sleep(ms);
		cout << '\b';
		cout << '|';
		Sleep(ms);
		cout << '\b';
		cout << '/';
		Sleep(ms);
		cout << '\b';
	}
}

string randomString(int num_of_char)
{
	string s;
	int i;
	for (i = 0; i < num_of_char/2; i++)
		s += randomChar();
	for (i = 0; i < num_of_char / 2; i++)
		s += 48 + rand() % 9;
	return s;
}

char randomChar()
{
	char c = 65 + rand() % 25;
	return c;
}

void printReceipt(linklist l, char *id, char type, double cash)
{
	ofstream gi;
	node *p;
	for (p = l.pHead; p != NULL; p = p->pNext)
		if (strcmp(p->data.id, id) == 0)
			break;

	if (p != NULL)
	{
		gi.open("BienLai.txt");
		gi << tab << dline << endl;
		gi << ttab << ttab << tab << "ANY BANK" << endl;
		gi << tab << dline << endl;
		gi << tab << "LOCATION: ";
		gi << setw(20) << "53 VO VAN NGAN" << endl;
		gi << setw(34) << "F. LINH CHIEU, THU DUC DIST." << setw(20) << "NY" << dendl;
		gi << tab << "CARD NO: " << p->data.id << dendl;
		gi << setw(20) << "Date" << setw(10) << "Time" << setw(14) << "Terminal" << endl;
		gi << setw(20) << getDate() << setw(10) << getTime() << setw(14) << randomString(6) << dendl;
		gi << tab << "SEQ NBR:" << setw(10) << 1000 + rand() % 9000;
		if (type == 'W')
		{
			gi << setw(18) << "AMT:" << setw(20) << cash << endl;
			gi << setw(40) << "ATM OWNER FEE:" << setw(20) << cash * atm_owner_fee << endl;
			gi << setw(40) << "TOTAL:" << setw(20) << cash + cash * atm_owner_fee << dendl;
			gi << tab << "SAVING WITHDRAWAL" << endl;
		}
		else
			gi << endl << endl << endl;
		gi << tab << "AVAILABLE BALANCE" << setw(30) << p->data.balance << endl;
		gi << tab << "BALANCE" << setw(40) << p->data.balance << dendl;
		gi << dtab << dtab << (char)3 << "Thank you for use our service!" << (char)3 << endl;
	}
}

void printBankLabel()
{
	cout << ttab << ttab << dtab;
	SetConsoleTextAttribute(out, 14);
	cout << (char)4 << space;
	SetConsoleTextAttribute(out, 15);
	cout << "WORLD BANK" << endl;
	eraseColor();
	int i;
	for (i = 0; i < 80; i++)
		cout << '_';
	cout << dendl << dendl;
}

void paintRed(char *string)
{
	SetConsoleTextAttribute(out, 12);
	cout << string;
	SetConsoleTextAttribute(out, 7);
}

void paintWhite(char *string)
{
	SetConsoleTextAttribute(out, 15);
	cout << string;
	SetConsoleTextAttribute(out, 7);
}

void paintWhite()
{
	SetConsoleTextAttribute(out, 15);
}

bool isValid(char *PIN)
{
	if (strlen(PIN) != 6)
		return false;
	int i;
	for (i = 0; i < strlen(PIN); i++)
	{
		if (PIN[i] < '0' || PIN[i] > '9')
			return false;
	}
	return true;
}
// Doi ma PIN
bool changePIN(linklist &l, char *id, char *NewPIN)
{
	// ************* KIÊM^ TRA DÔI´ CHIÊU´ ***************
	// Lay pin cu~
	char *CurrentPIN = new char[7];
	node *p;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		if (strcmp(p->data.id, id) == 0)
			break;
	}
	strcpy(CurrentPIN, p->data.pin);

	// So sánh pin cu~ và pin möí có trùng không
	int isDuplicate; // Kiem tra trung ma pin cu~
	isDuplicate = strcmp(NewPIN, CurrentPIN); // 0: trùng
	if (isDuplicate == 0)
		cout << "New PIN can not be the same with the current.\n";
	while (!isValid(NewPIN) || isDuplicate == 0) // Nêu´ trùng nhau
	{
		cout << "New PIN\t: ";
		cin >> NewPIN;
		isDuplicate = strcmp(NewPIN, CurrentPIN);
	}

	// Nhap pin lai lan 2 de xac nhan
	char *confirm = new char[7]; // Kiem tra pin nhap co khop khong;
	int isMatch; // Confirm PIN type
	do { // Kiem tra pin co khop khong

		do { // Kiem tra hop le
			cout << tab << "Confirm\t: ";
			cin >> confirm;
		} while (!isValid(confirm));

		isMatch = strcmp(NewPIN, confirm);
		if (isMatch != 0)
			cout << tab << "Not match!\n";
	} while (isMatch != 0);
	
	// **************** CÂP· NHÂT· DATABASE *****************
	// Cap nhat linklist
	strcpy(p->data.pin, NewPIN);
	// Cap nhat datafile
	writeData(l);
	delete p;
	return true;
}

void inputNum(double &num)
{
	string line;
	while (getline(cin, line))
	{
		stringstream ss(line);
		if (ss >> num)
		{
			if (ss.eof())
			{   // Success
				break;
			}
		}
		cout << "Must be number! Reinput: ";
	}
}

void paintGreen(char *string)
{
	SetConsoleTextAttribute(out, 10);
	cout << string;
	SetConsoleTextAttribute(out, 7);
}

void paintYellow(char *string)
{
	SetConsoleTextAttribute(out, 14);
	cout << string;
	SetConsoleTextAttribute(out, 7);
}

void alert(char *string)
{
	printBankLabel();
	paintYellow("\t\t*****************************************\n");
	paintYellow("\t\t*");
	cout << setw(41);
	paintYellow("*\n");
	paintYellow("\t\t*");
	cout << dtab;
	paintGreen(string);
	paintYellow("\t\t*\n");
	paintYellow("\t\t*");
	cout << setw(41);
	paintYellow("*\n");
	paintYellow("\t\t*****************************************");
	cout << endl << endl << dtab;
}

string getTime()
{
	string Time;

	int hour = aTime->tm_hour;
	int min = aTime->tm_min;
	int sec = aTime->tm_sec;
	
	Time = "";
	Time += convertI2S(hour) + colon + convertI2S(min) + colon + convertI2S(sec);
	return Time;
}

string getDate()
{
	string Date;

	int day = aTime->tm_mday;
	int month = aTime->tm_mon + 1; // Month is 0 – 11, add 1 to get a jan-dec 1-12 concept
	int year = aTime->tm_year + 1900; // Year is # years since 1900

	Date = "";
	Date = Date + convertI2S(day) + slash + convertI2S(month) + slash + convertI2S(year);
	return Date;
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
		cout << "Opps! Not found the data file of \"" << url << "\"." << endl << tab;
		return;
	}

	string confirm;
	seasion s;
	vector <seasion> seasion_list;
	// Doc file
	int i;
	for (i = 0; !doc.eof(); i++)
	{
		getline(doc, confirm);
		doc >> s.time >> s.date >> s.trans_type;
		doc >> s.amount >> s.balance;
		doc >> s.id_passive;
		doc >> s.name_passive;
		seasion_list.push_back(s);

		if (s.name_passive == "")
		{
			cout << "Opps! The database is not match. Please check the data file of \"" << url << "\"." << endl << tab;
			return;
		}
	}
	
	// Xuat len man hinh
	cout << "****************************** TRANSACTION HISTORY ******************************" << endl;
	cout << setw(8) << "Time" << setw(15) << "Date" << setw(15) << "Trans" <<  setw(10) 
		<< "Amount" << setw(10) << "Balance" << setw(20) << "Transaction ID"<< setw(20) << "Name" << endl;
	for (i = 0; i < seasion_list.size(); i++)
	{
		if (seasion_list[i].trans_type == "W")
			seasion_list[i].trans_type = "Withdraw";
		if (seasion_list[i].trans_type == "S")
			seasion_list[i].trans_type = "Sended";
		if (seasion_list[i].trans_type == "R")
			seasion_list[i].trans_type = "Received";
		cout << setw(8) << seasion_list[i].time 
			<< setw(15) << seasion_list[i].date 
			<< setw(15) << seasion_list[i].trans_type 
			<< setw(10) << seasion_list[i].amount 
			<< setw(10) << seasion_list[i].balance 
			<< setw(20) << seasion_list[i].id_passive 
			<< setw(20) << seasion_list[i].name_passive << endl;
	}

}

bool transfer(linklist &l, char *id, double amount, char *id_passive, string transaction_type)
{
	bool checkTransition = false; // Kiem tra giao dich co thanh cong hay khong
	// Kiem tra trung id
	if (strcmpi(id, id_passive) == 0)
	{
		cout << dtab << "Sorry! You can not make this transaction.\n";
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
		cout << dtab << "Error! Not found the id \"" << id_passive << "\" in database.\n";
	else // Thuc hien giao dich
	{
		cout << dtab << "******* Confirm *******" << endl << dtab
			// Xac nhan ID nhan tien
			<< "* Recipient ID\t: ";
		paintWhite(id_passive);
		cout << endl << dtab;

		cout << "* Recipient Name: ";
		paintWhite(pj->data.name);

		cout << endl << dtab;
		cout << "* Amount\t: ";

		paintWhite();
		cout << amount << space << pi->data.currency << endl << dtab;
		eraseColor();
		cout << endl;

		cout << dtab << "<> Submit? (y/n)>";
		char choice = getch();
		cout << endl << dtab;
		if (choice != 'n' && choice != 'N') // Dông` ý giao zich·
		{
			// Xu li tai khoan gui tien
			double cash = withDraw(l, id, amount, id_passive, pj->data.name, "S");
			// Xu li tai khoan nhan tien
			if (cash != 0.0) // Kiem tra neu tien da nhan duoc
			{
				// Chuyen doi so tien tuong ung theo don vi cua ID nhan tien
				cash = convertCurrency(cash, pi->data.currency, pj->data.currency); 
				// Xu li co so zu lieu trong list
				pj->data.balance = pj->data.balance + cash;
				// Ghi zu lieu vao file
				writeDataID(l, id_passive);
				writeHistory(l, id_passive, cash, id, pi->data.name, transaction_type);
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
		cout << "The amount is too low!" << endl << tab;
		result = false;
	}
	else if (amount > balance)
	{
		cout << "The balance is not enough!" << endl << tab;
		result = false;
	}
	else if (convertCurrency(balance - amount, currency, "VND") < 50000)
	{
		cout << "The balance after withdraw is not allow lower than 50000 VND." << endl << tab;
		result = false;
	}
	return result;
}
// Tru tien 
double withDraw(linklist &l, char *id, double amount, char *id_passive, char *name_passive, string transaction_type)
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
			while (!checkInput) // So tien khong hop le
			{
				counter++;
				if (counter >= 3 && !checkInput) // Too many time invalid inputed and the invalid input
				{
					cout << endl << tab << dline << dtab;
					cout << "Do you want to do the other transaction? (y/n)>";
					char choice = getch();
					cout << endl << tab;
					if (choice != 'n' && choice != 'N')
					{
						checkTransaction = false;
						break;
					}
					counter = 0;
				}
				// Nhap lai
				cout << dtab << "Enter Amount: ";
				inputNum(amount);
				checkInput = isValid(amount, p->data.balance, p->data.currency);
			}
			// Kiem tra neu khoan tien hop le thi thuc hien jao zix
			if (checkTransaction)
			{
				if (transaction_type == "W")
				{
					// Xac nhan so tien rut
					cout << dtab << "******* Confirm *******" << endl << ttab
						<< "* Amount: " << amount << space << p->data.currency << endl << ttab
						<< "* Agree? (y/n) >";
					char choice = getch();
					cout << endl << tab;
					if (choice == 'n' || choice == 'N')
					{
						cout << ttab << "The transaction is canceled!\n";
						checkTransaction = false;
						break;
					}
				}
				// Chap nhan so tien rut
				result = amount;
				// Xu li co so zu lieu trong list
				p->data.balance = p->data.balance - amount - amount*atm_owner_fee;
				// Ghi zu lieu vao file
				writeDataID(l, id);
				writeHistory(l, id, amount, id_passive, name_passive, transaction_type);
				break;
			}
		}
	}
	return result;
}

void checkBalance(linklist l, char *id)
{
	SetConsoleTextAttribute(out, 15);
	node *p;
	for (p = l.pHead; p != NULL; p = p->pNext)
		if (strcmpi(p->data.id, id) == 0)
			cout << "Your current balance: " << p->data.balance << space << p->data.currency << endl << tab;
	SetConsoleTextAttribute(out, 7);
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
	alert("SYSTEM LOGIN");
	cout << endl;
	// Enter the ID
	do {
		cout << dtab;
		paintGreen("Card ID");
		cout << tab << colon << space;
		fflush(stdin); cin >> id;
	} while (strlen(id) != 14);


	bool checkID = false;
	bool checkPIN = false;

	// Doi chieu id da nhap voi database trong "TheTu.dat"
	node *p = new node;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		if (strcmpi(id, p->data.id) == 0) // Neu id trung khop
		{
			checkID = true;
			break;
		}
	}

	if (checkID == true)
	{
		// NHAP MA PIN
		int e = 0; // (Error) wrong PIN code typed counter
		do { // Kiem tra va nhap lai ma PIN neu sai
			do { // Kiem tra PIN co hop le khong
				system("cls");
				alert("SYSTEM LOGIN");
				cout << endl << dtab;
				paintGreen("Card ID");
				cout << tab << colon << space << id << endl << tab;
				cout << tab;
				paintGreen("PIN code");
				cout << colon << space;
				string temp_pin = getpassword(""); cout << endl << tab;
				pin = convertStoC(temp_pin);
			} while (strlen(pin) != 6);
	
			if (strcmpi(pin, p->data.pin) == 0) // Neu pin trung khop
			{
				checkPIN = true;
				break;
			}
			else
			{
				e++; // Ghi lai so loi nhap pin
				if (e < 3)
				{
					cout << dline << endl << tab;
					paintRed("Incorrect PIN code!");
					cout << endl << tab;	
					// Hoi nguoi zung co muon thoat hay khong
					cout << "Press any key to try again. Press ESC to quit . . .";
					char c = getch();
					if (c == 27)
						exit(1);
				}
			}
		} while (e < 3 && checkPIN == false);

		if (checkPIN == false) // Neu sai ma PIN
		{
			id[0] = NULL;
		}

	}
	else
	{
		paintRed("\t\tInvalid card.\n");
		id[0] = NULL;
	}

	return id;
}

void displayData(linklist l)
{
	node *p = new node;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		cout << p->data.id << tab << p->data.pin << endl << tab;
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
// Ghi zu lieu vao file "TheTu.dat";
void writeData(linklist l)
{
	ofstream gi;
	gi.open("TheTu.dat");
	if (!gi) // Kiem tra tac vu doc file
	{
		cout << "Opps! Not found the data file of \"TheTu.dat\"" << endl << tab;
		return;
	}
	
	// Zuyêt· list lây´ zü~ liêu·
	node *p;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		gi << p->data.id << tab << p->data.pin;
		if (p->pNext != NULL)
			gi << endl;
	}
	gi.close();

}
// Ghi zu lieu vao file "\GiaoDich\[id].dat";
void writeHistory(linklist l, char *id, double amount, char *id_passive, char *name_passive, string transaction_type)
{
	ofstream gi;
	// Transaction type: w: withdraw, s: send, r: receive
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
				gi << getTime() << tab << getDate()
					<< tab << transaction_type 
					<< tab << amount 
					<< tab << pi->data.balance 
					<< tab << id_passive 
					<< tab << name_passive << endl;
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
		cout << "Opps! Not found the data file of \"TaiKhoan\\\"" << id << ".dat\"" << endl << tab;
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
	ifstream doc;
	node *p;
	char *url; // Bien tam luu tru duong dan file id
	bool checkingError = false; // Kiem tra doc file co bi loi hay khong
	for (p = l.pHead; p != NULL; p = p->pNext)
	{	
		url = getURL(p->data.id, "TaiKhoan");
		doc.open(url); // Mo^ file moi'
		if (!doc && p != NULL)
		{
			checkingError = true;
			cout << "Opps! Not found the data file of \"" << url << "\"." << endl << tab;
		}
		else
		{
			fflush(stdin); doc.getline(p->data.name, 32);
			doc >> p->data.balance;
			doc >> p->data.currency;
			doc.close(); // Dong' file
		}
	}
	//if (checkingError == false)
		//cout << "(+) Loading data file of \"[id].dat\" successfully.\n";
}
// Tai du lieu trong file "TheTu.dat";
void loadData(linklist &l)
{
	ifstream doc;
	doc.open("TheTu.dat");
	if (!doc)
	{
		cout << "Opps! Not found the data file \"TheTu.dat\"!" << endl << tab;
		return;
	}

	BankAcc Account;
	node *p;

	while (!doc.eof())
	{
		doc >> Account.id;
		doc >> Account.pin;
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
	doc.close();
	//cout << "(+) Loading data file of \"TheTu.dat\" successfully.\n";
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