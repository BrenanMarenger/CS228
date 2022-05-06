//Brenan Marenger
//The purpose of this program is a two player Othello game. The responsibility of the server is only to determine the color of each player and then to send moves back and forth

#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fstream>
#include <fdstream.h>
#include "pg4server.h"
using namespace std;

int main(int argc, char** argv) {
	int sc = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(12345);
	sin.sin_addr.s_addr = INADDR_ANY;
	int b = bind(sc, (sockaddr*)& sin, sizeof(sockaddr_in));
	if (b < 0) { perror("bind"); exit(1); }
	listen(sc, 2);
	int s = accept(sc, NULL, NULL);
	int s2 = accept(sc, NULL, NULL);

	fdistream black_in(s);
	fdostream black_out(s);
	fdistream white_in(s2);
	fdostream white_out(s2);
	
	if (s < 0) { perror("accept"); exit(1); }
	if (s > 0) {
		black_out << "B" << endl;
	}
	if (s2 < 0) { perror("accept"); exit(1); }
	if (s2 > 0) {
		white_out << "W" << endl;
	}
	
	string move = "E3";
	int turn = 0; //0black, 1white

	while (move != "end") { //client will send "end" when there are no more spaces left on the board. send a "\0" if they cannot make a move

		if (turn == 0) {
			//cout << "Waiting for black to enter move... " << endl;
			getline(black_in, move); // black enters a move
			white_out << move << endl; //white recieves blacks move
			turn = 1;
		}
		if (turn == 1) {
			//cout << "Waiting for white to enter move... " << endl;
			getline(white_in, move); //white enters a move
			black_out << move << endl; //black recieves move
			turn = 0;
		}
	}

	black_out.close();
	white_out.close();
	return 0;
}