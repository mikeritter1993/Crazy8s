//Michael Ritter due date: ??/??/2015
//crazy8s part 6
//description: Make the game work 100% (end conditions ect.). extra credit: ability to play with infite amount of players, let user choice which suit they want when they play a 8.

#define PRINT_HEART printf("%c", 3);
#define PRINT_DIAMOND printf("%c", 4);
#define PRINT_CLUB printf("%c", 5);
#define PRINT_SPADE printf("%c", 6);
#define CardS struct Card
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

//Structs
struct Card
{
	int card;
	CardS *link;
};

//Prototypes

	//print functions
int printHand(CardS *head);													//prints 1 players hand
void printAllHands(CardS heads[], int numPlayers);						//prints every hand in the game
void printCard(int cardNum);													//prints a single card
void printScores(int *scores, int numPlayers);							//prints each players score
void printStandings(int *scores, int numPlayers);						//prints each players score and what ranking they stand in relation to every other player
void printWinner(CardS *heads, CardS *winner, int numPlayers);		//prints out which player won the game
void printDirections();															//prints the directions for the game crazy8s

	//delete functions
void deleteHand(CardS *head);													//deletes 1 players hand
void deleteAllHands(CardS *heads, int numPlayers);						//deletes all the hands in the game
void deleteCard(CardS* head, CardS *cardToDelete);						//deletes a single card in a hand

	//score functions
int scoreCard(int cardNum);													//scores a single card
int scoreHand(CardS *head);													//scores 1 players hand
void scoreAllHands(CardS *heads, int *scores, int numPlayers);		//scores all the hands in the game

	//key game functions
void dealStartingHand(CardS *heads, int numPlayers);					//deals starting amount of cards to each player dependent on how many players are playing
int validPlay(int *pile, int cardPlayed, int whatToDo);				//checks if the cardPlayed is a valid card to play in the contexts of the rules
int playedEight();																//used when a human plays a 8 and asks them which suit they would like
CardS* isWinner(CardS *head);													//checks if a player has won the game

	//other functions
int getCard(int numPlayers);													//gets a random card from the deck
int addCard(CardS *head);														//adds a card to a players hand
CardS* findCard(CardS *head, int index);									//finds a card with a certain ordinal value in a players hand
int playerTurn(CardS *head, int *pile);									//completes 1 human players turn
int computerTurn(CardS *head, int *pile);									//completes 1 computer player turn



void main()
{
	srand(time(NULL));
	int numPlayers;			//user enetered number of players playing
	int numDecks;				//1 more deck used every 5 players playing the game
	int pile;					//top of the pile
	int outOfCards = 0;		//bool if the deck is out of cards
	int *scores;				//dynamic array that holds each players score
	char playAgain = 'y';	//user choice if they want to play another game
	CardS *winner = NULL;	//pointer to a players head if they are the winner
	CardS *heads;				//dynamic array of dummy head pointers for each player
	
	
	printDirections();

	do
	{
		printf("Enter how many players are going to play: ");
		scanf("%d", &numPlayers);
	} while (numPlayers < 2);

	numDecks = numPlayers / 5 + 1;
	getCard(numDecks);												//pass in number of decks to be used in order to set statics.
	
	scores = (int*)malloc(sizeof(int)*numPlayers);			//dynamicly create a array of head pointers and scores for each player, set defualt values.
	heads = (CardS*)malloc(sizeof(CardS)*numPlayers);		
	for (int i = 0; i < numPlayers; i++)
	{
		scores[i] = 0;
		heads[i].link = NULL;
		heads[i].card = -1;
	}
	

	while (playAgain == 'y')										//master loop: runs 1 game of crazy8s 
	{
			//reset game
		deleteAllHands(heads, numPlayers);						
		winner = NULL;													
		pile = getCard(0);
		validPlay(&pile, pile, 0);
		dealStartingHand(heads, numPlayers);

		while (winner == NULL && outOfCards == 0)
		{
				//player turn
			if (winner == NULL)
			{
				outOfCards = playerTurn(&heads[0], &pile);
				winner = isWinner(&heads[0]);
			}

				//computer turns
			for (int i = 1; i < numPlayers; i++)
			{
				if (winner == NULL)
				{
					printf("\nPlayer %d Turn.", i + 1);
					printf("\nPlayer %d Hand: ", i + 1);
					printHand(&heads[i]);
					outOfCards = computerTurn(&heads[i], &pile);
					winner = isWinner(&heads[i]);
					printf("\n");
				}
			}

		}
			//print winner, everyones hand and scores
		printWinner(heads, winner, numPlayers);
		printAllHands(heads, numPlayers);
		scoreAllHands(heads, scores, numPlayers);
		printScores(scores, numPlayers);

		printf("\n\nWould you like to play again? (y/n): ");
		fflush(stdin);
		scanf("%c", &playAgain);
	} 

	printStandings(scores, numPlayers);			//all games played, print rankings
	free(heads);

	printf("\n\n");
	system("pause");
}

