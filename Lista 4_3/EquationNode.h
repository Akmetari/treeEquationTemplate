#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include "CMyClass.h"

using namespace std;

//OperatorNodes consts
const int NUMBER_OF_OPERATOR_TYPES = 7;

const string DEFAULT_TAG = "+";
const string TAGS[NUMBER_OF_OPERATOR_TYPES] = { "+","-", "*", "/", "sin", "cos", "#" };
const int NUMBER_OF_PARAMS_FOR_TAG[NUMBER_OF_OPERATOR_TYPES] = { 2,2,2,2,1,1,2 };
const string NOT_AN_OPERATOR = "Error! not an operator on second position";
const string NOT_A_VARIABLE = "Error! not a variable on first position";

//VariableNodes consts
const string DEFAULT_VAR = "x";
const string EQUALS = "= ";



template <typename T> class EquationNode
{


public:
	EquationNode<T>** children;
	int childrenNumber;
	int expectedChildrenNumber;
	EquationNode<T>* parent;


	EquationNode<T>(int childrenNum = 1);
	EquationNode<T>(EquationNode<T>& original, EquationNode<T>* par);
	EquationNode<T>(EquationNode<T>& original);
	~EquationNode<T>();

	EquationNode<T>** getChildren() { return children; }
	int getChildrenNumber() { return childrenNumber; }
	int getExpectedChildrenNumber() { return expectedChildrenNumber; }
	EquationNode<T>* getParent() { return parent; }
	void setParent(EquationNode<T>* parent) { this->parent = parent; }

	virtual void addChild(EquationNode<T>* child, int index = -1);
	int deleteChild(EquationNode<T>** child);

	virtual void assignValue(string varTag, T val);
	virtual string toString();
	virtual T evaluateNode();
	virtual bool isVariable();
	virtual bool isValue();
	virtual bool isOperator();
	void operator=(const EquationNode<T>& original);
	void findVars(vector<EquationNode<T>*>* varVector);

};

//OPERATOR NODE *****************************************************************************
template <typename T>class OperatorNode : public EquationNode<T> {

private:
	string tag;

public:
	string getTag() { return tag; }

	OperatorNode();
	OperatorNode(string tag);
	OperatorNode(EquationNode<T>* original, EquationNode<T>* parent = nullptr);
	void addChild(EquationNode<T>* child, int index);

	bool isVariable() { return false; }
	bool isValue() { return false; }
	bool isOperator() { return true; }

	~OperatorNode();
	void operator=(OperatorNode& original);
	string toString();
	T evaluateNode();

	void assignValue(string varTag, T val);

};

//VARIABLE NODE **************************************************************************
template <typename T>class VariableNode : public EquationNode<T> {

private:
	string tag;
	T value;

public:

	T getValue() { return value; }
	string getTag() { return tag; }
	bool isVariable() { return true; }
	bool isValue() { return false; }
	bool isOperator() { return false; }


	bool setValue(T val) {
		if (val >= 0) { // przeciazyc dla stringow!!
			value = val;
			return true;
		}
		else return false;
	}
	void setTag(string tag) { this->tag = tag; }

	VariableNode();
	VariableNode(string tag);
	VariableNode(EquationNode<T>* original, EquationNode<T>* parent = nullptr);

	~VariableNode();
	void operator=(VariableNode<T>& original);

	string toStringVal();
	string toString();
	T evaluateNode();

	void assignValue(string varTag, T val);
	void addChild(EquationNode<T>* child, int index);
};

//VALUE NODE **********************************************************************************************
template <typename T > class ValueNode : public EquationNode<T>
{
private:
	T value;

public:
	ValueNode();
	ValueNode(T val);
	ValueNode(EquationNode<T>* original, EquationNode<T>* parent = nullptr);
	bool isVariable() { return false; }
	bool isValue() { return true; }
	bool isOperator() { return false; }

	~ValueNode();
	void operator=(ValueNode<T>& original);
	string toString();
	T evaluateNode();
	void assignValue(string varTag, T val);

	void addChild(EquationNode<T>* child, int index);

};

