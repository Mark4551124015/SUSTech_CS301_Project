// #define __CALC_DEMO_DEBUG

#ifndef __CALC_DEMO_DEBUG
#include "calc_algo.hpp"
#endif

#include <cmath>
#include <cstdio>
#include <string>

using std::isnan;
using std::nan;
using std::string;
using std::to_string;

using ll = long long;
using pdd = std::pair<double, double>;

int find_ch(string s, char ch) {
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == ch) return i;
    }
    return -1;
}

int find_ch_rev(string s, char ch) {
    for (int i = s.size() - 1; i >= 0; --i) {
        if (s[i] == ch) return i;
    }
    return -1;
}

int findSubstring(string str, string subStr) {
    int strLen = str.length();
    int subStrLen = subStr.length();

    for (int i = 0; i <= strLen - subStrLen; i++) {
        int j;
        for (j = 0; j < subStrLen; j++) {
            if (str[i + j] != subStr[j]) {
                break;
            }
        }
        if (j == subStrLen) {
            return i;
        }
    }

    return -1;
}

ll binary2Decimal(string binaryStr) {
    ll decimal = 0;
    ll power = 1;

    for (int i = binaryStr.length() - 1; i >= 0; i--) {
        if (binaryStr[i] == '1') {
            decimal += power;
        }
        power <<= 1;
    }

    return decimal;
}

string double2String(double value) {
    string res;
    if (value < 0) {
        res += "-";
        value = -value;
    }
    if (value > 1e63 - 1) {
        return "TOO BIG ABS";
    }

    ll Head = (ll)value;
    res += to_string(Head);
    int point_cnt = 0;
    int precision = 3;
    double Point = (value - Head) * 10.0;
    int temp = (int)Point;

    while (point_cnt < precision && temp != 0) {
        if (point_cnt == 0) res += ".";
        res += to_string(temp);
        point_cnt++;
        Point = (Point - temp) * 10.0;
        temp = (int)Point;
    }

    return res;
}

template <typename T>
class MyStack {
   private:
    T *data;
    int max_size;
    int top_p;

   public:
    MyStack() : max_size(100), top_p(-1) { data = new T[max_size]; }

    ~MyStack() { delete[] data; }

    void push(const T &element) { data[++top_p] = element; }

    void pop() {
        if (top_p == -1) {
            printf("Stack Empty .pop() Error!\n");
            return;
        }
        top_p--;
    }

    T &top() {
        if (top_p == -1) {
            printf("Stack Empty .top() Error!\n");
            return data[0];
        }
        return data[top_p];
    }

    bool empty() { return top_p == -1; }

    int size() { return top_p + 1; }
};

char pre_arr[9][9] = {
    {'>', '>', '<', '<', '<', '<', '>', '>', ' '},  // +
    {'>', '>', '<', '<', '<', '<', '>', '>', ' '},  // -
    {'>', '>', '>', '>', '<', '<', '>', '>', ' '},  // *
    {'>', '>', '>', '>', '<', '<', '>', '>', ' '},  // /
    {'>', '>', '>', '>', '<', '<', '>', '>', ' '},  // ^
    {'<', '<', '<', '<', '<', '<', '=', ' ', ' '},  // (
    {'>', '>', '>', '>', ' ', '>', '>', '>', ' '},  // )
    {'<', '<', '<', '<', '<', '<', ' ', '=', ' '},  // \n
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}   // invalid
};

/**
 * @brief Get the operator id for the given operator
 *
 * @param op The operator
 * @return The priority id of the operator
 */
int get_op_id(char op) {
    int id;
    switch (op) {
        case '+':
            id = 0;
            break;
        case '-':
            id = 1;
            break;
        case '*':
            id = 2;
            break;
        case '/':
            id = 3;
            break;
        case '^':
            id = 4;
            break;
        case '(':
            id = 5;
            break;
        case ')':
            id = 6;
            break;
        case '\n':
            id = 7;
            break;
        default:
            return 9;
    }
    return id;
}

/**
 * @brief Get the precedence of two operators
 *
 * @param op1 The first operator
 * @param op2 The second operator
 * @return The precedence of the two operators
 */

char precede(char op1, char op2) {
    return pre_arr[get_op_id(op1)][get_op_id(op2)];
}

/**
 * @brief Check if a character is an operator, a digit, or a dot
 *
 * @param ch The character to be checked
 * @param last The last character
 * @param con Flag to indicate if the last character was a digit
 * @return 1 if the character is an operator, 0 if it's a digit or a dot, and -1
 * if it's an invalid character
 */
int char_in(char ch, char last, bool con) {
    switch (ch) {
        case '+':
        case '-':
            if ((!con) && (last == '(')) {
                return 0;
            }
            return 1;
        case '(':
            if ((!con) && (last == ')' || isdigit(last) || last == '.')) {
                return -1;
            }
            return 1;
        case ')':
            if ((!con) && !(isdigit(last) || last == ')')) {
                return -1;
            }
            return 1;
        case '*':
        case '/':
        case '^':
        case '\n':
            return 1;
        default:
            return 0;
    }
}