/*****************************************************___PRINT FUNCTIONS___******************************************************************************************************/
//Return: number of cards in players hand.
//Parameters: CardS pointer to the players head.
//Description: prints a players hand with a index of what spot in the linked list that card is at.
int printHand(CardS *head)
{
	int count = 1;
	CardS *current = head->link;
	while (current != NULL)
	{
		printf("     %d: ", count);
		printCard(current->card);
		current = current->link;
		count++;
	}
	return count;
}

//Return: none.
//Parameters: CardS array, int, array of all the head pointers for each player, int how many players there are.
//Description: calls printHand and prints all the hands in the game.
void printAllHands(CardS heads[], int numPlayers)
{
	for (int i = 0; i < numPlayers; i++)	//print each players hand
	{
		printf("\nPlayer %d hand: ", i + 1);
		printHand(&heads[i]);
		printf("\n");
	}
}

//Return: none.
//Parameters: ordinal value of card to be printed.
//Description: prints a single card based on ordinal value.
void printCard(int cardNum)
{
	int faceNum;
	int suitNum;

	faceNum = cardNum % 13 + 2;
	switch (faceNum)
	{
	case 11:
		printf("%s", "Jack");
		break;

	case 12:
		printf("%s", "Queen");
		break;

	case 13:
		printf("%s", "King");
		break;

	case 14:
		printf("%s", "Ace");
		break;

	default:
		printf("%d", faceNum);
		break;
	}

	suitNum = cardNum / 13;
	switch (suitNum)
	{
	case 0:
		PRINT_CLUB
			break;

	case 1:
		PRINT_DIAMOND
			break;

	case 2:
		PRINT_HEART
			break;

	case 3:
		PRINT_SPADE
			break;

	default:
		break;
	}
}

//Return: none.
//Parameters: int array, int, The array that stores each players score, number of players in the game.
//Description: prints each players score.
void printScores(int scores[], int numPlayers)
{
	printf("\n");
	for (int i = 0; i < numPlayers; i++)
	{
		printf("\nPlayer %d Score: %d", i + 1, scores[i]);
	}
	printf("\n");
}

//Return: none.
//Parameters: int array, int, The array that stores each players score, number of players in the game.
//Description: sorts the scores array in ascending order (spot 1 is first place spot 2 is second place ect.), prints rankings with which player had that score and what the score was.
void printStandings(int scores[], int numPlayers)
{
	int bool = 1;
	int hold;
	int *indexOf = (int*)malloc(sizeof(int)*numPlayers);  //paralell array with scores array, keeps track of which score is which player after sorting.

	for (int i = 0; i < numPlayers; i++)
	{
		indexOf[i] = i;
	}

	while (bool == 1)											//bubble sort
	{
		bool = 0;
		for (int i = 0; i < numPlayers - 1; i++)
		{
			if (scores[i] > scores[i + 1])
			{
				bool = 1;
				hold = scores[i];
				scores[i] = scores[i + 1];
				scores[i + 1] = hold;

				hold = indexOf[i];
				indexOf[i] = indexOf[i + 1];
				indexOf[i + 1] = hold;
			}
		}
	}

	printf("Final rankings (lower the score the better, golf rules): \n");
	for (int i = 0; i < numPlayers; i++)
	{
		printf("\n%d place: Player %d with a score of: %d", i + 1, indexOf[i] + 1, scores[i]);
	}
	free(indexOf);
}