// BODY EQUATION NODE   *****************************************************
template <typename T>
EquationNode<T>::EquationNode<T>(int childrenNum) {
	childrenNumber = 0;
	expectedChildrenNumber = childrenNum;
	children = new EquationNode<T> *[expectedChildrenNumber];
	parent = nullptr;
}

template <typename T>
EquationNode<T>::EquationNode<T>(EquationNode<T>& original) {
	childrenNumber = original.childrenNumber;
	expectedChildrenNumber = original.expectedChildrenNumber;
	if (children != nullptr) delete[] children;
	children = new EquationNode<T> *[expectedChildrenNumber];
	for (int i = 0; i < childrenNumber; i++) {
		EquationNode<T>* pointer = original.children[i];
		if (pointer->isValue()) children[i] = new ValueNode<T>(pointer, this);
		if (pointer->isOperator()) children[i] = new OperatorNode<T>(pointer, this);
		if (pointer->isVariable()) children[i] = new VariableNode<T>(pointer, this);
	}

	parent = original.parent;

}

template <typename T>
EquationNode<T>::EquationNode<T>(EquationNode<T>& original, EquationNode<T>* par) {
	childrenNumber = original.childrenNumber;
	expectedChildrenNumber = original.expectedChildrenNumber;
	if (children != nullptr) delete[] children;
	children = new EquationNode<T> * [expectedChildrenNumber];
	for (int i = 0; i < childrenNumber; i++) {
		EquationNode<T>* pointer = original.children[i];
		if (pointer->isValue()) children[i] = new ValueNode<T>(pointer, this);
		if (pointer->isOperator()) children[i] = new OperatorNode<T>(pointer, this);
		if (pointer->isVariable()) children[i] = new VariableNode<T>(pointer, this);
	}
	parent = par;

}

template <typename T>
EquationNode<T>:: ~EquationNode<T>() {
	for (int i = 0; i < childrenNumber; i++) delete (children[i]);
	delete children;
}

template <typename T>
void  EquationNode<T>:: operator=(const EquationNode<T>& original) {
	childrenNumber = original.childrenNumber;
	expectedChildrenNumber = original.expectedChildrenNumber;
	if (children != nullptr) delete[] children;
	children = new EquationNode<T> * [expectedChildrenNumber];
	for (int i = 0; i < childrenNumber; i++) {
		EquationNode<T>* pointer = original.children[i];
		if (pointer->isValue()) children[i] = new ValueNode<T>(pointer, this);
		if (pointer->isOperator()) children[i] = new OperatorNode<T>(pointer, this);
		if (pointer->isVariable()) children[i] = new VariableNode<T>(pointer, this);
	}
	parent = original.parent;
}

template <typename T>
void EquationNode<T>::addChild(EquationNode<T>* child, int index) {
	if (index < 0) {
		child->parent = this;
		children[childrenNumber] = child;
		childrenNumber++;
	}
	else if (children[index] == nullptr) {
		child->parent = this;
		children[index] = child;
		childrenNumber++;
	}
}

template <typename T>
int EquationNode<T>::deleteChild(EquationNode<T>** child) {
	int number = -1;
	for (int i = 0; i < childrenNumber; i++) {
		if (children[i] == *child) {
			delete children[i];
			children[i] = nullptr;
			number = i;
			childrenNumber--;
			return number;
		}
	}
	return number;
}

template <typename T>
string EquationNode<T>::toString() {
	return "";
}

template <>
int EquationNode<int>::evaluateNode() { return 0; }

template <>
CMyClass EquationNode<CMyClass>::evaluateNode() { return CMyClass(0); }  //AAAAAAAAAAAAAAA

template <>
double EquationNode<double>::evaluateNode() { return 0; }

template <>
string EquationNode<string>::evaluateNode() { return ""; }

template <typename T>
bool EquationNode<T>::isVariable() { return false; }

template <typename T>
bool EquationNode<T>::isValue() { return false; }

template <typename T>
bool EquationNode<T>::isOperator() { return false; }

template <typename T>
void EquationNode<T>::assignValue(string varTag, T val) {}

