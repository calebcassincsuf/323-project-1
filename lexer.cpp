#include <iostream>
#include <string>
using namespace std;

//Checks if a given character ends a string/lexeme. Does not provide information on which token it is.
bool isEndString(char sym)
{
    return sym == '+' || sym == '=' || sym == '-' || sym == '*' || sym == '/' || sym == '(' || sym == ')' || sym == '[' || sym == ']' || sym == '{' || sym == '}' || sym == ';' || isspace(sym);
}

//Returns whether a character is an operator.
bool isOperator(char op)
{
    return op == '+' || op == '-' || op == '/' || op == '*' || op == '=' || op == '<' || op == '>';
}

//Returns whether a character is a separator.
bool isSeparator(char sep)
{
    return sep == ';' || sep == '{' || sep == '}' || sep == '(' || sep == ')' || sep == '[' || sep == ']';
}

//Returns whether a string is an integer.
bool isInteger(string lex) {
    string::const_iterator it = lex.begin();
    while (it != lex.end() && isdigit(*it)) ++it;
    return !lex.empty() && it == lex.end();
}

//Returns whether a string is a keyword.
bool isKeyword(string key) {
    string keywords[5] = {"if", "for", "bool", "int", "while"};
    for(int i = 0; i < 5; i++) {
        if(key.compare(keywords[i]) == 0) {
            
            return true;
        }
    }
    return false;
}

//Takes in a lexeme and prints an output of which token it is. 
void outFromStr(std::string str)
{
    if (str.size() == 1)
    {
        if (isOperator(str[0]))
        {
            cout << "Operator: " << str[0] << "\n";
        }
        else if (isSeparator(str[0]))
        {
            cout << "Separator: " << str[0] << "\n";
        }
        else
        {
            cout << "Identifier: " << str[0] << "\n";
        }
    } 
    else if (isInteger(str)) {
        cout << "Integer: " << str << "\n";
    }
    else if (isKeyword(str)) {
        cout << "Keyword: " << str << "\n";
    }
    else
    {
        cout << "Identifier: " << str << "\n";
    }
}

//The primary loop of the lexical analyzer.
void lexer(std::string str)
{
    //right and left variables keep track of the positions of the currently tracked string.
    int right = 1, left = 0;
    int length = str.size();
    while (right <= length)
    {
        //If the very start of the string is a delimiter it will immediately generate an output from that.
        if (isEndString(str[left]))
        {
            outFromStr(str.substr(left,1));

            //Reset the leftmost position and clear any excess whitespace.
            left = right;
            while (right < length && isspace(str[right]))
            {
                right++;
                left++;
            }
        } //If at the final character of the string force a token analysis.
        else if(right == length) {
            outFromStr(str.substr(left,right-left));
        }//If the rightmost char contains a delimiter then another lexeme has been identified and the token must be determined. 
        else if (isEndString(str[right]))
        {
            outFromStr(str.substr(left, right - left));

            //Reset the leftmost position and clear any excess whitespace.
            left = right;
            while (right < length && isspace(str[right]))
            {
                right++;
                left++;
            }
        }
        right++;
    }
}

int main()
{
    std::string input = "int alpha = x+(14/25);";
    lexer(input);

    return 0;
}