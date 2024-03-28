#include <iostream>
#include <string>
#include <cstring>
using namespace std;

// Checks if a given character ends a string/lexeme. Does not provide information on which token it is.
bool isEndString(char sym)
{
    return sym == '<' || sym == '>' || sym == '+' || sym == '=' || sym == '-' || sym == '*' || sym == '/' || sym == '(' || sym == ')' || sym == '[' || sym == ']' || sym == '{' || sym == '}' || sym == ';' || isspace(sym);
}

// Returns whether a character is an operator.
bool isOperator(string op)
{
    if (op.length() == 1)
    {
        return op[0] == '+' || op[0] == '-' || op[0] == '/' || op[0] == '*' || op[0] == '=' || op[0] == '<' || op[0] == '>';
    }
    return op.compare("<=") == 0 || op.compare(">=") == 0;
}

// Returns whether a character is a separator.
bool isSeparator(string sep)
{
    if (sep.length() == 1)
    {
        return sep[0] == ';' || sep[0] == '{' || sep[0] == '}' || sep[0] == '(' || sep[0] == ')' || sep[0] == '[' || sep[0] == ']';
    }
    return false;
}

// Returns whether a string is an integer.
bool isInteger(string lex)
{
    string::const_iterator it = lex.begin();
    while (it != lex.end() && isdigit(*it))
        ++it;
    return !lex.empty() && it == lex.end();
}

// Returns whether a string is a keyword.
bool isKeyword(string key)
{
    string keywords[5] = {"if", "for", "bool", "int", "while"};
    for (int i = 0; i < 5; i++)
    {
        if (key.compare(keywords[i]) == 0)
        {

            return true;
        }
    }
    return false;
}

// Takes in a lexeme and prints an output of which token it is.
void outFromStr(std::string str)
{

    if (isOperator(str))
    {
        cout << "Operator: " << str << "\n";
    }
    else if (isSeparator(str))
    {
        cout << "Separator: " << str << "\n";
    }
    else if (isInteger(str))
    {
        cout << "Integer: " << str << "\n";
    }
    else if (isKeyword(str))
    {
        cout << "Keyword: " << str << "\n";
    }
    else
    {
        cout << "Identifier: " << str << "\n";
    }
}

// The primary loop of the lexical analyzer.
void lexer(std::string str)
{
    // right and left variables keep track of the positions of the currently tracked string.
    int right = 1, left = 0;
    int length = str.size();
    while (right <= length)
    {
        // If the very start of the string is a delimiter it will immediately generate an output from that.
        if (isEndString(str[left]))
        {
            //logic to handle 2 character operators. In the future this should be standarized to use functions. This checks to see if the first
            //character is used in a two-character operator. If it is then it waits for another character before running.
            if(right-left == 1 && str[left] != '<' && str[left] != '>') {
                outFromStr(str.substr(left, 1));

                // Reset the leftmost position and clear any excess whitespace.
                left = right;
                while (right < length && isspace(str[right]))
                {
                    right++;
                    left++;
                }
            } else if(right-left == 2)
            {
                //checks if the held string is a double operator. If it is then it is pushed as a single lexeme, if not then they are separated.
                if(str[left+1] == '=') {
                    outFromStr(str.substr(left,2));
                }
                else{
                    outFromStr(str.substr(left,1));
                    right--;
                }
                left = right;
                while (right < length && isspace(str[right]))
                {
                    right++;
                    left++;
                }
            } 
        } // If at the final character of the string force a token analysis.
        else if (right == length)
        {
            outFromStr(str.substr(left, right - left));
        } // If the rightmost char contains a delimiter then another lexeme has been identified and the token must be determined.
        else if (isEndString(str[right]))
        {
            outFromStr(str.substr(left, right - left));

            // Reset the leftmost position and clear any excess whitespace.
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
    std::string input = "int alpha >= x+(14/25);";
    lexer(input);

    return 0;
}