#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <climits>
#include <cmath>

#define ROW 6
#define COL 7
#define C 2
#define MAXSCORE 1000000000
#define TRAINGAMES 3000

using namespace std;

int dx[4] = {1, 1, 1, 0};
int dy[4] = {1, 0, -1, 1};
vector<vector<char> > grid;
bool player1;
int turns = 0;

class Player {		//  Defines the states of a player (user / comp)

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

class Board {
	
	public:
		vector< vector< char > > grid;
		vector<int> lastMove;
	
		Board(vector< vector< char > > mat, vector<int> move) {
			grid = mat;
			lastMove = move;
		}

		Board(vector< vector< char > > mat) {
			grid = mat;
		}

		Board() {
			//vector< vector< char > > mat(ROW, vector<char>(COL, *));
			//grid = mat;
		}

		int tryMove(int move) {
			if(move < 0 || move > COL || grid[0][move] != '*') {
				return -1;
			}
			for(int  i = 0; i < ROW; ++i) {
				if(grid[i][move] != '*') {
					return i-1;
				}
			}
			return ROW-1;
		}

		bool terminal() {
			for(int i = 0; i < COL; ++i) {
				if(grid[0][i] == '*') {
					return false;
				}
			}
			return true;
		}

		vector<int> legalMoves() {
			vector<int> legal;
			for(int i = 0; i < COL; ++i) {
				if(grid[0][i] == '*') {
					legal.push_back(i);
				}
			}
			return legal;		
		}
		
		int winner() {
		
			int x = lastMove[0];
			int y = lastMove[1];
			int player, comp;
			for(int i = 0; i < 4; ++i) {
				player = 0;
				comp = 0;
				for(int j = -3; j <= 3; ++j) {
					int u = x + j * dx[i];
					int v = y + j * dy[i];
					if(u < 0 || u >= ROW || v < 0 || v >= COL) {
						continue;
					} 		
					if(grid[u][v] == 'X') {
						comp = 0;
						++player;
					}
					else if(grid[u][v] == 'O') {
						player = 0;
						++comp;					
					}
					else {
						player = 0;
						comp = 0;
					}
					if(player == 4) {
						return -1;
					}
					if(comp == 4) {
						return 1;
					}	
				}
			}
			return 0;
		}
};

Board nextState(Board board, char turn) {
		
	Board aux = board;
	vector<int> moves = aux.legalMoves();
	if(moves.size() > 0) {
		int ind = rand() % static_cast<int>(moves.size());
		int col = moves[ind];				
		int row = aux.tryMove(col);
		aux.grid[row][col] = (turn == -1) ? 'X' : 'O';
		vector<int> v(2);
		v[0] = row;
		v[1] = col;
		aux.lastMove = v;
	}
	return aux;		
}


struct Node {

