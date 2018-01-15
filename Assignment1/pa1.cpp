/*
 * pa1.cpp
 *
 *  Created on: 2017.02.07
 *      Author: Fang
 *
 *  This program will create four different N*N magic squares. The size of squares is odd number between
 *  3 and 15. The sum of rows, columns, and diagonals are the same.
 *
 */

#include <iostream>
#include "pa1.h"
using namespace std;

int main(){
	cout<<"Enter the size of a magic square: ";
	int n,i;
	cin>>n;
	while (!validSize(n)){
		cout<<"Enter the size of a magic square: ";
		cin>>n;
	}

	int magicSquare[20][20]={0};
	int magicSquareRight[20][20]={0};
	int magicSquareLeft[20][20]={0};
	int magicSquareFlip[20][20]={0};

	//create the first magic square
	int row = n-1, col = (n-1)/2;
	magicSquare[row][col] = 1; //assign the bottom middle element with value 1
	//then assign the rest of values from 2 to n*n
	for(i=2; i<=n*n; i++){
		//put the next number in correct position
		if ((row+1) > (n-1) && (col-1) >= 0){
			row = 0;
			col = col-1;
		}
		else if ((col-1) < 0 && (row+1) <= (n-1)){
			row = row + 1;
			col = n-1;
		}
		else if ((row+1) > (n-1) && (col-1) < 0){
			row = row - 1;
		}
		else {
			// check whether the element is empty
			if (magicSquare[row+1][col-1] != 0){
				row = row - 1;
			}
			else {
			row = row + 1;
			col = col - 1;
			}
		}
		magicSquare[row][col] = i;
	}

	//rotate the magic square to the right
	for(int b=0; b<n; b++)
		for(int a=0; a<n; a++)
			magicSquareRight[a][b] = magicSquare[n-1-b][a];

	//rotate the magic square to the left
	for(int b=0; b<n; b++)
		for(int a=0; a<n; a++)
			magicSquareLeft[a][b] = magicSquare[b][n-1-a];

	//rotate the magic square 180 degree
	for(int a=0; a<n; a++)
		for(int b=0; b<n; b++)
			magicSquareFlip[a][b] = magicSquare[n-1-a][n-1-b];

	//print out all the results
	cout<<"\nMagic Square #1 is:\n";
	printSquare(magicSquare,n);
	checkSums(magicSquare,n);
	cout<<"\nMagic Square #2 is:\n";
	printSquare(magicSquareRight,n);
	checkSums(magicSquareRight,n);
	cout<<"\nMagic Square #3 is:\n";
	printSquare(magicSquareLeft,n);
	checkSums(magicSquareLeft,n);
	cout<<"\nMagic Square #4 is:\n";
	printSquare(magicSquareFlip,n);
	checkSums(magicSquareFlip,n);

	return 0;
}

// check whether the input size of square is valid
bool validSize(int n){
    if (n%2==0 || n<3 || n>15)
    	return false;
    else
    	return true;
}

// print out the squares
void printSquare (int array[20][20], int n){
	for (int x=0; x<n; x++){
		for (int y=0; y<n; y++){
			cout<<array[x][y]<<"\t";
		}
		cout<<"\n";
	}
	cout<<"\n";
}

// check and print out the sums of rows, columns and diagonals
void checkSums(int array[20][20], int n){
	cout<<"Checking the sums of every row: \t";
	for(int j=0; j<n; j++){
		int sumOfRow = 0;
		for(int k=0; k<n; k++){
			sumOfRow += array[j][k];
		}
		cout<<sumOfRow<<"\t";
	}
	cout<<endl;

	cout<<"Checking the sums of every column: \t";
	for(int k=0; k<n; k++){
		int sumOfCol = 0;
		for(int j=0; j<n; j++){
			sumOfCol += array[j][k];
		}
		cout<<sumOfCol<<"\t";
	}
	cout<<endl;

	cout<<"Checking the sums of every diagonal:\t";
	int sumOfDia1 = 0, sumOfDia2 = 0;
	for (int j=0; j<n; j++) {
		sumOfDia1 += array[j][j];
	}
	for(int j=n-1; j>=0; j--){
		sumOfDia2 += array[j][j];
	}
	cout<<sumOfDia1<<"\t"<<sumOfDia2<<endl;
}
