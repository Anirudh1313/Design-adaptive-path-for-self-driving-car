#ifndef SUDOKU_H_
#define SUDOKU_H_

#include <thread>

using namespace std;

class Sudoku {
public:
	Sudoku(int board[9][9]);
	Sudoku(int board[9][9], bool multithread);
	void setMultithread(bool b);
	void newBoard();
	void newBoard(int board[9][9]);
	void printBoard();
	void printSolution();
	bool check();
	bool check(int row, int col);
	bool solve();
	void checkRow(int row);
	void checkCol(int col);
	void checkSqr(int sqr);
	thread getRowThread(int row);
	thread getColThread(int col);
	thread getSqrThread(int sqr);

	bool rowValid[9];
	bool colValid[9];
	bool sqrValid[9];

private:
	int board[9][9];
	int solution[9][9];
	bool solvable;
	bool multithread;
};

#endif /* SUDOKU_H_ */
