


#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <cctype>
#include <string>
#include <fstream>
#include <limits>
using namespace std; 
const string clientsFile = "ClientsData.txt";

struct stClient {
	string accountNumber;
	string pinCode;
	string name;
	string phone;
	int accountBalance;
};
stClient loggedIn_Client;
void mainMenue();
void mainMenueOperations();
void login();



void setLoggedInClient(stClient client) {
	loggedIn_Client = client;
}

vector<string> split(string stLine, string sep) {
	vector<string> vString;
	int pos = 0;
	string newSt;

	while ((pos = stLine.find(sep)) != string::npos) {
		newSt = stLine.substr(0, pos);
		if (newSt != "") {
			vString.push_back(newSt);
		}
		stLine.erase(0, pos + sep.length()); // Remove processed part
	}

	if (stLine != "") {
		vString.push_back(stLine); // Add last part
	}
	return vString;
}

stClient ConvertLineToRecord(string line, string sep = "#//#")
{

	stClient c;

	vector <string> vClientData = split(line, sep);
	c.accountNumber = vClientData[0];
	c.pinCode = vClientData[1];
	c.name = vClientData[2];
	c.phone = vClientData[3];
	c.accountBalance = stod(vClientData[4]);

	return c;

}
string ConvertRecordToLine(stClient client, string seprator = "#//#")
{
	string recordLine = "";
	recordLine += client.accountNumber + seprator;
	recordLine += client.pinCode + seprator;
	recordLine += client.name + seprator;
	recordLine += client.phone + seprator;
	recordLine += to_string(client.accountBalance);
	return recordLine;
}

vector <stClient> loadClientsDataFromFile(string clientsFile)

{

	fstream myfile;
	vector<stClient> vFileContent;

	myfile.open(clientsFile, ios::in); // Read Mode ; \

	if (myfile.is_open())
	{

		string line;
		stClient client;

		while (getline(myfile, line))
		{

			client = ConvertLineToRecord(line);
			vFileContent.push_back(client);
		}


		myfile.close();
	}

	return vFileContent;
}
vector <stClient> saveClientDataToFile(string filename, vector<stClient> vClients)
{


	fstream myfile;


	myfile.open(filename, ios::out);

	stClient c;
	string line;
	if (myfile.is_open())
	{

		for (stClient c : vClients)
		{

			
			

				line = ConvertRecordToLine(c);

				myfile << line << endl;
			


		}


		myfile.close();


	}


	return vClients;


}


void UpdateClientBalanceInVector(stClient& client, vector<stClient>& vClients) {
	for (stClient& c : vClients) {
		if (c.accountNumber == client.accountNumber) {
			c.accountBalance = client.accountBalance;
			break;
		}
	}
}
bool searchForAccountByNumber(string accountNumber, string pinCode, stClient& client)
{
	vector<stClient> vClients = loadClientsDataFromFile(clientsFile);

	for (stClient c : vClients)
	{


		if (c.accountNumber == accountNumber && c.pinCode == pinCode)
		{
			client = c;
			return true;
		}

	}

	return false;

}

void login() {
	while (true) {
		system("cls");
		cout << "==================================\n";
		cout << "          LOGIN SCREEN            \n";
		cout << "==================================\n";

		string accountNumber, pinCode;
		stClient client;

		cout << "Enter Account Number: ";
		getline(cin >> ws, accountNumber);
		cout << "Enter Account PinCode: ";
		getline(cin >> ws, pinCode);

		if (searchForAccountByNumber(accountNumber, pinCode, client)) {
			loggedIn_Client = client;

			cout << "\nLogin Successful! Welcome, " << loggedIn_Client.name << endl;
			cout << "Press Enter to continue to the main menu...";
			
			cin.get();

			system("cls");
			mainMenueOperations();
			return; 
		}

		cout << "\nInvalid Account Number or Pin Code! Try again.\n";
		cout << "Press any key to retry...";
		
		cin.get();
	}
}

void mainMenue()
{
	cout << "==========================================" << endl;
	cout << "	ATM Main Menue Screen		" << endl;
	cout << "==========================================" << endl;
	cout << "	[1] Qucik WithDraw ." << endl;
	cout << "	[2] Normal WithDraw ." << endl;
	cout << "	[3] Deposit." << endl;
	cout << "	[4] Check Balance ." << endl;
	cout << "	[5] LogOut ." << endl;
	cout << "==========================================" << endl;
	

}




bool withdraw(stClient& client, int amount, vector <stClient>& vClients)
{

	if (amount > client.accountBalance)
	{

		return false;
	}
	else {
		client.accountBalance -= amount;
		UpdateClientBalanceInVector(client, vClients); 
		return true;
	}

	
}