template <typename T>
void EquationNode<T>::findVars(vector <EquationNode<T>*>* varVector) {
	if (isVariable()) varVector->push_back(this);
	for (int i = 0; i < childrenNumber; i++) {
		children[i]->findVars(varVector);

	}
}



// BODY OPERATOR NODE  ********************************************************************************

template <typename T>
OperatorNode<T>::OperatorNode<T>() {
	tag = DEFAULT_TAG;
}

template <typename T>
OperatorNode<T>::OperatorNode<T>(EquationNode<T>* original, EquationNode<T>* parent) {
	OperatorNode* pointer = dynamic_cast<OperatorNode*>  (original);
	tag = pointer->tag;

	this->childrenNumber = pointer->childrenNumber;
	this->expectedChildrenNumber = pointer->expectedChildrenNumber;
	if (this->children != nullptr) delete[] this->children;
	this->children = new EquationNode<T>* [this->expectedChildrenNumber];
	for (int i = 0; i < this->childrenNumber; i++) {
		EquationNode<T>* childrenPtr = original->getChildren()[i];
		if (childrenPtr->isValue()) this->children[i] = new ValueNode<T>(childrenPtr, this);
		if (childrenPtr->isOperator()) this->children[i] = new OperatorNode<T>(childrenPtr, this);
		if (childrenPtr->isVariable()) this->children[i] = new VariableNode<T>(childrenPtr, this);
	}
	this->parent = parent;
}

template <typename T>
OperatorNode<T>::OperatorNode<T>(string tag) {
	this->tag = tag;

	for (int i = 0; i < NUMBER_OF_OPERATOR_TYPES; i++) {
		if (tag == TAGS[i]) this->expectedChildrenNumber = NUMBER_OF_PARAMS_FOR_TAG[i];
	}

	this->childrenNumber = 0;
	if (this->children != nullptr) delete[] this->children;
	this->children = new EquationNode<T> *[this->expectedChildrenNumber];
	this->parent = nullptr;

}

template <typename T>
OperatorNode<T>::~OperatorNode<T>() {}

template <typename T>
void OperatorNode<T>::operator=(OperatorNode<T>& original) {
	tag = original.tag;
	this->expectedChildrenNumber = original.expectedChildrenNumber;
	this->childrenNumber = original.childrenNumber;
	if (this->children != nullptr) delete[] this->children;
	this->children = new EquationNode<T> *[this->childrenNumber];
	for (int i = 0; i < this->childrenNumber; i++) this->children[i] = original.children[i];
	this->parent = original.parent;
}

template <typename T>
string OperatorNode<T>::toString() {
	return tag + " ";
}

template <>
string OperatorNode<string>::evaluateNode() {
	string eval = "";
	string leftChild = (*this->children[0]).evaluateNode();
	string rightChild = (*this->children[1]).evaluateNode();
	if (tag == TAGS[0]) for (int i = 0; i < this->childrenNumber; i++) eval += (*this->children[i]).evaluateNode(); //dodawanie stringow
	else if (tag == TAGS[2]) { //mnozenie stringow

		for (int i = 0; i < leftChild.length();i++)
		{
			if (leftChild[i] == rightChild[0]) {
				eval = eval + rightChild;
			}
			else eval = eval + leftChild[i];

		}
	}
	else if (tag == TAGS[1]) { //odejmowanie stringow
		int lastSubstrIndex=leftChild.rfind(rightChild);

		for (int i = 0; i < leftChild.length();i++)
		{
			if (i==lastSubstrIndex) {
				i = i + rightChild.length();
			}
			eval = eval + leftChild[i];

		}
	}
	else if (tag == TAGS[3]) { // dzielenie stringow
		int foundIndex = leftChild.find(rightChild);
		
		for (int i = 0; i < leftChild.length();i++)
		{
			if (i==foundIndex) {
				eval = eval + leftChild[i];
				i = i + rightChild.length() - 1;
				foundIndex = leftChild.substr(i).find(rightChild) +i;
			}
			else eval = eval + leftChild[i];

		}

	}
	return eval;
}

