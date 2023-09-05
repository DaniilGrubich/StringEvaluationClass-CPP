#include "StringExpression.h"
#include <iostream>
#include <string>
#include <math.h>

# define PI           3.14159265358979323846  /* pi */
using namespace std;


StringExpression::StringExpression(string* str)
{
	stringExpression = *str;

	answer = calculate(stringExpression);
}

float StringExpression::calculate(string str) {
	string innerStringExpression = str;
	//put to lower case
	for (int i = 0; i < innerStringExpression.size(); i++)
		innerStringExpression[i] = tolower(innerStringExpression[i]);

	//replace minus operations with  _
	for (int i = 0; i < innerStringExpression.length(); i++) {
		char c = innerStringExpression[i];
		char c2 = ' ';
		char preC = '@';
		if (i + 1 <= innerStringExpression.size() - 1)
			c2 = innerStringExpression[i + 1];
		if (i - 1 >= 0)
			preC = innerStringExpression[i - 1];
		if (c == '-' && !(preC == '*' || preC == '/' || preC == '-' || preC == '+' || preC == '_' || preC == '@' || preC == '(')) {
			innerStringExpression[i] = '_';
			//cout << "before target: " << preC << "\t";
			//cout << "target: " << c << "\t";
			//cout << "after target: " << c2 << endl;
		}

	}

	////replace -- to +
	//while (innerStringExpression.find("--") != string::npos) {
	//	innerStringExpression.replace(innerStringExpression.find("--"), 2, "+");
	//}

	//cout << "After replacing -: " << innerStringExpression << endl;

	//find functions
	int counter = 0;
	while (true) {
		if (counter > innerStringExpression.size() - 1) break;

		if (innerStringExpression.find("sin") != string::npos) {
			string functionName = "sin";
			int functionIndex = innerStringExpression.find(functionName);
			calculateInPrent(functionIndex + functionName.size(), &innerStringExpression, true, functionName);
			counter = -1;

		}
		else if (innerStringExpression.find("cos") != string::npos) {
			string functionName = "cos";
			int functionIndex = innerStringExpression.find(functionName);
			calculateInPrent(functionIndex + functionName.size(), &innerStringExpression, true, functionName);
			counter = -1;

		}
		else if (innerStringExpression.find("tan") != string::npos) {
			string functionName = "tan";
			int functionIndex = innerStringExpression.find(functionName);
			calculateInPrent(functionIndex + functionName.size(), &innerStringExpression, true, functionName);
			counter = -1;

		}
		else if (innerStringExpression.find("tr") != string::npos) {
			string functionName = "tr";
			int functionIndex = innerStringExpression.find(functionName);
			calculateInPrent(functionIndex + functionName.size(), &innerStringExpression, true, functionName);
			counter = -1;

		}

		counter++;
	}

	//cout << "After functions: " << innerStringExpression << endl;

	//calculate in ()
	counter = 0;
	while (true) {
		if (counter > innerStringExpression.length() - 1) break;

		char c = innerStringExpression[counter];
		char preC = '~';
		if (counter - 1 >= 0)
			preC = innerStringExpression[counter - 1];

		if (c == '(' && (preC != 'n')) {
			calculateInPrent(counter, &innerStringExpression, true, "");
			counter = -1;
		}
		counter++;
		//
		//
	}
	//
		//cout << "After (): " << innerStringExpression << endl;

		//power
	counter = 0;
	while (true) {
		if (counter > innerStringExpression.size() - 1) break;

		char c = innerStringExpression[counter];
		if (c == '^') {
			power(counter, &innerStringExpression);
			counter = -1;
		}
		counter++;
	}

	//cout << "After power: " << innerStringExpression << endl;

	//divide and multiply
	counter = 0;
	while (true) {
		if (counter > innerStringExpression.length() - 1) break;

		char c = innerStringExpression[counter];
		if (c == '*') {
			multiply(counter, &innerStringExpression);
			counter = -1;
		}
		else if (c == '/') {
			divide(counter, &innerStringExpression);
			counter = -1;
		}
		counter++;
	}

	//cout << "After * and /: " << innerStringExpression << endl;

	//add and subtract
	counter = 0;
	while (true) {
		if (counter > innerStringExpression.length() - 1) break;

		char c = innerStringExpression[counter];
		if (c == '_') {
			subtract(counter, &innerStringExpression);
			counter = -1;
		}
		else if (c == '+') {
			add(counter, &innerStringExpression);
			counter = -1;
		}
		counter++;
	}

	//cout << "After + and _: " << innerStringExpression << endl;


	string stringBeforeLeftOverMinuses = innerStringExpression;
	//do left over -
	counter = 0;
	while (true) {
		if (counter > innerStringExpression.length() - 1) break;

		char c = innerStringExpression[counter];

		if (c == '-') {
			try { subtract(counter, &innerStringExpression); }
			catch (...) { innerStringExpression = stringBeforeLeftOverMinuses; break; }
			counter = -1;
		}
		counter++;
	}

	//cout << innerStringExpression << endl;

	//replace -- to +
	while(innerStringExpression.find("--") != string::npos) {
	innerStringExpression.replace(innerStringExpression.find("--"), 2, "+");
	}
	string::size_type sz;
	return stod(innerStringExpression, &sz);
}


