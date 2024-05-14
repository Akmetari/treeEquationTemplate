#pragma once
#include "TreeCalc.h"

const string INTERFACE_END_MESSAGE = "Goodbye!";
const string INTERFACE_WELCOME_MESSAGE = "Welcome to TreeCalc interface. \n";
const string INTERFACE_HELP = "exit- end program\nnewCalc- start new calculator\n\nTo show this message again type 'help'. ";
const string INT_ADD_ORD = "newCalc";
const string INTERFACE_CONSOLE_POINTER = "Interface>";

class Interface
{
	
private:
	static bool interfaceExit;

public:

	static void runInterface();
	static void output(string message, string data = "");

	static void input(int* param1);
	static void input(string* dataStr);

	static void newCalc(int typeNumber);

};

// BODY ********************************************************************

bool Interface::interfaceExit = false;

void Interface::runInterface() {
	string userInput = "";
	int typeNumber=0;
	output(INTERFACE_WELCOME_MESSAGE, INTERFACE_HELP);
	while (!interfaceExit) {
		output(INTERFACE_CONSOLE_POINTER, "");
		input(&userInput);
		if (userInput == EXIT_ORD) interfaceExit = true;
		else {
			if (userInput == HELP_ORD) output(INTERFACE_HELP, "");
			else if (userInput == INT_ADD_ORD) {
				output("What type of tree calculator do you want to create?\n\n1. Integer\n2. Double\n3.String\n4.CMyClass\n\nType number... "); 
				input(&typeNumber);
				newCalc(typeNumber);
			}
			else output(WRONG_ORDER_MESSAGE);
		}
	}
	output(INTERFACE_END_MESSAGE, "");
}

void Interface::output(string message, string data) {
	if (data != "") cout << message + NEW_LINE + data;
	else cout << message;
}

void Interface::input(string* dataStr) {
	cin >> *dataStr;
}


void Interface::input(int* param1) {
	cin >> *param1;
	if (*param1 < 0) *param1 = 0;
}


void Interface::newCalc(int typeNumber) { // tworzenie drzewa booli
	
	switch (typeNumber){
	case 1:{
			TreeCalc<int>().run();
			break;
		}
	case 2:{
			TreeCalc<double>().run();
			break;
		}
	case 3:{
			TreeCalc<string>().run();
			break;
		}
	case 4: {
		TreeCalc<CMyClass>().run();
		break;
	}
	}
}

