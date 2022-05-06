//Brenan Marenger
//This program forks into two processes, one reverses the input and executes it with lastchar, the other will wait and get the return value from lastchar

#include<iostream>
#include<string>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include"firstchar.h"

using namespace std;

int main(int argc, char **argv){ 
cout << "Enter a word: ";
string input;
cin >> input;
int pid = fork();
if (pid == 0) {
	reverse(input.begin(), input.end());
	int n = input.length();
	char inputChars[n + 1];
	strcpy(inputChars, input.c_str()); //copies string to an array of charaters

	execl("./lastchar", "./lastchar", inputChars, NULL); //terminates current program and runs lastchar with the given input
}
else {
	int status;
	wait(&status);
	char n = WEXITSTATUS(status); //child process returned value after it dies
	cout << n << endl;
}
return 0;
}