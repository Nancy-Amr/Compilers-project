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

int isOperator(const string& token) {

    regex Unary("\\+\\+|\\-\\-|\\!|\\-\\>"); //1
    regex arithmetic("\\+|\\-|\\*|\\/|\\%");//2
    regex assignment("\\=|\\+\\=|\\-\\=|\\*\\=|\\/\\=|\\%\\=|\\&\\=|\\|\\=|\\^\\=");//4
    regex relational("\\=\\=|\\!\\=|\\>\\=|\\<\\=|\\>|\\<");//3
    regex logical("\\&\\&|\\|\\|");//5
    regex bitwise("\\&|\\||\\^|\\~|\\<\\<|\\>\\>");//6
    regex random("\\?|\\:|\\.");//7
    regex punctuation("\\,|\\;|\\[|\\]|\\(|\\)|\\{|\\}");//8

    if (regex_match(token, Unary)) {
        return 1;
    } else if (regex_match(token, arithmetic)) {
        return 2;
    } else if (regex_match(token, relational)) {
        return 3;
    } else if (regex_match(token, assignment)) {
        return 4;
    } else if (regex_match(token, logical)) {
        return 5;
    } else if (regex_match(token, bitwise)) {
        return 6;
    } else if (regex_match(token, random)) {
        return 7;
    }else if (regex_match(token, punctuation)) {
        return 8;
    }else {
        return 0;
    }
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

        if (c == '#') {
            // skip macro, because we will not support it
            while (code[i] != 0 && code[i] != '\n') {
                i++;
            }
            continue;
        }

        if (isalpha(c) || c == '_') {
        // Handle identifiers and keywords
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

        }else if(c=='0' && (code[i+1] == 'x' || code[i+1] == 'X')){
			currentToken += c;
			i++;
			currentToken += code[i];
			i++;
			while (i < code.length() && ((code[i] >= '0' && code[i] <= '9') || (code[i] >= 'A' && code[i] <= 'F') )) {
				currentToken += code[i];
				i++;
			}
			bool hasMoreChars = false;
			while (i < code.length() &&   isalnum(code[i])) {
				// If there are more digits, mark the flag and continue
				hasMoreChars = true;
				currentToken += code[i];
				i++;
			}

			// If there are no more digits after the octal number,
			// then currentToken represents an octal number
			if (!hasMoreChars) {
				cout << "Hexadecimal Number: " << currentToken << endl;
			} else {
				// Otherwise, it's an unrecognized token
				cout << "Unrecognized token: " << currentToken << endl;
			}
			currentToken.clear();
		}else if (isOperator(string(1, c))) {
            // Handle operators
            currentToken += c;
            i++;
            while (i < code.length() && isOperator(currentToken + code[i])) {
                currentToken += code[i];
                i++;
            }
            if(isOperator(string(1, c))==8)
            {
                cout<<"Punctuation: "<<currentToken<<endl;
                currentToken.clear();
            }
            else
            {
                cout << "Operator: " << currentToken << endl;
                currentToken.clear();

            }


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
        }  else if (c == '\'' || c == '\"') {
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
    regex singleLineComment(R"(\/\/[^\n]*)"); // Matches single-line comments (//...)
    regex multiLineComment(R"(\/\*[^*]*\*+([^/*][^*]*\*+)*\/)");
    string cleanCode = regex_replace(code, multiLineComment, "");
    cleanCode = regex_replace(cleanCode, singleLineComment, "");
    return cleanCode;
}

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
