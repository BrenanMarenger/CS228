//Brenan Marenger
//the purpose of this program is to send the name of a program, along with file data, to a
//server that will return new information to be put into a file

#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <fstream>
#include <sys/socket.h>
#include <fdstream.h>
#include <netinet/in.h>
#include<stdio.h>
#include <netdb.h>
#include "client.h"
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

	string input;
	while (1) { 
		string program;
		cout << "Enter a program name: ";
		getline(cin, program);
		sock_out << program << endl;
		sock_out.flush();

		cout << "Enter an input file: ";
		string inputFile;
		getline(cin, inputFile);
		ifstream myFile(inputFile);
		char ch;
		while (myFile.get(ch)) {
			sock_out << ch;
		}
		sock_out << '\0';
		sock_out.flush();
		myFile.close();

		cout << "Enter an output file: ";
		string outputFileName;
		getline(cin, outputFileName);
		ofstream outFile;
		outFile.open(outputFileName);
		while (sock_in.get(ch)) {
			if (ch == '\0') {
				break;
			}
			outFile << ch;
		}
		outFile.close();

		cout << "Would you like to run another program? [Y][N]" << endl;
		getline(cin, input);
		sock_out << input <<endl;
		if (input == "n" || input == "N") {
			break;
		}
	}
	sock_in.close();
	return 0;
}