/**
 * @brief Perform the operation on two numbers based on the given operator
 *
 * @param num1 The first number
 * @param num2 The second number
 * @param op The operator
 * @return The result of the operation
 */
double operate(double num1, double num2, char op) {
    double res;
    switch (op) {
        case '+':
            res = num1 + num2;
            break;
        case '-':
            res = num1 - num2;
            break;
        case '*':
            res = num1 * num2;
            break;
        case '/':
            res = num1 / num2;
            break;
        case '^':
            res = std::pow(num1, num2);
            break;
    }
    return res;
}

/**
 * @brief Evaluate the given mathematical expression
 *
 * @param ex The mathematical expression
 * @return The result of the evaluation, or "ERROR" for illegal input
 */
double evaluateExpression(string ex) {
    MyStack<char> ops;
    MyStack<double> ovs;
    ops.push('\n');

    if (ex[0] == '-' || ex[0] == '+') {
        ex.insert(0, "0");
    } else if (isdigit(ex[0])) {
        ex.insert(0, "0+");
    }
    ex += '\n';

    string buffer;
    char cur_c, last_c = '\n';
    bool con = false;

    int cur_id = 0;
    cur_c = ex[cur_id++];
    while ((cur_c != '\n') || (ops.top() != '\n')) {
        int is_char = char_in(cur_c, last_c, con);
        if (is_char == 1) {
            last_c = cur_c;
            if (!buffer.empty()) {
                ovs.push(std::stod(buffer));
            }
            buffer = "";

            switch (precede(ops.top(), cur_c)) {
                case ' ':
                    return nan("");
                case '<':
                    con = false;
                    ops.push(cur_c);
                    cur_c = ex[cur_id++];
                    break;
                case '=':
                    con = false;
                    ops.pop();
                    cur_c = ex[cur_id++];
                    break;
                case '>':
                    con = true;
                    char op = ops.top();
                    ops.pop();
                    if (ovs.size() < 2) {
                        return nan("");
                    }
                    double num1, num2;
                    num2 = ovs.top();
                    ovs.pop();
                    num1 = ovs.top();
                    ovs.pop();
                    ovs.push(operate(num1, num2, op));
                    break;
            }
        } else if (is_char == 0) {
            if (isdigit(cur_c) || (cur_c == '.')) {
                if (last_c == ')') {
                    return nan("");
                }

                last_c = cur_c;
                buffer += cur_c;
                cur_c = ex[cur_id++];
            } else if ((cur_c == '+') || (cur_c == '-')) {
                last_c = cur_c;
                buffer += cur_c;
                cur_c = ex[cur_id++];
            } else {
                return nan("");
            }
        } else {
            return nan("");
        }

        if (ops.empty()) {
            return nan("");
            ;
        }
    }

    if (ovs.empty()) {
        return nan("");
    }
    if (ovs.size() > 1) {
        return nan("");
    }

    double res = ovs.top();
    if (isnan(res) || std::isinf(res)) {
        return nan("");
    }
    return ovs.top();
}

/**
 * @brief Get the result of the given expression
 *
 * Like "11+2^(2+1)*2-6/(3-1)"
 *
 * @param expression The expression to be evaluated
 * @return The result of the expression, or "ERROR" for illegal input
 */
string getExRes(string expression) {
    double res = evaluateExpression(expression);
    if (isnan(res)) {
        return "ERROR";
    }
    return double2String(res);
}

class EqCoe {
   public:
    double a;
    double b;
    double c;

    EqCoe(double a, double b, double c) : a(a), b(b), c(c) {}
};

EqCoe getCoe(string eq_exp, const string &sym) {
    string qu = sym + "^2";
    double a = 0.0, b = 0.0, c = 0.0;
    if (findSubstring(eq_exp, qu) != -1) {
        int pos = findSubstring(eq_exp, qu);
        string coe;
        if (pos == 0 || (pos == 1 && (eq_exp[0] == '+' || eq_exp[0] == '-'))) {
            coe += eq_exp.substr(0, pos) + "1";
        } else {
            coe = eq_exp.substr(0, pos);
        }
        a = evaluateExpression(coe);
        eq_exp.erase(0, pos + 3);
    }
    if (findSubstring(eq_exp, sym) != -1) {
        int pos = findSubstring(eq_exp, sym);
        string coe;
        if (pos == 0 || (pos == 1 && (eq_exp[0] == '+' || eq_exp[0] == '-'))) {
            coe += eq_exp.substr(0, pos) + "1";
        } else {
            coe = eq_exp.substr(0, pos);
        }
        b = evaluateExpression(coe);
        eq_exp.erase(0, pos + 1);
    }
    if (!eq_exp.empty()) {
        c = evaluateExpression(eq_exp);
    }
    return EqCoe(a, b, c);
}

/**
 * @brief Solve the given quadratic equation
 *
 * @param eq The quadratic equation
 * @return The pair of solutions, (res, nan) for linear, (nan, 0) for no
 * solution, or (nan, nan) for invalid input
 */