//Return: none.
//Parameters: CardS array, CardS pointer, int, array of head pointers, pointer to the winner, number of players.
//Description: Finds and prints which player was the winner.
void printWinner(CardS heads[], CardS *winner, int numPlayers)
{
	for (int i = 0; i < numPlayers; i++)
	{
		if (&heads[i] == winner)
		{
			printf("\n\nPlayer %d is the winner!!!!", i + 1);
		}
	}
}

//Return: none.
//Parameters: none.
//Description: prints the directions for the game crazy8s.
void printDirections()
{
	printf("\t\t\tCRAZY EIGHTS DIRECTIONS\n\n");
	printf("Each player gets 5 cards (7 if only 2 people are playing), from the undelt deck the top card is placed to the side of the deck and turned face up (this is \ncalled the discard pile).\n\n");
	printf("The goal of the game is to be the first player to get rid of all of their cards, the other players get penalty points for cards left in their CardS. 50 for a 8, 10 for a face card, and face value for all other cards (ace is 1).\n\n");
	printf("Each player takes their turn by either \n1: placing a legal card on top of the discard pile, or \n2: drawing a card from the deck.\n\n");
	printf("There are 3 main rules to this game.\n");
	printf("1: If the top card on the discard pile is not a 8 you may play any card of equal face value or suit type on top of the pile.\n\n");
	printf("2: You may play a 8 on top of any card, whoever places the 8 must pick a suit.\n\n");
	printf("3: If an 8 is on top of the pile, you must play either another 8 or any card of the suit picked by the player who played the 8.\n\n\n\n");
}


/*****************************************************___DELETE FUNCTIONS___******************************************************************************************************/
//Return: none.
//Parameters: CardS pointer, pointer to the head of which player is to be deleted.
//Description: deletes 1 players hand (linked list).
void deleteHand(CardS *head)
{
	CardS *current = head->link;
	CardS *previous;

	while (current->link != NULL)
	{
		previous = current;
		current = current->link;
		free(previous);
	}
	free(current);
}

//Return: none.
//Parameters: CardS array, int, array of head pointers to each players hand, number of players.
//Description: calls deleteHand in a loop to delete all player hands in the game.
void deleteAllHands(CardS heads[], int numPlayers)
{
	for (int i = 0; i < numPlayers; i++)
	{
		if (heads[i].link != NULL)
		{
			deleteHand(&heads[i]);
			heads[i].link = NULL;
		}
	}
}

//Return: none.
//Parameters: CardS pointer, CardS pointer, pointer to which hand to delete from, pointer to which card to delete in the hand.
//Description: searches through players hand to find the card to be deleted, frees and sets links up after deletion.
void deleteCard(CardS *head, CardS *cardToDelete)
{
	CardS *current = head->link;
	CardS *previous = head;
	while (current != cardToDelete)
	{
		previous = current;
		current = current->link;
	}
	current = current->link;
	previous->link = current;
	free(cardToDelete);
}



/*****************************************************___SCORE FUNCTIONS___******************************************************************************************************/
//Return: int, score of the card passed in.
//Parameters: ordinal value of card to be scored.
//Description: scores a single card according to game rules.
int scoreCard(int cardNum)
{
	int faceValue = cardNum % 13 + 2;
	int cardScored;

	switch (faceValue)
	{
	case 8:
		cardScored = 50;
		break;
	case 11:
	case 12:
	case 13:
		cardScored = 10;
		break;
	case 14:
		cardScored = 1;

	default:
		cardScored = faceValue;
		break;
	}

	return cardScored;
}

//Return: int, total score of the hand.
//Parameters: CardS pointer, pointer to the hand to be scored.
//Description: calls scoreCard and scores each card in the players hand.
int scoreHand(CardS *head)
{
	CardS *current = head->link;
	int total = 0;

	while (current != NULL)
	{
		total += scoreCard(current->card);
		current = current->link;
	}

	return total;
}

//Return: none
//Parameters: CardS array, int array, int, array of head pointers, array of each players scores, number of players.
//Description: calls scoreHand and scores every hand in the game.
void scoreAllHands(CardS heads[], int scores[], int numPlayers)
{
	for (int i = 0; i < numPlayers; i++)
	{
		scores[i] += scoreHand(&heads[i]);
	}
}



