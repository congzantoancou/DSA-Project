
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
#define atm_owner_fee 4/10000 // 0.04%

#define dot '.'
#define tab '\t'
#define dtab "\t\t"
#define ttab "\t\t\t"
#define dash '_'
#define colon ':'
#define space ' '
#define slash '/'
#define dendl "\n\n"
#define pause system("pause")
#define dline "_________________________________________________________\n"
// Color
#define red SetConsoleTextAttribute(out, 12)
#define royal SetConsoleTextAttribute(out, 11)
#define yellow SetConsoleTextAttribute(out, 14)
#define green SetConsoleTextAttribute(out, 10)
#define white SetConsoleTextAttribute(out, 15)
#define grey SetConsoleTextAttribute(out, 7)
using namespace std;

// GLOBAL VARIABLE
HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
time_t theTime = time(NULL);
struct tm *aTime = localtime(&theTime);
int limit = 0;

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
	/* color_code = "color1 color2 difer/same"
	* if (type == same)
		{
			border = color1;
			text = color2;
		}
	else
		{
			border1 = color1;
			border2 = color2;
		}
	*
	*/
};

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

// Other functions
string getTime();
string getDate();
char *getURL(char *file_name, char *folder); // DIEU CHINH DIA CHI FILES 
char *convertStoC(string s); // Convert string to char*
string convertI2S(long long num); //Convert integer to string
double convertCurrency(double Amount, char *from, char *to); // Chuyen doi ty gia tien te
void alert(char *string); // To mau thong bao
void paintGreen(char *string);
void paintRed(char *string);
void paintYellow(char *string);
void paintWhite(char *string);
void paintWhite();
void eraseColor();
double inputNum();
void printBankLabel();
void printReceipt(linklist l, char *ID, string type, double); // In bien lai
char randomChar();
string randomString(int);
void waiting(int);
void drawFunction(char *function_name, char*, color code);
char *separateField(char *source, char *field, int &from, char separate_by);
bool isFileEmpty(char *url);
void printIndex(char *url);
void stopCondition(int &limit);
void align(columns &c);
void hold(int);
void welcome(int);
void goodbye();
string displayCurrency(double amount, char *currency);
string setPrecision(double amount, int streamsize);
void setColor(int colorcode);
color setColor(int color1, int color2, string type);
bool isLocked(linklist l, char *id);
void printLine(int length);

// ++++++++++++++++++ MAIN +++++++++++++++++++

