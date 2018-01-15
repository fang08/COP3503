/*
 * pa2.h
 *
 *  Created on: 2017.03.01
 *      Author: Fang
 *
 *  This header file defines the Node and LinkedList classes and their main functions.
 */

#ifndef PA2_H_
#define PA2_H_

#include <iostream>
#include <string>
using namespace std;

class Node{
	private:
		string name;
		int page;
		Node * next;
	public:
		//constructor
		Node(string name, int page){
			this->name = name;
			this->page = page;
			this->next = NULL;
		}
		//accessors and mutators
		string getName() { return name; }
		void setName(string name) { this->name = name; }
		int getPage() { return page; }
		void setPage(int page) { this->page = page; }
		Node* getNext() { return next; }
		void setNext(Node * next){ this->next = next; }

};

class LinkedList{
	private:
		Node * head;
	public:
		//constructor
		LinkedList(){
			this->head = new Node("Head",0);
			//initiate 32 "Free" nodes
			head->setNext(new Node("Free",32));
		}

		//accessors and mutators
		Node* getHead(){ return head; }
		void setHead(Node * head){this->head = head;}
};

void printMenu();
void printMemory(LinkedList * list);
void bestAdd(LinkedList * list, string name, int size);
void worstAdd(LinkedList * list, string name, int size);
void killProgram(LinkedList * list, string name);
void printFragment(LinkedList * list);

#endif /* PA2_H_ */
