// RPS_Andre_Reano_101075780.cpp : Defines the entry point for the console application.
//
/*
	Student Name: Andre Reano
	Student ID: 101075780
*/

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include <math.h>
#include <cstdlib>

using namespace std;

struct playerInfo 
{
	int rank = 0;  
	int wins = 0;
	char name[50];
	int numOfplayers = 0;
};

enum objectType{ROCK,PAPER,SCISSORS};

void displayRules();
bool validSelection(char selection);
objectType retrievePlay(char selection);
objectType winningObject(objectType player1, objectType aiPlayer);
void gameResult(objectType player1, objectType aiPlayer, int& winner);
//void displayResults(int gCount, int wCount1, int wCount2);


int main()
{
	int winCount1 = 0;


	int gameWinner;
	int ai;
	int choice;
	char response, selection;
	char aiSelection;

	objectType player1, aiPlayer;
	playerInfo playerOne;

	ifstream inFile;
	ofstream outFile;
	outFile.open("playerData.out");
	inFile.open("playerData.out");


	inFile >> playerOne.rank >> playerOne.name >> playerOne.wins;
	cout << endl;

	displayRules();
 
	cout << " Enter your choice 1, 2, 3, or 4:" << endl;
	cout << "Current Player Name: <" << playerOne.name << ">" << endl;
	cout << "1. Start the game" << endl;
	cout << "2. Change the player name" << endl;
	cout << "3. Show highest scores" << endl;
	cout << "4. Exit" << endl;
	cin >> choice;

	while (choice == 1 || choice == 2 || choice == 3)
	{
		if (choice == 1)
		{
			do
			{
				cout << playerOne.name << ", enter your choice: ";
				cin >> selection;
				cout << endl;
				ai = rand() % 3 + 1;

				if (ai == 1)
					aiSelection = 'r';

				else if (ai == 2)
					aiSelection = 'p';

				else
					aiSelection = 's';


				if (validSelection(selection) && validSelection(aiSelection))
				{
					player1 = retrievePlay(selection);
					aiPlayer = retrievePlay(aiSelection);
					gameResult(player1, aiPlayer, gameWinner);

					if (gameWinner == 1)
						playerOne.wins += 1;

				}

				cout << "Enter Y/y to play again: ";
				cin >> response;
				cout << endl;
			} while (response == 'Y' || response == 'y');

			outFile << endl;
			outFile << playerOne.rank << " " << playerOne.name << " " << playerOne.wins << endl;
			cout << flush;
			system("CLS");

		}

		if (choice == 2)
		{
			playerOne.rank += 1;
			playerOne.wins = 0;
			for (int i = 0; i < sizeof(playerOne.name); i++)
			{
				playerOne.name[i] = '\0';
			}

			cout << "Enter your name: ";
			cin >> playerOne.name;
			cout << endl;
		}

		if (choice == 3)
		{
			cout << "Rank   Player Name   No Wins" << endl;
			cout << "------------------------------" << endl;
			for (int j = 0; j < 5; j++)
			{
				inFile >> playerOne.rank >> playerOne.name >> playerOne.wins;
				cout << playerOne.rank << " " << playerOne.name << " " << playerOne.wins;
				cout << endl;
			}
			cout << endl;
			system("pause");
		}
		cout << "Current Player Name: <" << playerOne.name << ">" << endl;
		cout << "1. Start the game" << endl;
		cout << "2. Change the player name" << endl;
		cout << "3. Show highest scores" << endl;
		cout << "4. Exit" << endl;
		cout << "Enter your choice (1, 2, 3, 4): ";
		cin >> choice;
		cout << endl;
	}

	if (choice == 4)
	{
		cout << "Until next time!" << endl;
		outFile.close();
		inFile.close();
		return 0;
	}

	outFile << playerOne.rank << " " << playerOne.name << " " << playerOne.wins << endl;
	cout << "Thank you for playing!" << endl;
	cout << "Created by: Andre Reano 101075780" << endl;
	system("pause");
	outFile.close();
	inFile.close();
    return 0;
}

void displayRules()
{
	cout << "Welcome to the game of Rock, Paper, and Scissors." << endl;
	cout << "This game is for 2 players. For each game, each player will select either Rock, Paper, or Scissors." << endl;
	cout << "The rules for the game are the following: " << endl;
	cout << "1. If both players selects the same object, it is a tie." << endl;
	cout << "2. Rock breaks Scissors: The player who selects Rock wins." << endl;
	cout << "3. Paper covers Rock: The player who selects Paper wins." << endl;
	cout << "4. Scissors cuts Paper: The player who selects Sciossor wins." << endl;
	cout << "Enter R or r for Rock, P or p for Paper, S or s for Scissors." << endl << endl;
}

bool validSelection(char selection)
{
	switch (selection)
	{
	case 'R':
	case 'r':
	case 'S':
	case 's':
	case 'P':
	case 'p':
		return true;
	default:
		return false;
	}
}

objectType retrievePlay(char selection)
{
	objectType object;
	switch (selection)
	{
	case 'R':
	case 'r':
		object = ROCK;
		break;
	case 'P':
	case 'p':
		object = PAPER;
		break;
	case 'S':
	case 's':
		object = SCISSORS;
		break;
	}
	return object;
}

void convertEnum(objectType object)
{
	switch (object)
	{
	case ROCK:
		cout << "Rock";
		break;
	case PAPER:
		cout << "Paper";
		break;
	case SCISSORS:
		cout << "Scissors";
		break;
	default:
		break;
	}
}

objectType winningObject(objectType player1, objectType aiPlayer)
{
	if ((player1 == ROCK && aiPlayer == SCISSORS) || (aiPlayer == ROCK && player1 == SCISSORS))
		return ROCK;
	if ((player1 == ROCK && aiPlayer == PAPER) || (aiPlayer == ROCK && player1 == PAPER))
		return PAPER;
	else
		return SCISSORS;
}

void gameResult(objectType player1, objectType aiPlayer, int& winner)
{
	objectType winnerObject;
	if (player1 == aiPlayer)
	{
		winner = 0;
		cout << "Both players selected: ";
		convertEnum(player1);
		cout << endl;
		cout << "This game is a tie." << endl;
	}
	else
	{
		winnerObject = winningObject(player1, aiPlayer);
		cout << "Player 1 selected ";
		convertEnum(player1);
		cout << endl;
		cout << "AI player selected ";
		convertEnum(aiPlayer);
		cout << endl;
		if (player1 == winnerObject)
		{
			winner = 1;
			cout << "Player wins the game." << endl;
		}
		else if (aiPlayer == winnerObject)
		{
			winner = 2;
			cout << "AI player wins this game." << endl;
		}
	}
}