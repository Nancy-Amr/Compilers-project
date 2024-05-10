// Online C++ compiler to run C++ program online
#include <iostream>
#include <string.h>
#include <fstream>
#include<stack>
using namespace std;

// Function prototypes
void firstOf(string[], string);
void followOf(string[], string);
void firstFollow(string[], string);

bool noEpsilonInFollow(string[]);
void addToResultSet(string[], string);
void chdg();

// Constants
const int numberOfProductionRules = 100;
int global_i = 0;
int global_j = 0;

string nonTerminalChar[6]; /*= {  "IDENTIFIER", "Letter", "LetterorDigit", "Digit"};*/
string first[6][100];
string follow[6][100];
string parsingTable[100][100];


// Grammar is hard coded into the 2-D array
string productionSet[100][100];
stack<string> st;

void init() {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            productionSet[i][j] = "";
        }
    }
    for (int i = 0; i < 4; i++) {
           for (int j = 0; j < 100; j++) {
               first[i][j] = "";
           }
       }
    for (int i = 0; i < 4; i++) {
              for (int j = 0; j < 100; j++) {
                  follow[i][j] = "";
              }
          }
    for (int i = 0; i < 100; i++) {
                  for (int j = 0; j < 100; j++) {
                	  parsingTable[i][j] = "";
                  }
              }

}



void nonterm(){
	 for (int u = 0; u < 6; u++) {
		 nonTerminalChar[u]="";
	    }
	int j = 0 ;
	int flag =0;
	for(int i =0; i<100; i++){
		flag=0;
		for(int k =0; k<6;k++){
			if(nonTerminalChar[k]==productionSet[i][0]){
				 flag = 1;
				 break;
			}
		}
		if(flag==0){
		nonTerminalChar[j]=productionSet[i][0];
		j++;
		}

	}

}

void  getRules(string a[100][100], string c) {
	int index = 0;
	for (int i = 0; i < 100; i++) {
		        for (int j = 0; j < 100; j++) {
		            a[i][j] = "";
		        }
		    }

	for (int i = 0;  productionSet[i][0] != ""; i++) {
		//keep  looping to find production rule with X on the LHS, as the productionSet array is 2d, LHS will always be in the first column so
		if (productionSet[i][0] == c) {
			string n[100];
			//init array of n
			for (int u = 0; u < 100; u++) {
					 n[u]="";
				    }

			for (int j = 2; productionSet[i][j] != "" && productionSet[i][2]!="#"; j++) {
				n[j-2] =  productionSet[i][j];
				/*cout<<n[j-2];
				cout<<endl;*/
			}

			for(int k=0;n[k]!="";k++){
			a[index][k] = n[k];




			}
						index++;

		}
	}
/*for(int i=0; a[i][0]!="";i++){
	for(int j=0; a[i][j]!=""; j++){
		cout<<a[i][j];
	}
	cout<<endl;
}*/

}
bool epsilonSearch(string Result[]) {
	for (int i = 0; Result[i] != ""; i++) {
		if (Result[i] == "#") {
			return true;

		}

	}
	return false;
}
int findPos(string c) {
	 char x = 'a';

	    for (int i = 0; i < 62; i++) {
	       if(c==string(1, x)){
	    	   return i;
	       }

	       if(x=='z'){
	    	   x='A';
	       }
	       else if(x=='Z'){
	    	   x= '0';

	       }
	       else{
	    	   x++;
	       }

	    }
	    if(c=="_")
	    return 62;
	    else if(c=="$"){
	    	return 63;
	    }
	    else{
	    	return -1;
	    }

}




