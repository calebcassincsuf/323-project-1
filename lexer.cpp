#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdexcept>

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

//Returns whether a character is a string
bool isString(string str) {
    return str.length() > 1 && ((str[0] == '"' && str[str.length()-1] == '"') || (str[0] == '\'' && str[str.length()-1] == '\''));
}

//returns whether a string is a number with a decimal point
bool isDecimal(string dec) {
    string::const_iterator it = dec.begin();
    while (it != dec.end() && isdigit(*it))
        ++it;
    if(*it == '.') {
        ++it;
        while (it != dec.end() && isdigit(*it))
            ++it;
        return !dec.empty() && it == dec.end();
    }
    return false;
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

bool isIdentifier(string id) {
    string::const_iterator it = id.begin();
    while (it != id.end() && (isdigit(*it) || isalpha(*it) || *it == '_'))
        ++it;
    return !id.empty() && it == id.end() && !isdigit(id[0]);
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
    else if (isDecimal(str))
    {
        cout << "Decimal: " << str << "\n";
    }
    else if (isKeyword(str))
    {
        cout << "Keyword: " << str << "\n";
    }
    else if (isString(str))
    {
        cout << "String: " << str << "\n";
    }
    else if (isIdentifier(str))
    {
        cout << "Identifier: " << str << "\n";
    } else {
        cout << "Invalid lexeme: " << str << "\n";
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
            //logic to handle 2 exceptions to something being a delmiter. At the moment this is used to detect comments and prevent them from being displayed.
            if(right-left == 1 && str[left] != '/') {
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
                if(str[left+1] == '/') {
                    left = right;
                    while (right < length && str[right] != '\n')
                    {
                        right++;
                        left++;
                    }
                } else if (str[left+1] == '*') {
                    left = right;
                    while (right < length && !(str[right] == '/' && str[right-1] == '*'))
                    {
                        right++;
                        left++;
                    }
                    right++;
                    left++;
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
    std::ifstream inputFile("files/input.txt");
    string input;
    //getline(inputFile, input);
    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) { // Read each line from the file
            input += line + '\n'; // Append each line to the input variable
            cout << line << '\n';
        }
        cout << input <<'\n';
        inputFile.close(); // Close the input file
    }
    else {
        std::cerr << "Unable to open file" << std::endl;
        return 1; // Exit with error if file opening failed
    }
    //std::string input = "int alpha >= x+(14/25.0); /* does this show up? */ alpha = \"hi\";";

    lexer(input);

    return 0;
}
