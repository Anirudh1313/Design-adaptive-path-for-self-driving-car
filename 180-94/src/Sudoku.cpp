#include "Sudoku.h"
#include <thread>
#include <iostream>

Sudoku::Sudoku(int board[9][9]) {
	this->multithread = true;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			this->board[i][j] = board[i][j];
			this->solution[i][j] = board[i][j];
		}
	}
	solvable = solve();
}

Sudoku::Sudoku(int board[9][9], bool multithread) {
	this->multithread = multithread;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			this->board[i][j] = board[i][j];
			this->solution[i][j] = board[i][j];
		}
	}
	solvable = solve();
}

void Sudoku::setMultithread(bool b) {multithread = b;}

void Sudoku::newBoard(int board[9][9]) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			this->board[i][j] = board[i][j];
			this->solution[i][j] = board[i][j];
		}
	}
	solvable = solve();
}


void Sudoku::printBoard() {
	cout << "\n|-----|-----|-----|" << endl;
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++) {
			if (board[i][j]) cout << "|" << board[i][j];
			else cout << "| ";
		}
		cout << "|" << endl;

		if((i+1)%3 == 0)
			cout << "|-----|-----|-----|" << endl;
	}
}
void Sudoku::printSolution() {
	if (!solvable) {
		cout << "The board is not solvable.\n";
		return;
	}
	cout << "\n|-----|-----|-----|" << endl;
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++) {
			if (solution[i][j]) cout << "|" << solution[i][j];
			else cout << "| ";
		}
		cout << "|" << endl;

		if((i+1)%3 == 0)
			cout << "|-----|-----|-----|" << endl;
	}
}
bool Sudoku::check() {
	if (multithread) {
		thread rowCheckers[9], colCheckers[9], sqrCheckers[9];
		for (int i = 0; i < 9; i++) {
			rowCheckers[i] = getRowThread(i);
			colCheckers[i] = getColThread(i);
			sqrCheckers[i] = getSqrThread(i);
		}
		for (int i = 0; i < 9; i++) {
			rowCheckers[i].join();
			colCheckers[i].join();
			sqrCheckers[i].join();
		}
	}

	else {
		for (int i = 0; i < 9; i++){
			checkRow(i);
			checkCol(i);
			checkSqr(i);
		}
	}

	for (bool b : rowValid) if (!b) return false;
	for (bool b : colValid) if (!b) return false;
	for (bool b : sqrValid) if (!b) return false;
	return true;
}

bool Sudoku::check(int row, int col) {
	int sqr = (row/3)*3+(col/3);

	if (multithread) {
		thread rowChecker = getRowThread(row);
		thread colChecker = getColThread(col);
		thread sqrChecker = getSqrThread(sqr);
		rowChecker.join();
		colChecker.join();
		sqrChecker.join();

	}

	else {
		checkRow(row);
		checkCol(col);
		checkSqr(sqr);
	}

	if (rowValid[row] && colValid[col] && sqrValid[sqr]) return true;
	return false;
}

bool Sudoku::solve() {
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c < 9; c++) {
			if (!solution[r][c]) {
				for (int n = 1; n <= 9; n++) {
					solution[r][c] = n;
					if (check(r,c) && solve()) return true;
					solution[r][c] = 0;
				}
				return false;
			}
		}
	}
	return true;
}

void Sudoku::checkRow(int row) {
	bool nums[10];
	for (int i = 0; i < 10; i++) nums[i] = false;

	for (int c = 0; c < 9; c++) {
		int n = solution[row][c];
		if (n && nums[n]) {
			rowValid[row] = false;
			return;
		}
		nums[n] = true;
	}
	rowValid[row] = true;
}

void Sudoku::checkCol(int col) {
	bool nums[10];
	for (int i = 0; i < 10; i++) nums[i] = false;

	for (int r = 0; r < 9; r++) {
		int n = solution[r][col];
		if (n && nums[n]) {
			colValid[col] = false;
			return;
		}
		nums[n] = true;
	}
	colValid[col] = true;
}

void Sudoku::checkSqr(int sqr) {
	bool nums[10];
	for (int i = 0; i < 10; i++) nums[i] = false;

	int rstart = (sqr/3)*3, cstart = (sqr%3)*3;
	for (int r = rstart; r < rstart + 3; r++) {
		for (int c = cstart; c < cstart + 3; c++) {
			int n = solution[r][c];
			if (n && nums[n]) {
				sqrValid[sqr] = false;
				return;
			}
			nums[n] = true;
		}
	}
	sqrValid[sqr] = true;
}

thread Sudoku::getRowThread(int row) {
	return thread(&Sudoku::checkRow, this, row);
}

thread Sudoku::getColThread(int col) {
	return thread(&Sudoku::checkCol, this, col);
}

thread Sudoku::getSqrThread(int sqr) {
	return thread(&Sudoku::checkSqr, this, sqr);
}