void findParsingTable() {
	string a[100][100];
	//init for a array
	 for (int i = 0; i < 100; i++) {
	        for (int j = 0; j < 100; j++) {
	            a[i][j] = "";
	        }
	    }


	for (int n = 0; nonTerminalChar[n]!=""; n++) {

				if ( (epsilonSearch(first[n]))) {

//if epsilon in the first
					for (int k = 0; follow[n][k] != ""; k++) {
						int m= findPos(follow[n][k]);
						parsingTable[n][m] = "#";
						//cout<<"check here"<<n <<m;

					}

				}
				//get rules that are not #

					getRules(a, nonTerminalChar[n]);  //s7
					for(int w=0; a[w][0]!="";w++){   //nm4y 3ly kol l rules
					int termFlag = 1;

					// If c is terminal, firstOf(X) = {x}
					for (int i = 0; nonTerminalChar[i]!=""; i++) {
						if (a[w][0] == nonTerminalChar[i]) {
							termFlag = 0;
							break;
						}
					}

					//if terminal
					if (termFlag) {
						for(int x=0;first[n][x]!="";x++){
							if(a[w][0]==first[n][x] && a[w][0]!="#"){
								/*cout<<a[w][0];
								cout<<endl;*/
								int po=findPos(first[n][x]);
								/*cout<<n<<"    hsjabfjknabjkfdsjnas";
								cout<<endl;
								cout<<po;
								cout<<endl;*/
								parsingTable[n][po]="";
								for(int v=0; a[w][v]!="";v++){
								parsingTable[n][po] += a[w][v];
								parsingTable[n][po] += " ";

								/*cout<< parsingTable[n][po];
								cout<<endl;*/
								}
								/*cout<<parsingTable[n][po];
								cout<<endl;*/
							}
						//parsingTable[n][findPos(first[n][x])] = a[x];
						}
					}
					//nonTerminal
					else{
						//g is position for first non terminal
						int g;
						for( g =0; nonTerminalChar[g]!=""; g++){
							if(nonTerminalChar[g]==a[w][0]){ //l2inaha
								//cout<<nonTerminalChar[g]<<endl;
								break;
							}
						}

						for(int f=0;f<100;f++){
							for(int q=0;q<100;q++){
								if(first[n][f]==first[g][q]){

									int po=findPos(first[n][f]);
									if(parsingTable[n][po]!= ""){
										continue;
									}
									parsingTable[n][po]="";
									for(int v=0; a[w][v]!="";v++){


										parsingTable[n][po] = parsingTable[n][po] + a[w][v];
										parsingTable[n][po] +=" ";
										//cout<<endl;
										//cout<<po;
										//parsingTable[n][findPos(first[n][f])] = a[x];

									}
								}
							}

						}

					}


					}
				//for (int x = 0; first[n][x] != ""; x++) {
					//get rules will fill the array of a wiht all the possible prdouction rules for every given vn


					//parsingTable[n][findPos(first[n][x])] = a[x];
				//}


	}}




void readFromFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    int row = 0;
    string line;
    while (getline(file, line) && row < 100) {
        // Assuming columns are separated by space or tab
        size_t pos = 0;
        int col = 0;
        while ((pos = line.find_first_of(" \t")) != string::npos && col < 100) {
        	productionSet[row][col++] = line.substr(0, pos);
            line.erase(0, pos + 1);
        }
        if (!line.empty() && col < 100) {
        	productionSet[row][col++] = line;
        }
        row++;
    }

    file.close();
}

int findnontermpos(string nonterm){
	for(int i=0;nonTerminalChar[i]!="";i++){
		if(nonterm == nonTerminalChar[i]){
			return i;
		}
	}
	return -1;
}

int check_TermOrNonOrEp(string str){
	int termFlag = 1;
	if(str == "#"){
		return -1;  //epsilon
	}
	// If c is terminal, firstOf(X) = {x}
	for (int i = 0; nonTerminalChar[i]!=""; i++) {
		if (str == nonTerminalChar[i]) {
			termFlag = 0;
			break;
		}
	}
	if(termFlag){
		return 1;  //terminal
	}
	else{
		return 0; //non terminal
	}

}

void parsingStack(string buffer[], int ln){

	st.push("$");
	int k=0;
		//Push the starting symbol into the stack
		st.push(productionSet[0][0]);

//cout<<ln;
//cout<<buffer[2];
		//Add end delimeter to the end of the input string
		while (st.top() != "$" && buffer[k] != "$"){
			//cout<<"checking entry"<<endl;
			int c = check_TermOrNonOrEp(st.top());
			if(c==-1){
				st.pop();
			}
			if(c==1 && buffer[k]!=st.top()){

				cout<<"not valid"<<endl;
				return;
			}
			else if(c==1 && buffer[k]==st.top()){
				st.pop();
				k++;
				/*cout<<k;
				cout<<buffer[k]<<endl;
				cout<<st.top()<<endl;*/
			}
			else if(c==0){
				int po = findPos(buffer[k]);
				if(po == -1){
					cout<<"not valid"<<endl;
					return;
				}
				int npos =findnontermpos(st.top());
				if(parsingTable[npos][po] ==""){
					cout<<"not valid"<<endl;
					return;
				}
				else{
					st.pop(); //non teminal
					string a[100];
					int sz= parsingTable[npos][po].length();
					int j;
					int d=0;
					for(int s=0;s<100;s++){
						a[s]="";
					}
					for( j=0;j<sz-1;j++){
						if(string(1,parsingTable[npos][po][j])==" "){
							d++;
							j++;
						}
						a[d] += string(1,parsingTable[npos][po][j]);
					}
					for(int w=d; w>=0; w--){
						st.push(a[w]);
						//cout<<"check "<<a[w]<<endl;
					}
				}
			}



		}
		if(st.top() != "$" && buffer[k] == "$"){
			int u = check_TermOrNonOrEp(st.top());
			if(u==-1){
				st.pop();
			}
			else if(u==1){
				cout<<"invalid"<<endl;
				return;
			}
			else if(u==0){


				string tr= st.top();
				st.pop();
				int nn = findnontermpos(tr);
				if(st.top()=="$" && parsingTable[nn][63]!=""){
					st.push("#");
					st.pop();
				}
				else{
					st.push(tr);
				}
			}
		}

		if (st.top() == "$" && buffer[k] == "$"){
				cout << "Parsing is complete language is accepted" << endl;
		}
		else{
			cout<<"invalid"<<endl;
		}

}



