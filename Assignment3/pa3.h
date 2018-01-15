/*
 * pa3.h
 *
 *  Created on: 2017.03.24
 *      Author: Fang
 *
 *  This header file defines stack class and important function such as push and pop.
 */

#ifndef PA3_H_
#define PA3_H_

#include <iostream>
#include <vector>

using namespace std;

class Stack{
	private:
		vector<string> words;

	public:
		// no constructor. Use default one.

		int size(){
			return words.size();
		}

		bool empty() {
			return words.empty();
		}

		// push a string to the stack
		void push(string s) {
			words.push_back(s); // add a new element at the end of the vector
		}

		// pop the top string of the stack
		void pop() {
			if (!words.empty()) // check whether the vector is empty
				words.pop_back(); //remove the last element in the vector
		}

		string top() {
			return words.back(); //return the last element in the vector
		}

};

int maxDepthLoop(vector<string> keywords);
void pushUniqueWord(vector<string>& st, string candidateWord);
string checkParanthesesBalanced(vector<string> st);
string checkKeywordsBalanced(vector<string> keywords);
void printSyntaxErrors(vector<string> others, vector<string> keywords);
void print(vector<string> st);

#endif /* PA3_H_ */
