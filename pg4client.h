//Brenan Marenger
//this is used to initialize functions and to make sure only one instance will run
#ifndef _CLIENT_
#define _CLIENT_

#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

int main(int argc, char** argv);
void printBoard(string board[][8]); //prints the game board
bool endOfGame(string board[][8]); //detects if gameover
string moveCoord(string move); //converts the input into readible coordinates
//bool nextTo(string board[][8], string color, string move); DIDNT MAKE THE CUT
bool isValid(string board[][8], string move, string color); //master function to see if a move is legal
string enterMove(string board[][8], string color); //allows user to enter a valid input
bool canMove(string board[][8], string color); //checks if the player can play a move, forfeits if not
bool canFlip(string board[][8], string color, string move); //checks if the asked move will flip atleast one stone
void flip(string board[][8], string color, string move); //correctly flips stones from the new move
int count(string board[][8], string color); //counts how many of a given color is on the board, used for finding score

#endif