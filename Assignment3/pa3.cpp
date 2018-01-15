/*
 * pa3.cpp
 *
 *  Created on: 2017.03.24
 *      Author: Fang
 *
 *  This program implements the lexical analysis task for a programming language. It analyze the code
 *  and extract all tokens include keywords, identifiers, constants, operators and delimiters. It also
 *  compute the depth of the nested loops and detect the possible syntax errors using stack structure.
 *
 */

#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include "pa3.h"

using namespace std;

// this function calculate the maximum depth of nested loop
int maxDepthLoop(vector<string> keywords){
	stack<string> temp;
	int current = 0;
	int max = 0;

	if (keywords.size() == 0)
		return 0; // if the vector is empty, return 0 depth
	else {
		for (unsigned i=0; i < keywords.size(); i++){
			if (keywords[i] == "FOR" && (i+1)< keywords.size() && keywords[i+1] == "BEGIN"){
				if (current<0)
					current = 0; // if there are extra ENDs before FOR
				current ++;
				if (current > max)
					max = current;
			}
			else if (keywords[i] == "END"){
				current --;
			}
			else
				temp.push(keywords[i]);
		}
		if (current > 0)
			max = max - current; // find the matched pairs of FOR-BEFOREs and ENDs

		return max;
	}
}


// this function is used to push words in correct stacks
void pushUniqueWord(vector<string>& st, string candidateWord){
	bool duplicate = false;
	for (unsigned k = 0 ; k < st.size(); k++){
		if (candidateWord == st[k]){
			duplicate = true;
			break;
		}
	}
	if (!duplicate)
		st.push_back(candidateWord);
}


// this function check unbalanced parentheses
string checkParanthesesBalanced(vector<string> st){
	stack<string> errors;
	string output = "";
	// search for every unpaired ( and ) then store in a stack
	for(unsigned i = 0; i < st.size(); i++){
		if (st[i] == "("){
			errors.push(st[i]);
		}
		else if (st[i] == ")"){
			if(errors.empty())
				errors.push(st[i]);
			else if(!errors.empty() && errors.top()== ")")
				errors.push(st[i]);
			else
				errors.pop(); // only if there is a ( in the stack, it should pop out
		}
	}
	// pop all the errors store in the stack
	while(!errors.empty()){
		output = errors.top() + " " + output; // add to the back of the string
		errors.pop();
	}
	return output;
}


// this function check unbalanced keywords (FOR-BEGIN-END)
string checkKeywordsBalanced(vector<string> keywords){
	stack<string> temp, typo;
	string output = "";

	for (unsigned i=0; i < keywords.size(); i++){
		if (keywords[i] == "FOR" && (i+1)< keywords.size()){
			temp.push(keywords[i]); // push "FOR"
		}

		else if (keywords[i] == "BEGIN"){
			if (i != 0 && keywords[i-1] == "FOR"){
				temp.pop(); // pop "FOR"
				temp.push(keywords[i]); // then push "BEGIN"
			}
		}

		else if (keywords[i] == "END"){
			if (temp.empty()){
				temp.push(keywords[i]); // push "END"
			}
			else if (!temp.empty() && temp.top() == "END"){
				temp.push(keywords[i]); // push "END"
			}
			else if (!temp.empty() && temp.top() == "BEGIN"){
				temp.pop(); // only if the loop is correct, pop "BEGIN"
			}
		}

		else {  //typos
			if ((i+1)<keywords.size() && keywords[i+1] == "BEGIN"){
				typo.push(keywords[i]); // push the typo
				i = i+2; // skip the next two elements
			}
			else if ((i+1)<keywords.size() && keywords[i+1] == "END" && keywords[i-1] == "FOR"){
				temp.pop(); // pop "FOR"
				typo.push(keywords[i]); // push the typo
				i = i+1; // skip the next element
			}
			else {
				temp.pop(); // pop "BEGIN"
				typo.push(keywords[i]); // then push typo
			}
		}
	}

	while(!temp.empty()){
		if (temp.top() == "BEGIN"){
			output = "END " + output;
			temp.pop();
		}
		else if (temp.top() == "END"){
			output = "FOR " + output;
			temp.pop();
		}
		else if (temp.top() == "FOR"){
			output = "BEGIN " + output;
			temp.pop();
		}
	}
	while(!typo.empty()){
		output = typo.top() + " " + output;
		typo.pop();
	}

	return output;
}


