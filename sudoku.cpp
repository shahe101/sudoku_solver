#include "sudoku.h"
using namespace std;

bool Sudoku::eliminate(int row, int col, int val) {
	bool boolValue;
	cout << "Did row for " << val << endl;
	//remove val as possible candidate from cells in same row
	for (int i = 0; i < 9; i++) {
		if (i == col){continue;}//avoid deleting val at [row][col]
		if (puzzle[row][i]->possibilities() == 0){return false;}//check if cell has no candidates
		if (puzzle[row][i]->at(val)){
			boolValue = puzzle[row][i]->removeCandidate(val);
			if (boolValue == false) {//if removing candidate results in false, return false
				return false;
			}
			if (puzzle[row][i]->possibilities() == 1){//if after removing we have one candidate
				int num = puzzle[row][i]->first();
				SolvedCell *stackElement = new SolvedCell(row, i, num);
				finalStack.push(stackElement);//then add the candidate to finalStack
			}
		}
		if (puzzle[row][i]->possibilities() == 0){
			return false;
		}
	}
	cout << "Did col for " << val << endl;
	//remove val as possible candidate from cells in same column
	 for (int j = 0; j < 9; j++) {
                if (j == row){continue;}//avoid deleting val at [row][col]
                if (puzzle[j][col]->possibilities() == 0){return false;}//check if cell has no candidates
                if (puzzle[j][col]->at(val)){
                        boolValue = puzzle[j][col]->removeCandidate(val);
                        if (boolValue == false) {//if removing candidate results in false, return false
                                return false;
                        }
                        if (puzzle[j][col]->possibilities() == 1){//if after removing we have one candidate, add candidate to stack
                                int num = puzzle[j][col]->first();
                                SolvedCell *stackElement = new SolvedCell(j, col, num);
                                finalStack.push(stackElement);//then add the candidate to finalStack
                        }
                }
                if (puzzle[j][col]->possibilities() == 0){
                        return false;
                }
        }
	cout << "Did box for " << val << endl;
	// remove val as possible candidate from cells in same box
	int lowRow = 3*(row/3);
	int lowCol = 3*(col/3);
	for (int i = 0; i<9; i++) {
		int rowCurrent = lowRow + (i/3);
		int colCurrent = lowCol + (i%3);
		if (rowCurrent == row && colCurrent == col) {//avoid deleting val at [row][col]
			continue; }
		if(puzzle[rowCurrent][colCurrent]->possibilities() == 0){//if there are no candidates, return 0
			return false;
		}
		if (puzzle[rowCurrent][colCurrent]->at(val)){
			boolValue = puzzle[rowCurrent][colCurrent]->removeCandidate(val);//if removing candidate results in false, return false
			if (boolValue == false) {
				return false;
			}
			if (puzzle[rowCurrent][colCurrent]->possibilities()==1) {//if after removing we have one candidate, add the candidate to final stack
				int num = puzzle[rowCurrent][colCurrent]->first();
				SolvedCell *stackElement = new SolvedCell(rowCurrent, colCurrent, num);
				finalStack.push(stackElement);
			}
		}
		if (puzzle[rowCurrent][colCurrent]->possibilities() == 0){
			return false;
		}
	}
	return true;
}

//Constructor
Sudoku::Sudoku() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			puzzle[i][j] = new Cell();
		}
	}
}

//Initialize a Sudoku with the translated string
Sudoku::Sudoku(std::string s) {
	int i = 0;
	for (int j = 0; j < 9; j++) {
		for (int k = 0; k < 9; k++) {
			if (s[i]=='.') { //if the character is ".", then create cell with 9 candidates
				i++;
				puzzle[j][k] = new Cell();
			} //if string is ., do nothing
			else { //else create a Cell to the int and add it to finalStack
				int num  = s[i]-48;
				i++;
				puzzle[j][k] = new Cell(num);
				SolvedCell *stackElement = new SolvedCell(j, k, num);
                                finalStack.push(stackElement);
			}
		}
	}
}

//Output
std::ostream &operator<<(std::ostream & stream, const Sudoku & puzzleState) {
	for (int i=0; i<9; i++) {
		for (int j=0; j<9; j++) {
			if (puzzleState.puzzle[i][j]->possibilities() > 1) { //output . if the cell has more than 1 candidate
				stream << ".";
			}
			else { //else output the 1 candidate
				stream << puzzleState.puzzle[i][j]->first(); 
			}
			stream << " ";
		}
		stream << std::endl;
	}
	return stream;
}

//Destructor
Sudoku::~Sudoku() {
	for (int i = 0; i < 9; i++) { //delete all cells in puzzle
		for (int j = 0; j < 9; j++) {
			delete puzzle[i][j];
		}
	}
	while (!finalStack.empty()){//while finalStack is not empty, delete the SolvedCells in finalStack
		delete finalStack.top();
		finalStack.pop();
	}
}

