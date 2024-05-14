#pragma once
class CMyClass
{

private:
	double number;

public:
	double getNumber() { return number; }
	void setNumber(double n) { number = n; }

	CMyClass() {
		number = 0;
	}

	CMyClass(double num) {
		number = num;
	}

};

