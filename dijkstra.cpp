/**
 * Implementation of Dijkstra's Algorithm in C++.
 *
 * Created by: Benjamin M. Singleton
 * Created: 08-17-2015
 * Modified: 08-19-2015
 */

#include <iostream>
#include <vector>

using namespace std;

#define VACANT 0

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#define INFINITE 30000

const int MAX_LOCATION_LIST_SIZE = 500;
const int BOARD_SIZE = 10;

class GraphNode
{
public:
	GraphNode();
	GraphNode(int new_x, int new_y);
	int x, y, distance;
	bool visited;
};

GraphNode::GraphNode()
{
	x = -1;
	y = -1;
	distance = INFINITE;
	visited = false;
}

GraphNode::GraphNode(int new_x, int new_y)
{
	this->x = new_x;
	this->y = new_y;
	distance = INFINITE;
	visited = false;
}

GraphNode*** myNodes;

/**
 * Populates the global array myNodes usings the provided array, starting, and
 * end locations.
 */
void buildGraphList(int** myBoard, int start_x, int start_y, int end_x, int end_y)
{
	// first, build all the nodes/
	myNodes = new GraphNode**[BOARD_SIZE];
	for (int j = 0; j < BOARD_SIZE; j++)
	{
		myNodes[j] = new GraphNode*[BOARD_SIZE];
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			if (myBoard[j][i] == VACANT)
			{
				myNodes[j][i] = new GraphNode();
				myNodes[j][i]->x = i;
				myNodes[j][i]->y = j;
				if (i == start_x && j == start_y)
				{
					// zero starting distance.
					myNodes[j][i]->distance = 0;
				}
			}else
			{
				myNodes[j][i] = 0;
			}
		}
	}
}

void printDistances()
{
	for (int j = 0; j < BOARD_SIZE; j++)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			if (myNodes[j][i] != 0)
			{
				cout << myNodes[j][i]->distance << " ";
			}else
			{
				cout << "! ";
			}
		}
		cout << endl;
	}
}

GraphNode* findClosestUnvisited()
{
	int closest = INFINITE;
	GraphNode* ret = 0;
	for (int j = 0; j < BOARD_SIZE; j++)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			if (myNodes[j][i] != 0 && myNodes[j][i]->visited == false && myNodes[j][i]->distance < closest)
			{
				closest = myNodes[j][i]->distance;
				ret = myNodes[j][i];
			}
		}
	}

	return ret;
}

/**
 * Finds the shortest path to the end location using the priority queue l.
 */
vector<GraphNode*>* dijkstra(int end_x, int end_y)
{
	vector<GraphNode*>* path = new vector<GraphNode*>();

	while (findClosestUnvisited() != 0)
	{
		GraphNode* g = findClosestUnvisited();
		myNodes[g->y][g->x]->visited = true;

		// check the spot above.
		if (g->y > 0 && myNodes[g->y-1][g->x] != 0 && myNodes[g->y-1][g->x]->distance == INFINITE) //> g->distance+1)
		{
			// update distance
			myNodes[g->y-1][g->x]->distance = g->distance + 1;
		}

		// check the spot to the right.
		if (g->x < BOARD_SIZE-1 && myNodes[g->y][g->x+1] != 0 && myNodes[g->y][g->x+1]->distance == INFINITE) //> g->distance+1)
		{
			// update distance
			myNodes[g->y][g->x+1]->distance = g->distance + 1;
		}

		// check the spot below.
		if (g->y < BOARD_SIZE-1 && myNodes[g->y+1][g->x] != 0 && myNodes[g->y+1][g->x]->distance == INFINITE) //> g->distance+1)
		{
			// update distance
			myNodes[g->y+1][g->x]->distance = g->distance + 1;
		}

		// check the spot to the left.
		if (g->x > 0 && myNodes[g->y][g->x-1] != 0 && myNodes[g->y][g->x-1]->distance == INFINITE) //> g->distance+1)
		{
			// update distance
			myNodes[g->y][g->x-1]->distance = g->distance + 1;
		}
	}

	// so, now that all the distances are correct, let's backtrack from the goal to the start.
	vector<GraphNode*>* v = new vector<GraphNode*>();
	GraphNode* g = myNodes[end_y][end_x];
	while (g->distance != 0)
	{
		v->push_back(g);
		//cout << "At " << "(" << g->x << ", " << g->y << ")" << endl;
		// check the spot above.
		if (g->y > 0 && myNodes[g->y-1][g->x] != 0 && myNodes[g->y-1][g->x]->distance < g->distance)
		{
			g = myNodes[g->y-1][g->x];
			continue;
		}

		// check the spot to the right.
		if (g->x < BOARD_SIZE-1 && myNodes[g->y][g->x+1] != 0 && myNodes[g->y][g->x+1]->distance < g->distance)
		{
			g = myNodes[g->y][g->x+1];
			continue;
		}

		// check the spot below.
		if (g->y < BOARD_SIZE-1 && myNodes[g->y+1][g->x] != 0 && myNodes[g->y+1][g->x]->distance < g->distance)
		{
			g = myNodes[g->y+1][g->x];
			continue;
		}

		// check the spot to the left.
		if (g->x > 0 && myNodes[g->y][g->x-1] != 0 && myNodes[g->y][g->x-1]->distance < g->distance)
		{
			g = myNodes[g->y][g->x-1];
			continue;
		}
	}

	v->push_back(g);
	return v;
}

int main(int argc, char* arg[])
{
	//priorityQueueTest();

	// initialize the board
	int** myBoard;
	myBoard = new int*[BOARD_SIZE];
	for (int j = 0; j < BOARD_SIZE; j++)
	{
		myBoard[j] = new int[BOARD_SIZE];
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			myBoard[j][i] = VACANT;
		}
	}

	// set up some obstacles
	myBoard[9][3] = 1;
	myBoard[8][1] = 1;
	myBoard[7][1] = 1;
	myBoard[1][8] = 1;
	myBoard[1][7] = 1;
	myBoard[1][6] = 1;
	myBoard[2][8] = 1;
	myBoard[3][8] = 1;

	for (int j = 0; j < BOARD_SIZE; j++)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			cout << myBoard[j][i] << " ";
		}
		cout << endl;
	}

	// build our list of graph nodes
	buildGraphList(myBoard, 0, 9, 9, 0);

	// get the shortest path from the dijkstra function
	vector<GraphNode*>* v = dijkstra(9, 0);

	// print out the path from start to end
	cout << endl << "Shortest path:" << endl;
	while (v->size() > 0)
	{
		GraphNode* g = v->back();
		v->pop_back();
		cout << "(" << g->x << ", " << g->y << ")" << endl;
	}

	return 0;
}
