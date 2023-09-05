#include <iostream>
using namespace std;

class StringExpression
{
public:
	StringExpression(string* str);
	float calculate(string str);
	float answer;

private:
	string stringExpression;

	float calculateInPrent(int openingPrentIndex, string* originalString, bool replaceOriginalString, string addFunction);
	void power(int& singPos, string* Inexp);
	void multiply(int& singPos, string* Inexp);
	void divide(int& singPos, string* Inexp);
	void subtract(int& singPos, string* Inexp);
	void add(int& singPos, string* Inexp);
	int index_of_next_sign(int& singPos, string* s);
	int index_of_number_before_sign(int& singPos, string* s);
};

