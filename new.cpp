#include <iostream>
#include <string>
#include <cctype>
#include <regex>
#include <unordered_set>

using namespace std;

unordered_set<string> keywords = {"default","if", "else", "while", "for", "return","auto","break"
,"case","const","continue","do","double","else", "enum","extern", "for", "goto",
"if","inline","nullptr","register","restrict","return","sizeof", "static", "struct", "switch","typedef","union",
"void","volatile","while" /* ... and more keywords */};
unordered_set<string> dataTypes = {"int", "char", "float", "double","long","signed","unsigned",}; // Data types

bool isValidNumber(const string& number) {
    regex no(R"((\+|-)?\d+(\.\d+)?([Ee](\+|-)?\d+)?)");
    return regex_match(number, no);
}

bool isValidIdentifierChar(char c) {
    return isalnum(c) || c == '_'; // isalnum checks alphanumeric and underscore
}

bool isKeyword(const string& token) {
    return keywords.count(token) > 0;
}

bool isDataType(const string& token) {
    return dataTypes.count(token) > 0;
}

void lexAnalyze(const string& code) {
    int i = 0;
    string currentToken;

    while (i < code.length()) {
        char c = code[i];

        if (isspace(c)) {
            i++;
            continue;
        }

        if (isalpha(c) || c == '_') {
            if (isdigit(c)) {
                cout << "Identifier: " << c;
                i++;
                while (i < code.length() && isValidIdentifierChar(code[i])) {
                    cout << code[i];
                    i++;
                }
                cout << endl;
            } else {
                currentToken += c;
                i++;
                while (i < code.length() && isValidIdentifierChar(code[i])) {
                    currentToken += code[i];
                    i++;
                }

                if (isKeyword(currentToken)) {
                    cout << "Keyword: " << currentToken << endl;
                } else if (isDataType(currentToken)) {
                    cout << "Data Type: " << currentToken << endl;
                } else {
                    cout << "Identifier: " << currentToken << endl; // Other identifiers
                }
                currentToken.clear();
            }
        } else if (isdigit(c) || c == '+' || c == '-') {
            currentToken += c;
            i++;
            while (i < code.length() && (isdigit(code[i]) || code[i] == '.' || code[i] == 'e' || code[i] == 'E' || (i > 0 && (code[i - 1] == 'e' || code[i - 1] == 'E') && (code[i] == '-' || code[i] == '+')))) {
                currentToken += code[i];
                i++;
            }

            if (isValidNumber(currentToken)) {
                cout << "Number: " << currentToken << endl;
            } else {
                cout << "Unrecognized token: " << currentToken << endl;
            }
            currentToken.clear();
        } else {
            cout << "Unrecognized token: " << c << endl;
            i++;
        }
    }
}

string removeComments(const string& code) {
    regex singleLineComment(R"(/\/.*)");  // Matches single-line comments (//...)
    regex multiLineComment(R"(\/\*.*?\*/)"); // Matches multi-line comments (/* ... */)
    string cleanCode = regex_replace(code, singleLineComment, "");
    cleanCode = regex_replace(cleanCode, multiLineComment, "");
    return cleanCode;
}

int main() {

    string code = "int x = 10;" // This is a comment\n
                    "float y = 3.14;" /* This is another comment */
                    "char default initial = 'A'";
    cout << "Code without comments::" << endl;
    string cleanCode = removeComments(code);
    cout << cleanCode << endl;
    cout << "Symbol table:: " << endl;
    lexAnalyze(code);
    return 0;
}
