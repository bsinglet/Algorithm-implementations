/**
 * A simple recursive, depth-first implementation of a Sudoku solver.
 * Completes puzzles extremely quickly.
 *
 * Created by: Benjamin M. Singleton
 * Created: 08-03-2015
 * Finished: 08-03-2015
 */
#include <iostream>

using namespace std;

const int BOARD_SIZE = 9;
int** board;
int*** candidates;

int bestGuessRow;
int bestGuessColumn;

void printBoard()
{
	for (int y = 0; y < BOARD_SIZE; y++)
	{
		for (int x = 0; x < BOARD_SIZE; x++)
		{
			cout << board[y][x] << " ";
		}
		cout << endl;
	}
}

bool isInRow(int row, int val)
{
	if (row < 0 || row > BOARD_SIZE - 1)
	{
		return false;
	}

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		if (board[row][i] == val)
			return true;
	}

	return false;
}

bool isInColumn(int col, int val)
{
	if (col < 0 || col > BOARD_SIZE - 1)
	{
		return false;
	}

	for (int j = 0; j < BOARD_SIZE; j++)
	{
		if (board[j][col] == val)
			return true;
	}

	return false;
}

int getBox(int col, int row)
{
	if (row < 0 || row > BOARD_SIZE - 1 || col < 0 || col > BOARD_SIZE - 1)
		return -1;

	int ret = col / 3;
	if (row > 2 && row < 6)
		ret += 3;
	else if (row > 5)
		ret += 6;
	return ret;
}

bool isInBox(int b, int val)
{
	for (int j = (b / 3) * 3; j < ((b / 3) * 3) + 3; j++)
	{
		for (int i = (b % 3) * 3; i < ((b % 3) * 3) + 3; i++)
		{
			if (board[j][i] == val)
				return true;
		}
	}
	return false;
}

// Returns true if this row contains one and only copy of each number in 1 to
// BOARD_SIZE.
bool solvedRow(int j)
{
	for (int x = 1; x < BOARD_SIZE + 1; x++)
	{
		bool haveFound = false;
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			if (board[j][i] == x)
				haveFound = true;
		}
		if (!haveFound)
			return false;
	}
	return true;
}

bool solvedColumn(int i)
{
	for (int x = 1; x < BOARD_SIZE + 1; x++)
	{
		bool haveFound = false;
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (board[j][i] == x)
				haveFound = true;
		}
		if (!haveFound)
			return false;
	}

	return true;
}

/**
 *  Check if a given box has all the numbers.
 *
 *  Boxes are laid out on the main board like:
 *
 *	0	1	2
 *
 *	3	4	5
 *
 *	6	7	8
 */
bool solvedBox(int b)
{
	for (int x = 1; x < BOARD_SIZE + 1; x++)
	{
		if (!isInBox(b, x))
			return false;
	}
	return true;
}

// Returns true if the board is solved, false if it is incomplete or incorrect.
bool isSolved()
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		if (!solvedRow(i))
		{
			//cout << "Row " << i << " is unsolved." << endl;
			return false;
		}
		if (!solvedColumn(i))
		{
			//cout << "Column " << i << " is unsolved." << endl;
			return false;
		}
		if (!solvedBox(i))
		{
			//cout << "Box " << i << " is unsolved." << endl;
			return false;
		}
	}

	return true;
}

void removeCandidateRow(int row, int val)
{
	val--;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		//if (board[row][i] == 0)
		candidates[row][i][val] = 0;
	}
}

void removeCandidateColumn(int col, int val)
{
	val--;
	for (int j = 0; j < BOARD_SIZE; j++)
	{
		//if (board[j][col] == 0)
		candidates[j][col][val] = 0;
	}
}

void removeCandidateBox(int b, int val)
{
	val--;
	for (int j = (b / 3) * 3; j < ((b / 3) * 3) + 3; j++)
	{
		for (int i = (b % 3) * 3; i < ((b % 3) * 3) + 3; i++)
		{
			//if (board[j][i] == 0)
			candidates[j][i][val] = 0;
		}
	}
}

