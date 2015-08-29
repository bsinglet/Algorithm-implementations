/**
 * N-Queens Problem Solver
 * Uses a simple recursive, depth-first approach to finding every solution to
 * the n-queens problem for a given n.
 *
 * Created by Benjamin M. Singleton
 * Created: 07-31-2015
 * Modified:
*/

#include <iostream>
#include <cstdlib>
#include <ctime>

int BOARD_SIZE;
const int SOLUTION_SIZE = 400;
int** board;
int*** solutions;
int num_solutions;

using namespace std;

/**
 * Checks to see if a given space is safe for a queen to occupy.  It does this
 * this by looking for other queens in the same row as it, the same column,
 * the same upper-left to lower-right diagonal, and lastly the same lower-
 * left to upper-right diagonal.  It doesn't check whether or not the given
 * space itself is occupied, since this function is needed both when placing
 * a queen and when seeing if the board is solved Returns true if the space is
 * safe to occupy, false if not.
 */
bool isValid(int x, int y)
{
	if (x < 0 || x > BOARD_SIZE - 1 || y < 0 || y > BOARD_SIZE - 1)
		return false;

	// check other spaces in this row
	for (int col = 0; col < BOARD_SIZE; col++)
	{
		if (col != x && board[y][col] == 1)
			return false;
	}

	// check other spaces in this column
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		if (row != y && board[row][x] == 1)
			return false;
	}

	// check the upper-left to lower-right diagonal
	int diag_y = y - x;
	for (int diag_x = 0; diag_x < BOARD_SIZE; diag_x++)
	{
		if (diag_y < 0 || diag_y > BOARD_SIZE - 1 || (diag_x == x && diag_y == y))
		{
			diag_y += 1;
			continue;
		}else if (board[diag_y][diag_x] == 1)
		{
			return false;
		}
		diag_y += 1;
	}

	// check the lower-left to upper-right diagonal
	diag_y = y + x;
	for (int diag_x = 0; diag_x < BOARD_SIZE; diag_x++)
	{
		if (diag_y < 0 || diag_y > BOARD_SIZE - 1 || (diag_x == x && diag_y == y))
		{
			diag_y -= 1;
			continue;
		}else if (board[diag_y][diag_x] == 1)
		{
			return false;
		}
		diag_y -= 1;
	}

	return true;
}

/**
 * Checks if the current board is solved by making sure each row has a queen in
 * it and that each queen is in a spot where they aren't threatening another
 * queen.
 */
bool isSolved()
{
	for (int y = 0; y < BOARD_SIZE; y++)
	{
		int sum = 0;
		for (int x = 0; x < BOARD_SIZE; x++)
		{
			sum += board[y][x];
			//if (board[y][x] == 1 && !isValid(x, y))
			//	return false;
		}
		if (sum != 1)
			return false;
	}
	return true;
}

/**
 * Returns true if the current global board is not contained in the set of
 * solutions found so far.
 */
bool notDuplicate()
{
	for (int i = 0; i < num_solutions; i++)
	{
		bool skipToNext = false;
		for (int y = 0; y < BOARD_SIZE; y++)
		{
			for (int x = 0; x < BOARD_SIZE; x++)
			{
				if (board[y][x] != solutions[i][y][x])
				{
					skipToNext = true;
					break;
				}
			}
			if (skipToNext)
			{
				break;
			}
		}
		if (!skipToNext)
		{
			return false;
		}
	}

	return true;
}

/**
 * Recursive, depth-first search algorithm for the n queens problem.  This is a
 * pretty brute-force approach that is not very memory-intensive but is extremely
 * processor intensive as n increases.
 */
void findSolutions(int depth)
{
	if (isSolved())
	{
		if (notDuplicate())
		{
			solutions[num_solutions] = new int*[BOARD_SIZE];
			for (int y = 0; y < BOARD_SIZE; y++)
			{
				solutions[num_solutions][y] = new int[BOARD_SIZE];
				for (int x = 0; x < BOARD_SIZE; x++)
				{
					solutions[num_solutions][y][x] = board[y][x];
				}
			}
			num_solutions++;
		}
		return;
	}

	for (int y = 0; y < BOARD_SIZE; y++)
	{
		// make sure this row is empty
		int sum = 0;
		for (int x = 0; x < BOARD_SIZE; x++)
		{
			sum += board[y][x];
		}
		if (sum > 0)
			continue;
		for (int x = 0; x < BOARD_SIZE; x++)
		{
			if (isValid(x, y))
			{
				board[y][x] = 1;
				findSolutions(depth+1);
				board[y][x] = 0;
			}
		}
	}
}

/**
 * Prints out a given board.
 */
void printBoard(int** b)
{
	for (int y = 0; y < BOARD_SIZE; y++)
	{
		for (int x = 0; x < BOARD_SIZE; x++)
		{
			if (b[y][x] == 0)
			{
				cout << "_ ";
			}else
			{
				cout << "Q ";
			}
		}

		cout << endl;
	}
}

/**
 * Prints out the board for every solution found.
 */
void printSolutions()
{
	for (int i = 0; i < num_solutions; i++)
	{
		cout << "Solution #" << i + 1 << ":" << endl;
		printBoard(solutions[i]);
		cout << endl;
	}
}

/**
 * Run the search, using a nxn board as specified from the command line.
 */
int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "Usage:" << endl;
		cout << "nqueens n" << endl;
		cout << "where n is the number of queens." << endl;
		return -1;
	}

	BOARD_SIZE = 0;
	try
	{
		BOARD_SIZE = atoi(argv[1]);
	}catch (int e)
	{
		cout << "Invalid argument " << argv[1] << endl;
		return -1;
	}

	if (BOARD_SIZE < 5 || BOARD_SIZE > 20)
	{
		cout << "Whoa, there!  We're not running anything that big." << endl;
		return -1;
	}

	board = new int*[BOARD_SIZE];
	solutions = new int**[SOLUTION_SIZE];

	for (int y = 0; y < BOARD_SIZE; y++)
	{
		board[y] = new int[BOARD_SIZE];
		for (int x = 0; x < BOARD_SIZE; x++)
		{
			board[y][x] = 0;
		}
	}

	for (int i = 0; i < SOLUTION_SIZE; i++)
	{
		solutions[i] = 0;
	}

	num_solutions = 0;

	clock_t start = clock();
	findSolutions(0);
	clock_t end = clock();

	double seconds = double(end - start) / CLOCKS_PER_SEC;

	printSolutions();

	cout << "Found  " << num_solutions << " solutions in " << seconds << " seconds." << endl;
}

