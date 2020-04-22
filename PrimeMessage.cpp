// File: PrimeMessage.cpp
// @author Jon Dose, Dylan Greko


#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

/*
Prints message about the program
*/
void about();

/*
Gets and returns a message from user
*/
string getMsg();

/*
Returns the value of the string
*/
int getValue(string msg);

/*
Checks if given int value is prime
*/
bool isPrime(int value);

int main() {

	// About
	about();

	// Get message from user
	string msg = getMsg();

	// Get value of message
	int value = getValue(msg);
	cout << "Value is " << value << endl;
	
	// Return value, and isPrime(message);
	cout << "Your message " << (isPrime(value) ? "is " : "is not ") << "prime" << endl;

	return EXIT_SUCCESS;
}

void about() {
	cout << string(10, '+') << " A B O U T " << string(10, '+') << endl;
	cout << "This program computes a given 'string' into \na value and checks if the value is prime" << endl;
	cout << string(30, '+') << endl << endl;

	// Write own "pause" TODO
	system("pause");
}

string getMsg(){
	// Msg variable to return
	string msg;

	// Input variable for ensuring use of message
	string input;

	cout << "Please enter a message: ";

	// Gets multiline input
	getline(cin, msg);

	cout << "Are you sure you want to use this message? [y/n] ";
	cin >> input;
	tolower(input[0]);


	if (input != "y") {
		cout << endl;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getMsg();
	};

	
	return msg;
}

int getValue(string msg) {

	int value{ 0 };

	for (char c : msg) {
		value += c;
	}

	return value;
}

bool isPrime(int value){

	int stop = value / 2;

	for (int i{ 2 }; i <= stop; i++) {
		if (value % i == 0) {
			return false;
		}
	}

	return true;
}

