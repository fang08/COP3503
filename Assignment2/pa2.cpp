/*
 * pa2.cpp
 *
 *  Created on: 2017.03.01
 *      Author: Fang
 *
 *  This program emulate the operating system's responsibility of allocating memory to certain programs.
 *  There are two algorithms to allocate memory: best-fit and worst-fit algorithms.
 *  Main function includes add programs, kill programs(reclaim memory), get number of fragments,
 *  prints out the memory and exit the program.
 *
 */

#include <iostream>
#include <cstring>
#include <string>
#include <cctype>
#include <cmath>
#include <limits>
#include "pa2.h"

using namespace std;

// print out the main menu
void printMenu(){
	cout<<"1. Add program\n2. Kill program\n3. Fragmentation"
		<<"\n4. Print memory\n5. Exit"<<endl;
}

// print out the whole memory
void printMemory(LinkedList * list){
	// if LinkedList is empty
	if(list->getHead()->getNext() == NULL){
		return;
	}
	int count = 0;
	Node * current = list->getHead()->getNext();
	// iterate until the last pointer
	while(current != NULL){
		for (int i=0; i<current->getPage();i++){
			cout<<current->getName()<<"\t";
			count++;
			//a new line every 8 pages
			if(count == 8){
				cout<<"\n";
				count = 0;
			}
		}
		current = current->getNext(); // move to the next pointer
	}
}

// the function adding program implements best-fit algorithm
void bestAdd(LinkedList * list, string name, int size){
	Node * add = new Node(name,size);
	Node * current = list->getHead()->getNext();
	Node * previous = list->getHead();
	Node * current2 = previous->getNext();
	int diff = 32;
	int min = 32;
	bool valid = false;

	/* first iteration : find whether the program is existing,
	 * find the smallest free space for the new program,
	 * and decide whether the space large enough for the new program
	 */
	while(current != NULL){
		if (current->getName() == name){
			cout<<"\nError, Program "<<name<<" already running.\n";
			return;
		}
		if (current->getName() == "Free"){
			diff = current->getPage()-size;
			if (diff >=0 && diff < min){
				min = diff;
				valid = true; //means there is at least one space large enough for the new program
			}
		}
		current = current->getNext();
	}
	// find no space large enough for the new program
	if (!valid){
		cout<<"\nError, Not enough memory for Program "<<name<<"\n";
		return;
	}

	// Second iteration, insert the new node at correct place
	while(current2 != NULL){
		// find the correct insert place for the new node based on first iteration
		if (current2->getName() == "Free" && (current2->getPage()-size) == min){
			//same size with Free node: just change the node's name, no need to change pointers
			if (min == 0){
				current2->setName(name);
			}
			// Free space is larger than new node's size:
			// add new node between previous and current2 pointers
			// change the size of free node
			else{
				current2->setPage(min);
				add->setNext(current2);
				previous->setNext(add);
			}
			cout<<"\nProgram "<<name<<" added successfully: "<<size<<" page(s) used.\n";
			break; //successful add the page, then exit the loop
		}
		previous = previous->getNext();
		current2 = current2->getNext();
	}
}

// the function adding program implements worst-fit algorithm
void worstAdd(LinkedList * list, string name, int size){
	Node * add = new Node(name,size);
	Node * current = list->getHead()->getNext();
	Node * previous = list->getHead();
	Node * current2 = previous->getNext();
	int diff = -1;
	int max = -1;
	bool valid = false;

	/* first iteration : find whether the program is existing,
	 * find the smallest free space for the new program,
	 * and decide whether the space large enough for the new program
	 */
	while(current != NULL){
		if (current->getName() == name){
			cout<<"\nError, Program "<<name<<" already running.\n";
			return;
		}
		if (current->getName() == "Free"){
			diff = current->getPage()-size;
			if (diff >=0 && diff > max){
				max = diff;
				valid = true; //means there is at least one space large enough for the new program
			}
		}
		current = current->getNext();
	}

	// find no space large enough for the new program
	if (!valid){
		cout<<"\nError, Not enough memory for Program "<<name<<"\n";
		return;
	}

	// Second iteration, insert the new node at correct place
	while(current2 != NULL){
		// find the correct insert place for the new node based on first iteration
		if (current2->getName() == "Free" && (current2->getPage()-size) == max){
			//same size with Free node: just change the node's name, no need to change pointers
			if (max == 0){
				current2->setName(name);
			}
			// Free space is larger than new node's size:
			// add new node between previous and current2 pointers
			// change the size of free node
			else{
				current2->setPage(max);
				add->setNext(current2);
				previous->setNext(add);
			}
			cout<<"\nProgram "<<name<<" added successfully: "<<size<<" page(s) used.\n";
			break;
		}
		previous = previous->getNext();
		current2 = current2->getNext();
	}
}

