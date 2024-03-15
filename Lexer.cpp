#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

// Token types
enum TokenType {
    IDENTIFIER,
    KEYWORD,
    OPERATOR,
    LITERAL,
    OTHER
};

// Token structure
struct Token {
    string lexeme;
    TokenType type;
};


// Function to check if a character is a valid identifier character
bool isValidIdentifierChar(char ch) {
    return isalnum(ch) || ch == '_';
}

// Function to tokenize a C code snippet
vector<Token> tokenize(const string& code) {
    vector<Token> tokens;
    string buffer;
    TokenType bufferType = OTHER;

    for (char ch : code) {
        if (isspace(ch)) {
            // Flush buffer if not empty
            if (!buffer.empty()) {
                tokens.push_back({buffer, bufferType});
                buffer.clear();
                bufferType = OTHER;
            }
        } else if (isValidIdentifierChar(ch)) {
            // Add character to buffer
            buffer += ch;
            bufferType = IDENTIFIER;
        } else {
            // Flush buffer if not empty
            if (!buffer.empty()) {
                tokens.push_back({buffer, bufferType});
                buffer.clear();
                bufferType = OTHER;
            }

            // Handle operators, literals, etc.
            switch (ch) {
                // Handle operators, keywords, etc.
                // Add cases for different operators, keywords, etc. as needed
                default:
                    tokens.push_back({string(1, ch), OPERATOR});
            }
        }
    }

    // Flush buffer if not empty
    if (!buffer.empty()) {
        tokens.push_back({buffer, bufferType});
    }

    return tokens;
}

int main() {
    cout << "Enter C code snippet (press Ctrl+D on a new line to finish):\n";

    string code;
    getline(cin, code);

    vector<Token> tokens = tokenize(code);

    cout << "Tokens:\n";
    for (const auto& token : tokens) {
        cout << "Lexeme: " << token.lexeme << ", Type: " << token.type << endl;
    }

    return 0;
}