/*****************************************************___KEY GAME FUNCTIONS___******************************************************************************************************/
//Return: none
//Parameters: CardS array, int, array of player hands, number of players.
//Description: deals the appropriate amount of cards to each player depending on how many players there are playing.
void dealStartingHand(CardS heads[], int numPlayers)
{
	if (numPlayers > 2)						//if more then 2 players deal starting hand of 5 cards
	{
		for (int i = 0; i < numPlayers; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				addCard(&heads[i]);
			}
		}
	}
	else									//else deal 7
	{
		for (int i = 0; i < numPlayers; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				addCard(&heads[i]);
			}
		}
	}
}

//Return: int, bool to represent if a valid play happened or not.
//Parameters: int, int, int, top of the pile by reference, card trying to be played, 0 if reseting game 1 if human called function 2 if computer called function.
//Description: checks if the card being played is a valid play to make in the contexts of the rules.
int validPlay(int *pile, int cardPlayed, int whatToDo)
{
	int bool = 0;										//valid play or not occured.
	int pileFace = *pile % 13 + 2;				
	int pileSuit = *pile /13;
	int playedFace = cardPlayed % 13 + 2;
	int playedSuit = cardPlayed / 13;
		
	
	if (whatToDo == 0)			//reset pile
	{
		pile = cardPlayed;
	}	
	else if (whatToDo == 2 && (pileSuit == playedSuit || pileFace == playedFace || playedFace == 8))	//if computer calls function and plays a 8 just leave the suit the same as what the 8 was.
	{
		*pile = cardPlayed;
		bool = 1;
		printf("\nNew top of Pile: ");
		printCard(*pile);
	}
	else if (whatToDo == 1 && (pileSuit == playedSuit || pileFace == playedFace || playedFace == 8))	//human called 
	{
		if (playedFace == 8)
		{
			*pile = playedEight();
			bool = 1;
			printf("\nNew top of Pile: ");
			printCard(*pile);
		}
		else
		{
			*pile = cardPlayed;
			bool = 1;
			printf("\nNew top of Pile: ");
			printCard(*pile);
		}
	}

	return bool;
}

//Return: int, new ordinal value.
//Parameters: none.
//Description: if a human player played a 8 ask them which suit they want and switch the ordinal value of the card to be the 8 of that suit.
int playedEight()
{
	int choice;
	printf("\nYou played a 8 pick any suit you want. 1: "); PRINT_CLUB
		printf("  2: "); PRINT_DIAMOND
		printf("  3: "); PRINT_HEART
		printf("  4: "); PRINT_SPADE
		scanf("%d", &choice);
	choice--;

	switch (choice)
	{
	case 0:
		choice = 6;
		break;
	case 1:
		choice = 19;
		break;
	case 2:
		choice = 32;
		break;
	case 3:
		choice = 45;
		break;
	default:
		break;
	}

	return choice;
}

//Return: CardS pointer, pointer to which player has won the game if any.
//Parameters: CardS pointer, pointer to which players hand to be checked.
//Description: really simple just checks if the dummy head pointer is pointing to NULL, if it is then they are out of cards and have won the game.
CardS* isWinner(CardS *head)
{
	CardS* winner = NULL;

	if (head->link == NULL)
	{
		winner = head;
	}

	return winner;
}



/*****************************************************___OTHER FUNCTIONS___******************************************************************************************************/
//Return: int, ordinal value of a card
//Parameters: int, first time called has number of decks to be used, if 0 then it means reset the deck, all other values are garbage and ignored.
//Description: generates a random card and makes sure that only the correct amounts of copies of that card are allowed to be delt.
int getCard(int numDecks)
{
	static int cardList[52] = { 0 };		//my "deck"
	static int decks = -1;					//used to know how many copys of a card are ok (2 decks being used means 2 copys of each card are valid)
	static int totalPossibleCards;		//number of decks * 52 = how many cards can be delt by this function
	static int totalDelt = 0;				//counter of how many cards have been delt

	if (decks == -1)							//if first time called set statics
	{
		decks = numDecks;
		totalPossibleCards = decks * 52;
	}
	if (numDecks == 0)						//reset deck
	{
		totalDelt = 0;
		for (int i = 0; i < 52; i++)
		{
			cardList[i] = 0;
		}
	}

	
	int randNum = -1;
	int bool = 0;

	while (bool != 1 && totalDelt < totalPossibleCards)
	{
		randNum = rand() % 52;
		if (cardList[randNum] < decks)
		{
			cardList[randNum]++;
			bool = 1;
		}
	}

	totalDelt++;
	return randNum;
}