int main()
{
	welcome(11);
	cout << dtab;
	pause;
	system("cls");
	cout << fixed;
	cout << setprecision(2);
	linklist l;
	initial(l);
	loadData(l);
	loadDataID(l);
	node *p = login(l);
	char ID[15]; // Lay ID
	if (p == NULL)
	{
		cout << tab << dline << endl << dtab;
		cout << "Login failed. Please take back the card!\n";
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
			drawFunction("1. BALANCE ENQUIRY", "2. WITHDRAW MONEY", color);
			drawFunction("3. TRANSFER MONEY", "4. VIEW TRANSACTIONS", color);
			color = setColor(11, 12, "difer");
			drawFunction("5. CHANGE PIN CODE", "6. DEPOSITE", color);
			color = setColor(11, 11, "difer");
			drawFunction("0. GETBACK CARD", "right", color);
			cout << tab << dline << endl;
			cout << tab << "Choose the service: ";
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
				alert("BALANCE ENQUIRY");
				cout << tab;
				checkBalance(l, ID);
				cout << endl << tab << dline << endl << dtab;
				cout << "Do you want to print a receipt? (y/n)" << tab;
				selection = getch();
				cout << endl;
				if (selection == 'y' || selection == 'Y'|| selection == 13)
				{
					cout << dtab << "Printing ";
					printReceipt(l, ID, NULL, 0);
					waiting(4);
					cout << endl;
					cout << dtab << "Done" << endl << tab;
					pause;
				}

				break;
			case '2':
				alert("WITHDRAW MONEY");
				// Nhap
				cout << dtab << "Enter Amount: ";
				amount = inputNum();
				cout << tab << dline << endl << tab;
				// Thuc thi
				cash = withDraw(l, ID, amount, "Not Applicable", "N/A", "with");
				// Kiem tra
				if (cash != 0.0)
				{
					cout << dtab << "Transaction was successful!" << endl;
					cout << endl << tab << dline << endl;
					cout << dtab << "Do you want to print a receipt? (y/n) >";
					selection = getch();
					cout << endl;
					if (selection == 'y' || selection == 'Y'|| selection == 13)
					{
						cout << dtab << "Printing ";
						printReceipt(l, ID, "with", cash);
							waiting(4);
						cout << endl;
						cout << dtab <<"Done" << endl << dtab;
						pause;
					}
				}
				break;
			case '3':
				alert("TRANSFER");
				// Nhap
				cout << dtab << "Enter receipt ID: ";
				cin >> recipient;
				cout << dtab << "Enter Amount\t: ";
				amount = inputNum();
				cout << tab << dline << endl;
				// Thuc thi
				checkTransaction = transfer(l, ID, amount, recipient, "rece");
				if (checkTransaction)
				{
					cout << dtab << "Transaction was successful!" << endl;
					cout << endl << tab << dline << endl;
					cout << dtab << "Do you want to print a receipt? (y/n) >";
					selection = getch();
					cout << endl;
					if (selection == 'y' || selection == 'Y'|| selection == 13)
					{
						cout << dtab << "Printing ";
						printReceipt(l, ID, "send", amount);
							waiting(4);
						cout << endl;
						cout << dtab <<"Done" << endl << dtab;
						pause;
					}
					cout << endl << tab << dline << endl;
					cout << dtab << "Do you want to do other transtaction? (y/n)!";
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
					alert("CHANGE PIN");
					cout << dtab << "New pin\t: ";
					string temp_pin = getpassword(""); cout << endl;
					NewPIN = convertStoC(temp_pin);

					if (limit == 1 && !isvalid(NewPIN))
					{
						Beep(400, 400);
						cout << tab << dline << endl <<  dtab << "Invalid PIN\n" << dtab;
						pause;
					}
				} while (!isvalid(NewPIN));
				if (changePIN(l, ID, NewPIN))
					cout << tab << dline << endl << dtab << "Change PIN succesfully!\n";
				Sleep(2000);
				break;
			case '6':
				Beep(400, 400);
				cout << dtab << "Sorry! This function is building\n" << dtab;
				pause;
				break;
			case '0':
				exit(1);
			exit:default:
				cout << endl;
				cout << tab << dline << endl;
				cout << dtab << (char)3 << " Thank you for using our service! " << (char)3;
				hold(4);
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
		char url[128] = "BienLai.txt";
		gi.open(url);
		gi << tab << dline << endl;
		gi << ttab << ttab << tab << "ANY BANK" << endl;
		gi << tab << dline << endl;
		gi << tab << "LOCATION: ";
		gi << setw(20) << "53 VO VAN NGAN" << endl;
		gi << setw(34) << "F. LINH CHIEU, THU DUC DIST." << setw(20) << "NY" << dendl;
		gi << tab << "CARD NO: " << p->data.ID << dendl;
		gi << setw(20) << "Date" << setw(10) << "Time" << setw(14) << "Terminal" << endl;
		gi << setw(20) << getDate() << setw(10) << getTime() << setw(14) << randomString(6) << dendl;
		gi << tab << "SEQ NBR:" << setw(10) << 1000 + rand() % 9000;
		if (type != "")
		{
			gi << setw(18) << "AMT:" << setw(20) << displayCurrency(cash, p->data.currency) << endl;
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
		gi << dtab << tab << (char)3 << "Thank you for using our service!" << (char)3 << endl;
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
		cout << dtab << "New PIN can not be the same with the current.\n";
	limit = 0;
	while (!isvalid(NewPIN) || isDuplicate == 0) // Nêu´ trùng nhau
	{
		stopCondition(limit);

		cout << dtab << "New PIN\t: ";
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
			cout << dtab << "Confirm\t: ";
			string temp_pin = getpassword(""); cout << endl;
			confirm = convertStoC(temp_pin);

			if (limit == 1 && !isvalid(confirm))
			{
				cout << tab << dline << endl << dtab << "Invalid PIN\n" << dtab;
				pause;
			}
		} while (!isvalid(confirm));
		stopCondition(limit);
		isMatch = strcmp(NewPIN, confirm);
		if (isMatch != 0)
			cout << tab << dline << dtab << "Not match!\n";
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
		return;
	}

	char *line = new char[128];
	strcpy(line, "");
	session s;
	char amount[14];
	char balance[14];
	vector <session> session_list;

	// DOC FILE
	// Bo qua 2 zòng chi^ muc·
	string temp;
	char temp_date[128];
	getline(doc, temp);
	getline(doc, temp);

	bool isFileEmpty = true;
	int i, j;
	while (!doc.eof())
	{
		doc.getline(line, 128);
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
		if (strlen(line) > 0)
			session_list.push_back(s);

		if (strlen(s.name_passive) < 1)
		{
			Beep(400, 400);
			cout << "Warning! The database is not match. Please check the data file of \"" << url << "\"." << endl << tab;
			return;
		}
	}

	// Xuat len man hinh
	cout << "****************************** TRANSACTION HISTORY *****************************" << endl;
	printIndex("");

	if (isFileEmpty)
		cout << tab << dline << endl << dtab << "Sorry! There have no data write down before.\n";

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
		cout << dtab << "Sorry! You can not make this transaction.\n" << dtab;
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
		cout << dtab << "************** Confirm **************" << endl << dtab
			// Xac nhan ID nhan tien
			<< "* Recipient ID\t: ";
		paintWhite(ID_passive);
		cout << endl << dtab;

		cout << "* Recipient Name: ";
		paintWhite(pj->data.name);

		cout << endl << dtab;
		cout << "* Amount\t: ";

		paintWhite();
		cout << amount << space << pi->data.currency << endl;
		eraseColor();
		cout << endl;

		cout << dtab << "<> Submit? (y/n)>";
		char choice = getch();
		cout << endl;
		if (choice == 'y' || choice == 'Y' || choice == 13) // Dông` ý giao zich·
		{
			// Xu li tai khoan gui tien
			double cash = withDraw(l, ID, amount, ID_passive, pj->data.name, "send");
			// Xu li tai khoan nhan tien
			if (cash != 0.0) // Kiem tra neu tien da nhan duoc
			{
				cout << dtab << "The transaction is doing ";
				hold(3);
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
					cout << "Do you want to do the other transaction? (y/n)>";
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
				cout << dtab << "Enter Amount: ";
				amount = inputNum();
				checkInput = isvalid(amount, p->data.balance, p->data.currency);
			}
			// Kiem tra neu khoan tien hop le thi thuc hien jao zix
			if (checkTransaction)
			{
				if (transaction_type == "with")
				{
					// Xac nhan so tien rut
					cout << endl << dtab << "************** Confirm ***************" << endl
						<< dtab	<< "* Amount: " << amount << space << p->data.currency << endl 
						<< dtab << "* Agree? (y/n) >";
					char choice = getch();
					cout << endl << tab;
					if (choice == 'n' || choice == 'N')
					{
						cout << ttab << "The transaction is canceled!\n";
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
			cout << "Your current balance: " << p->data.balance << space << p->data.currency << endl << tab;
	SetConsoleTextAttribute(out, 7);
}

node *login(linklist &l)
{
	char *ID = new char[15];
	char *pin = new char[7];
	alert("SYSTEM LOGIN");
	cout << endl;

	// Enter the ID
	limit = 0;
	do {
		stopCondition(limit);
		cout << dtab;
		paintGreen("Card ID");
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
			cout << tab << dline << endl << "Sorry! This card is temporary locked. Please contact the Anybank system to unlock\n";
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
				alert("SYSTEM LOGIN");
				cout << endl << dtab;
				paintGreen("Card ID");
				cout << tab << colon << space << ID << endl << tab;
				cout << tab;
				paintGreen("PIN code");
				cout << colon << space;
				string temp_pin = getpassword(""); cout << endl;
				pin = convertStoC(temp_pin);
				if (strlen(pin) != 6 && limit < 3)
				{
					Beep(400, 400);
					cout << dtab << "Invalid PIN\n" << dtab;
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
					paintRed("Incorrect PIN code!");
					cout << endl;	
					// Hoi nguoi zung co muon thoat hay khong
					cout << dtab << "Press any key to try again. Press ESC to quit . . .";
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
					red; cout << tab << dline << endl << "Sorry! Your card is temporarily locked. "
						<< "Please contact the nearest bank to get back.\n"; grey;
					cout << dtab; pause;
					exit(1);
				}
			}
		} while (e < 3 && checkPIN == false);
	}
	else
	{
		paintRed("\t\tInvalid card.\n");
	}

	return p;
}

//__________________________________________________________
//
// ****************** RELATED FUNCTIONS ********************
//__________________________________________________________

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

color setColor(int color1, int color2, string type)
{
	color c;
	c.color1 = color1;
	c.color2 = color2;
	c.type = type;
	return c;
}

string setPrecision(double amount, int streamsize)
{
	stringstream stream;
	stream << fixed << setprecision(streamsize) << amount;
	string sAmount = stream.str();
	return sAmount;
}

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

void displayData(linklist l)
{
	node *p = new node;
	for (p = l.pHead; p != NULL; p = p->pNext)
	{
		cout << p->data.ID << tab << p->data.pin << endl << tab;
	}
}

void printIndex(char *url)
{
	ofstream gi;
	int URLState = strcmp(url, "");
	ostream &out = (URLState != 0? gi : cout);
	if (URLState != 0)
	{
		gi.open(url, std::ios::out);
		out << tab << "Time"
			<< tab << "Date"
			<< dtab << "Amount"
			<< tab << "Balance"
			<< dtab << "Trans type"
			<< tab << "Transaction ID"
			<< dtab << "Name" << endl;
		out << tab << dline;
		gi.close();
	}
	else
	{
		columns c;
		align(c);
		out << "Time"
			<< setw(c.c2) << "Date"
			<< setw(c.c3) << "Amount"
			<< setw(c.c4) << "Balance"
			<< setw(c.c5) << "Trans type"
			<< setw(c.c6) << "Transaction ID"
			<< setw(c.c7) << "Name" << endl;
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

//_________________________________________
//
// ********** OTHER FUNCTIONS *************
//_________________________________________

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

void hold(int seconds)
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

void goodbye()
{
	cout << tab << dline << endl;
	cout << dtab << (char)3 << " Thank you for using our service! " << (char)3 << tab;
	hold(4);
	exit(1);
}

void stopCondition(int &limit)
{
	limit++;
	if (limit > 10)
	{
		cout << tab << dline << endl;
		cout << dtab << (char)3 << " Thank you for using our service! " << (char)3 << tab;
		Sleep(4000);
		exit(1);
	}
}

void setColor(int colorcode)
{
	SetConsoleTextAttribute(out, colorcode);
}
// draw function color_code (function_name, site_or_function_name)
void drawFunction(char *function_name, char *site, color code)
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

	if (strcmpi(site,"left") == 0)
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
	else if (strcmpi(site,"right") == 0)
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
	cout << ttab << ttab << dtab;
	yellow;  cout << (char)4 << space;
	white; cout << "WORLD BANK" << endl;
	grey;
	int i;
	for (i = 0; i < 80; i++)
		cout << '_';
	cout << dendl;
}

void paintRoyal(char *string)
{
	SetConsoleTextAttribute(out, 11);
	cout << string;
	SetConsoleTextAttribute(out, 7);
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

void paintWhite()
{
	SetConsoleTextAttribute(out, 15);
}

void eraseColor()
{
	SetConsoleTextAttribute(out, 7);
}

void alert(char *string)
{
	printBankLabel();
	// Line 1
	yellow; cout << "\t\t*****************************************\n";
	// Line 2
	cout << "\t\t*" << setw(41) << "*\n";
	// Line 3
	cout << "\t\t*" << dtab;
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
	char *url = new char[128];
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
// Kiem tra so tien nhap co hop le hay khong
bool isvalid(double amount, double balance, char *currency)
{
	bool result = true;
	double amount_VND = convertCurrency(amount, currency, "VND");
	if (amount_VND < 50000)
	{
		Beep(400, 400);
		cout << dtab << "The amount is too low!" << endl;
		result = false;
	}
	else if (amount > balance)
	{
		Beep(400, 400);
		cout << dtab << "The balance is not enough!" << endl;
		result = false;
	}
	else if (convertCurrency(balance - amount, currency, "VND") < 50000)
	{
		Beep(400, 400);
		cout << dtab << "The balance after withdraw is not allow lower than 50000 VND." << endl;
		result = false;
	}
	cout << endl;
	return result;
}

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

void printLine(int length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		cout << dash;
	}
}