void WithDraw(vector <stClient>& vClients)
{
	int amount;

	cout << "Enter The Amount you want to WithDraw :" << endl; 
	cin >> amount; 
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	if (withdraw(loggedIn_Client, amount,vClients))
	{
		
		cout << "WithDraw a  " << amount << "$ Successfully !" << endl << endl;;
		cout << "============================" << endl;
		cout << "Current Account Balance :" << loggedIn_Client.accountBalance << endl;
		cout << "============================" << endl;
		saveClientDataToFile(clientsFile, vClients);
	}
	else {
		cout << "Amount exceeds Account Balance, Enter Lesser Amount!" << endl;
	}

	

}


void QuickwithdrawMenu()
{

	cout << "==========================================" << endl;
	cout << "	Quick WithDraw		" << endl;
	cout << "==========================================" << endl;

	cout << "[1] 20" << "\t\t" << "[2] 50" << endl;
	cout << "[3] 100" << "\t\t" << "[4] 200" << endl;
	cout << "[5] 400" << "\t\t" << "[6] 600" << endl;
	cout << "[7] 800" << "\t\t" << "[8] 1000" << endl;
	cout << "[9] Exit" << endl;
	cout << "==========================================" << endl;
	cout << "Your Balance is " << loggedIn_Client.accountBalance << endl;

}

enum enQuickWithDrawChoices
{
	twenty = 20, fifty = 50, oneHundred = 100, twoHundred = 200
	, fourHundred = 400, sixHundred = 600, eightHundred = 800,
	thousand = 1000
};

void Quickwithdraw(stClient& client, vector <stClient>& vClients)
{

	QuickwithdrawMenu();

	short choice;
	cout << "Choose what to withdraw [1-9]: "<<endl;
	cin >> choice;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	int withdrawAmount = 0;
	if (choice == 9)
	{
		return;
	}
	switch (choice)
	{
	case 1: withdrawAmount = twenty; break;
	case 2: withdrawAmount = fifty; break;
	case 3: withdrawAmount = oneHundred; break;
	case 4: withdrawAmount = twoHundred; break;
	case 5: withdrawAmount = fourHundred; break;
	case 6: withdrawAmount = sixHundred; break;
	case 7: withdrawAmount = eightHundred; break;
	case 8: withdrawAmount = thousand; break;
	case 9:
		return;
		
	default:
		cout << "\nInvalid choice.\n";
	}
		
	if (withdrawAmount > loggedIn_Client.accountBalance) {
		cout << "\nInsufficient balance!\n";
		return;
	}

	loggedIn_Client.accountBalance -= withdrawAmount;

	UpdateClientBalanceInVector(loggedIn_Client, vClients);
	

	cout << "\nSuccessfully withdrawn " << withdrawAmount << "$\n" << endl;
	cout << "Your new balance: " << loggedIn_Client.accountBalance << "$\n";
	saveClientDataToFile(clientsFile, vClients);
}

void Deposit(vector<stClient>& vClients)
{

	int amount; 
	cout << "Enter The Amount You Want To Deposit :" << endl;
	cin >> amount; 
	cin.ignore(numeric_limits<streamsize>::max(), '\n');


	loggedIn_Client.accountBalance += amount; 
	UpdateClientBalanceInVector(loggedIn_Client, vClients);
	saveClientDataToFile(clientsFile, vClients);


	cout << "\nSuccessfully deposited " << amount << "$\n";
	cout << "Your new balance: " << loggedIn_Client.accountBalance << "$\n";



}

void CheckBalance() {
	cout << "============================" << endl;
	cout << "Your Current Balance is: " << loggedIn_Client.accountBalance << "$" << endl;
	cout << "============================" << endl;
}







void mainMenueOperations()
{

	vector<stClient> vClients = loadClientsDataFromFile(clientsFile);
	int choice;

	do {
		
		system("cls");
		mainMenue();
		
		cout << "\n\nChoose What Do You Want To Do ? [1 to 5] : ";
		cin >> choice;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		system("cls");

		switch (choice)
		{
		case 1 :
			Quickwithdraw(loggedIn_Client, vClients);
			cout << "\nPress any key to return to the main menu...";
			cin.get();
			break;
		case 2 :
			WithDraw(vClients);
			cout << "\nPress any key to return to the main menu...";
			cin.get();
			break;

		case 3 :
			Deposit(vClients);
			cout << "\nPress any key to return to the main menu...";
			cin.get();
			break;
		case 4 :
			CheckBalance();
			cout << "\nPress any key to return to the main menu...";
			cin.get();
			break;
		case 5 :
			login();
		default :
			cout << "Invalid Choice Try Again !" << endl;

		}







	} while (choice != 6); 

	







}



int main()
{
	login();
	return 0; 
}