//Return: int, bool if the deck is out of cards.
//Parameters: CardS pointer, pointer to the players hand that is to be added 2.
//Description: makes a newNode and adds it to the back of players hand, if getCard returns a -1 then the deck is out of cards then delete the node and return bool of true.
int addCard(CardS *head)
{
	int outOfCards = 0;
	CardS *current = head;
	CardS *newNode = (CardS*)malloc(sizeof(CardS));
	newNode->link = NULL;
	newNode->card = getCard(-1);

	if (newNode->card != -1)
	{
		while (current->link != NULL)
		{
			current = current->link;
		}
		current->link = newNode;
	}
	else
	{
		free(newNode);
		outOfCards = 1;
	}
	return outOfCards;
}

//Return: CardS pointer, points to the card at that index of the linked list.
//Parameters: CardS pointer, int, pointer to players hand, index we are looking for. 
//Description: moves current to currents link "index" amount of times and then returnes a pointer to that node. (index verifyed to not go off end of linked list before function is called)
CardS* findCard(CardS *head, int index)
{
	CardS *current = head->link;
	for (int i = 0; i < index - 1; i++)
	{
		current = current->link;
	}

	return current;
}

//Return: int, bool if deck is out of cards.
//Parameters: CardS pointer, int, pointer to players hand, top of the pile by reference.
//Description: goes through a human players turn in the game, verifys that the card they pick is a valid play and that they dont go off the end of the linked list.
int playerTurn(CardS *head, int *pile)
{
	CardS *choice;			//pointer to players choice of card
	int bool = 0;			//bool if card picked was a valid play
	int indexChoice;		//index of card selected
	int numCards;			//amount of cards in players hand
	int outOfCards = 0;	//bool if deck is out of cards

	printf("\nPile is: ");
	printCard(*pile);
	printf("\nYour Hand: ");
	numCards = printHand(head);

	printf("\n\nSelect which card you want to play (enter a '0' to draw a card): ");
	scanf("%d", &indexChoice);

	while (indexChoice >= numCards || indexChoice < 0)				//off the end of linked list check
	{
		printf("That was not a valid index choice try again.");
		printf("\n\nSelect which card you want to play (enter a '0' to draw a card): ");
		scanf("%d", &indexChoice);
	}

	if (indexChoice == 0)							//drawing a card
	{
		outOfCards = addCard(head);
		bool = 1;
	}
	else												//check for valid play, remove if true
	{
		choice = findCard(head, indexChoice);
		bool = validPlay(pile, choice->card, 1);
		if (bool == 1)
		{
			deleteCard(head, choice);
		}
	}

	while (bool == 0)									//card selected was not a valid play
	{
		printf("\nThat was not a valid play try again");
		printf("\nPile is: ");
		printCard(*pile);
		printf("\nYour Hand: ");
		printHand(head);
		printf("\n\nSelect which card you want to play (enter a '0' to draw a card): ");
		scanf("%d", &indexChoice);
		if (indexChoice == 0)
		{
			outOfCards = addCard(head);
			bool = 1;
		}
		else
		{
			choice = findCard(head, indexChoice);
			bool = validPlay(pile, choice->card, 1);
			if (bool == 1)
			{
				deleteCard(head, choice);
			}
		}
	}

	return outOfCards;
}

//Return: int, bool if deck is out of cards.
//Parameters: CardS pointer, int, pointer to players hand, top of the pile by reference.
//Description: searches through a computers hand for a valid card to play (first occurence is picked), draws a card if no valid play is found.
int computerTurn(CardS *head, int *pile)
{
	CardS *current = head->link;
	int bool = 0;
	int outOfCards = 0;

	while (bool == 0 && head->link != NULL && current != NULL)
	{
		bool = validPlay(pile, current->card, 2);
		if (bool == 0)
		{
			current = current->link;
		}
		else
		{
			deleteCard(head, current);
		}
	}
	if (bool == 0)
	{
		outOfCards = addCard(head);
	}
	return outOfCards;
}

//problems: had to do some getto-ish changes to some functions to get more information from them or things along that line, 
//prolly did some "not ok programming practices" in some places to get it to work without rewriting a ton of code.