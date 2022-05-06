//Brenan Marenger
//The purpose of this program is for a two player Othello game. This client will carry the functionality, making the calculations and authorizing legal moves and communicate through the server by passing moves

#include <iostream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <fstream>
#include <sys/socket.h>
#include <fdstream.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <ctime>
#include "pg4client.h"
using namespace std;

int main(int argc, char** argv) {
	struct hostent* h;
	sethostent(1);
	h = gethostbyname("127.0.0.1"); 
	endhostent();
	int s = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(12345);
	sin.sin_addr = *(in_addr*)h->h_addr_list[0];
	int c = connect(s, (sockaddr*)& sin, sizeof(sockaddr_in));
	if (c < 0) { perror("connect"); exit(1); }

	fdistream sock_in(s);
	fdostream sock_out(s);

	string board[8][8];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j] = "_";
		}
	}

	board[4][4] = "W";
	board[3][3] = "W";
	board[4][3] = "B";
	board[3][4] = "B";

	printBoard(board);

	string color;
	getline(sock_in, color); //server will send client their given color
	cout << "Your color is: " << color << endl;
	string move;
	int i;
	int j;

	if (color == "B") {
		cout << "Black starts" << endl;
		move = enterMove(board, color);
		cout << "Move: " << move << endl;
		printBoard(board);
		sock_out << move << endl; //black to start the loop
	}

	while (!endOfGame(board)) {
		if (color == "B") {
			while (1) {
				if (getline(sock_in, move)) { //if recieves a move from white
					if (move != "\0") { 
						i = (int)move[0] - 48;
						j = (int)move[1] - 48;
						board[j][i] = "W";
						flip(board, "W", move);
					}
					else {
						cout << "White cannot move, your turn!" << endl;
					}

					printBoard(board);
					cout << endl << "White:" << count(board, "W") << " Black:" << count(board, "B") << endl;
					//blacks turn
					move = enterMove(board, color);
					printBoard(board);
					sock_out << move << endl;
				}
				if (count(board, "W") + count(board, "B") == 64) {
					break;
				}
			}
		}

		if (color == "W") {
			while (1) {
				if (getline(sock_in, move)) { //if recieves a move from black
					if (move != "\0") {
						i = (int)move[0] - 48;
						j = (int)move[1] - 48;
						board[j][i] = "B";
						flip(board, "B", move);
					}
					else {
						cout << "Black cannot move, your turn!" << endl;
					}
					printBoard(board);
					cout << endl << "White:" << count(board, "W") << " Black:" << count(board, "B") << endl;
					//whites turn
					move = enterMove(board, color);
					printBoard(board);
					sock_out << move << endl;
				}
				if (count(board, "W") + count(board, "B") == 64) {
					break;
				}
			}
		}
	}

	sock_out << "end"; //end server loop

	cout << "Game Over!" << endl;

	if (count(board, "W") > count(board, "B")) {
		cout << "White wins!" << endl;
	}

	if (count(board, "W") < count(board, "B")) {
		cout << "Black wins!" << endl;
	}

	if (count(board, "W") == count(board, "B")) {
		cout << "Tie!" << endl;
	}

	sock_out.close();
	return 0;
}

void printBoard(string board[][8]) { //Prints the board
	cout << "  A B C D E F G H" << endl;
	for (int i = 0; i < 8; i++) {
		cout << i + 1 << "|";
		for (int j = 0; j < 8; j++) {
			cout << board[i][j] << "|";
		}
		cout << endl;
	}
}

bool endOfGame(string board[][8]) { //check if the game has ended
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] == "_") {
				return false;
			}
		}
	}
	return true;
}

string moveCoord(string move) { //converts input into readible coordinates
	int i;
	int j;
	if (move == "") {
		return "O";
	}

	j = (int)move[1] - 49;
	if (move.front() == 'A') {
		i = 0;
	}
	else if (move.front() == 'B') {
		i = 1;
	}
	else if (move.front() == 'C') {
		i = 2;
	}
	else if (move.front() == 'D') {
		i = 3;
	}
	else if (move.front() == 'E') {
		i = 4;
	}
	else if (move.front() == 'F') {
		i = 5;
	}
	else if (move.front() == 'G') {
		i = 6;
	}
	else if (move.front() == 'H') {
		i = 7;
	}
	else {
		//cout << "Move out of bounds!" << endl;
		return "O";
	}

	if (j != 0 && j != 1 && j != 2 && j != 3 && j != 4 && j != 5 && j != 6 && j != 7) {
		//cout << "Move out of bounds!" << endl;
		return "O";
	}
	string newMove;
	newMove += to_string(i);
	newMove += to_string(j);

	return newMove;
}

/*
bool nextTo(string board[][8], string color, string move) { //takes a move, and returns true if an oppisite color is around it
	int i = (int)move[0] - 48;
	int j = (int)move[1] - 48;
	int tempi = i;
	int tempj = j;

	if (color == "B") {
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				i = i + y;
				j = j + x;
				if (i == 8 || i == -1 || j == 8 || j == -1) { break; }
				if (board[j][i] == "W") {
					return true;
				}
				i = tempi;
				j = tempj;
			}
			i = tempi;
			j = tempj;
		}
	}

	if (color == "W") {
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				i = i + y;
				j = j + x;
				if (i == 8 || i == -1 || j == 8 || j == -1) { break; }
				if (board[j][i] == "B") {
					return true;
				}
				i = tempi;
				j = tempj;
			}
			i = tempi;
			j = tempj;
		}
	}
	return false;
}
*/

