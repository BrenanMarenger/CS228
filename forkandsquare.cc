//Brenan Marenger
//This program forks into two processes and uses a pipe to communicate. The parent takes the input, send it to the child, then the child will square the value and send it back
//and the parent will display it

#include<iostream>
#include<string>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <fdstream.h>
#include "forkandsquare.h"

using namespace std;

int p[2];
int q[2];

int main(int argc, char **argv){ 
	int d = pipe(p);
	if (d < 0) {
		exit(0);
	}
	d = pipe(q);
	if (d < 0) {
		exit(0);
	}

int pid = fork();
if(pid==0){
	child();
}
else {
	parent();
}
return 0;
}

void parent() { //parent process that uses 2 pipes with the child, sends an integer then recieves the new squared integer
	close(p[0]);
	close(q[1]);

	fdostream pout(p[1]);
	fdistream qin(q[0]);
	int input;
	cout << "Enter an integer to be squared: ";
	cin >> input;
	pout << input;
	pout.close();
	qin >> input;
	cout << "Squared value: " << input;
	qin.close();
	int status;
	wait(&status);
}

void child() { //child process that uses 2 pipes with the parent, recieves an integer, squares that value then sends it to the parent
	close(p[1]);
	close(q[0]);

	fdistream pin(p[0]);
	fdostream qout(q[1]);
	int input;
	pin >> input;
	input *= input;
	pin.close();
	qout << input;
	qout.close();
}