void Sudoku::horSolve() {
	for (int r = 0; r < 9; r++) {
		int count = 0;//number of times i is a candidate in the row
		int row=0;
		int col=0;
		for (int i = 1; i < 10; i++) {
			for (int c = 0; c < 9; c++) {
				if(puzzle[r][c]->at(i)){
					count++;//increment number of times i is a candidate in the row
					row = r;
					col = c;
				}
			}
			if (count == 1 && (puzzle[row][col]->possibilities() > 1)) {//if the count is 1 and if the cell has 1 candidate, add the candidate to finalStack
				SolvedCell *stackElement = new SolvedCell(row, col, i);
				finalStack.push(stackElement);
			}
		}
	}
}

void Sudoku::verSolve() {
        for (int c = 0; c < 9; c++) {
		int count = 0;//number of times i is a candidate in the column
		int row = 0;
		int col = 0;
                for (int i = 1; i < 10; i++) {
                        for (int r = 0; r < 9; r++) {
                                if(puzzle[r][c]->at(i)==true){
                                        count++;//increment number of times i is a candidate in the column
                                        row = r;
                                        col = c;
                                }
                        }
                        if (count == 1 && puzzle[row][col]->possibilities() > 1) {//if the count is 1 and if the cell has 1 candidate, add the candidate to finalStack
                                SolvedCell *stackElement = new SolvedCell(row, col, i);
                                finalStack.push(stackElement);
                        }
                }
        }
}

void Sudoku::boxSolve(){
        for (int cand = 1; cand < 10; cand++) {
		int rowIndex = 0;
		int colIndex = 0;
                for (int r = 0; r < 3; r++) {
			int count = 0;//number of times i is a candidate in the box
                        for (int c = 0; c < 3; c++) {
                                for (int i = 0; i < 3; i++) {
                                        for (int j = 0; j < 3; j++) {
                                             	 int row = i + (r*3);
						 int col = j + (c*3);
						 if (puzzle[row][col]->at(cand)){
                                                        count++;//increment number of times i is a candidate in the box
                                                        rowIndex = row;
                                                        colIndex = col;
                                                }
                                        }
                                }
                                if (count == 1 && puzzle[rowIndex][colIndex]->possibilities() > 1) {//if the count is 1 and if the cell has 1 candidate, add the candidate to finalStack
                                        SolvedCell *stackElement = new SolvedCell(rowIndex, colIndex, cand);
                                        finalStack.push(stackElement);
                                }
                        }
                }
        }
}

//Copy Constructor
Sudoku::Sudoku (const Sudoku& other){
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c < 9; c++) {
			puzzle[r][c] = new Cell(*other.puzzle[r][c]);
		}
	}
}


int Sudoku::finalize() {
	if (finalStack.empty()) {//return 0 if finalStack is empty
		return 0;
	}
	while( !finalStack.empty() ) {//whiel finalStack is not empty
		SolvedCell *element = finalStack.top();
		delete puzzle[element->row][element->col];
		puzzle[element->row][element->col] = new Cell(element->val); //add the element from finalStack to the puzzle
		finalStack.pop(); //remove the element from finalStack
		bool boolValue = eliminate(element->row, element->col, element->val); //apply eliminate to the element
		delete element;
		if (boolValue == false) { return -1; }
	}
	return 1;
}

Sudoku* Sudoku::guess() {
	int minNumProb = 10; //minimum number of candidates in a cell
	int num;
	int col;
	int row;
	Sudoku *temp = new Sudoku(*this); //create a copy of the puzzle
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			// if the cell has more than one possibility and if the number of possibilities is less than minimum number of candidates...
			if ((puzzle[i][j]->possibilities() > 1) && (puzzle[i][j]->possibilities() < minNumProb)) {
				minNumProb = puzzle[i][j]->possibilities();//then set the cell with the least number of candidates to be number of possibilities for that cell
				num = puzzle[i][j]->first();
				row = i;
				col = j;
			}
		}
	}
	puzzle[row][col]->removeCandidate(num); //remove num (the lowest candidate) as a candidate from cell
	if (puzzle[row][col]->possibilities() == 1){ //if the cell has one possibility, then add the value of the cell to finalStack
		int firstVal = puzzle[row][col]->first();
		SolvedCell *stackElement = new SolvedCell(row, col, firstVal);
                finalStack.push(stackElement);
	}
	SolvedCell *branchStackElement = new SolvedCell(row, col, num);
	temp->finalStack.push(branchStackElement);
	return temp;
}

//returns true if the Sudoke is solve (that is, all cells have 1 candidate)
bool Sudoku::isSolved() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (puzzle[i][j]->possibilities() != 1) {
				return false;
			}
		}
	}
	return true;
}