	int visits;
	Board state;
	vector<Node * > children;
	vector<int> childrenMove;
	Node * parent;
	double reward;
};

Node * createNode(Board board) {

	Node * node = new Node;
	node->visits = 1;
	node->reward = 0.0;
	node->state = board;
	node->parent = NULL;
	return node;
}

void addChild(Node * parent, Board childState, int move) {

	Node * child = createNode(childState);
	child->parent = parent;
	vector<Node *> v = parent->children;
	v.push_back(child);
	parent->children = v;
	vector<int> m = parent->childrenMove;
	m.push_back(move);
	parent->childrenMove = m;	
}

bool fullyExplored(Node * node) {

	if((node->children).size() == (node->state).legalMoves().size()) {
		return true;			
	}		
	return false;
}

Node * expand(Node * node, int turn) {

	vector<int> triedChildrenMove = node->childrenMove;
	vector<int> possibleMoves = (node->state).legalMoves();
	Board newState;

	int i, j;

	for(i = 0; i < possibleMoves.size(); ++i) {
		bool found = false;
		for(j = 0; j < triedChildrenMove.size(); ++j) {
			if(possibleMoves[i] == triedChildrenMove[j]) {
				found = true;
				break;
			}
		}
		if(!found) {
			int row = (node->state).tryMove(possibleMoves[i]);
			newState = node->state;
			newState.grid[row][possibleMoves[i]] = (turn == -1) ? 'X' : 'O';
			vector<int> v(2);
			v[0] = row;
			v[1] = possibleMoves[i];
			newState.lastMove = v;
			break;
		}
	}

	addChild(node, newState, possibleMoves[i]);
	return node->children[(node->children).size()-1];
}

Node * bestChild(Node * node, double k) {

	double bestScore = -MAXSCORE;
	vector<Node * > bestChildren;
	vector<Node * > children = node->children;
	for(int i = 0; i < children.size(); ++i) {
		double score = (children[i]->reward / children[i]->visits) + k * sqrt(log(C * node->visits) / (double)(children[i]->visits));
		if(score == bestScore) {
			bestChildren.push_back(children[i]);
		}
		if(score > bestScore) {
			bestChildren.clear();
			bestChildren.push_back(children[i]);
			bestScore = score;
		}
	}
	int randIndex = rand() % static_cast<int>(bestChildren.size());
	return bestChildren[randIndex];
}

pair<Node *, int> treePolicy(Node * node, int turn, double k) {

	Node * temp = node;
	while((temp->state).terminal() == false && (temp->state).winner() == 0) {
		if(fullyExplored(temp) == false) {
			return make_pair(expand(temp, turn), -1*turn);
		}
		else {
			temp = bestChild(temp, k);
			turn *= -1;
		}
	}
	return make_pair(temp, turn);
}

int defaultPolicy(Board board, int turn) {

	while(board.terminal() == false && board.winner() == 0) {
		board = nextState(board, turn);
		turn *= -1;
	}
	return board.winner();
}

void backup(Node * node, double reward, int turn) {

	Node * temp = node;
	while(temp) {
		temp->visits += 1;
		temp->reward -= turn * reward;
		temp = temp->parent;
		turn *= -1;
	}
 }

Node * MTCS(int maxIter, Node * root) {

	for(int i = 0; i < maxIter; ++i) {
		pair<Node *, int> frontTurn = treePolicy(root, 1, 2.0);
		Node * front = frontTurn.first;
		int turn = frontTurn.second;
		double reward = defaultPolicy(front->state, turn);
		backup(front, reward, turn);
	}
	Node * ans = bestChild(root, 0);
	return ans;
}

void printGrid(vector< vector<char> > grid) {		// Prints the grid

	cout<<"\nGrid : \n\n";
	for(int i = 0; i < ROW; ++i) {
		for(int j = 0; j < COL; ++j) {
			cout<<"| "<<grid[i][j]<<" ";
		}
		cout<<"|\n";
	}
	cout<<"\n";
}

Board findBestMove(Board board) {

	Node * o = createNode(board);
	Node * bestMove = MTCS(TRAINGAMES, o);
	return bestMove->state; 
}

void buildGrid() {		// Initializes the game

	//memset(grid, '*', sizeof grid);
	for(int i  = 0; i < ROW; ++i) {
		grid.push_back(vector<char> (COL, '*'));
	}
}

bool askWhoPlayer1() {	// Promts the user to ensure if he wants to begin as player 1

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

void updateTurn() {		// Updates states after each turn
	
	player1 = player1 ? false : true;
	++turns;
}

int main() {

	int row, col;
	bool ok;
	int result = 0; 
	buildGrid();
	Board board(grid);
	Board lastBoard;
	bool winner = false;

	while(!winner) {
		lastBoard = board;

		if(!winner) {
			cout<<"Select column (1-7) : ";
			cin>>col;
			--col;
			ok = false;
			row = board.tryMove(col);
			if(row < 0) {
				cout<<"Wrong move. Please try again."<<endl;
				continue;
			}
			else {
				vector<int> v;
				v.push_back(row);
				v.push_back(col);
				board.lastMove = v;
				ok = true;
				board.grid[row][col] = 'X';
			}
			if(ok) {
				cout<<"Computer's Turn"<<endl;
				result = board.winner();
			}
			if(result == 1) {
				winner = true;
				cout<<"Game over : You lost"<<endl;
			}
			else if(result == -1) {
				winner = true;
				cout<<"Game over : You won"<<endl;
			}
			else if(board.terminal()) {
				winner = true;
				cout<<"Game over : It's a tie"<<endl;
			}
		}

		printGrid(board.grid);

		if(!winner) {
			
			board = findBestMove(board);
			if(ok) {
				cout<<"Your Turn"<<endl;
				result = board.winner();
			}
			if(result == 1) {
				winner = true;
				cout<<"Game over : You lost"<<endl;
			}
			else if(result == -1) {
				winner = true;
				cout<<"Game over : You won"<<endl;
			}
			else if(board.terminal()) {
				winner = true;
				cout<<"Game over : It's a tie"<<endl;
			}
		}

		printGrid(board.grid);
	}

	return 0;
}
