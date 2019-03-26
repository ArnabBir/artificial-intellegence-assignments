#include <iostream>
#include <cstdio>
#include <cstring>

#define ROW 6
#define COL 7
#define DEPTH 4

using namespace std;

bool player1;
char grid[ROW][COL];
int turns;

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
}

bool findWin() {
	
	for(int j = 0; j < COL-3; ++j) {
		for(int i = 0; i < ROW; ++i) {
			cout<<grid[i][j]<<" "<<grid[i][j+1]<<" "<<grid[i][j+2]<<" "<<grid[i][j+3]<<endl;
			if((grid[i][j] != '*') && (grid[i][j] == grid[i][j+1]) && (grid[i][j] == grid[i][j+2]) && (grid[i][j] == grid[i][j+3])) {
				cout<<"Horizontal : "<<i<<" "<<j<<endl;
				return true;
			}
		}
	}

	for(int j = 0; j < COL; ++j) {
		for(int i = 0; i < ROW-3; ++i) {
			if((grid[i][j] != '*') && (grid[i][j] == grid[i+1][j]) && (grid[i][j] == grid[i+2][j]) && (grid[i][j] == grid[i+3][j])) {
				cout<<"Vertical : "<<i<<" "<<j<<endl;
				return true;
			}
		}
	}

	for(int j = 0; j < COL-3; ++j) {
		for(int i = 0; i < ROW-3; ++i) {
			if((grid[i][j] != '*') && (grid[i][j] == grid[i+1][j+i]) && (grid[i][j] == grid[i+2][j+2]) && (grid[i][j] == grid[i+3][j+3])) {
				cout<<"Pos Diag : "<<i<<" "<<j<<endl;
				return true;
			}
		}
	}	

	for(int j = 0; j < COL-3; ++j) {
		for(int i = 3; i < ROW; ++i) {
			if((grid[i][j] != '*') && (grid[i][j] == grid[i-1][j+i]) && (grid[i][j] == grid[i-2][j+2]) && (grid[i][j] == grid[i-3][j+3])) {
				cout<<"Neg Diag : "<<i<<" "<<j<<endl;
				return true;
			}
		}
	}

	return false;
}


void play(Player p) {

	int col, i = ROW;
	cout<<"Please select a column (1 - 7) : ";
	cin>>col;
	while(i >= 0) {
		if(grid[i][col-1] == '*') {
			grid[i][col-1] = p.getPiece();
			break;
		}
		--i;
	}
	if(i == -1) {
		cout<<"Invalid move ! Please try again.\n\n";
	}
	else {
		updateTurn();
		++turns;
	}
}

void miniMax(int depth, int alpha, int beta, bool maximizingPlayer) {
	

}

void playComp(Player p) {
	cout<<player1<<" ";
	updateTurn();
	cout<<player1<<endl;
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
			playComp(comp);
		}
		printGrid();
	}while(!findWin());

	cout<<"\n\nGame over !\n\n";

	return 0;
}