float StringExpression::calculateInPrent(int openingPrentIndex, string* originalString, bool replaceOriginalString, string addFunction) {
	float prentAns;
	string innerStringExpress = *originalString;

	int prentStartIndex;
	int prentEndIndex;
	prentStartIndex = openingPrentIndex;
	int prentInd = 0;

	for (int i = openingPrentIndex + 1; i < innerStringExpress.size(); i++) {
		if (innerStringExpress[i] == '(') prentInd++;
		else if (innerStringExpress[i] == ')') prentInd--;

		if (prentInd == -1) {
			prentEndIndex = i;

			prentAns = calculate(innerStringExpress.substr(prentStartIndex + 1, prentEndIndex - (prentStartIndex + 1)));

			//calculate functions
			if (addFunction.compare("") != 0) {
				if (addFunction.compare("sin") == 0) {
					string replacePart = innerStringExpress.substr(prentStartIndex - addFunction.size(), prentEndIndex + 1 - (prentStartIndex - addFunction.size()));
					innerStringExpress.replace(innerStringExpress.find(replacePart), replacePart.size(), to_string(sin(prentAns)));

				}
				else if (addFunction.compare("cos") == 0) {
					string replacePart = innerStringExpress.substr(prentStartIndex - addFunction.size(), prentEndIndex + 1 - (prentStartIndex - addFunction.size()));
					innerStringExpress.replace(innerStringExpress.find(replacePart), replacePart.size(), to_string(cos(prentAns)));

				}
				else if (addFunction.compare("tan") == 0) {
					string replacePart = innerStringExpress.substr(prentStartIndex - addFunction.size(), prentEndIndex + 1 - (prentStartIndex - addFunction.size()));
					innerStringExpress.replace(innerStringExpress.find(replacePart), replacePart.size(), to_string(tan(prentAns)));

				}
				else if (addFunction.compare("tr") == 0) {
					string replacePart = innerStringExpress.substr(prentStartIndex - addFunction.size(), prentEndIndex + 1 - (prentStartIndex - addFunction.size()));
					innerStringExpress.replace(innerStringExpress.find(replacePart), replacePart.size(), to_string(prentAns*PI/180));

				}
			}
			else {
				if (replaceOriginalString) {
					string replacePart = innerStringExpress.substr(prentStartIndex, prentEndIndex + 1 - (prentStartIndex));
					innerStringExpress = innerStringExpress.replace(innerStringExpress.find(replacePart), replacePart.size(), to_string(prentAns));
				}
			}
			break;
		}
	}

	*originalString = innerStringExpress;
	return prentAns;
}

void StringExpression::power(int& singPos, string* Inexp) {
	string innerExpression = *Inexp;
	//find index of number before sign
	int previousNumIndex = index_of_number_before_sign(singPos, &innerExpression);

	//find index of next sign
	int nextSignIndex = index_of_next_sign(singPos, &innerExpression);

	//cout << innerExpression.substr(previousNumIndex, singPos - previousNumIndex) << endl;
	float num1 = stod(innerExpression.substr(previousNumIndex, singPos - previousNumIndex));
	float num2 = stod(innerExpression.substr(singPos + 1, nextSignIndex - singPos + 2));
	float raised = pow(num1, num2);

	//replace multiplication expression with product
	string replacePart = innerExpression.substr(previousNumIndex, nextSignIndex - previousNumIndex);
	innerExpression = innerExpression.replace(innerExpression.find(replacePart), replacePart.size(), to_string(raised));
	*Inexp = innerExpression;
}