/**
 *
 */
void updateCandidates()
{
	// First, set all occupied spaces to zero candidates,
	// unoccupied spaces have every number as a candidate.
	for (int x = 0; x < BOARD_SIZE; x++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			for (int i = 0; i < BOARD_SIZE; i++)
			{
				if (board[j][i] == 0)
				{
					candidates[j][i][x] = 1;
				}else
				{
					candidates[j][i][x] = 0;
				}
			}
		}
	}

	// Look through the board.  Every time we find an occupied space,
	// remove that number from every candidate space in its row, column,
	// and box.
	for (int j = 0; j < BOARD_SIZE; j++)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			/**
			if ((j == 7 && i ==8) || (j == 8 && i == 1))
			{
				for (int x = 0; x < BOARD_SIZE; x++)
				{
					cout << candidates[8][0][x] << " ";
				}
			}
			**/
			if (candidates[8][0][8] == 1)
			{
				//cout << "Still alive at (" << i << ", " << j << ") " << endl;
			}

			if (board[j][i] != 0)
			{
				if (i == 0 && j == 8 && candidates[8][0][8] == 1)
					cout << "Row broken." << endl;
				removeCandidateRow(j, board[j][i]);
				if (i == 0 && j == 8 && candidates[8][0][8] == 1)
					cout << "Row works." << endl;
				removeCandidateColumn(i, board[j][i]);
				if (i == 0 && j == 8 && candidates[8][0][8] == 1)
					cout << "Column works." << endl;
				removeCandidateBox(getBox(i, j), board[j][i]);
				if (i == 0 && j == 8 && candidates[8][0][8] == 1)
					cout << "Box works." << endl;
			}
		}
	}
}

int findBestGuess()
{
	int lowestRow = -1;
	int lowestColumn = -1;
	int lowestSum = 10;
	for (int j = 0; j < BOARD_SIZE; j++)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			int sum = 0;
			for (int x = 0; x < BOARD_SIZE; x++)
			{
				sum += candidates[j][i][x];
			}
			if (sum < lowestSum && sum > 0)
			{
				lowestColumn = i;
				lowestRow = j;
				lowestSum = sum;
			}
		}
	}
	bestGuessRow = lowestRow;
	bestGuessColumn = lowestColumn;
}

bool findSolution()
{
	if (isSolved())
	{
		// add to solution list
		printBoard();
		return true;
	}
	updateCandidates();
	findBestGuess();
	if (bestGuessRow == -1 || bestGuessColumn == -1)
	{
		// there are no vacant spaces, so this is a dead end.
		//cout << "Invalid guess (" << bestGuessColumn << ", " << bestGuessRow << ")" << endl;
		return false;
	}

	int row = bestGuessRow;
	int col = bestGuessColumn;
	for (int x = 0; x < BOARD_SIZE; x++)
	{
		if (candidates[row][col][x] == 1)
		{
			board[row][col] = x + 1;
			//cout << "Searching with a best guess of (" << col << ", " << row << ") == " << board[row][col] << endl;
			if (findSolution())
			{
				return true;
			}
			board[row][col] = 0;
		}
	}

	return false;
}

/**
bool findSolution()
{
	if (isSolved())
	{
		// add to solution list
		printBoard();
		return true;
	}

	for (int j = 0; j < BOARD_SIZE; j++)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			if (board[j][i] != 0)
				continue;

			// we have an empty space, so try each number that can go in there.
			for (int x = 1; x < BOARD_SIZE + 1; x++)
			{
				// don't try to place a number if it's already
				// one of it in the same column or row.
				if (candidates[j][i][x] == 0)//isInRow(j, x) || isInColumn(i, x))// isInBox(getBox(i, j), x))
					continue;

				//cout << "Setting (" << i << ", " << j << ") to " << x << endl;
				board[j][i] = x;
				candidates[j][i][x] = 0;
				if (findSolution())
					return true;
				candidates[j][i][x] = 1;
				board[j][i] = 0;
			}
		}
	}

	//printBoard();
	//cout << "Dead end." << endl;
	return false;
}
**/