bool isValid(string board[][8], string move, string color) { //master check function to check if a move is legal
	if (move == "O") {
		return false;
	}
	int i = (int)move[0] - 48;
	int j = (int)move[1] - 48;

	if (board[j][i] == "_" && canFlip(board, color, move)) {
		return true;
	}
	else {
		return false;
	}
}

string enterMove(string board[][8], string color) {//promts user to enter a move, check if it is valid, and plays the move

	if (!canMove(board, color)) {
		cout << "Turn forfeit!" << endl;
		return "\0";
	}

	string move;
	do {
		cout << "Enter a move: ";
		getline(cin, move);
		move = moveCoord(move);

		if (!isValid(board, move, color)) {
			cout << "Invalid move! " << endl;
		}
	} while (!isValid(board, move, color));

	int i = (int)move[0] - 48;
	int j = (int)move[1] - 48;

	if (color == "B") {
		board[j][i] = "B";
	}
	if (color == "W") {
		board[j][i] = "W";
	}
	flip(board, color, move);
	return move;
}

bool canMove(string board[][8], string color) { //checks if there is a avalible move for the given color
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[j][i] == "_") {
				string position = "";
				position += to_string(i);
				position += to_string(j);
				if (canFlip(board, color, position)) { //canFlip?
					return true;
				}
			}
		}
	}
	return false;
}


bool canFlip(string board[][8], string color, string move) { //checks if the given move would be able to flip atleast one stone

	int i = (int)move[0] - 48;
	int j = (int)move[1] - 48;
	int tempi = i;
	int tempj = j;
	
	int flag = 0;
	int flag2 = 0;

	if (color == "B") {
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				for (int z = 0; z < 8; z++) {
					i = i + y;
					j = j + x;
					if (i > 7 || i < 0 || j > 7 || j < 0) { break; }
					if (board[j][i] == "W") {
						flag = 1;
					}
					if (z == 0 && flag == 0) {
						break;
					}
					if (flag == 1 && board[j][i] == "_") {
						flag = 0;
						break;
					}
					if (flag == 1 && board[j][i] == "B") {
						flag2 = 1;
					}
					if (flag == 1 && flag2 == 1) {
						return true;
					}
				}
				i = tempi;
				j = tempj;
				flag = 0;
				flag2 = 0;

			}
		}
	}

	if (color == "W") {
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				for (int z = 0; z < 8; z++) {
					i = i + y;
					j = j + x;
					if (i > 7 || i < 0 || j > 7 || j < 0) { break; }
					if (board[j][i] == "B") {
						flag = 1;
					}
					if (z == 0 && flag == 0) {
						break;
					}
					if (flag == 1 && board[j][i] == "_") {
						flag = 0;
						break;
					}
					if (flag == 1 && board[j][i] == "W") {
						flag2 = 1;
					}
					if (flag == 1 && flag2 == 1) {
						return true;
					}

				}
				i = tempi;
				j = tempj;
				flag = 0;
				flag2 = 0;

			}
		}
	}

	return false;
}

void flip(string board[][8], string color, string move) { //flips the oppisite colors stones if requirements are met

	int i = (int)move[0] - 48;
	int j = (int)move[1] - 48;
	int tempi = i;
	int tempj = j;
	int flipi = i;
	int flipj = j;
	int flag = 0;
	int flag2 = 0;

	if (color == "B") {
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				for (int z = 0; z < 8; z++) {
					i = i + y;
					j = j + x;
					if (i > 7 || i < 0 || j > 7 || j < 0) { break; }
					if (board[j][i] == "W") {
						flag = 1;
					}
					if (z == 0 && flag == 0) {
						break;
					}
					if (flag == 1 && board[j][i] == "_") {
						flag = 0;
						break;
					}
					if (flag == 1 && board[j][i] == "B") {
						flag2 = 1;
					}
					if (flag == 1 && flag2 == 1) {
						for (int a = 0; a < z; a++) {
							flipi += y;
							flipj += x;
							if (board[flipj][flipi] == "W") {
								board[flipj][flipi] = "B";
							}
						}
						break;
					}
				}
				flipi = tempi;
				flipj = tempj;
				i = tempi;
				j = tempj;
				flag = 0;
				flag2 = 0;
			}
		}
	}

	if (color == "W") {
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {//each loop is new direction
				for (int z = 0; z < 8; z++) { //check upto 8 spots in that direction
					i = i + y;
					j = j + x;
					if (i > 7 || i < 0 || j > 7 || j < 0) { break; } //off board
					if (board[j][i] == "B") { // if it passes a black
						flag = 1;
					}
					if (z == 0 && flag == 0) { //first spot didnt have an opp color
						break;
					}
					if (flag == 1 && board[j][i] == "_") { //if it passes a blank
						flag = 0;
						break;
					}
					if (flag == 1 && board[j][i] == "W") { //already passed a black and now reaches a white spot
						flag2 = 1;
					}
					if (flag == 1 && flag2 == 1) { //if both conditions met, can flip what is inbetween
						for (int a = 0; a < z; a++) {
							flipi += y; //from the starting position move in direction z times
							flipj += x;
							if (board[flipj][flipi] == "B") {
								board[flipj][flipi] = "W"; //chang to white
							}
						}
						break;
					}

				}
				i = tempi;
				j = tempj; //reset back to original spot to check a new direction
				flipi = tempi;
				flipj = tempj;
				flag = 0;
				flag2 = 0;

			}
		}
	}
}

int count(string board[][8], string color) { //passes the color and counts the amount on the board
	int counter = 0;
	if (color == "B") {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (board[j][i] == "B") {
					counter++;
				}
			}
		}
	}
	if (color == "W") {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (board[j][i] == "W") {
					counter++;
				}
			}
		}
	}

	return counter;
}