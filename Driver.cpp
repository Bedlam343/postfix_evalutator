#include <iostream>
#include <stack>
#include <regex>
#include <exception>
#include <fstream>

using namespace std;

//Empties the stack
void clearStack(stack<double>&);
//Returns true if the passed operator is Supported
bool validOperator(string);

int main () {

    stack<double>Stack;
    bool loop = true;
    regex reg ("[^ ]-?\\d*\\.?\\d*|[^ \\w]");

    do {
        clearStack(Stack);

        string input;
        string errorOutput;
        bool valid = true;
        cout << "Please type a postfix expression or press Enter to quit: ";
        getline(cin, input);
        if(input.empty()) {
            exit (0);
        }

        //Use a regex iterator to look for the very first in the string with the regex
        sregex_iterator currentMatch(input.begin(), input.end(), reg);
        sregex_iterator lastMatch;

        while(currentMatch != lastMatch && valid) {
            smatch match = *currentMatch;
            string str = match.str();

            //First of all check if the character is an alphabet
            if(isalpha(str[0])) {
                errorOutput = "Bad operand";
                valid = false;
                break;
            }
            //Check if the current Match is a digit or an operator
            //str[0] or str[1] because the number could be a negative so number would be second index
            else if(isdigit(str[0]) || isdigit(str[1])) {
                Stack.push(stod(str));
            }
            //We know it's not a digit, if operator valid, time for some calculations
            else if (validOperator(str)) {

                char oper = str[0];

                //If stack is empty, not enough operands, break the while loop
                if(Stack.empty()) {
                    errorOutput = "Not enough operands";
                    valid = false;
                    break;
                }
                //If Stack size is 1 and operator isn't ~ or !
                else if (Stack.size() == 1 && oper != '~' && oper != '!') {
                    errorOutput = "Not enough operands";
                    valid = false;
                    break;
                }
                //If enough operands for every single operator, proceed to calculations
                else {

                    double v2, v1;
                    v2 = Stack.top();
                    Stack.pop();

                    switch(oper) {
                        case '+':
                            v1 = Stack.top();
                            Stack.pop();
                            Stack.push(v1 + v2);
                            break;
                        case '-':
                            v1 = Stack.top();
                            Stack.pop();
                            Stack.push(v1 - v2);
                            break;
                        case '*':
                            v1 = Stack.top();
                            Stack.pop();
                            Stack.push(v1 * v2);
                            break;
                        case '/':
                            v1 = Stack.top();
                            Stack.pop();
                            if(v2 == 0) {
                                errorOutput = "Range error: Can't divide by zero.";
                                valid = false;
                            }
                            else {
                                Stack.push(v1 / v2);
                            }
                            break;
                        case '~':
                            v2 *= -1;
                            Stack.push(v2);
                            break;
                        case '!':
                            v2 = (int)v2;
                            if(v2 <= 1) {
                                v2 = 1;
                            }
                            else {
                                for(int n = v2 - 1; n >= 1; n--) {
                                    v2 *= n;
                                }
                            }
                            Stack.push(v2);
                            break;
                    }
                }
            }
            //If invalid operator, exit the while loop
            else {
                errorOutput = "Unsupported Operator";
                valid = false;
                break;
            }

            //Advance the iterator to the next match
            currentMatch++;
        }

        //If something was invalid, display appropriate error message.
        if(!valid) {
            cout << errorOutput;
        }
        //If the stack has only one thing, mission complete
        else if(Stack.size() == 1) {
            cout << input << " = " << Stack.top();
        }
        //If more than one things in the stack, mission failed
        else if (Stack.size() > 1) {
            cout << "Malformed Expression";
        }
        cout << "\n";
    }while(loop);

    return 0;
}

void clearStack(stack<double> &Stack) {
    while(!Stack.empty()) {
        Stack.pop();
    }
}

bool validOperator(string str) {
    char c = str[0];
    return c == '+' || c == '-' || c == '!' || c == '~' || c == '/' || c == '*';
}

