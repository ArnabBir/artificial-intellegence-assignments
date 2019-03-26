#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>

#define ROW 6
#define COL 7
#define MAXDEPTH 4
#define MAXSCORE 1000000000

using namespace std;

bool player1;
char grid[ROW][COL];
int turns;
int inpPos[COL];

class Player {

 	char piece;
 	bool won;

 	public:
 		Player(bool b) {
 			setPiece(b);
 			setWon(false); 
 		}
 		void setPiece(bool b) {
 			piece = b ? 'x' : 'o';
 		} 
 		char getPiece() {
 			return piece;
 		}
 		void setWon(bool b) {
 			won = b;
 		}
 		bool getWon() {
 			return won;
 		}
};

bool isMaxTurns() {
	return turns >= ROW * COL;
}

vector<int>	getNextPositions() {

	vector<int> v;
	for(int i = 0; i < COL; ++i) {
		if(inpPos[i] != -1) {
			v.push_back(i);
		}
	}
	return v;
}

void updateTurn() {
	
	player1 = player1 ? false : true;
	++turns;
}

bool askWhoPlayer1() {

	string s;

	cout<<"Do you want to start? (y/n)"<<endl;
	cin>>s;
	if(s == "y" || s == "Y") {
		player1 = true;
		return true;
	}
	else if(s == "n" || s == "N") {
		player1 = false;
		return true;
	}
	else {
		cout<<"Please enter a valid input!\n\n";
		return false;
	}
}

void printGrid() {

	cout<<"\nGrid : \n\n";
	for(int i = 0; i < ROW; ++i) {
		for(int j = 0; j < COL; ++j) {
			cout<<"| "<<grid[i][j]<<" ";
		}
		cout<<"|\n";
	}
	cout<<"\n";
}

void buildGrid() {
	
	memset(grid, '*', sizeof grid);
	memset(inpPos, ROW-1, sizeof inpPos);
}

bool findWin(Player p) {

	char color = p.getPiece();
	for(int j = 0; j < COL-3; ++j) {
		for(int i = 0; i < ROW; ++i) {
			if((grid[i][j] == color) && (grid[i][j+1] == color) && (grid[i][j+2] == color) && (grid[i][j+3] == color)) {
				cout<<"Horizontal : "<<i<<" "<<j<<endl;
				return true;
			}
		}
	}

	for(int j = 0; j < COL; ++j) {
		for(int i = 0; i < ROW-3; ++i) {
			if((grid[i][j] == color) && (grid[i+1][j] == color) && (grid[i+2][j] == color) && (grid[i+3][j] == color)) {
				cout<<"Vertical : "<<i<<" "<<j<<endl;
				return true;
			}
		}
	}

	for(int j = 0; j < COL-3; ++j) {
		for(int i = 0; i < ROW-3; ++i) {
			if((grid[i][j] == color) && (grid[i+1][j+i] == color) && (grid[i+2][j+2] == color) && (grid[i+3][j+3] == color)) {
				cout<<"Pos Diag : "<<i<<" "<<j<<endl;
				return true;
			}
		}
	}	

	for(int j = 0; j < COL-3; ++j) {
		for(int i = 3; i < ROW; ++i) {
			if((grid[i][j] == color) && (grid[i-1][j+i] == color) && (grid[i-2][j+2] == color) && (grid[i-3][j+3] == color)) {
				cout<<"Neg Diag : "<<i<<" "<<j<<endl;
				return true;
			}
		}
	}

	return isMaxTurns();
}


void play(Player p) {

	int col, i = ROW;
	cout<<"Please select a column (1 - 7) : ";
	cin>>col;
	if(inpPos[col-1] == -1) {
		cout<<"Invalid move ! Please try again.\n\n";
	}
	else {
		grid[inpPos[col-1]][col-1] = p.getPiece();
		updateTurn();
	}
}

int getWindowScore(int count, int empty) {

	if(count == 4)	return 100;
	if(count == 3 && empty == 1)	return 5;
	if(count == 2 && empty == 2)	return 4;
	if(count == 1 && empty == 3)	return 3;
	if(count == 3 && empty == 0)	return -1;
	if(count == 2 && empty == 1)	return -1;
	if(count == 2 && empty == 0)	return -3;
	if(count == 1 && empty == 2)	return -3;
	if(count == 1 && empty == 1)	return -4;
	return 0;
}

int getScore(player p) {

	int score = 0, countP, empty;
	char color = p.getPiece();
	for(int i = 0; i < ROW; ++i) {
		if(grid[i][COL/2] == color) {
			score += 3;
		}
	}
	for(int i = 0; i < ROW; ++i) {
		for(int j = 0; j < COL-3; ++j) {
			countP = 0, empty = 0;
			for(int k = 0; k < 3; ++k) {
				if(grid[i][j+k] == color) {
					++countP;
				}
				else if(grid[i][j+k] == '*') {
					++empty;
				}
			}
			score += getWindowScore(countP, empty);
		}
	}
	for(int j = 0; j < COL; ++j) {
		for(int i = 0; i < ROW-3; ++i) {
			countP = 0, empty = 0;
			for(int k = 0; k < 3; ++k) {
				if(grid[i+k][j] == color) {
					++countP;
				}
				else if(grid[i+k][j] == '*') {
					++empty;
				}
			}
			score += getWindowScore(countP, empty);
		}
	}
	for(int i = 0; i < ROW-3; ++i) {
		for(int j = 0; j < COL-3; ++j) {
			countP = 0, empty = 0;
			for(int k = 0; k < 3; ++k) {
				if(grid[i+k][j+k] == color) {
					++countP;
				}
				else if(grid[i+k][j+k] == '*') {
					++empty;
				}
			}
			score += getWindowScore(countP, empty);
		}
	}
	for(int i = 0; i < ROW-3; ++i) {
		for(int j = 3; j < COL; ++j) {
			countP = 0, empty = 0;
			for(int k = 0; k < 3; ++k) {
				if(grid[i+k][j-k] == color) {
					++countP;
				}
				else if(grid[i+k][j-k] == '*') {
					++empty;
				}
			}
			score += getWindowScore(countP, empty);
		}
	}
	return score;
}

int miniMax(Player p1, Player p2, int depth, int alpha, int beta, bool maximizingPlayer) {
	
	vector<int> nextPos = getNextPositions(p1);
	bool winP1 = findWin(p1);
	bool winP2 = findWin(p2);
	if(winP1)	return -MAXSCORE;
	if(winP2)	return MAXSCORE;
	if(isMaxTurns())	return 0;
	if(depth == 0) {
		return getScore(p2);
	}
	if(maximizingPlayer) {
		
	}
}

void playComp(Player p, bool realTurn) {
	if(realTurn)	updateTurn();
}

int main() {

 	buildGrid();
	while(!askWhoPlayer1());
	Player user(player1), comp(!player1);
	printGrid();
	turns = 0;	
	do {
		if(player1) {
			play(user);
		}
		else {
			playComp(comp, true);
		}
		printGrid();
	}while(!findWin());

	cout<<"\n\nGame over !\n\n";

	return 0;
}