#include "Set.h"  // with ItemType being a type alias for char
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

bool toPostfix(string infix, string&postfix);
int precedence(char c);
int postfixToTfPostfix(string postfix, string& fixedPostfix, const Set& trueValues, const Set& falseValues);
bool evalPostfix(string postfix);

int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result) {
    bool valid = toPostfix(infix, postfix);
    if(!valid) return 1;
    if(valid) {
        string tfpostfix;
        int canEval = postfixToTfPostfix(postfix, tfpostfix, trueValues, falseValues);
        if(canEval == 0) {
            result = evalPostfix(tfpostfix);
            return 0;
        }
        else {
            if(canEval == 2) return 2;
            return 3;
        }
    }
    return -1;
}

bool toPostfix(string infix, string& postfix) {
    postfix = "";
    stack<char> stack;
    string fixedInfix = "";
    for(char c : infix) {
        if(c != ' ') fixedInfix += c;
    }
    infix = fixedInfix;
    if(infix == "") return false;
    
    //beginning or end is an invalid symbol
    if(infix[0] == '|' || infix[0] == '&' || infix[infix.size()-1] == '|' ||
       infix[infix.size()-1] == '&' || infix[infix.size()-1] == '!' ||
       infix[infix.size()-1] == '(') return false;
    
    for(int i = 0; i < infix.size(); i++) {
        char c = infix[i];
        char after = ' '; //uses to check if symbol after is bad
        if((i+1)<infix.size())
            after = infix[i+1];
        if(islower(c)) {
            if(islower(after) || after == '(' || after == '!') return false;
            postfix += c;
        }
        else if(c == '(' || c == '!') {
            if(after == ')' || after == '&' || after == '|') return false;
            stack.push(c);
        }
        else if(c == ')') {
            if(islower(after) || after == '(') return false;
            while(!stack.empty() && stack.top() != '(') {
                postfix += stack.top();
                stack.pop();
            }
            if(stack.empty()) return false;
            stack.pop();
        }
        else if(c == '|' || c == '&') {
            if(after == ')' || after == '&' || after == '|') return false;
            while(!stack.empty() && stack.top() != '(' &&
                  precedence(c) <= precedence(stack.top())) {
                postfix += stack.top();
                stack.pop();
            }
            stack.push(c);
        }
        else return false;
    }
    
    while(!stack.empty())
    {
        if(stack.top() == '(' || stack.top() == ')') return false;
        postfix += stack.top();
        stack.pop();
    }
    return true;
}


int precedence(char c) {
    switch (c) {
        case '!':
            return 3;
        case '&':
            return 2;
        case '|':
            return 1;
        default:
            return 0;
    }
}

int postfixToTfPostfix(string postfix, string& fixedPostfix, const Set& trueValues, const Set& falseValues) {
    fixedPostfix = "";
    for(char c : postfix) {
        if(isalpha(c)) {
            if(trueValues.contains(c)) {
                if(falseValues.contains(c)) return 3;
                fixedPostfix+= 't';
            }
            else if(falseValues.contains(c)) {
                if(trueValues.contains(c)) return 3;
                fixedPostfix += 'f';
            }
            else return 2;
        }
        else if(c == '|' || c == '&' || c == '!')
            fixedPostfix += c;
        else return -1;
    }
    return 0;
}

bool evalPostfix(string postfix) {
    stack<bool> stack;
    for(char c : postfix) {
        if (c == 't' || c == 'f') {
            switch (c) {
                case 't':
                    stack.push(true);
                    break;
                case 'f':
                    stack.push(false);
                    break;
            }
        }
        else if (c == '!') {
            bool b = stack.top();
            stack.pop();
            stack.push(!b);
        }
        else if (c == '&' || c == '|') {
            bool operand2 = stack.top();
            stack.pop();
            bool operand1 = stack.top();
            stack.pop();
            switch (c) {
                case '&':
                    stack.push(operand1 & operand2);
                    break;
                case '|':
                    stack.push(operand1 | operand2);
                    break;
                
            }
        }
    }
    return stack.top();
}

int main() {
    string trueChars  = "tywz";
    string falseChars = "fnx";
    Set trues;
    Set falses;
    for (int k = 0; k < trueChars.size(); k++)
        trues.insert(trueChars[k]);
    for (int k = 0; k < falseChars.size(); k++)
        falses.insert(falseChars[k]);
    string pf;
    bool answer;
    assert(evaluate("w| f", trues, falses, pf, answer) == 0  &&  pf == "wf|" &&  answer);
    assert(evaluate("y|", trues, falses, pf, answer) == 1);
    assert(evaluate("n t", trues, falses, pf, answer) == 1);
    assert(evaluate("nt", trues, falses, pf, answer) == 1);
    assert(evaluate("()", trues, falses, pf, answer) == 1);
    assert(evaluate("()z", trues, falses, pf, answer) == 1);
    assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1);
    assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
    assert(evaluate("(n&(t|y)", trues, falses, pf, answer) == 1);
    assert(evaluate("n+y", trues, falses, pf, answer) == 1);
    assert(evaluate("", trues, falses, pf, answer) == 1);
    assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0
                            &&  pf == "ff!tn&&|"  &&  !answer);
    assert(evaluate(" x  ", trues, falses, pf, answer) == 0  &&  pf == "x"  &&  !answer);
    trues.insert('x');
    assert(evaluate("((x))", trues, falses, pf, answer) == 3);
            falses.erase('x');
    assert(evaluate("((x))", trues, falses, pf, answer) == 0  &&  pf == "x"  &&  answer);
            trues.erase('w');
    assert(evaluate("w| f", trues, falses, pf, answer) == 2);
            falses.insert('w');
    assert(evaluate("w| f", trues, falses, pf, answer) == 0  &&  pf == "wf|" &&  !answer);
    cout << "Passed all tests" << endl;
}