template <>
int OperatorNode<int>::evaluateNode() {
	int eval = 0;
	if (tag == TAGS[0]) for (int i = 0; i < this->childrenNumber; i++) eval += (*this->children[i]).evaluateNode();
	else if (tag == TAGS[2]) eval = (*this->children[0]).evaluateNode() * (*this->children[1]).evaluateNode();
	else if (tag == TAGS[1]) eval = (*this->children[0]).evaluateNode() - (*this->children[1]).evaluateNode();
	else if (tag == TAGS[3]) {
		if ((*this->children[1]).evaluateNode() == 0) return 0;
		else eval = (*this->children[0]).evaluateNode() / (*this->children[1]).evaluateNode();
	}
	else if (tag == TAGS[4]) eval = sin((*this->children[0]).evaluateNode());
	else if (tag == TAGS[5]) eval = cos((*this->children[0]).evaluateNode());
	else if (tag == TAGS[6]) for (int i = 0; i < this->childrenNumber; i++) eval += (*this->children[i]).evaluateNode();
	return eval;
}

template <>
double OperatorNode<double>::evaluateNode() {
	double eval = 0;
	if (tag == TAGS[0]) for (int i = 0; i < this->childrenNumber; i++) eval += (*this->children[i]).evaluateNode();
	else if (tag == TAGS[2]) eval = (*this->children[0]).evaluateNode() * (*this->children[1]).evaluateNode();
	else if (tag == TAGS[1]) eval = (*this->children[0]).evaluateNode() - (*this->children[1]).evaluateNode();
	else if (tag == TAGS[3]) {
		if ((*this->children[1]).evaluateNode() == 0) return 0;
		else eval = (*this->children[0]).evaluateNode() / (*this->children[1]).evaluateNode();
	}
	else if (tag == TAGS[4]) eval = sin((*this->children[0]).evaluateNode());
	else if (tag == TAGS[5]) eval = cos((*this->children[0]).evaluateNode());
	else if (tag == TAGS[6]) for (int i = 0; i < this->childrenNumber; i++) eval += (*this->children[i]).evaluateNode();
	return eval;
}

template <>
CMyClass OperatorNode<CMyClass>::evaluateNode() {
	double eval = 0;
	if (tag == TAGS[0]) for (int i = 0; i < this->childrenNumber; i++) eval += (*this->children[i]).evaluateNode().getNumber();
	else if (tag == TAGS[2]) eval = (*this->children[0]).evaluateNode().getNumber() * (*this->children[1]).evaluateNode().getNumber();
	else if (tag == TAGS[1]) eval = (*this->children[0]).evaluateNode().getNumber() - (*this->children[1]).evaluateNode().getNumber();
	else if (tag == TAGS[3]) {
		if ((*this->children[1]).evaluateNode().getNumber() == 0) return 0;
		else eval = (*this->children[0]).evaluateNode().getNumber() / (*this->children[1]).evaluateNode().getNumber();
	}
	else if (tag == TAGS[4]) eval = sin((*this->children[0]).evaluateNode().getNumber());
	else if (tag == TAGS[5]) eval = cos((*this->children[0]).evaluateNode().getNumber());
	else if (tag == TAGS[6]) for (int i = 0; i < this->childrenNumber; i++) eval += (*this->children[i]).evaluateNode().getNumber();
	return CMyClass(eval);
}

template <typename T>
void OperatorNode<T>::assignValue(string varTag, T val) {

	for (int i = 0; i < this->childrenNumber; i++) this->children[i]->assignValue(varTag, val);
}

