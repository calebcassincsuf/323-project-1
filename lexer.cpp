#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;
// Arrays containing all values of operators, separators, and keywords.
char operators[15] = {'<', '>', '+', '=', '!', '&', '*', '-', '/', '%', '|', '^', '~', '?', ':'};
char separators[7] = {'[', ']', '{', '}', '(', ')', ';'};
string keywords[45] = {"auto", "bool", "break", "case", "class", "catch", "char", "const", "continue", "default", "delete", "do", "double", "else", "enum", "explicit", "export", "false", "float", "for", "if", "int", "long", "mutable", "namespace", "new"
                                                                                                                                                                                                                                                        "nullptr",
                       "private", "protected", "public", "register", "requires", "return", "short", "signed", "static", "struct", "switch", "template", "this"
                                                                                                                                                        "throw",
                       "true", "try", "unsigned", "using", "void", "while"};

// Checks if a given character ends a string/lexeme. Does not provide information on which token it is.
bool isEndString(char sym)
{
    return sym == '<' || sym == '>' || sym == '+' || sym == '=' || sym == '-' || sym == '*' || sym == '/' || sym == '(' || sym == ')' || sym == '[' || sym == ']' || sym == '{' || sym == '}' || sym == ';' || isspace(sym);
}

// Returns whether a character is an operator.
bool isOperator(string op)
{
    for (int i = 0; i < sizeof(operators) / sizeof(char); i++)
    {
        if (op[0] == operators[i])
        {

            return true;
        }
    }
    return false;
}

// Returns whether a character is a string
bool isString(string str)
{
    return str.length() > 1 && ((str[0] == '"' && str[str.length() - 1] == '"') || (str[0] == '\'' && str[str.length() - 1] == '\''));
}

// returns whether a string is a number with a decimal point
bool isDecimal(string dec)
{
    string::const_iterator it = dec.begin();
    while (it != dec.end() && isdigit(*it))
        ++it;
    if (*it == '.')
    {
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
    for (int i = 0; i < sizeof(separators) / sizeof(char); i++)
    {
        if (sep[0] == separators[i])
        {

            return true;
        }
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
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        if (key.compare(keywords[i]) == 0)
        {

            return true;
        }
    }
    return false;
}

//Checks the validity of any string to see if it is a valid identifier.
bool isIdentifier(string id)
{
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
        cout << "\"" << str << "\" = operator\n";
    }
    else if (isSeparator(str))
    {
        cout << "\"" << str << "\" = separator\n";
    }
    else if (isInteger(str))
    {
        cout << "\"" << str << "\" = integer\n";
    }
    else if (isDecimal(str))
    {
        cout << "\"" << str << "\" = decimal\n";
    }
    else if (isKeyword(str))
    {
        cout << "\"" << str << "\" = keyword\n";
    }
    else if (isString(str))
    {
        cout << "\"" << str << "\" = string\n";
    }
    else if (isIdentifier(str))
    {
        cout << "\"" << str << "\" = identifier\n";
    }
    else
    {
        cout << "\"" << str << "\" = unknown\n";
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
            // logic to handle 2 exceptions to something being a delmiter. At the moment this is used to detect comments and prevent them from being displayed.
            if (right - left == 1 && str[left] != '/')
            {
                outFromStr(str.substr(left, 1));

                // Reset the leftmost position and clear any excess whitespace.
                left = right;
                while (right < length && isspace(str[right]))
                {
                    right++;
                    left++;
                }
            }
            else if (right - left == 2)
            {
                // checks if the held string is a double operator. If it is then it is pushed as a single lexeme, if not then they are separated.
                if (str[left + 1] == '/')
                {
                    left = right;
                    while (right < length && str[right] != '\n')
                    {
                        right++;
                        left++;
                    }
                }
                else if (str[left + 1] == '*')
                {
                    left = right;
                    while (right < length && !(str[right] == '/' && str[right - 1] == '*'))
                    {
                        right++;
                        left++;
                    }
                    right++;
                    left++;
                }
                else
                {
                    outFromStr(str.substr(left, 1));
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
    string fileName = "input.txt";
    cout << "Please enter the file you wish to compile:" << "\n";
    cin >> fileName;
    std::ifstream inputFile(fileName);
    string input;
    // getline(inputFile, input);
    if (inputFile.is_open())
    {
        std::string line;
        while (std::getline(inputFile, line))
        {                         // Read each line from the file
            input += line + '\n'; // Append each line to the input variable
        }
        inputFile.close(); // Close the input file
    }
    else
    {
        std::cerr << "Unable to open file" << std::endl;
        return 1; // Exit with error if file opening failed
    }

    lexer(input);

    return 0;
}
