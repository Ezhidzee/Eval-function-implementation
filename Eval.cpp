#include "Eval.h"
#include <stack>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

static int characterPriority(const string &);

static vector<string> taskToRpn(const string &);

auto eval(const string &task) {
    stack<long double> nums;
    vector<string> rpn;
    string modifiedTask = task;
    long double n1 = 0, n2 = 0, res = 0;

    for (int i = 0; i < modifiedTask.length(); i++) {
        if (modifiedTask[i] == ' ') {
            modifiedTask.erase(i, 1);
        }
    }
    rpn = taskToRpn(modifiedTask);

    for (int i = 0; i < rpn.size(); i++) {
        if (!characterPriority(rpn[i])) {
            nums.push(stold(rpn[i]));
        } else {
            n2 = nums.top();
            nums.pop();
            n1 = nums.top();
            nums.pop();
            if (rpn[i] == "+") {
                res = n1 + n2;
            } else if (rpn[i] == "-") {
                res = n1 - n2;
            } else if (rpn[i] == "*") {
                res = n1 * n2;
            } else if (rpn[i] == "/") {
                res = n1 / n2;
            } else if (rpn[i] == "**") {
                res = pow(n1, n2);
            } else {
                res = fmodl(n1, n2);
            }
            nums.push(res);
        }
    }
    return res;
}

static vector<string> taskToRpn(const string &task) {
    vector<string> rpn;
    stack<string> arithmetic_symbols;
    char *taskSymbols = new char[task.length()];
    strcpy_s(taskSymbols, task.length() + 1, task.c_str());
    for (int i = 0; i < task.length(); i++) {
        if (characterPriority(string(1, taskSymbols[i])) && taskSymbols[i] != ')') {
            string simb = string(1, taskSymbols[i]);
            if (taskSymbols[i] == '*' && taskSymbols[i + 1] == '*') {
                simb = "**";
                i++;
            }
            while (!arithmetic_symbols.empty() &&
                   characterPriority(simb) <= characterPriority(arithmetic_symbols.top()) &&
                   simb != "(") {
                rpn.push_back(arithmetic_symbols.top());
                arithmetic_symbols.pop();
            }
            arithmetic_symbols.push(simb);
        } else if (taskSymbols[i] == ')') {
            string topSimb = arithmetic_symbols.top();
            while (topSimb != "(") {
                rpn.push_back(arithmetic_symbols.top());
                arithmetic_symbols.pop();
                topSimb = arithmetic_symbols.top();
            }
            arithmetic_symbols.pop();
        } else {
            string number = string(1, taskSymbols[i]);
            if (i != task.length() - 1) {
                while (i != task.length() - 1 && !characterPriority(string(1, taskSymbols[i + 1]))) {
                    number += string(1, taskSymbols[i + 1]);
                    i++;
                }
            }
            rpn.push_back(number);
        }
    }

    while (!arithmetic_symbols.empty()) {
        rpn.push_back(arithmetic_symbols.top());
        arithmetic_symbols.pop();
    }
    return rpn;
}

static int characterPriority(const string &character) {
    if (character == "**") {
        return 5;
    } else if (character == "/" || character == "*") {
        return 4;
    } else if (character == "%") {
        return 3;
    } else if (character == "+" || character == "-") {
        return 2;
    } else if (character == "(") {
        return 1;
    } else if (character == ")") {
        return -1;
    } else {
        return 0;
    }
}