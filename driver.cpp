#include <iostream>
#include <cstdlib>
#include "sudoku.h"
using namespace std;

Sudoku* solve(Sudoku *puzzle){
	puzzle->horSolve(); //initiate horizontal solve
	puzzle->verSolve(); //initiate vertical solve
	puzzle->boxSolve(); //initiate box solve
	int result = puzzle->finalize(); //initiate finalize and store boolean
	if ( result == -1) {//if failure
		delete puzzle;
		return NULL;
	}
	else if (puzzle->isSolved()) {//if puzzle is solved, return puzzle
		return puzzle;
	}
	else if (result==0) {//if no more solvable sells
		//create a new puzzle with a cell finalized to the guessed candidate
		//and eliminate the guesses candidate from the original puzzle
		Sudoku *newPuzzle = puzzle->guess();//create a puzzle with a guesses value
		newPuzzle = solve(newPuzzle);//recurse with the new puzzle
		if (newPuzzle == NULL) {//if newPuzzle fails
			return solve(puzzle);
                }
		delete puzzle;
		return newPuzzle;
	}
	else {
		return solve(puzzle);//return the original puzzle
	}
}

int main() {
	//remove the spaces from the input
        std::string input;
 	string s;
        do {
                cin >> input;
                s.append(input);
        }
        while(s.length() < 81);

	Sudoku *givenSudoku = new Sudoku(s); // stores given input into Sudoku puzzle
	cout << *givenSudoku << endl;//outputs the input puzzle
	Sudoku *sudokuNew = solve(givenSudoku);//solve the sudoku
	if (sudokuNew == NULL){
	cerr << "ERROR bad Sudoku" << endl;
	return 0;
	}
	else {
	cout << *sudokuNew;//output the solved sudoku
	delete sudokuNew;
	return 0;}
}
