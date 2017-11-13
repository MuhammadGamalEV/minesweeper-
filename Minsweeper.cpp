#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <ctime>
#include <time.h>
#include <string>

#include <windows.h>
#include <chrono>

#include <utility>
#include <conio.h>
#include <functional>

using namespace std;
/****************************************************Copied*/
#pragma warning(disable : 4996)
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;
void gotoXY(int x, int y)
{
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}
/****************************************************/

//typedef
typedef vector<vector<char> > matrix;

//below are our minesweeper map dimensions and number of mines
 int x_dim, y_dim, numMines;

/********************************************
Add a mine to our map
returns true if mine added and false when not.
updates proximity values in the map.
/*********************************************/
bool addMine(matrix& map)
{
	//Generate a random location for this mine
	int x = rand() % x_dim, y = rand() % y_dim;

	//Add the mine, if one isn't already there
	if (map[x][y] != '*')
	{
		map[x][y] = '*';
		//walk through all neighboring locations and increment their proximity counts
		for (int dx = x - 1; dx <= x + 1; dx++)
		for (int dy = y - 1; dy <= y + 1; dy++)
		if (dx >= 0 && dx < x_dim && dy >= 0 && dy < y_dim)
		if (map[dx][dy] != '*') //Don't update proximity count for mine locations
		if (map[dx][dy] == '.')
			map[dx][dy] = '1'; //initial proximity, set to 1
		else
			map[dx][dy]++; //otherwise, increment current count (works even with chars so long as count never exceeds 9)
		return true;
	}
	return false;
}


/*****************************************************************
reveals the specified location of the map/mask and expands areas that are not a mine
returns true if a mine was revealed.
******************************************************************/
bool revealLocation(matrix& map, matrix& mask, int x, int y)
{
	//reveal the location
	mask[x][y] = '.';

	//if location is a mine, game over, just return true
	if (map[x][y] == '*')
		return true;

	//If our location isn't in proximity to a mine
	//we reveal all neighboring locations
	//dots indicate no neighboring mine
	if (map[x][y] == '.')
	{
		//openLocations holds neighboring locations that also
		//are not in proximity to a mine
		queue<pair<int, int> > openLocations;
		openLocations.push(make_pair(x, y));

		//Walk through all dot locations and reveal their neighboars
		while (!openLocations.empty())
		{
			//Get the next location from our queue
			pair<int, int> next = openLocations.front();

			//The two for loops iterate over a 3x3 block within our map
			//surrounding the point next.  It will check the point itself
			//as well, which is redundant, but we hardly need highly
			//optimized code here
			for (int dx = next.first - 1; dx <= next.first + 1; dx++)
			{
				for (int dy = next.second - 1; dy <= next.second + 1; dy++)
				{
					//Let's make sure the current location is within the
					//bounds of our map.  If next is an edge location, then
					//we'll be iterating over some points outside the map
					//So just ignore those points
					if (dx >= 0 && dx < x_dim && dy >= 0 && dy < y_dim)
					{
						//if this neighbor is a dot location and hasn't
						//previously been revealed, add it to our list
						if (map[dx][dy] == '.' && mask[dx][dy] == '#')
							openLocations.push(make_pair(dx, dy));

						//reveal this neighboring location
						mask[dx][dy] = '.';

					}
				}
			}
			//We're done with the current location in our queue, so we can remove it
			openLocations.pop();
		}
	}

	return false;
}

/******************************************************
returns a count of the remaining masked locations
used to determine if only mines are masked (ie our win condition)
*******************************************************/
int countMask(matrix& mask)
{
	int count = 0;
	for (int x = 0; x < x_dim; x++)
	for (int y = 0; y < y_dim; y++)
	if (mask[x][y] == '#') count++;

	return count;
}
/******************************************************
returns a count of the unmasked locations and it's not a mine.
used to calculate the score.
*******************************************************/
int countMap(matrix& map)
{
	int count = 0;
	for (int x = 0; x < x_dim; x++)
	for (int y = 0; y < y_dim; y++)
	if (map[x][y] != '#'||map[x][y]!='*') count++;

	return count;
}




int main()
{
	string names[10000];
	double arr[10000];
	int i = 0;
	/*************************Copied*/
	//gotoXY(10, 10);
	/**************************/
	//int x=0;
	cin >> x_dim >> y_dim >> numMines;
	



		/************************************Copied*/
		srand((unsigned int)time(NULL));
		/************************************/

		/********************************** Copied*/
		clock_t start;
		double duration;
		start = clock();
		/**********************************/

		matrix map;
		matrix mask;


		//Generate our map and mask
		for (int i = 0; i < x_dim; i++)
		{
			map.push_back(vector<char>(y_dim, '.'));
			mask.push_back(vector<char>(y_dim, '#'));
		}
		int mineCount = 0;


		//loop to add mines until we have a full minefield
		while (mineCount != numMines)
		{
			if (addMine(map))
				mineCount++;
		}


		int x_in, y_in, c = 0;
		do
		{
			//Display the masked minefield
			//output column indices first
			cout << "  Enter The raw number and the column number respectivly" << endl;
			cout << "  ";
			for (int i = 0; i < y_dim; i++)
			{
				cout << i;
			}
			cout << endl;

			//loop through our rows
			for (int x = 0; x < x_dim; x++)
			{
				//output each row index before the row itself
				cout << x << "|";

				//loop through all the columns for this row
				for (int y = 0; y < y_dim; y++)
				{

					//if location is still masked, display mask char
					//otherwise display the underlying map value
					if (mask[x][y] == '#')
						cout << '#';
					else
					{

						cout << map[x][y];
						/*if (map[x][y] == '.' || isdigit(map[x][y]){
							c++;
							}*/
					}
				}
				cout << endl;
			}

			//Wait for user input to reveal a location
			cin >> x_in >> y_in;


			//revealLocation returns true if a mine was unmasked
			if (revealLocation(map, mask, x_in, y_in))
			{
				cout << "You set off a mine.  Game over!" << endl;
				for (int x = 0; x < x_dim; x++)
				{
					for (int y = 0; y < y_dim; y++)
					{

						cout << map[x][y];
					}
					cout << endl;
				}
				
					break;
				
			}

			//Count our masked location, if == numMines, then only mines are left and player has won the game

			if (countMask(mask) == numMines)
			{
				int x;
				cout << "You have found all the mines.  Congratulations!" << endl;
				cout << "Enter your Name" << endl;
				string str;
				cin >> str;
				names[i] = str;
				duration = (clock() - start) / (double)CLOCKS_PER_SEC;
				arr[i] = duration;
				i++;
				cout << str << " you Took " << duration << "to finish the game!" << endl;
				x = countMap(map);
				cout << "Your Score is" << duration*x << endl;
				break;
			}

		} while (true);


		map.clear();
		mask.clear();

		cout << endl;

	system("pause");
	return 0;
}