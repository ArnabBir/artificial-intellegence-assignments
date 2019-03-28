#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <climits>

#define ROW 6
#define COL 7
#define MAXDEPTH 4
#define MAXSCORE 1000000000

using namespace std;

bool player1;
vector<vector<char> > grid;
int turns;
int inpPos[COL];
int won;

class Player {

 	char piece;
 	bool won;
 	bool isComp;

 	public:
 		Player(bool b, bool comp) {
 			setPiece(b);
 			setIsComp(comp);
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
 		void setIsComp(bool b) {
 			isComp = b;
 		}
 		bool getIsComp() {
 			return isComp;
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

int getRandomPosition(vector<int> & v) {

	int randIndex = rand() % static_cast<int>(v.size()+1);
	return v[randIndex];

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

	//memset(grid, '*', sizeof grid);
	for(int i  = 0; i < ROW; ++i) {
		grid.push_back(vector<char> (COL, '*'));
	}
	for(int i = 0; i < COL; ++i) {
		inpPos[i] = ROW-1;
	}
}

bool findWin(Player & p, bool real) {

	char color = p.getPiece();
	for(int j = 0; j < COL-3; ++j) {
		for(int i = 0; i < ROW; ++i) {
			if((grid[i][j] == color) && (grid[i][j+1] == color) && (grid[i][j+2] == color) && (grid[i][j+3] == color)) {
				if(real){
					cout<<"\nHorizontal : "<<i+1<<" "<<j+1<<endl;
					won = p.getIsComp() ? 2 : 1;
					p.setWon(true);
				}
				return true;
			}
		}
	}

	for(int j = 0; j < COL; ++j) {
		for(int i = 0; i < ROW-3; ++i) {
			if((grid[i][j] == color) && (grid[i+1][j] == color) && (grid[i+2][j] == color) && (grid[i+3][j] == color)) {
				if(real){
					cout<<"\nVertical : "<<i+1<<" "<<j+1<<endl;
					won = p.getIsComp() ? 2 : 1;
					p.setWon(true);
				}
				return true;
			}
		}
	}

	for(int j = 0; j < COL-3; ++j) {
		for(int i = 0; i < ROW-3; ++i) {
			if((grid[i][j] == color) && (grid[i+1][j+1] == color) && (grid[i+2][j+2] == color) && (grid[i+3][j+3] == color)) {
				if(real){
					cout<<"\nFirst Diagonal : "<<i+1<<" "<<j+1<<endl;
					won = p.getIsComp() ? 2 : 1;
					p.setWon(true);
				}
				return true;
			}
		}
	}	

	for(int j = 0; j < COL-3; ++j) {
		for(int i = 3; i < ROW; ++i) {
			if((grid[i][j] == color) && (grid[i-1][j+1] == color) && (grid[i-2][j+2] == color) && (grid[i-3][j+3] == color)) {
				if(real){
					cout<<"\nSecond Diagonal : "<<i+1<<" "<<j+1<<endl;
					won = p.getIsComp() ? 2 : 1;
					p.setWon(true);
				}
				return true;
			}
		}
	}

	if(isMaxTurns()) {
		won = 3;
	}
}


void play(Player & p) {

	int col, i = ROW;
	cout<<"Please select a column (1 - 7) : ";
	cin>>col;
	if(inpPos[col-1] <= 0) {
		cout<<"Invalid move ! Please try again.\n\n";
	}
	else {
		cout<<inpPos[col-1]<<" "<<col-1<<endl;
		grid[inpPos[col-1]--][col-1] = p.getPiece();
		findWin(p, true);
		updateTurn();
	}
}

// int getWindowScore(int count, int empty) {

// 	if(count == 4)	return 10000000;
// 	if(count == 3 && empty == 1)	return 50;
// 	if(count == 2 && empty == 2)	return 40;
// 	if(count == 1 && empty == 3)	return 20	;

// 	if(count == 3 && empty == 0)	return 15;
// 	if(count == 2 && empty == 1)	return 10;
// 	if(count == 2 && empty == 0)	return -5;
// 	if(count == 1 && empty == 2)	return -10;
// 	if(count == 1 && empty == 1)	return -20;
// 	if(count == 1 && empty == 0)	return -50;

// 	if(count == 0 && empty == 4)	return 0;
// 	if(count == 0 && empty == 3)	return -5;
// 	if(count == 0 && empty == 2)	return -20;
// 	if(count == 0 && empty == 1)	return -100;
// 	if(count == 0 && empty == 0)	return -100000;

// 	return 0;
// }

int getWindowScore(int count, int empty) {
	
	if(count == 4)	return 100;
	if(count == 3 && empty == 1)	return 5;
	if(count == 2 && empty == 2)	return 2;
	if(count == 0 && empty == 1)	return -100;
	return 0;
}

int getScore(Player & p) {

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

pair<int,int> miniMax(Player & p1, Player & p2, int depth, int alpha, int beta, bool maximizingPlayer) {

	pair<int,int>  colScore;
	vector<int> nextPos = getNextPositions();
	bool winP1 = findWin(p1, false);
	bool winP2 = findWin(p2, false);
	int col, score = 0, optScore;
	if(winP1)	return make_pair(-1, -MAXSCORE);
	if(winP2)	return make_pair(-1, MAXSCORE);
	if(isMaxTurns())	return make_pair(-1, 0);
	if(depth == 0) {
		if(MAXDEPTH % 2)	return make_pair(-1, getScore(p2));
		else	return make_pair(-1, getScore(p1));
	}
	col = getRandomPosition(nextPos);
	if(maximizingPlayer) {
		optScore = INT_MIN;
		for(int i = 0; i < nextPos.size(); ++i) {
			if(inpPos[nextPos[i]] < 0)	return make_pair(-1, INT_MIN);
			grid[inpPos[nextPos[i]]--][nextPos[i]] = p2.getPiece();
			colScore = miniMax(p1, p2, depth-1, alpha, beta, false);
			score = colScore.second;
			grid[++inpPos[nextPos[i]]][nextPos[i]] = '*';
			if(score > optScore) {
				optScore = score;
				col = nextPos[i];
			}
			alpha = max(alpha, optScore);
			if(alpha >= beta) {
				break;
			}
		}
	}
	else {
		optScore = INT_MAX;
		for(int i = 0; i < nextPos.size(); ++i) {
			if(inpPos[nextPos[i]] < 0)	return make_pair(-1, INT_MAX);
			grid[inpPos[nextPos[i]]--][nextPos[i]] = p1.getPiece();
			colScore = miniMax(p1, p2, depth-1, alpha, beta, true);
			score = colScore.second;
			grid[++inpPos[nextPos[i]]][nextPos[i]] = '*';
			if(score < optScore) {
				optScore = score;
				col = nextPos[i];
			}
			beta = min(beta, optScore);
			if(alpha >= beta) {
				break;
			}
		}
	}
	//cout<<depth<<" : "<<col<<" : "<<score<<endl;
	return make_pair(col, optScore);
}

void playComp(Player & p1, Player & p2) {
	
	if(turns == 0) {
		grid[inpPos[COL/2]--][COL/2] = p2.getPiece();
	}
	else {
		pair<int, int>  colScore = miniMax(p1, p2, MAXDEPTH, INT_MIN, INT_MAX, true);
		int optCol = colScore.first;
		grid[inpPos[optCol]--][optCol] = p2.getPiece();
	}
	findWin(p2, true);
	updateTurn();
}

int main() {

 	buildGrid();
	while(!askWhoPlayer1());
	Player user(player1, false), comp(!player1, true);
	printGrid();
	won = 0;
	turns = 0;	
	do {
		if(player1) {
			play(user);
		}
		else {
			playComp(user, comp);
		}
		printGrid();
	}while(!won);
	cout<<"\n\nGame over ! "<<"\n\n";
	if(won == 3) {
		cout<<"\nResult : It is a tie.\n\n";
	}
	else {
		if(user.getWon()) {
			cout<<"\nResult : User won the game.\n\n";
		}
		else if(comp.getWon()) {
			cout<<"\nResult : Computer won the game.\n\n";
		}
	}
	return 0;
}