int main() {
    init();
    string filename = "index.txt";
    readFromFile(filename);

    chdg();
nonterm();
    cout << "LL(1) Parser to calculate the first and follow sets of a given grammar, calculate and fill the parsing table, and parsing program to check if language is accepted or not when the parsing is complete " << endl;

    cout << endl << endl << "Grammar: " << endl;
    cout << "A->aBa" << endl;
    cout << "B->bB|c" << endl;
    cout << endl << endl;

    // To calculate the first functions of the Vn
    for (int i = 0; nonTerminalChar[i] != ""; i++) {
        firstOf(first[i], nonTerminalChar[i]);
    }

    // Printing out the first array to show the first set calculated for each non-terminal character
    for (int i = 0; i < 4; i++) {
        cout << "The first of " << nonTerminalChar[i] << ": {";
        for (int j = 0; first[i][j] != ""; j++) {
            cout << first[i][j] << " ";
        }
        cout << " }" << endl;
    }
    cout << endl;

	for (int i = 0; nonTerminalChar[i] != ""; i++) {
		//loop through each Vn character and find its folow set, after that add the set to the corresponding row in the follow2d array
		//first parameter is a 1d array at index i, and the corresponding nonTerminalChar
		followOf(follow[i], nonTerminalChar[i]);
	}

	//printing out the follow array to show the follow set calculated for each non termianl character
	for (int i = 0; i < 4; i++) {
		cout << "The follow of " << nonTerminalChar[i] << ": {";
		for (int j = 0; follow[i][j] != ""; j++) {
			cout << follow[i][j] << " ";
		}
		cout << " }" << endl;
	}
	cout << endl;


	findParsingTable();//call this function to fill the parsing table
		cout << "Parsing Table" << endl;
		cout << "\t\t\ta\tb\tc\td" << endl;
		cout << "-----------------------------------" << endl;

		for (int i = 0; i<4 ; i++) {
			/*if (i > 0) {
				if (productionSet[i][0] ==  productionSet[i - 1][0])break;
			}*/
			cout << nonTerminalChar[i] << "| ";


			for (int j = 0;j<100; j++) {
				/*if (parsingTable[i][j]!="") {
					cout << "\t";
				}*/

					cout << "\t" << parsingTable[i][j];

			}
			cout << endl;
		}
		string en;
		cout<<"enter code"<<endl;
			cin>>en;
		int ln =en.length();
		string buffer[ln+1];
		for(int r=0;r<ln;r++){
			buffer[r]=string(1,en[r]);
		}
		buffer[ln]="$";

		parsingStack(buffer, ln+1);


    return 0;
}

void firstOf(string Result[], string c) {
    int j, k;
    string tempResult[100];
    int epsilonFlag;
    tempResult[0] = "";
    Result[0] = "";
    int termFlag = 1;

    // If c is terminal, firstOf(X) = {x}
    for (int i = 0; nonTerminalChar[i]!=""; i++) {
        if (c == nonTerminalChar[i]) {
            termFlag = 0;
            break;
        }
    }
    if (termFlag && c!="#") {
        addToResultSet(Result, c);
        return;
    }

    // If c is non-terminal, read each production
    for (int i = 0; productionSet[i][0] != ""; i++) {
        // Keep looping to find production rule with X on the LHS
        if (productionSet[i][0] == c) {
            // Check if epsilon is a production rule, and then add it to firstOf(X)
            if (productionSet[i][2] == "#" && productionSet[i][3]=="") {
                addToResultSet(Result, "#");
            } else {
                // Start counter at RHS
                j = 2;
                while (productionSet[i][j] != "") {
                    epsilonFlag = 0;
                    // Keep calling first of RHS and put results in a temp array called tempResult
                    firstOf(tempResult, productionSet[i][j]);
                    // As long as tempResult has characters, add them to the original result array passed through the parameter

                    // If epsilon found, no need to check next element in the particular production rule
                    for (k = 0; tempResult[k] != ""; k++) {
                        if (tempResult[k] == "#") {
                        	tempResult[k]= " ";
                            epsilonFlag = 1;
                            break;
                        }
                    }
                    for (k = 0; tempResult[k] != ""; k++)
                                            addToResultSet(Result, tempResult[k]);

                    /*for (k = 0; tempResult[k] != ""; k++) {
                                            if (tempResult[k] != "#") {
                                                addToResultSet(Result, tempResult[k]);
                                            } else {
                                            	tempResult[k] == " ";
                                                epsilonFlag = 1;
                                            }
                                        }*/
                    if (!epsilonFlag)
                        break;
                    j++;
                }
            }
        }
    }
}