template <typename T>
void OperatorNode<T>::addChild(EquationNode<T>* child, int index) { // ulepszyc modyfikacje z poprzednigo zadania

	if (this->tag == TAGS[6]) {
		if (this->childrenNumber == 0 && !child->isVariable()) {
			cout << NOT_A_VARIABLE;

			if (index < 0) {
				this->children[this->childrenNumber] = new VariableNode<T>();
				this->children[this->childrenNumber]->setParent(this);

				this->childrenNumber++;
			}
			else if (this->children[index] == nullptr) {
				this->children[index] = new VariableNode<T>();
				this->children[index]->setParent(this);
				this->childrenNumber++;
			}
		}
		else if (this->childrenNumber == 1 && !child->isOperator()) {
			cout << NOT_AN_OPERATOR;

			if (index < 0) {
				this->children[this->childrenNumber] = new OperatorNode<T>();
				this->children[this->childrenNumber]->setParent(this);
				this->childrenNumber++;
			}
			else if (this->children[index] == nullptr) {
				this->children[index] = new OperatorNode<T>();
				this->children[index]->setParent(this);
				this->childrenNumber++;
			}
		}
		else {

			if (index < 0) {
				child->setParent(this);
				this->children[this->childrenNumber] = child;
				this->childrenNumber++;
			}
			else if (this->children[index] == nullptr) {
				child->setParent(this);
				this->children[index] = child;
				this->childrenNumber++;
			}
		}
	}
	else {

		if (index < 0) {
			child->setParent(this);
			this->children[this->childrenNumber] = child;
			this->childrenNumber++;
		}
		else if (this->children[index] == nullptr) {
			child->setParent(this);
			this->children[index] = child;
			this->childrenNumber++;
		}
	}
}

// BODY VARIABLE NODE **************************************************************************


template <>
VariableNode<int>::VariableNode<int>() {
	tag = DEFAULT_VAR;
	value = 0;
	this->expectedChildrenNumber = 0;
}

template <>
VariableNode<double>::VariableNode<double>() {
	tag = DEFAULT_VAR;
	value = 0;
	this->expectedChildrenNumber = 0;
}

VariableNode<CMyClass>::VariableNode<CMyClass>() {
	tag = DEFAULT_VAR;
	value = CMyClass(0);
	this->expectedChildrenNumber = 0;
}

template <>
VariableNode<string>::VariableNode<string>() {
	tag = DEFAULT_VAR;
	value = "";
	this->expectedChildrenNumber = 0;
}

template <>
VariableNode<int>::VariableNode<int>(string tag) {
	this->tag = tag;
	value = 0; 
	this->expectedChildrenNumber = 0;
}

template <>
VariableNode<double>::VariableNode<double>(string tag) {
	this->tag = tag;
	value = 0;
	this->expectedChildrenNumber = 0;
}

VariableNode<CMyClass>::VariableNode<CMyClass>(string tag) {
	this->tag = tag;
	value = CMyClass(0);
	this->expectedChildrenNumber = 0;
}

template <>
VariableNode<string>::VariableNode<string>(string tag) {
	this->tag = tag;
	value = "";
	this->expectedChildrenNumber = 0;
}

template <typename T>
VariableNode<T>::VariableNode<T>(EquationNode<T>* original, EquationNode<T>* parent) {
	VariableNode* pointer = dynamic_cast<VariableNode*>  (original);
	tag = pointer->tag;
	value = pointer->value;

	this->childrenNumber = pointer->childrenNumber;
	this->expectedChildrenNumber = pointer->expectedChildrenNumber;
	if (this->children != nullptr) delete[] this->children;
	this->children = new EquationNode<T> *[this->expectedChildrenNumber];
	for (int i = 0; i < this->childrenNumber; i++) {
		EquationNode<T>* childrenPtr = original->getChildren()[i];
		if (childrenPtr->isValue()) this->children[i] = new ValueNode<T>(childrenPtr, this);
		else if (childrenPtr->isOperator()) this->children[i] = new OperatorNode<T>(childrenPtr, this);
		else if (childrenPtr->isVariable()) this->children[i] = new VariableNode<T>(childrenPtr, this);
	}
	this->parent = parent;
}

template <typename T>
VariableNode<T>::~VariableNode<T>() {}

template <typename T>
void VariableNode<T>::operator=(VariableNode<T>& original) {
	tag = original.tag;
	value = original.value;
	this->expectedChildrenNumber = original.expectedChildrenNumber;
	this->childrenNumber = original.childrenNumber;
	if (this->children != nullptr) delete[] this->children;
	this->children = new EquationNode<T> * [this->childrenNumber];
	for (int i = 0; i < this->childrenNumber; i++) this->children[i] = original.children[i];
	this->parent = original.parent;
}

template <typename T>
string VariableNode<T>::toStringVal() {
	return tag + EQUALS + to_string(value);
}

