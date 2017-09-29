#include <iostream>
#include <chrono>
#include "Sudoku.h"

using namespace std;
typedef chrono::high_resolution_clock Time;
typedef chrono::nanoseconds ns;

int main()
{
	int board[9][9] = {
			{0,2,0, 5,0,9, 0,8,0},
			{5,0,9, 0,2,0, 6,0,4},
			{0,3,0, 6,0,4, 0,9,0},

			{1,4,3, 8,6,5, 7,2,9},
			{9,5,8, 2,4,7, 3,6,1},
			{7,6,2, 3,9,1, 4,5,8},

			{3,7,1, 9,5,6, 8,4,2},
			{4,9,6, 1,8,2, 5,7,3},
			{2,8,5, 4,7,3, 9,1,6}};

	auto start = Time::now();

	Sudoku s(board, false); // set true for multithreading, false for traditional
	cout << "Entered board:\n";
	s.printBoard();
	cout << "\nSolution:\n";
	s.printSolution();

	auto end = Time::now();
	cout << "Time Taken: " << chrono::duration_cast<ns>(end-start).count() << endl;

	return 0;
}