bool noEpsilonInFollow(string Result[]) {
	for (int i = 0; Result[i] != ""; i++) {
		if (Result[i] == "#") {
			Result[i]=" ";
			return false;

		}
	}
	return true;
}


void followOf(string Result[], string c)
{    //start
	//1)Place $ in followOf(starting symbol)
	if (productionSet[0][0] == c)addToResultSet(Result, "$");
	//loop through each production Rules
	for (global_i = 0; productionSet[global_i][0]!=""; global_i++)
	{
		//loop through the end of each production starting from the first elemetn in the RHS
		for (global_j = 2; productionSet[global_i][global_j]!=""; global_j++)
		{
			//reach the Vn we are calculating the followOf function
			if (productionSet[global_i][global_j] == c)
			{

				//call the first function to find the follow
				if (productionSet[global_i][global_j + 1] != "")
					firstFollow(Result, productionSet[global_i][global_j + 1]);
//nonterminal at end so it takes the follow of nonterminal at beginning
				//recurively call the follow of until calculating the follow result array
				if (productionSet[global_i][global_j + 1] == "" && c != productionSet[global_i][0])
					followOf(Result, productionSet[global_i][0]);
			}
		}
	}
}
void firstFollow(string  Result[], string c)
{

//terminal
	int termFlag = 1;

	    // If c is terminal, firstOf(X) = {x}
	    for (int i = 0; i < 4; i++) {
	        if (c == nonTerminalChar[i]) {
	            termFlag = 0;
	            break;
	        }
	    }
	    if (termFlag) {
	        addToResultSet(Result, c);

	    }
	//nonterminal
	else{
	//for (k = 0; k < numberOfProductionRules; k++)
	//{
		/*if (productionSet[k][0] == c)
		{
			if (productionSet[k][2] == '$') followOf(Result, productionSet[global_i][0]);
			else if (!(isupper(productionSet[k][2]))) addToResultSet(Result, productionSet[k][2]);
			else firstFollow(Result, productionSet[k][2]);
		}*/

		firstOf(Result,c);



	//}
	//checker to see if epsilon was added to the follow set, and if so it will call the firstFollow function to the next element int the production rule
	//int g = 1;
	//A=aAB

		if (!noEpsilonInFollow(Result)) {
			if (productionSet[global_i][global_j + 2] != "") {
				int termmFlag = 1;


				for (int i = 0; i < 4; i++) {
					if (productionSet[global_i][global_j + 2] == nonTerminalChar[i]) {
						termmFlag = 0;
						break;
					}
				}
				if (termmFlag) {
					addToResultSet(Result, productionSet[global_i][global_j + 2]);

				}


				/*if (isupper(productionSet[global_i][global_j + 2])) {

				}*/
				else {
					firstOf(Result, productionSet[global_i][global_j + 2]);
				}
			} else {
				if(productionSet[global_i][0]==productionSet[global_i][global_j]){
					return;
				}
				else{

					followOf(Result, productionSet[global_i][0]);
				}
			}
		}


	}
}

void addToResultSet(string Resul[], string element) {
    int i;
    for (i = 0; Resul[i] != ""; i++) {
        // If the element is found
        if (Resul[i] == element) {
            return;
        }
    }
    Resul[i] = element;
    Resul[i + 1] = "";
}

void chdg() {
    char x = 'a';
    for (int i = 6; i < 58; i++) {
        productionSet[i][0] = "Letter";
        productionSet[i][1] = "=";
        productionSet[i][2] = string(1, x);
        if (x == 'z') {
            x = 'A';  // Reset x to 'a'
        } else {
            x++;
        }
    }
    char j ='0';
    for (int k = 58; k < 68; k++) {
        productionSet[k][0] = "Digit";
        productionSet[k][1] = "=";
        productionSet[k][2] = string(1, j);
        j++;
    }
}