template <>
string VariableNode<CMyClass>::toStringVal() {
	return tag + EQUALS + to_string(value.getNumber());
}

template <>
string VariableNode<string>::toStringVal() {
	return tag + EQUALS + '"' + value+'"';
}

template <typename T>
string VariableNode<T>::toString() {
	return tag;
}

template <typename T>
T VariableNode<T>::evaluateNode() {
	return value;
}

template <typename T>
void VariableNode<T>::assignValue(string varTag, T val) {
	if (tag == varTag) value = val;
}

template <typename T>
void VariableNode<T>::addChild(EquationNode<T>* child, int index) {
	if (index < 0) {
		child->setParent(this);
		this->children[this->childrenNumber] = child;
		this->childrenNumber++;
	}
	else if (this->children[index] == nullptr) {
		child->setParent(this);
		this->children[index] = child;
		this->childrenNumber++;
	}
}


// BODY VALUE NODE ******************************************************************
template <>
ValueNode<int>::ValueNode() {
	value = 1;
	this->expectedChildrenNumber = 0;
}

template <>
ValueNode<double>::ValueNode() {
	value = 1;
	this->expectedChildrenNumber = 0;
}

ValueNode<CMyClass>::ValueNode() {
	value = CMyClass(1);
	this->expectedChildrenNumber = 0;
}

template <>
ValueNode<string>::ValueNode() {
	value = "";
	this->expectedChildrenNumber = 0;
}

template <typename T>
ValueNode<T>::ValueNode(T val) {
	value = val;
	this->expectedChildrenNumber = 0;
	this->childrenNumber = 0;
	if (this->children != nullptr) delete[] this->children;
	this->children = new EquationNode<T> *[this->expectedChildrenNumber];
	this->parent = nullptr;
}


template <typename T>
ValueNode<T>::ValueNode(EquationNode<T>* original, EquationNode<T>* parent) {
	ValueNode<T>* pointer = dynamic_cast<ValueNode<T>*>  (original);
	value = pointer->value;
	this->childrenNumber = pointer->childrenNumber;
	this->expectedChildrenNumber = pointer->expectedChildrenNumber;
	if (this->children != nullptr) delete[] this->children;
	this->children = new EquationNode<T> *[this->expectedChildrenNumber];
	for (int i = 0; i < this->childrenNumber; i++) {
		EquationNode<T>* childrenPtr = original->getChildren()[i];
		if (childrenPtr->isValue()) this->children[i] = new ValueNode<T>(childrenPtr, this);
		else if (childrenPtr->isOperator()) this->children[i] = new OperatorNode<T>(childrenPtr, this);
		else if (childrenPtr->isVariable()) this->children[i] = new VariableNode<T>(childrenPtr, this);
	}
	this->parent = original->getParent();
}

template <typename T>
ValueNode<T>::~ValueNode() {}

template <typename T>
void ValueNode<T>::operator=(ValueNode<T>& original) {
	value = original.value;
	this->childrenNumber = original.childrenNumber;
	if (this->children != nullptr) delete[] this->children;
	this->children = new EquationNode<T> *[this->childrenNumber];
	for (int i = 0; i < this->childrenNumber; i++) this->children[i] = original.children[i];
	this->parent = original.parent;
}

template <typename T>
string ValueNode<T>::toString() {
	return to_string(value) + " ";
}

template <>
string ValueNode<CMyClass>::toString() {
	return to_string(value.getNumber()) + " ";
}

template <>
string ValueNode<string>::toString() {
	return value + " ";
}

template <typename T>
T ValueNode<T>::evaluateNode() {
	return value;
}

template <>
CMyClass ValueNode<CMyClass>::evaluateNode() {
	return value.getNumber();
}

template <typename T>
void ValueNode<T>::assignValue(string varTag, T val) {}

template <typename T>
void ValueNode<T>::addChild(EquationNode<T>* child, int index) {
	if (index < 0) {
		child->setParent(this);
		this->children[this->childrenNumber] = child;
		this->childrenNumber++;
	}
	else if (this->children[index] == nullptr) {
		child->setParent(this);
		this->children[index] = child;
		this->childrenNumber++;
	}
}