void loadTestBoard0()
{
	int b[][9] =
	{
		{0, 2, 0, 4, 5, 6, 7, 8, 9},
		{4, 5, 6, 7, 8, 9, 1, 2, 3},
		{7, 0, 0, 0, 2, 3, 0, 5, 6},

		{2, 3, 4, 5, 6, 7, 8, 9, 1},
		{5, 6, 7, 8, 9, 1, 2, 3, 4},
		{0, 0, 0, 0, 0, 0, 5, 6, 7},

		{3, 4, 5, 6, 0, 8, 9, 1, 2},
		{0, 7, 0, 9, 0, 2, 3, 4, 5},
		{0, 1, 2, 3, 0, 5, 6, 7, 0}
	};

	for (int j = 0; j < BOARD_SIZE; j++)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			board[j][i] = b[j][i];
		}
	}
}

void loadTestBoard1()
{
	int b[][9] =
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0},

		{0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0},

		{0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0}
	};

	for (int j = 0; j < BOARD_SIZE; j++)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			board[j][i] = b[j][i];
		}
	}
}

void loadTestBoard2()
{
	int b[][9] =
	{
		{5, 3, 0, 0, 7, 0, 0, 0, 0},
		{6, 0, 0, 1, 9, 5, 0, 0, 0},
		{0, 9, 8, 0, 0, 0, 0, 6, 0},

		{8, 0, 0, 0, 6, 0, 0, 0, 3},
		{4, 0, 0, 8, 0, 3, 0, 0, 1},
		{7, 0, 0, 0, 2, 0, 0, 0, 6},

		{0, 6, 0, 0, 0, 0, 2, 8, 0},
		{0, 0, 0, 4, 1, 9, 0, 0, 5},
		{0, 0, 0, 0, 8, 0, 0, 7, 9}
	};

	for (int j = 0; j < BOARD_SIZE; j++)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			board[j][i] = b[j][i];
		}
	}
}

void loadTestBoard3()
{
	int b[][9] =
	{
		{0, 2, 3, 4, 5, 6, 7, 8, 9},
		{4, 5, 6, 7, 8, 9, 1, 2, 3},
		{7, 8, 0, 1, 2, 3, 4, 5, 6},

		{2, 3, 4, 5, 6, 7, 8, 9, 1},
		{5, 6, 7, 8, 9, 1, 2, 3, 4},
		{8, 9, 1, 2, 3, 4, 5, 6, 7},

		{3, 4, 5, 6, 7, 8, 9, 1, 2},
		{6, 7, 8, 9, 1, 2, 3, 4, 5},
		{9, 1, 2, 3, 4, 5, 6, 7, 8}
	};

	for (int j = 0; j < BOARD_SIZE; j++)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			board[j][i] = b[j][i];
		}
	}
}

int main(int argc, char** argv)
{
	board = new int*[BOARD_SIZE];
	for (int j = 0; j < BOARD_SIZE; j++)
	{
		board[j] = new int[BOARD_SIZE];
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			board[j][i] = 0;
		}
	}

	candidates = new int**[BOARD_SIZE];
	for (int k = 0; k < BOARD_SIZE; k++)
	{
		candidates[k] = new int*[BOARD_SIZE];
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			candidates[k][j] = new int[BOARD_SIZE];
			for (int i = 0; i < BOARD_SIZE; i++)
			{
				candidates[k][j][i] = 1;
			}
		}
	}

	loadTestBoard2();
	updateCandidates();

	/**
	for (int j = 7; j < BOARD_SIZE; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			cout << "Candidates for (" << i << ", " << j << "):" << endl;
			for (int x = 0; x < BOARD_SIZE; x++)
			{
				if (candidates[j][i][x] == 1)
				{
					cout << (x + 1) << " ";
				}else
				{
					cout << "X ";
				}
			}
			cout << endl;
		}
	}
	**/

	//findBestGuess();
	findSolution();

	return 0;
}