// print out unbalanced keywords and parantheses
void printSyntaxErrors(vector<string> others, vector<string> keywords){
	string result = checkKeywordsBalanced(keywords);
	string result2 = checkParanthesesBalanced(others);

	if (result == "" && result2 == ""){
		cout << "NA";
	}
	else{
		cout << result << " " << result2;
	}
}


// this function prints out vector elements
void print(vector<string> st){
	for (unsigned i = 0; i < st.size(); i++)
		cout << st[i] << ' ';
}

int main(int argc, const char * argv[]) {
	string filename;
	cout << "Please enter the name of the input file:" << endl;
	cin >> filename;

	ifstream file;
	file.open(filename.c_str());
	while (!file.is_open()){
		cout << "Unable to open file " << filename << ". Please try again.\n";
		cin.clear();
		cout << "Please enter the name of the input file:" << endl;
		cin >> filename;
		file.open(filename.c_str());
	}

	string resource = "";
	// read input ignore space, tab or newline
	while (file.good()){
		string temp = "";
		file >> temp;
		resource += temp + "@"; //just use @ to separate words
	}

	// initiate all the Stacks
	vector<string> keywords, keywordsFull, identifiers, constants, operators, delimiters, others;

	// categorize different types of words and push to the correct stacks
	for (unsigned i=0; i<resource.size()-1; i++){
		char currentChar = resource.at(i);

		if (currentChar >= 'A' && currentChar <= 'Z'){
			string keywordCandidate = "";
			keywordCandidate = resource.at(i);
			int j = i+1;
			while (resource.at(j) >= 'A' && resource.at(j) <= 'Z'){
				keywordCandidate += resource.at(j);
				j++;
			}
			i = j-1; // mark current position

			keywordsFull.push_back(keywordCandidate);
			if (keywordCandidate == "FOR" || keywordCandidate == "BEGIN" || keywordCandidate == "END"){
				pushUniqueWord(keywords, keywordCandidate);
			}
		}

		else if (currentChar >= 'a' && currentChar <= 'z'){
			string identifierCandidate = "";
			identifierCandidate = resource.at(i);
			int j = i+1;
			while (resource.at(j) >= 'a' && resource.at(j) <= 'z'){
				identifierCandidate += resource.at(j);
				j++;
			}
			i = j-1; // mark current position
			pushUniqueWord(identifiers, identifierCandidate);
		}

		else if (currentChar >= '0' && currentChar <= '9'){
			string constantCandidate = "";
			constantCandidate = resource.at(i);
			int j = i+1;
			while (resource.at(j) >= '0' && resource.at(j) <= '9'){
				constantCandidate += resource.at(j);
				j++;
			}
			i = j-1; // mark current position
			pushUniqueWord(constants, constantCandidate);
		}

		else if (currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/'
				|| currentChar == '='){
			string operatorCandidate = "";
			operatorCandidate = resource.at(i);
			int j = i+1;
			while (resource.at(j) == '+' || resource.at(j) == '-' || resource.at(j) == '*'
					|| resource.at(j) == '/' || resource.at(j) == '='){
				operatorCandidate += resource.at(j);
				j++;
			}
			i = j-1; // mark current position
			pushUniqueWord(operators, operatorCandidate);
		}

		else if (currentChar == '(' || currentChar == ')'){
			string otherCandidate = "";
			otherCandidate = resource.at(i);
			others.push_back(otherCandidate);
		}

		else if (currentChar == ',' || currentChar == ';'){
			string delimiterCandidate = "";
			delimiterCandidate = resource.at(i);
			pushUniqueWord(delimiters, delimiterCandidate);
		}

		else
			continue;
	}

	// final output
	cout << "\nThe depth of nested loop(s) is " << maxDepthLoop(keywordsFull)<<"\n";
	cout << "\nKeywords: ";
	print(keywords);
	cout << "\nIdentifier: ";
	print(identifiers);
	cout << "\nConstant: ";
	print(constants);
	cout << "\nOperators: ";
	print(operators);
	cout << "\nDelimiter: ";
	print(delimiters);
	cout << "\n\nSyntax Error(s): ";
	printSyntaxErrors(others, keywordsFull);

	file.close();

	return 0;
}
