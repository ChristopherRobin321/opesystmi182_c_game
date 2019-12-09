#include <stdio.h>
#include <stdlib.h> //for random()
#include <time.h>

void RockPaperScissors(char usrVal);

int main(){

char input;
int i;
	for (i = 0; i < 3; ++i){
	printf("\nEnter r, p, or s for rock paper scissors: ");
	input = getchar();
	RockPaperScissors(input);
	}
	printf("\n\n\nTHE GAME IS OVER\n\n");
	}




void RockPaperScissors(char usrVal){

const int rock = 0;
const int paper = 1;
const int scissors = 2;

	srand(time(NULL));
int comVal = rand() % 2 + 0;

int usrScore = 0;
int comScore = 0;
int tie = 0;

	switch (usrVal){
	case 'r':
	if (comVal == paper)
	printf("\nYOU LOSE!!!\n");
	++comScore;
	break;
	if (comVal == scissors)
	printf("\nYOU WIN!!!\n");
	++usrScore;
	break;
	case 'p':
	if (comVal == rock)
	printf("\nYOU WIN!!!\n");
	++usrScore;
	break;
	if (comVal == scissors)
	printf("\nYOU LOSE!!!\n");
	++comScore;
	break;
	case 's':
	if (comVal == rock)
	printf("\nYOU LOSE!!!\n");
	++comScore;
	break;
	if (comVal == scissors)
	printf("\nYOU WIN!!!\n");
	++usrScore;
	break;
	default:
	printf("\nTIE\n");
	++tie;
	printf("\ncomVal: %d  usrVal: %c\n", comVal, usrVal);


	}

	printf("\nscore is user: %d   computer: %d  Tie: %d \n\n\n\n", usrScore, comScore, tie);
	}

