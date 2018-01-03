
/*
* CHUONG TRINH MO PHONG MAY ATM
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
#include <locale>
#include <string>
#include <time.h>
#include <ctime>
#include <vector>

#define string_size 256
#define atm_owner_fee 4/10000 // 0.04%

#define dot '.'
#define tab '\t'
#define dtab "\t\t"
#define ttab "\t\t\t"
#define dash '_'
#define colon ':'
#define space ' '
#define asterisk '*'
#define slash '/'
#define dendl "\n\n"
#define br cout << endl
#define pause system("pause")
#define clear system("cls")
#define dline "_________________________________________________________\n"
// Color
#define red SetConsoleTextAttribute(out, 12)
#define royal SetConsoleTextAttribute(out, 11)
#define yellow SetConsoleTextAttribute(out, 14)
#define green SetConsoleTextAttribute(out, 10)
#define white SetConsoleTextAttribute(out, 15)
#define grey SetConsoleTextAttribute(out, 7)
using namespace std;


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

// STRUCTURES

struct BankAcc
{
	char ID[15];
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

struct session
{
	char time[10];
	char date[10];
	double amount; // So tien jao zix
	double balance; // So zu hien tai
	char trans_type[10]; // transaction_type
	char ID_passive[15]; // ID doi tac
	char name_passive[32]; // Ten tai khoan doi tac
};

struct command
{
	char *name;
	char *content;
};

struct columns
{
	int c1;
	int c2;
	int c3;
	int c4;
	int c5;
	int c6;
	int c7;
};

struct color
{
	int color1;
	int color2;
	string type;
};

// GLOBAL VARIABLE
HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
time_t theTime = time(NULL);
struct tm *aTime = localtime(&theTime);
int limit = 0;
vector <command> al;

// MAIN FUNCTIONS
void initial(linklist &l);
node *getNode(BankAcc Account);
void addTail(linklist &l, node *pNew);
void loadData(linklist &l);
void loadDataID(linklist &l);
void writeData(linklist l);
void writeDataID(linklist l, char *ID); 
void writeHistory(linklist l, char *ID, double amount, char *ID_passive, char *name_passive, string transaction_type);
void displayData(linklist l); // for deverloper
node *login(linklist &l); // Dang nhap
bool isvalid(char *PIN); // Kiem tra pin hop le
void checkBalance(linklist l, char *ID); // Kiem tra so zu
double withDraw(linklist &l, char *ID, double amount, char *ID_passive, char *name_passive, string transaction_type); // Rut tien
bool isvalid(double amount, double balance, char *currency);
bool transfer(linklist &l, char *ID, double amount, char *ID_passive, string transaction_type); // Chuyen tien
void reviewTransactions(char *ID); // Xem lai noi zung jao zich
bool changePIN(linklist &l, char *ID, char *NewPIN);
void deposite(linklist &l, char *ID, double cash); // Nap tien
void printReceipt(linklist l, char *ID, string type, double); // In bien lai

// Featured 
double inputNum(); // Nhâp· sô´, nêu´ nhâp· ki´ tu· se~ bi· vang ra ngoai`
void stopCondition(int &limit); // Gioi´ han· sô´ lân` nhâp· sai
bool isFileEmpty(char *url); // Kiêm^ tra data file có rông~ không
bool isLocked(linklist l, char *id); // Khóa tài khoan^ sau 3 lân` nhâp· sai ma~ PIN
double convertCurrency(double Amount, char *from, char *to); // Chuyen doi ty gia tien te
void chooseLanguage(vector <command> &al); // Chon ngon ngu
void loadLanguage(vector <command> &command_list, string lang); // Tai ngon ngu
string print(vector <command> al, string command_name); // In thong bao
void askPrintRec(linklist l, char *ID, string type, double cash); // Hoi nguoi dung in bien lai

// Other functions
void initial(command &cm); // Khoi tao command
string getTime();
string getDate();
char *getURL(char *file_name, char *folder); // DIEU CHINH DIA CHI FILES 
char *convertStoC(string s); // Convert string to char*
string convertI2S(long long num); //Convert integer to string
void alert(string string); // To mau thong bao
void paintGreen(string string);
void paintRed(string string);
void paintYellow(string string);
void paintWhite(string string);
void paintWhite();
void eraseColor();
void printBankLabel();
char randomChar();
string randomString(int);
void waiting(int);
void drawFunction(string, string, color code);
char *separateField(char *source, char *field, int &from, char separate_by);
void printIndex(char *url);
void align(columns &c);
void holding(int);
void welcome(int);
void goodbye();
string displayCurrency(double amount, char *currency);
string setPrecision(double amount, int streamsize);
void setColor(int colorcode);
color setColor(int color1, int color2, string type);
string printLine(int length, char c);

// ++++++++++++++++++ MAIN +++++++++++++++++++

int main()
{
	welcome(11);
	cout << dtab; pause; clear;
	chooseLanguage(al);
	cout << fixed << setprecision(2); // In 2 so thap phan
	linklist l;
	initial(l);
	loadData(l);
	loadDataID(l);
	node *p = login(l);
	char ID[15]; // Lay ID
	if (p == NULL)
	{
		cout << tab << dline << endl << dtab;
		cout << print(al, "LoginFail");
		cout << dtab;
		pause;
	}
	else
	{
		strcpy(ID, p->data.ID); // Copy ID
		char choice;
		do {
			system("cls");
			printBankLabel();
			color color;
			color = setColor(11, 7, "same");
			drawFunction("1. " + print(al, "BalanceEnquiry"), "2. " + print(al, "Withdraw"), color);
			drawFunction("3. " + print(al, "Transfer"), "4. " + print(al, "ViewTrans"), color);
			color = setColor(11, 12, "difer");
			drawFunction("5. " + print(al, "ChangePin"), "6. " + print(al, "Deposite"), color);
			color = setColor(11, 11, "difer");
			drawFunction("0. " + print(al, "Getback"), "right", color);
			cout << tab << dline << endl;
			cout << tab << print(al, "ChooseService") << colon << space;
			cin >> choice;


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
			case '1':
				alert(print(al, "BalanceEnquiry"));
				cout << tab;
				checkBalance(l, ID);
				cout << endl << tab << dline << endl;
				askPrintRec(l, ID, "", 0);
				break;
			case '2':
				alert(print(al, "Withdraw"));
				// Nhap
				cout << dtab << print(al, "EnterAmount") << tab << colon << space;
				amount = inputNum();
				cout << tab << dline << endl << tab;
				// Thuc thi
				cash = withDraw(l, ID, amount, "Not Applicable", "N/A", "with");
				// Kiem tra
				if (cash != 0.0)
				{
					cout << dtab << print(al, "TransSuccess") << endl;
					cout << endl << tab << dline << endl;
					askPrintRec(l, ID, "with", cash);
				}
				break;
			case '3':
				alert(print(al, "Transfer"));
				// Nhap
				cout << dtab << print(al, "EnterRecID") << tab << colon << space;
				cin >> recipient;
				cout << dtab << print(al, "EnterAmount") << tab << colon << space;
				amount = inputNum();
				cout << tab << dline << endl;
				// Thuc thi
				checkTransaction = transfer(l, ID, amount, recipient, "rece");
				if (checkTransaction)
				{
					cout << dtab << print(al, "TransSuccess") << endl;
					cout << endl << tab << dline << endl;
					askPrintRec(l, ID, "send", amount);
					cout << endl << tab << dline << endl;
					cout << dtab << print(al, "AskOtherTrans");
					selection = getch();
					if (selection == 'n')
						goto exit;
				}
				break;
			case '4':
				reviewTransactions(ID);
				cout << tab << dline << endl << tab;
				pause;
				break;
			case '5':
				limit = 0;
				do { // Kiem tra hop le
					stopCondition(limit);
					system("cls");
					alert(print(al, "ChangePin"));
					cout << dtab << print(al, "NewPin") << tab << colon << space;
					string temp_pin = getpassword(""); cout << endl;
					NewPIN = convertStoC(temp_pin);

					if (limit == 1 && !isvalid(NewPIN))
					{
						Beep(400, 400);
						cout << tab << dline << endl <<  dtab << print(al, "InvalidPin") << endl << dtab;
						pause;
					}
				} while (!isvalid(NewPIN));
				if (changePIN(l, ID, NewPIN))
					cout << tab << dline << endl << dtab << print(al, "ChangePinSuccess") << endl;
				Sleep(2000);
				break;
			case '6':
				Beep(400, 400);
				cout << dtab << print(al, "SorryBuilding") << endl << dtab;
				pause;
				break;
			case '0':
				exit(1);
			exit:default:
				cout << endl;
				cout << tab << dline << endl;
				cout << dtab << (char)3 << space << print(al, "ThankUsing") << space << (char)3 << space;
				holding(4);
				exit(1);
				break;
			}
		} while (choice != 0);
	}

	return 0;
}


//_______________________________________________________
//
// ****************** MAIN FUNCTIONS ********************
//_______________________________________________________


void askPrintRec(linklist l, char *ID, string type, double cash)
{
	cout << dtab << print(al, "AskPrintReceipt") << tab;
	char selection = getch();
	cout << endl;
	
	if (selection == 'y' || selection == 'Y'|| selection == 13)
	{
		cout << dtab << print(al, "Printing"); br;
		waiting(4);
		printReceipt(l, ID, type, cash);
		cout << endl;
		cout << dtab << print(al, "Done") << endl << dtab;
		pause;
	}
}

void printReceipt(linklist l, char *ID, string type, double cash)
{
	ofstream gi;
	node *p;
	for (p = l.pHead; p != NULL; p = p->pNext)
		if (strcmp(p->data.ID, ID) == 0)
			break;
	gi << fixed;
	gi << setprecision(2);
	if (p != NULL)
	{
		char url[string_size] = "BienLai.txt";
		gi.open(url);
		gi << setw(4) << space << dline << endl;
		gi << setw(50) << "ANY BANK" << endl;
		gi << setw(4) << space << dline << endl;
		gi << tab << "LOCATION: ";
		gi << setw(20) << "53 VO VAN NGAN" << endl;
		gi << setw(40) << "F. LINH CHIEU, THU DUC DIST." << setw(16) << "NY" << dendl;
		gi << tab << "CARD NO: " << p->data.ID << dendl;
		gi << setw(20) << "Date" << setw(10) << "Time" << setw(14) << "Terminal" << endl;
		gi << setw(20) << getDate() << setw(10) << getTime() << setw(14) << randomString(6) << dendl;
		gi << tab << "SEQ NBR:" << setw(10) << 1000 + rand() % 9000;
		if (type != "")
		{
			gi << setw(14) << "AMT:" << setw(20) << displayCurrency(cash, p->data.currency) << endl;
			double fee; // Phí chuyên^ tiên`
			if (type == "send")
				 fee = cash * atm_owner_fee;
			gi << setw(40) << "ATM OWNER FEE:" 
				<< setw(20) << displayCurrency(fee, p->data.currency) << endl;
			gi << setw(40) << "TOTAL:" << setw(20) << displayCurrency(cash + fee, p->data.currency) << dendl;
			gi << tab << "SAVING WITHDRAWAL" << endl;
		}
		else
			gi << dendl << endl;
		gi << tab << "AVAILABLE BALANCE" << setw(30) << displayCurrency(p->data.balance, p->data.currency) << endl;
		gi << tab << "BALANCE" << setw(40) << displayCurrency(p->data.balance, p->data.currency) << dendl;
		gi << dtab << tab << (char)3 << space << "Thank you for using our service!" << space << (char)3 << endl;
	}
}

bool changePIN(linklist &l, char *ID, char *NewPIN)
{
	// ************* KIÊM^ TRA DÔI´ CHIÊU´ ***************
	// Lay pin cu~
	char *CurrentPIN = new char[7];
	node *p;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		if (strcmp(p->data.ID, ID) == 0)
			break;
	}
	strcpy(CurrentPIN, p->data.pin);

	// So sánh pin cu~ và pin möí có trùng không
	int isDuplicate; // Kiem tra trung ma pin cu~
	isDuplicate = strcmp(NewPIN, CurrentPIN); // 0: trùng
	if (isDuplicate == 0)
		cout << dtab << print(al, "NewPinCannot") << endl;
	limit = 0;
	while (!isvalid(NewPIN) || isDuplicate == 0) // Nêu´ trùng nhau
	{
		stopCondition(limit);

		cout << dtab << print(al, "NewPin") << tab << colon << space;
		string temp_pin = getpassword(""); cout << endl;
		NewPIN = convertStoC(temp_pin);

		isDuplicate = strcmp(NewPIN, CurrentPIN);
	}

	// Nhap pin lai lan 2 de xac nhan
	char *confirm = new char[7]; // Kiem tra pin nhap co khop khong;
	int isMatch; // Confirm PIN type
	limit = 0;
	do { // Kiem tra pin co khop khong
		do { // Kiem tra hop le
			stopCondition(limit);
			cout << dtab << print(al, "Confirm") << tab << colon << space;
			string temp_pin = getpassword(""); cout << endl;
			confirm = convertStoC(temp_pin);

			if (limit == 1 && !isvalid(confirm))
			{
				cout << tab << dline << endl << dtab << print(al, "InvalidPin") << endl << dtab;
				pause;
			}
		} while (!isvalid(confirm));
		stopCondition(limit);
		isMatch = strcmp(NewPIN, confirm);
		if (isMatch != 0)
			cout << tab << dline << dtab << print(al, "NotMatch") << endl;
	} while (isMatch != 0);
	
	// **************** CÂP· NHÂT· DATABASE *****************
	// Cap nhat linklist
	strcpy(p->data.pin, NewPIN);
	// Cap nhat datafile
	writeData(l);
	delete p;
	return true;
}

void reviewTransactions(char *ID)
{
	ifstream doc;
	char *url = getURL(ID, "GiaoDich");
	doc.open(url);
	if (!doc)
	{
		Beep(400, 400);
		cout << "Opps! Not found the data file of \"" << url << "\"." << endl << tab;
		doc.close();
		return;
	}
	
	char *line = new char[string_size];
	strcpy(line, "");
	session s;
	char amount[14];
	char balance[14];
	vector <session> session_list;

	// DOC FILE
	// Bo qua 2 zòng chi^ muc·
	string temp;
	getline(doc, temp);
	getline(doc, temp);
	
	bool isFileEmpty = true;
	int i, j;
	while (!doc.eof())
	{
		doc.getline(line, string_size);
		i = 0; j = 0;
		while (i < strlen(line))
		{
			isFileEmpty = false;
			separateField(line, s.time, i, tab);
			i++;
			separateField(line, s.date, i, tab);
			i++;
			separateField(line, amount, i, tab);
			i++;
			separateField(line, balance, i, tab);
			i++;
			separateField(line, s.trans_type, i, tab);
			i++;
			separateField(line, s.ID_passive, i, tab);
			i++;
			separateField(line, s.name_passive, i, NULL);

			s.amount = atoi(amount);
			s.balance = atoi(balance);
		}
		
		if (strlen(line) > 0) // Nêu´ file line doc· duoc· không fai^ la` line rông~
			session_list.push_back(s);

		if (strlen(s.name_passive) < 1)
		{
			Beep(400, 400);
			cout << "Warning! The database is not match. Please check the data file of \"" << url << "\"." << endl << tab;
			return;
		}
	}
	doc.close();


	// Xuat len man hinh
	cout << "****************************** TRANSACTION HISTORY *****************************" << endl;
	printIndex("");

	if (isFileEmpty)
		cout << tab << dline << endl << dtab << print(al, "SorryNoData") << endl;

	columns c;
	align(c);
	for (i = 0; i < session_list.size(); i++)
	{
		cout << setw(c.c1) << session_list[i].time
			<< setw(c.c2) << session_list[i].date
			<< setw(c.c3) << session_list[i].amount
			<< setw(c.c4) << session_list[i].balance
			<< setw(c.c5) << session_list[i].trans_type
			<< setw(c.c6) << session_list[i].ID_passive
			<< setw(c.c7) << session_list[i].name_passive << endl;
	}

}

bool transfer(linklist &l, char *ID, double amount, char *ID_passive, string transaction_type)
{
	bool checkTransition = false; // Kiem tra giao dich co thanh cong hay khong
	// Kiem tra trung ID
	if (strcmpi(ID, ID_passive) == 0)
	{
		Beep(400, 400);
		cout << dtab << print(al, "SorryCanMakeThis") << endl << dtab;
		pause;
		return false;
	}

	// Dinh vi ID (initiative) trong list
	node *pi;
	for (pi = l.pHead; pi != NULL; pi = pi->pNext)
		if (strcmpi(pi->data.ID, ID) == 0)	break;
	// Dinh vi ID_passive trong list
	node *pj;
	for (pj = l.pHead; pj != NULL; pj = pj->pNext)
		if (strcmpi(pj->data.ID, ID_passive) == 0)	break;

	if (pj == NULL) // Khong tim thay ID_passive trong list
	{
		cout << dtab << "Error! Not found the ID \"" << ID_passive << "\" in database.\n" << dtab;
		pause;
	}
	else // Thuc hien giao dich
	{
		cout << dtab << printLine(14, asterisk) << space << print(al, "Confirm") << printLine(14, asterisk) << endl << dtab
			// Xac nhan ID nhan tien
			<< asterisk << space << print(al, "RecipID") << colon << tab; paintWhite(ID_passive);
		cout << endl << dtab;

		cout << asterisk << space << print(al, "RecipName") << colon << space;
		paintWhite(pj->data.name);

		cout << endl << dtab;
		cout << asterisk << space << print(al, "Amount") << tab << colon << space;

		paintWhite();
		cout << amount << space << pi->data.currency << endl;
		eraseColor();
		cout << endl;

		cout << dtab << "<>" << print(al, "Submit");
		char choice = getch();
		cout << endl;
		if (choice == 'y' || choice == 'Y' || choice == 13) // Dông` ý giao zich·
		{
			// Xu li tai khoan gui tien
			double cash = withDraw(l, ID, amount, ID_passive, pj->data.name, "send");
			// Xu li tai khoan nhan tien
			if (cash != 0.0) // Kiem tra neu tien da nhan duoc
			{
				cout << dtab << print(al, "TransDoing") << space; holding(3);
				cout << endl;
				// Chuyen doi so tien tuong ung theo don vi cua ID nhan tien
				cash = convertCurrency(cash, pi->data.currency, pj->data.currency); 
				// Xu li co so zu lieu trong list
				pj->data.balance = pj->data.balance + cash;
				// Ghi zu lieu vao file
				writeDataID(l, ID_passive);
				writeHistory(l, ID_passive, cash, ID, pi->data.name, transaction_type);
				checkTransition = true;
			}
		}
	}
	return checkTransition;
}
// Tru tien 
double withDraw(linklist &l, char *ID, double amount, char *ID_passive, char *name_passive, string transaction_type)
{
	double result = 0.0; // So tien rut duoc
	node *p;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		// Dinh vi ID trong list
		if (strcmpi(p->data.ID, ID) == 0)
		{
			// Kiem tra nhap khoan tien muon giao dich
			bool checkTransaction = true; // Kiem tra giao dich. (true: dong y jao zix)
			bool checkInput = isvalid(amount, p->data.balance, p->data.currency); // Kt nhap so tien
			int counter = 0; // Dem so lan nhap khogn hop le
			while (!checkInput) // So tien khong hop le
			{
				counter++;
				if (counter >= 3) // Too many time invalid inputed
				{
					Beep(400, 400);
					cout << endl << tab << dline << endl << dtab;
					cout << print(al, "AskOtherTrans");
					char choice = getch();
					cout << endl;
					if (choice != 'n' && choice != 'N')
					{
						checkTransaction = false;
						break;
					}
					if (choice == 27)	exit(1);
					counter = 0;
				}
				// Nhap lai
				cout << dtab << print(al, "EnterAmount") << tab << colon << space;
				amount = inputNum();
				checkInput = isvalid(amount, p->data.balance, p->data.currency);
			}
			// Kiem tra neu khoan tien hop le thi thuc hien jao zix
			if (checkTransaction)
			{
				if (transaction_type == "with")
				{
					// Xac nhan so tien rut
					cout << endl << dtab << printLine(14, asterisk) << print(al, "Confirm") << printLine(14, asterisk) << endl
						<< dtab	<< asterisk << space << print(al, "Amount") << colon << space << amount << space << p->data.currency << endl 
						<< dtab << asterisk << space << print(al, "Submit");
					char choice = getch();
					cout << endl << tab;
					if (choice == 'n' || choice == 'N')
					{
						cout << ttab << print(al, "TransCancel") << endl;
						checkTransaction = false;
						break;
					}
					if (choice == 27)	exit(1);
				}
				// Chap nhan so tien rut
				result = amount;
				// Xu li co so zu lieu trong list
				p->data.balance = p->data.balance - amount;
				if (transaction_type == "send") 
					p->data.balance = p->data.balance - amount*atm_owner_fee; // Phí chuyên^ tiên`
					
				// Ghi zu lieu vao file
				writeDataID(l, ID);
				writeHistory(l, ID, amount, ID_passive, name_passive, transaction_type);
				break;
			}
		}
	}
	return result;
}

void checkBalance(linklist l, char *ID)
{
	SetConsoleTextAttribute(out, 15);
	node *p;
	for (p = l.pHead; p != NULL; p = p->pNext)
		if (strcmpi(p->data.ID, ID) == 0)
			cout << dtab << print(al, "YourBalance") << colon << space << p->data.balance << space << p->data.currency << endl;
	SetConsoleTextAttribute(out, 7);
}

node *login(linklist &l)
{
	char *ID = new char[15];
	char *pin = new char[7];
	alert(print(al, "SystemLogin"));
	cout << endl;

	// Enter the ID
	limit = 0;
	do {
		stopCondition(limit);
		cout << dtab;
		paintGreen(print(al, "CardId"));
		cout << tab << colon << space;
		fflush(stdin); cin >> ID;
	} while (strlen(ID) != 14);


	bool checkID = false;
	bool checkPIN = false;

	// Doi chieu ID da nhap voi database trong "TheTu.dat"
	node *p = new node;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		if (strcmpi(ID, p->data.ID) == 0) // Neu ID trung khop
		{
			checkID = true;
			break;
		}
	}

	if (checkID == true)
	{
		// Kiem tra the co bi khoa hay khong
		if (isLocked(l, p->data.ID))
		{
			Beep(400, 400);
			cout << tab << dline << endl << print(al, "SorryLocked") << endl;
			Sleep(3);
			goodbye();
		}

		// NHAP MA PIN
		int e = 0; // (Error) wrong PIN code typed counter
		do { // Kiem tra va nhap lai ma PIN neu sai
			limit = 0;
			do { // Kiem tra PIN co hop le khong
				stopCondition(limit);
				system("cls");
				alert(print(al, "SystemLogin"));
				cout << endl << dtab;
				paintGreen(print(al, "CardId"));
				cout << tab << colon << space << ID << endl << tab;
				cout << tab;
				paintGreen(print(al, "PinCode"));
				cout << tab << colon << space;
				string temp_pin = getpassword(""); cout << endl;
				pin = convertStoC(temp_pin);
				if (strlen(pin) != 6 && limit < 3)
				{
					Beep(400, 400);
					cout << dtab << print(al, "InvalidPin") << endl << dtab;
					pause;
				}

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
					cout << tab << dline << endl << dtab;
					paintRed(print(al, "IncorrectPin"));
					cout << endl;	
					// Hoi nguoi zung co muon thoat hay khong
					cout << dtab << print(al, "PressAny");
					char c = getch();
					if (c == 27)
						exit(1);
				}
				else // SAI MA PIN. TAM KHOA THE
				{
					Beep(400, 400);
					strcpy(p->data.pin, "locked");
					writeData(l);
					writeHistory(l, p->data.ID, 0, "Not Applicable", "N/A", "lock");
					red; cout << tab << dline << endl << print(al, "SorryTemp") << endl; grey;
					cout << dtab; pause;
					exit(1);
				}
			}
		} while (e < 3 && checkPIN == false);
	}
	else
	{
		cout << dtab;
		paintRed(print(al, "InvalidCard")); br;
	}

	return p;
}

void chooseLanguage(vector <command> &al)
{
	printBankLabel();
	cout << dendl << dendl;
	cout << dtab << "Choose language" << endl;
	royal; cout << tab << dline << endl; grey;
	cout << dtab << "1. English\n" 
		<< dtab << "2. Vietnamese\n";
	royal; cout << tab << dline << endl; grey;
	cout << dtab << "Choose the service: ";
	char c = getch();
	switch(c)
	{
	case '1': loadLanguage(al, "en"); break;
	case '2': loadLanguage(al, "vi"); break;
	default: loadLanguage(al, "en"); break;
	}
	clear;
}

//__________________________________________________________
//
// ****************** RELATED FUNCTIONS ********************
//__________________________________________________________

// Kiem tra tinh trang ID có bi· khóa hay không
bool isLocked(linklist l, char *id)
{
	node *p;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		if (strcmpi(p->data.ID, id) == 0)
			break;
	}
	if (strcmp(p->data.pin, "locked") == 0)
		return true;
	else
		return false;
}
// Kiem tra so tien nhap co hop le hay khong
bool isvalid(double amount, double balance, char *currency)
{
	bool result = true;
	double amount_VND = convertCurrency(amount, currency, "VND");
	if (amount_VND < 50000)
	{
		Beep(400, 400);
		cout << dtab << print(al, "AmountLow") << endl;
		result = false;
	}
	else if (amount > balance)
	{
		Beep(400, 400);
		cout << dtab << print(al, "BalanceNotEnough") << endl;
		result = false;
	}
	else if (convertCurrency(balance - amount, currency, "VND") < 50000)
	{
		Beep(400, 400);
		cout << dtab << print(al, "BalanceAfter") << endl;
		result = false;
	}
	cout << endl;
	return result;
}
// Kiem tra PIN
bool isvalid(char *PIN)
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
// Hien thi database
void displayData(linklist l)
{
	node *p = new node;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		cout << p->data.ID << tab << p->data.pin << endl << tab;
	}
}
// In chi muc
void printIndex(char *url)
{
	ofstream gi;
	int URLState = strcmp(url, "");
	ostream &out = (URLState != 0? gi : cout);
	if (URLState != 0)
	{
		gi.open(url, std::ios::out);
		out << tab << print(al, "Time")
			<< tab << print(al, "Date")
			<< dtab << print(al, "Amount")
			<< tab << print(al, "Balance")
			<< dtab << print(al, "Type")
			<< tab << print(al, "TransID")
			<< dtab << print(al, "Name") << endl;
		out << tab << dline;
		gi.close();
	}
	else
	{
		columns c;
		align(c);
		out << print(al, "Time")
			<< setw(c.c1) << print(al, "Date")
			<< setw(c.c2) << print(al, "Amount")
			<< setw(c.c3) << print(al, "Balance")
			<< setw(c.c4) << print(al, "Type")
			<< setw(c.c5) << print(al, "TransID")
			<< setw(c.c6) << print(al, "Name") << endl;
		out << tab << dline;
	}
}
// Ghi zu lieu vao file "TheTu.dat";
void writeData(linklist l)
{
	ofstream gi;
	gi.open("TheTu.dat");
	if (!gi) // Kiem tra tac vu doc file
	{
		Beep(400, 400);
		cout << "Opps! Not found the data file of \"TheTu.dat\"" << endl << tab;
		return;
	}
	
	// Zuyêt· list lây´ zü~ liêu·
	node *p;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		gi << p->data.ID << tab << p->data.pin;
		if (p->pNext != NULL)
			gi << endl;
	}
	gi.close();

}
// Ghi zu lieu vao file "\GiaoDich\[ID].dat";
void writeHistory(linklist l, char *ID, double amount, char *ID_passive, char *name_passive, string transaction_type)
{
	ofstream gi;
	// Transaction type: w: withdraw, s: send, r: receive
	char *url = getURL(ID, "GiaoDich");
	gi.open(url, ios::app);
	if (!gi) // Kiem tra tac vu doc file ID (initiative)
	{
		Beep(400, 400);
		cout << "Opps! Not found the data file of \"GiaoDich\\\"" << ID << ".dat\"" << endl;
	}
	else
	{
		// Zuyet list de lay data
		bool found = false;
		node *pi;
		for (pi = l.pHead; pi != NULL; pi = pi->pNext)
		{
			if (strcmpi(pi->data.ID, ID) == 0)
			{
				found = true;
				break;
			}
		}
		// Ghi file neu tim thay
		if (found)
		{
			if (isFileEmpty(url))
			{
				printIndex(url);
			}
			gi << fixed;
			gi << setprecision(2);
			gi << getTime() << tab << getDate()
				<< tab << amount
				<< tab << pi->data.balance
				<< tab << transaction_type
				<< tab << ID_passive
				<< tab << name_passive << endl;
		}
	}
	gi.close();
}
// Ghi zu lieu vao file "\TaiKhoan\[ID].dat";
void writeDataID(linklist l, char *ID)
{
	ofstream gi;
	char *url = getURL(ID, "TaiKhoan");
	gi.open(url);
	if (!gi) // Kiem tra tac vu doc file
	{
		Beep(400, 400);
		cout << "Opps! Not found the data file of \"TaiKhoan\\\"" << ID << ".dat\"" << endl << tab;
	}
	else
	{
		// Zuyet list de lay data
		node *p;
		for (p = l.pHead; p != NULL; p = p->pNext)
		{
			if (strcmpi(p->data.ID, ID) == 0)
			{
				gi << fixed;
				gi << setprecision(2);
				gi << p->data.name << endl;
				gi << p->data.balance << endl;
				gi << p->data.currency << endl;
				break;
			}
		}
		gi.close();
	}
}
// Tai du lieu trong file "\TaiKhoan\[ID].dat";
void loadDataID(linklist &l)
{
	ifstream doc;
	node *p;
	char *url; // Bien tam luu tru duong dan file ID
	bool checkingError = false; // Kiem tra doc file co bi loi hay khong
	for (p = l.pHead; p != NULL; p = p->pNext)
	{	
		url = getURL(p->data.ID, "TaiKhoan");
		doc.open(url); // Mo^ file moi'
		if (!doc && p != NULL)
		{
			checkingError = true;
			Beep(400, 400);
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
		//cout << "(+) Loading data file of \"[ID].dat\" successfully.\n";
}
// Tai du lieu trong file "TheTu.dat";
void loadData(linklist &l)
{
	ifstream doc;
	doc.open("TheTu.dat");
	if (!doc)
	{
		Beep(400, 400);
		cout << "Opps! Not found the data file \"TheTu.dat\"!" << endl << tab;
		return;
	}

	BankAcc Account;
	node *p;

	while (!doc.eof())
	{
		doc >> Account.ID;
		doc >> Account.pin;
		// Dua du lieu vao node
		p = getNode(Account);
		if (strlen(p->data.ID) < 1 || strlen(p->data.pin) < 1)
		{
			Beep(400, 400);
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

void align(columns &c)
{
	c.c1 = 8;
	c.c2 = 11;
	c.c3 = 11;
	c.c4 = 12;
	c.c5 = 6;
	c.c6 = 15;
	c.c7 = 15;
}

//_________________________________________
//
// ********** OTHER FUNCTIONS *************
//_________________________________________



// Tách riêng môi~ thuôc· tính 
char *separateField(char *source, char *field, int &from, char separate_by)
{
	int j = 0;
	while (source[from] != separate_by)
	{
		field[j++] = source[from++];
	}
	field[j] = NULL;
	return field;
}

string print(vector <command> al, string command_name)
{
	int i;
	for (i = 0; i < al.size(); i++)
	{
		if (al[i].name == command_name)
		{
			return al[i].content;
		}
	}
	return "Language data file lost!\n";
}

void initial(command &cm)
{
	cm.name = new char[string_size];
	cm.content = new char[string_size];
}

void loadLanguage(vector <command> &command_list, string lang)
{

	ifstream doc;
	if (lang == "en")
		doc.open("Language/en.txt");
	if (lang == "vi")
		doc.open("Language/vi.txt");
	if (!doc)
	{
		cout << "Oop! Not found the language file of " << lang << ".txt" << endl;
		pause;
		return;
	}

	command cm;
	
	char *line = new char[string_size];
	int i, j;
	while(!doc.eof())
	{
		doc.getline(line, string_size);
		
		if (line[0] == slash && line[1] == slash) // Nêu´ line vua` doc· duoc. la comments
			doc.getline(line, string_size); // Doc· line moi´

		initial(cm); // Khoi^ tao· cho dia· chi^ con tro^ moi´
		i = 0; j = 0;
		while (i < strlen(line))
		{
			separateField(line, cm.name, i, '=');
			i+=2;
			separateField(line, cm.content, i, NULL);
		}

		if (strlen(line) > 0)
			command_list.push_back(cm);
	}
	
}
// Zâu´ thâp· fân
string setPrecision(double amount, int streamsize)
{
	stringstream stream;
	stream << fixed << setprecision(streamsize) << amount;
	string sAmount = stream.str();
	return sAmount;
}
// Hiên^ thi· don vi· tiên` tê·
string displayCurrency(double amount, char *currency)
{
	string s = "";
	if (strcmpi(currency, "VND") == 0)
		s = s + setPrecision(amount, 2) + space + 'd';
	else if (strcmpi(currency, "USD") == 0)
		s = s + "$" + setPrecision(amount, 2);
	else if (strcmpi(currency, "EUR") == 0)
		s = s + "€" + setPrecision(amount, 2);
	else if (strcmpi(currency, "JPY") == 0)
		s = s + "¥" + setPrecision(amount, 2);
	else
		s = s + setPrecision(amount, 2) + space + currency;
	return s;
}

void welcome(int color)
{
	printBankLabel();
	SetConsoleTextAttribute(out, color);
	cout << endl << endl << endl
		<< dtab << " _          ________ _      _____  ____  __  __ ______" << endl
		<< dtab << "\\ \\        / /  ____| |    / ____ / __ \\|  \\/  |  ____|" << endl
		<< dtab << " \\ \\   \\  / /| |__  | |   | |    | |  | | \\  / | |__" << endl
		<< dtab << "  \\ \\ / \\/ / |  __| | |   | |    | |  | | |\\/| |  __|" << endl
		<< dtab << "   \\ / \\  /  | |____| |___| |___ | |__| | |  | | |____" << endl
		<< dtab << "    \\ / \\/   |______|______\\_____ \\____/|_|  |_|______|" << endl << endl << endl;
	cout << dtab << dline << endl << endl << endl;
	SetConsoleTextAttribute(out, 7);
}

void goodbye()
{
	cout << tab << dline << endl;
	cout << dtab << (char)3 << space << print(al, "ThankUsing") << space << (char)3 << space;
	holding(4);
	exit(1);
}

void holding(int seconds)
{
	int n = 0;
	for (int i = 0; i < seconds*2; i++)
	{
		n++;
		cout << dot;
		Sleep(500);
		if (n == 3)
		{
			n = 0;
			cout << "\b\b\b";
			cout << "   ";
			cout << "\b\b\b";
			Sleep(500);
		}
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

void stopCondition(int &limit)
{
	limit++;
	if (limit > 10)
	{
		cout << tab << dline << endl;
		cout << dtab << (char)3 << print(al, "ThankUsing") << (char)3 << tab;
		Sleep(4000);
		exit(1);
	}
}

void setColor(int colorcode)
{
	SetConsoleTextAttribute(out, colorcode);
}
// Cài dat· màu sac´ cho menu chuc´ nang (draw function)
color setColor(int color1, int color2, string type)
{
	color c;
	c.color1 = color1;
	c.color2 = color2;
	c.type = type;
	return c;
}
// draw function color_code (function_name, site_or_function_name)
void drawFunction(string function_name, string site, color code)
{
	int text_color;
	int border_left;
	int border_right;

	if (code.type == "same")
	{
		border_left = border_right = code.color1;
		text_color = code.color2;
	}
	else
	{
		border_left = code.color1;
		border_right = code.color2;
		text_color = 7;
	}

	if (site == "left")
	{
		// Line 1
		setColor(border_left);  cout << setw(3) << "   _______________________" << endl;
		// Line 2
		cout << setw(3) << "|" << setw(24) << "\\" << endl;
		// Line 3
		cout << setw(3) << "|" << setw(20);
		setColor(text_color); cout << function_name;
		setColor(border_left); cout << setw(5) << ">" << endl;
		// Line 4
		cout << setw(4) << "  |_______________________/" << endl; grey;
	}
	else if (site == "right")
	{
		// Line 1
		setColor(border_right);  cout << setw(76) << "_______________________" << endl;
		// Line 2
		cout << setw(53) << "/" << setw(24) << "|" << endl;
		// Line 3
		cout << setw(52) << "<" << setw(20);
		setColor(text_color); cout << function_name;
		setColor(border_right); cout << setw(5) << "|" << endl;
		// Line 4
		cout << setw(77) << "\\_______________________|" << endl; grey;
	}
	else
	{
		/// LINE 1
		// Left																	
		setColor(border_left); cout << setw(3) << "   _______________________";
		// Right
		setColor(border_right);
		cout << setw(50) << "_______________________" << endl;
		/// LINE 2
		// Left
		setColor(border_left);
		cout << setw(3) << "|" << setw(24) << "\\";
		// Right
		setColor(border_right);
		cout << setw(26) << "/" << setw(24) << "|" << endl;
		/// LINE 3
		// Left
		setColor(border_left);
		cout << setw(3) << "|" << setw(20);
		setColor(text_color); cout << function_name;
		setColor(border_left); cout << setw(5) << ">";
		// Right
		setColor(border_right);
		cout << setw(24) << "<" << setw(20);
		setColor(text_color); cout << site;
		setColor(border_right); cout << setw(5) << "|" << endl;
		/// LINE 4
		// Left
		setColor(border_left);
		cout << setw(4) << "  |_______________________/";
		// Right
		setColor(border_right);
		cout << setw(50) << "\\_______________________|" << endl; grey;
	}
}

void printBankLabel()
{
	cout << endl;
	cout << setw(63);
	yellow;  cout << "<><><>" << space << (char)4 << space;
	white; cout << "WORLD BANK" << endl;
	grey;
	int i;
	for (i = 0; i < 80; i++)
		cout << '_';
	cout << dendl;
}

void paintRoyal(string string)
{
	SetConsoleTextAttribute(out, 11);
	cout << string;
	SetConsoleTextAttribute(out, 7);
}

void paintRed(string string)
{
	SetConsoleTextAttribute(out, 12);
	cout << string;
	SetConsoleTextAttribute(out, 7);
}

void paintWhite(string string)
{
	SetConsoleTextAttribute(out, 15);
	cout << string;
	SetConsoleTextAttribute(out, 7);
}

void paintGreen(string string)
{
	SetConsoleTextAttribute(out, 10);
	cout << string;
	SetConsoleTextAttribute(out, 7);
}

void paintYellow(string string)
{
	SetConsoleTextAttribute(out, 14);
	cout << string;
	SetConsoleTextAttribute(out, 7);
}

void paintWhite()
{
	SetConsoleTextAttribute(out, 15);
}

void eraseColor()
{
	SetConsoleTextAttribute(out, 7);
}

void alert(string string)
{
	printBankLabel();
	// Line 1
	yellow; cout << "\t\t*****************************************\n";
	// Line 2
	cout << "\t\t*" << setw(41) << "*\n";
	// Line 3
	cout << "\t\t*" << setw(30);
	paintGreen(string);
	yellow; cout << "\t\t*\n";
	// Line 4
	cout << "\t\t*" << setw(41) << "*\n";
	// Line 5
	paintYellow("\t\t*****************************************");
	cout << endl << endl;
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

char *getURL(char *ID, char* folder)
{
	char *url = new char[string_size];
	// /* ATTENTION! CHANGE URL TO YOUR CURRENT URL OF DIRECTORY OF MACHINE
	// * (Trong truong hop may khong nhan dien duoc dia chi tuong doi):
	// * strcpy(url, "D:\\Source\\DSA\\Team-Project\\Team-Project\\"); */
	strcpy(url, folder);
	strcat(url, "\\");
	strcat(url, ID);
	strcat(url, ".dat");
	return url;
}

