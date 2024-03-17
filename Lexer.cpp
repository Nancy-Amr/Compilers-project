#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TABLE_SIZE 31
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
//Nancy//
typedef struct Node {
    char* keyword;
    struct Node* next;
} Node;

Node* hashtable[TABLE_SIZE] = {NULL};

// Hash function
int hash(char* keyword) {
    int hashValue = 0;
    for (int i = 0; i < strlen(keyword); i++) {
        hashValue = (hashValue * 31 + keyword[i]) % TABLE_SIZE;
    }
    return hashValue;
}

// Insert keyword into hashtable
void insert(char* keyword) {
    int index = hash(keyword);
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->keyword = strdup(keyword);
    newNode->next = hashtable[index];
    hashtable[index] = newNode;
}

// Insert all keywords into hashtable
void insertAllKeywords() {
    char* keywords[] = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else",
                        "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return",
                        "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned",
                        "void", "volatile", "while"};
    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < numKeywords; i++) {
        insert(keywords[i]);
    }
}

// Search for keyword in hashtable
int search(char* keyword) {
    int index = hash(keyword);
    Node* current = hashtable[index];
    while (current != NULL) {
        if (strcmp(current->keyword, keyword) == 0) {
            return 1; // Found
        }
        current = current->next;
    }
    return 0; // Not found
}
int isIdentifier(char* word) {
    if (!isalpha(word[0]) && word[0] != '_') {
        return 0; // Not an identifier
    }
    for (int i = 1; i < strlen(word); i++) {
        if (!isalnum(word[i]) && word[i] != '_') {
            return 0; // Not an identifier
        }
    }
    return 1; // Is an identifier
}
/// Nancy ///

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
 insertAllKeywords();

    printf("Identifying keywords and identifiers in the file:\n");
    while (fscanf(file, "%s", word) == 1) {
        if (search(word)) {
            printf("%s is a keyword.\n", word);
        } else if (isIdentifier(word)) {
            printf("%s is an identifier.\n", word);
            // Optionally, insert the identifier into your hashtable or perform other operations
        }
    }
    
    return 0;
}