// the function delete selected program and reclaim the memory to free
void killProgram(LinkedList * list, string name){
	Node * current = list->getHead();
	bool findit = false;
	// find the program and reclaim the memory to free
	while(current != NULL){
		if (current->getName() == name){
			current->setName("Free");
			cout<<"\nProgram "<<name<<" successfully killed, "<<current->getPage()<<" page(s) reclaimed.\n"<<endl;
			findit = true;
			break;
		}
		else{
			current = current->getNext();
		}
	}

	// if the program doesn't exist
	if (!findit){
		cout<<"Error! Can't find Program name "<<name<<"\n";
		return;
	}

	// find and merge the continuous Free nodes
	Node * current2 = list->getHead();
	while(current2 != NULL){
		Node * next = current2->getNext();
		// merge two continuous Free nodes
		if (next != NULL && current2->getName() == "Free" && next->getName() == "Free"){
			int page1 = current2->getPage();
			int page2 = next->getPage();
			current2->setPage(page1+page2);
			current2->setNext(next->getNext());
			delete next;
			// if there are three continuous Free nodes, merge twice
			if (current2->getNext() != NULL && current2->getNext()->getName() == "Free"){
				continue;
			}
		}
		current2 = current2->getNext();
	}
}

// find and print out the number of fragments
void printFragment(LinkedList * list){
	Node * current = list->getHead()->getNext();
	int count = 0;
	while(current != NULL){
		if (current->getName() == "Free"){
			count ++;
		}
		current = current->getNext();
	}
	cout<<"\nThere are "<<count<<" fragment(s).\n";
}


int main(int argc, char *argv[]){
	int bestMode = -1;
	string mode;
	// should be only two arguments passed to main(), the program name argv[0]
	// and the memory management algorithm given by the user argv[1]
	if (argc == 2 && argv[1] != 0){
		mode = argv[1];
		if (mode == "best"){
			bestMode = 1;
			cout<<"Using best fit algorithm\n"<<endl;
		}
		else if (mode == "worst"){
			bestMode = 0;
			cout<<"Using worst fit algorithm\n"<<endl;
		}
		else {
			cout<<"Error! Please enter either 'best' or 'worst'."<<endl;
			return 1; //exit the program
		}
	}
	else {
		cout<<"Error numbers of arguments! Please enter either 'best' or 'worst'."<<endl;
		return 1; //exit the program
	}

	// print out the main menu for selection
	printMenu();
	LinkedList list;

	int choice = -1;
	while (choice !=5){
		cout<<"\nchoice - ";
		cin>>choice;

		switch (choice){
			// select to add program
			case 1:{
				string name;
				int size;
				bool valid1 = false;
				while (!valid1){
					cout<<"Program name - ";
					cin>>name;
					cout<<"Program size (KB) - ";
					cin>>size;
					if (name != "Free" && size >0){
						valid1 = true;
					}
					else if (name == "Free"){
						cout<<"Program name cannot be 'Free', change to another name!"<<endl;
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
					}
					else{
						cout<<"Program size must be a positive number!"<<endl;
						//clear and ignore input to avoid infinite loop
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
					}
				}
				//roundup to get the number of pages for the program
				int adjustSize = ceil(size/4.0);

				//if best-fit algorithm is selected
				if (bestMode == 1){
					bestAdd(&list, name, adjustSize);
				}
				//if worst-fit algorithm is selected
				else {
					worstAdd(&list, name, adjustSize);
				}
				break;
			}
			//select to kill program
			case 2:{
				string name2;
				cout<<"Program name - ";
				cin>>name2;
				killProgram(&list, name2);
				break;
			}
			//select to print number of fragments
			case 3:{
				printFragment(&list);
				break;
			}
			//select to print out memory
			case 4:{
				printMemory(&list);
				break;
			}
			//select to exit the program
			case 5:{
				break;
			}
			//if user input is not a number from 1 to 5
			default:{
				cout<<"\nInvalid input. Please input a number between 1 and 5."<<endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
		}
	}
	return 0;
}
