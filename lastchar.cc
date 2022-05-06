//Brenan Marenger
//this program takes a string from a command line arguement and returns the last character in that input
#include<iostream>
#include<string>
#include <cstdlib>
#include"lastchar.h"
using namespace std;

int main(int argc, char **argv){ 
string input = argv[1];
char last = input[input.length() - 1];
return last;
}