void StringExpression::multiply(int& singPos, string* Inexp) {
	string innerExpression = *Inexp;
	//find index of number before sign
	int previousNumIndex = index_of_number_before_sign(singPos, &innerExpression);

	//find index of next sign
	int nextSignIndex = index_of_next_sign(singPos, &innerExpression);

	float num1 = stod(innerExpression.substr(previousNumIndex, singPos - previousNumIndex));
	float num2 = stod(innerExpression.substr(singPos + 1, nextSignIndex - singPos + 2));
	float product = num1 * num2;

	//replace multiplication expression with product
	string replacePart = innerExpression.substr(previousNumIndex, nextSignIndex - previousNumIndex);
	innerExpression = innerExpression.replace(innerExpression.find(replacePart), replacePart.size(), to_string(product));
	*Inexp = innerExpression;
}

void StringExpression::divide(int& singPos, string* Inexp) {
	string innerExpression = *Inexp;
	//find index of number before sign
	int previousNumIndex = index_of_number_before_sign(singPos, &innerExpression);

	//find index of next sign
	int nextSignIndex = index_of_next_sign(singPos, &innerExpression);

	float num1 = stod(innerExpression.substr(previousNumIndex, singPos - previousNumIndex));
	float num2 = stod(innerExpression.substr(singPos + 1, nextSignIndex - singPos + 2));
	float product = num1 / num2;

	//replace multiplication expression with product
	string replacePart = innerExpression.substr(previousNumIndex, nextSignIndex - previousNumIndex);
	innerExpression = innerExpression.replace(innerExpression.find(replacePart), replacePart.size(), to_string(product));
	*Inexp = innerExpression;
}

void StringExpression::subtract(int& singPos, string* Inexp) {
	string innerExpression = *Inexp;
	//find index of number before sign
	int previousNumIndex = index_of_number_before_sign(singPos, &innerExpression);

	//find index of next sign
	int nextSignIndex = index_of_next_sign(singPos, &innerExpression);


	float num1 = stod(innerExpression.substr(previousNumIndex, singPos - previousNumIndex));
	float num2 = stod(innerExpression.substr(singPos + 1, nextSignIndex - singPos + 2));
	float difference = num1 - num2;

	//replace multiplication expression with product
	string replacePart = innerExpression.substr(previousNumIndex, nextSignIndex - previousNumIndex);
	innerExpression = innerExpression.replace(innerExpression.find(replacePart), replacePart.size(), to_string(difference));

	*Inexp = innerExpression;
}

void StringExpression::add(int& singPos, string* Inexp) {
	string innerExpression = *Inexp;
	//find index of number before sign
	int previousNumIndex = index_of_number_before_sign(singPos, &innerExpression);

	//find index of next sign
	int nextSignIndex = index_of_next_sign(singPos, &innerExpression);

	//cout << innerExpression.substr(previousNumIndex, singPos - previousNumIndex) << endl;
	float num1 = stod(innerExpression.substr(previousNumIndex, singPos - previousNumIndex));
	float num2 = stod(innerExpression.substr(singPos + 1, nextSignIndex - singPos + 2));
	float sum = num1 + num2;

	//replace multiplication expression with product
	string replacePart = innerExpression.substr(previousNumIndex, nextSignIndex - previousNumIndex);
	innerExpression = innerExpression.replace(innerExpression.find(replacePart), replacePart.size(), to_string(sum));

	*Inexp = innerExpression;
}

int StringExpression::index_of_next_sign(int& singPos, string* s) {
	int nextSignIndex = singPos;
	string innerExpression = *s;
	while (true) {
		nextSignIndex++;

		char selectedChar;
		if (nextSignIndex > innerExpression.size() - 1) { nextSignIndex = innerExpression.length(); break; }
		else  selectedChar = innerExpression[nextSignIndex];


		if (selectedChar == '_' || selectedChar == '+' || selectedChar == '/' || selectedChar == '*' || selectedChar == '^')
			break;
	}
	return nextSignIndex;
}

int StringExpression::index_of_number_before_sign(int& singPos, string* s) {
	string innerExpression = *s;
	int previousNumIndex = singPos;
	while (true) {
		previousNumIndex--;

		char selectedChar;
		if (previousNumIndex < 0) {
			previousNumIndex++;
			break;
		}
		else selectedChar = innerExpression[previousNumIndex];

		if (selectedChar == '_' || selectedChar == '+' || selectedChar == '/' || selectedChar == '*' || selectedChar == '^') {
			previousNumIndex++;     break;
		}
	}

	return previousNumIndex;
}