string convertI2S(long long num)
{
	string string;
	if (num < 10)
		string += "0" + to_string(num);
	else
		string += to_string(num);
	return string;
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
// Kiem tra chuôi~ ki tu co fai^ la chuoi ki tu sô´ hay không
bool isNumber(char *num)
{
	int i;
	for (i = 0; i < strlen(num); i++)
	{
		if (num[i] < '0' || num[i] > '9')
		{
			return false;
		}
	}
	return true;
}
// Chi^ cho fep´ nhâp· kí tu· sô´
double inputNum()
{
	double num = 0;
	SetConsoleTextAttribute(out, 0x81);
	cin >> num;
	SetConsoleTextAttribute(out, 7);
	if (num <= 0)
		goodbye();
	return num;

	/*string line;
	SetConsoleTextAttribute(out, 0x81);
	cout << tab;
	limit = 0;
	while (getline(cin, line))
	{
		stopCondition(limit);
		stringstream ss(line);
		if (ss >> num)
			if (ss.eof())// Success
				break;
		SetConsoleTextAttribute(out, 7);
		cout << dtab << "Must be number! Reinput: ";
	}
	SetConsoleTextAttribute(out, 7);
	return num;*/

	/*//Solution 2
	char line[16];
	cin.ignore();
	cin.getline(line, 16);

	limit = 0;
	while (!isNumber(line))
	{
		stopCondition(limit);
		if (limit == 1)
			cout << tab << dline << dtab << "Must be number! Reinput: ";
		if (limit > 1)
			cout << dtab << "Reinput: ";
		cin.getline(line, 16);
	}
	num = atoi(line);*/
}
// Kiêm^ tra file co´ rông~ hay không
bool isFileEmpty(char *url)
{
	ifstream fi;
	fi.open(url);
	if (!fi)
		return false;
	string content = "";
	while (!fi.eof())
	{
		fi >> content;
		break;
	}
	fi.close();
	return content == "";
}

string randomString(int num_of_char)
{
	string s;
	int i;
	for (i = 0; i < num_of_char / 2; i++)
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

string printLine(int length, char c)
{
	string s = "";
	int i;
	for (i = 0; i < length; i++)
		s += c;
	return s;
}