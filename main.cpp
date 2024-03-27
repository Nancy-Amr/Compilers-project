#include <iostream>
#include <string>
#include <cctype>
#include <regex>
#include <unordered_set>
#include <fstream>

using namespace std;

unordered_set<string> keywords = {"default","if", "else", "while", "for", "return","auto","break"
        ,"case","const","continue","do","double","else", "enum","extern", "for", "goto",
                                  "if","inline","nullptr","register","restrict","return","sizeof", "static", "struct", "switch","typedef","union",
                                  "void","volatile","while","struct" /* ... and more keywords */};
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

bool isOperator(const string& token) {
    regex op("(?:\\+|\\-|\\*|\\|\\/|\\=|\\==|\\!=|\\>|\\>=|\\<|\\<=|\\&\\&|\\|\\||\\!|\\&|\\||\\^|\\~|\\<\\<|\\>\\>|\\+\\+|\\-\\-|\\+=|\\-=|\\=|\\/=|\\%=|\\&=|\\|=|\\^=|\\<\\<=|\\>\\>=|\\-\\>|\\.|\\-\\>|\\.\\|\\-\\>\\|\\+|\\-|\\|\\%|\\/|\\&|\\||\\^|\\<|\\>|\\=|\\(|\\)|\\[|\\]|\\{|\\}|\\,|\\;|\\.|\\:\\:|\\?\\:|\\.\\.\\.|<%|<:|<::|%:|%:%:|new|delete|new\\[\\]|\\[\\]|\\(.\\)|\\<\\<\\=|\\>\\>\\=)");
    return regex_match(token, op);
}


void lexAnalyze(const string& code) {
    int i = 0;
    string currentToken;
    int line = 1;

    while (i < code.length()) {
        char c = code[i];
//        if (c == '\n') {
//            ++line;
//        }

        if (isspace(c)) {
            i++;
            continue;
        }

        if (c == '#') {
            // skip macro, because we will not support it
            while (code[i] != 0 && code[i] != '\n') {
                i++;
            }
            continue;
        }

        if (isalpha(c) || c == '_') {
            // Handle identifiers and keywords
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
        } else if (isOperator(string(1, c))) {
            // Handle operators
            currentToken += c;
            i++;
            while (i < code.length() && isOperator(currentToken + code[i])) {
                currentToken += code[i];
                i++;
            }

            cout << "Operator: " << currentToken << endl;
            currentToken.clear();
        } else if (isdigit(c) || c == '+' || c == '-') {
            // Handle numbers
            currentToken += c;
            i++;
            while (i < code.length() && (isdigit(code[i]) || code[i] == '.' || code[i] == 'e' || code[i] == 'E' || (i > 0 && (code[i - 1] == 'e' || code[i - 1] == 'E') && (code[i] == '-' || code[i] == '+')))) {
                currentToken += code[i];
                i++;
            }

            if (isValidNumber(currentToken)) {
                cout << "Number: " << currentToken << endl;
            }
            else {
                cout << "Unrecognized token: " << currentToken << endl;
            }
            currentToken.clear();
        } else if (c == '\'' || c == '\"') {
            // Handle string literals
            char quote = c;
            cout<< "punctuation: "<<quote<<endl;
            i++;
            while (i < code.length() && code[i] != quote) {

                currentToken += code[i];
                i++;
            }



            if (i < code.length() && code[i] == quote) {

                i++;
            }
            if (c == '\'' ){
                cout << "Character: " << currentToken << endl;}
            else if( c == '\"'){
                cout << "String: " << currentToken << endl;
            }

            cout<< "punctuation: "<<quote<<endl;
            currentToken.clear();
        } else {
            cout << "Unrecognized token: " << c << endl;
            i++;
        }
    }

}


string removeComments(const string& code) {
    regex singleLineComment(R"(/\/.*)");  // Matches single-line comments (//...)
    regex multiLineComment(R"(\/\.?\/)"); // Matches multi-line comments (/ ... */)
    string cleanCode = regex_replace(code, singleLineComment, "");
    cleanCode = regex_replace(cleanCode, multiLineComment, "");
    return cleanCode;
}


// void read_file(std::string filepath, std::string& buffer) {
//  std::ifstream file(filepath);
//  std::string line;
//  while (getline(file, line)) {
//  buffer.push_back('\n');
//  buffer += line;
//  }
//  }

string readFromFile(string filepath) {
    ifstream inputFile;
    string code = " ", line;
    inputFile.open(filepath, ios::in);
    while (inputFile.good()) {
        getline(inputFile, line);
        if (inputFile.eof())
            break;
        code += line;
        code += '\n';
    }
    return code;
}

int main() {
    string filepath;

    cout << "Enter file path: ";
    cin >> filepath;
    // Read the entire file content into the 'code' string
    string code = readFromFile(filepath);

    // Check if there was an error reading the file
    if (code.empty()) {
        std::cerr << "Error: Could not read file '" << filepath << "'" << std::endl;
        return 1;
    }

    // Print the code without comments
    std::cout << "Code without comments::" << std::endl;
    std::string cleanCode = removeComments(code);
    std::cout << cleanCode << std::endl;

    // Perform lexical analysis on the clean code
    std::cout << "Tokens:: " << std::endl;
    lexAnalyze(cleanCode);

    return 0;
}