pdd solveQuadraticEquation(string eq) {
    pdd null_res = {nan(""), nan("")};
    pdd res;
    int pos_a = find_ch(eq, '=');
    int pos_b = find_ch_rev(eq, '=');
    if (pos_a == -1 || pos_a != pos_b) {
        return null_res;
    }
    int x_pp = find_ch(eq, 'x');
    int y_pp = find_ch(eq, 'y');
    if ((x_pp == -1) == (y_pp == -1)) {
        return null_res;
    }

    string sym = (x_pp == -1 ? "y" : "x");

    string le = eq.substr(0, pos_a);
    string ri = eq.substr(pos_a + 1, eq.size() - (pos_a + 1));
    EqCoe coe_pos = getCoe(le, sym);
    EqCoe coe_neg = getCoe(ri, sym);

    double a = coe_pos.a - coe_neg.a;
    double b = coe_pos.b - coe_neg.b;
    double c = coe_pos.c - coe_neg.c;

    /*
     *  invalid
     */
    if (isnan(a) || isnan(b) || isnan(c)) {
        return null_res;
    }
    if (a == 0 && b == 0) {
        return null_res;
    }
    /*
     *  linear
     */
    if (a == 0) {
        return {-c / b, nan("")};
    }
    /*
     * Quartic;
     */
    double delta = (b * b - 4 * a * c);
    if (delta < 0) {
        return {nan(""), 0};
    } else {
        double base = (-b) / (2 * a);
        double diff = sqrt(delta) / (2 * a);
        return {base + diff, base - diff};
    }
}

/**
 * @brief Get the result of the given equation
 *
 * The equation must like "ax^2+bx+c=0" for both side ('a' can be zero),
 * Symbol only can be either 'x' or 'y' Like "(2^2-4)y^2+y+4=0", "x^2+x+4=0",
 * "-x-200/2=0"
 *
 * @param equation The equation to be solved
 * @return The double float-point number string result of the equation, or
 * "ERROR" for illegal input
 */

string getEqRes(string equation) {
    pdd res = solveQuadraticEquation(equation);
    if (isnan(res.first) && isnan(res.second)) {
        return "ERROR";
    }
    if (isnan(res.first) && res.second == 0) {
        return "No Solution In R";
    }
    int x_pp = find_ch(equation, 'x');
    int y_pp = find_ch(equation, 'y');
    string sym_eq = (x_pp == -1 ? "y=" : "x=");
    string rtn = sym_eq + double2String(res.first);
    if (isnan(res.second)) {
        return rtn;
    }
    return rtn + " || " + sym_eq + double2String(res.second);
}

/**
 * @brief Check if the given character is an operator for binary expression
 *
 * @param ch The character to be checked
 * @return True if the character is an operator, false otherwise
 */
inline bool is_op(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' ||
            ch == '(' || ch == ')');
}

/**
 * @brief A function to calculate binary expression;
 *
 * Like "10001001/10100+1001"
 *
 * @param binaryExperssion A binary expression
 * @return The decimal double float-point number string result of the
 * expression, "ERROR" for illegal input
 */
string getBinRes(string binaryExperssion) {
    string dec, binBuff;
    for (char ch : binaryExperssion) {
        if (ch == '1' || ch == '0') {
            binBuff += ch;
        } else if (is_op(ch)) {
            if (!binBuff.empty()) {
                dec += to_string(binary2Decimal(binBuff));
                binBuff = "";
            }
            dec += ch;
        } else {
            return "ERROR";
        }
    }
    if (!binBuff.empty()) {
        dec += to_string(binary2Decimal(binBuff));
        binBuff = "";
    }
    return getExRes(dec);
}

#ifdef __CALC_DEMO_DEBUG

#include <iostream>
#include <vector>

using vs = std::vector<string>;

int main() {
    std::cout << "Expressions" << std::endl;
    vs exs;
    exs.emplace_back("11+2^(2+1)*2-6/(3-1)");
    exs.emplace_back("-1*7-9^2");
    exs.emplace_back("-3*(2*5)+2^4");
    exs.emplace_back("2*256");   // too large
    exs.emplace_back("(-3)^2");  // format error
    for (string ex : exs) {
        std::cout << ex << " = " << getExRes(ex) << std::endl;
    }

    std::cout << "\nEquations" << std::endl;
    vs eqs;
    eqs.emplace_back("-x-200/2=0");
    eqs.emplace_back("(2^2-4)y^2+y+4=0");
    eqs.emplace_back("4y^2+4y-12=0");
    eqs.emplace_back("-x^2+3x-2=0");
    eqs.emplace_back("-x-200/2=2");
    eqs.emplace_back("y^2=4");
    eqs.emplace_back("y^2-3y=-2");
    eqs.emplace_back("(2^2)y^2+4x-12=0");  // x&y mix error
    for (string eq : eqs) {
        std::cout << eq << ": " << getEqRes(eq) << std::endl;
    }

    std::cout << "\nBinary" << std::endl;
    vs bins;
    bins.emplace_back("1001001/11+1001");
    bins.emplace_back("10001/11*1001010--");  // format error
    bins.emplace_back("10001/11*1001  1");    // format error
    for (string bin : bins) {
        std::cout << bin << " = " << getBinRes(bin) << std::endl;
    }
}

#endif
