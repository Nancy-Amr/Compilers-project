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

typedef std::vector<std::pair<std::string, std::vector<int>>> symbol_table_t;
symbol_table_t symbol_table;
int add_to_symbol_table(std::string token_name, int offset) {

    int index = 0;
    for (auto& symbol : symbol_table) {
        if (token_name == symbol.first) {
            symbol.second.push_back(offset);
            return index;
        }
        index++;
    }
    symbol_table.push_back({ token_name, { offset } });
    return index;

}



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
    regex other("\\?|\\:|\\.");//7
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
    } else if (regex_match(token, other)) {
        return 7;
    } else if (regex_match(token, punctuation)) {
        return 8;
    } else {
        return 0;
    }
}


void lexAnalyze(const string& code) {
    int i = 0;
    string currentToken;
    int counter=0;

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
                counter++;

            } else if (isDataType(currentToken)) {
                cout << "Data Type: " << currentToken << endl;
                counter++;

            }


            else {
                int index=add_to_symbol_table(currentToken, counter);

                cout << "Identifier: " << currentToken << endl; // Other identifiers
                counter++;
            }

            currentToken.clear();

        } else if (c == '0' && isdigit(code[i+1]) ) {
			currentToken += c;
			i++;
			while (i < code.length() && (code[i] >= '0' && code[i] <= '7')) {
				currentToken += code[i];
				i++;
			}

			// Check if there are any more digits after the octal number
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
				cout << "Octal Number: " << currentToken << endl;
				counter++;
			} else {
				// Otherwise, it's an unrecognized token
				cout << "Unrecognized token: " << currentToken << endl;
				counter++;
			}
			currentToken.clear();
		}
		else if( c == '0' && (code[i+1] == 'b' || code[i+1] == 'B')){
			currentToken += c;
			i++;
			currentToken += code[i];
			i++;
			while (i < code.length() && (code[i] == '0' || code[i] == '1')) {
				currentToken += code[i];
				i++;
			}
			bool hasMoreChars = false;
			while (i < code.length() &&  isalnum(code[i])) {
				// If there are more digits, mark the flag and continue
				hasMoreChars = true;
				currentToken += code[i];
				i++;
			}
			if(currentToken == "0b" || currentToken == "0B"){
				hasMoreChars = true;
			}

			// If there are no more digits after the octal number,
			// then currentToken represents an octal number
			if (!hasMoreChars) {
				cout << "Binary Number: " << currentToken << endl;
				counter++;
			} else {
				// Otherwise, it's an unrecognized token
				cout << "Unrecognized token: " << currentToken << endl;
				counter++;
			}
			currentToken.clear();
		}
		else if(c == '0' && (code[i+1] == 'x' || code[i+1] == 'X')){
			currentToken += c;
			i++;
			currentToken += code[i];
			i++;
			while (i < code.length() && ((code[i] >= '0' && code[i] <= '9') || (code[i] >= 'A' && code[i] <= 'F')
                                                                               || (code[i] >= 'a' && code[i] <= 'f'))) {
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
			if(currentToken == "0x" || currentToken == "0X"){
				hasMoreChars = true;
			}

			// If there are no more digits after the octal number,
			// then currentToken represents an octal number
			if (!hasMoreChars) {
				cout << "Hexadecimal Number: " << currentToken << endl;
				counter++;
			} else {
				// Otherwise, it's an unrecognized token
				cout << "Unrecognized token: " << currentToken << endl;
				counter++;
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
                counter++;
                currentToken.clear();
            }
            else
            {
                cout << "Operator: " << currentToken << endl;
                counter++;
                currentToken.clear();

            }


        } else if ((c >='0' && c <= '9') || c == '+' || c == '-') {
			// Handle numbers
			currentToken += c;
			i++;
			while (i < code.length() && (isdigit(code[i]) || code[i] == '.' || code[i] == 'e' || code[i] == 'E' ||
            (i > 0 && (code[i - 1] == 'e' || code[i - 1] == 'E') && (code[i] == '-' || code[i] == '+')))) {
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

			if (isValidNumber(currentToken) && !hasMoreChars) {
				cout << "Number: " << currentToken << endl;
				counter++;
			}
			else {
				cout << "Unrecognized token: " << currentToken << endl;
				counter++;
			}
			currentToken.clear();
		}  else if (c == '\'' || c == '\"') {
            // Handle string literals
            char quote = c;
            cout<< "punctuation: "<<quote<<endl;
            counter++;
            i++;
            while (i < code.length() && code[i] != quote) {
                currentToken += code[i];
                i++;
            }

            if (i < code.length() && code[i] == quote) {
                i++;
            }
            if (c == '\'' ) {
                cout << "Character: " << currentToken << endl;
                counter++;
            }
            else if( c == '\"') {
                cout << "String: " << currentToken << endl;
                counter++;
            }

            cout<< "punctuation: "<<quote<<endl;
            counter++;
            currentToken.clear();
        } else {
            cout << "Unrecognized token: " << c << endl;
            counter++;
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

        for (const auto& entry : symbol_table) {
        cout << "Symbol: " << entry.first<<"\t\t\t";
        cout << "Offset: ";
        for (int value : entry.second) {
            cout << value << " ";
        }
        cout << endl;
    }
    return 0;
}
