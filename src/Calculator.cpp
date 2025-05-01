#include <string>
#include <stack>
#include <tuple>
#include <utility>

class Solution {
private:
    bool isdigit(char c) {
        return (c >= '0' && c <= '9');
    }

public:
    int calculate(std::string s) {
        int eval = 0;

        // LValue, operation, paren level
        std::stack<std::tuple<int, char, int>> operations;
        std::stack<int> negateparens;
        int nparens = 0;

        char lastc = '\0';
        std::string digits;
        int lastval;
        bool negate = false;
        for (char c : s) {
            // if whitespace, skip
            if (c == ' ') continue;
            // if number store digits until we hit something else
            if (isdigit(c)) {
                digits += c;
            } else if (isdigit(lastc)) {
                lastval = atoi(digits.c_str());
                if (negate) {
                    lastval *= -1;
                    negate = false;
                }
                digits.clear();
                if (!operations.empty() && std::get<2>(operations.top()) == nparens) {
                    int val = std::get<0>(operations.top());
                    char op = std::get<1>(operations.top());
                    if (op == '+')
                        lastval = val + lastval;
                    else if (op == '-')
                        lastval = val - lastval;
                    operations.pop();
                }
            }
            
            // if (, note new nesting level
            if (c == '(') {
                nparens++;
                if (negate) {
                    negate = false;
                    negateparens.push(nparens);
                }
            }
            // if ), close last nesting level and use value for next operator
            else if (c == ')') {
                // evaluate current paren level, store as lastval
                if (!negateparens.empty() && negateparens.top() == nparens) {
                    lastval *= -1;
                    negateparens.pop();
                }
                nparens--;
                if (!operations.empty() && std::get<2>(operations.top()) == nparens) {
                    int val = std::get<0>(operations.top());
                    char op = std::get<1>(operations.top());
                    if (op == '+')
                        lastval = val + lastval;
                    else if (op == '-')
                        lastval = val - lastval;
                    operations.pop();
                }
            }
            // if + store as operation with last value
            else if (c == '+') {
                operations.push(std::make_tuple(lastval, c, nparens));
            }
            // if -, depends if we have a pending operation
            //    if last read was not a digit or ), then negate next
            //    otherwise just store as an operation like +
            else if (c == '-') {
                if (isdigit(lastc) || lastc == ')') {
                    operations.push(std::make_tuple(lastval, c, nparens));
                } else {
                    negate = true;
                }
            }

            lastc = c;
        }

        // cleanup because we won't evaluate final character by default above (if its a digit)
        if (isdigit(lastc)) {
            lastval = atoi(digits.c_str());
            if (negate) {
                lastval *= -1;
                negate = false;
            }
        }
        if (!operations.empty() && std::get<2>(operations.top()) == nparens) {
            int val = std::get<0>(operations.top());
            char op = std::get<1>(operations.top());
            if (op == '+')
                lastval = val + lastval;
            else if (op == '-')
                lastval = val - lastval;
            operations.pop();
        }

        return lastval;
    }
};