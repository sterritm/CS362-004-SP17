/*
Name: Michael Sterritt
Date: 5/11/17
Description: This file is a random tester for dominion.c's village card effect
*/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rngs.h"
#include "testHelper.h"
#include <math.h>
#include <time.h>


/*Random tests Village card*/
int testVillage(int p, int handpos, struct gameState *post);

int main() {
	srand(time(NULL));
	int i, n, p, handpos;
	struct gameState G;
	int error = 0;
	printf("----------------------Test Card: village----------------------\n");
	for (n = 0; n < 200000; n++) {
		for (i = 0; i < sizeof(struct gameState); i++) {
			((char*)&G)[i] = floor(Random() * 256);				//create gamestate G filled with random bytes - code from CS 362 lesson 11 lecture
		}
		//randomize key variables
		p = floor(Random() * MAX_PLAYERS);
		G.whoseTurn = p;
		G.deckCount[p] = floor(Random() * MAX_DECK);
		G.discardCount[p] = floor(Random() * MAX_DECK);
		G.handCount[p] = floor(Random() * MAX_HAND);
		G.numActions = floor(Random() * 1000);
		handpos = floor(Random() * G.handCount[p]);
		G.playedCardCount = floor(Random() * MAX_DECK);
		if (DEBUG)
			printf("player: %d, deck: %d, discard: %d, hand: %d, numActions: %d, played: %d\n", p, G.deckCount[p], G.discardCount[p], G.handCount[p], G.numActions, G.playedCardCount);
		error += testVillage(p, handpos, &G);
	}
	if (error != 0) {
		printf("%d of %d tests failed.\n", error, n);
	}
	else
		printf("All tests successful!\n");
	return 0;
}

/*
Descriptions: function tests if village draws the appropriate number of cards for the player and increases the number of actions as expected. Returns boolean value stating if test was successful or not.
Parameters: player, handPos, random gameState
Returns: 0 if no errors, 1 if error
*/
int testVillage(int p, int handpos, struct gameState *post) {
	int choice1 = 0;
	int choice2 = 0;
	int choice3 = 0;
	int bonus = 0;
	int newCards;
	int discarded = 1;
	int shuffled = 0;
	int extraActions = 2;
	int error = 0;

	post->hand[p][handpos] = village;

	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));

	int r;
	r = cardEffect(village, choice1, choice2, choice3, post, handpos, &bonus);

	//change values for number of cards possible to draw
	if (pre.deckCount[p] + pre.discardCount[p] < 1) {
		newCards = pre.deckCount[p] + pre.discardCount[p];
	}
	else {
		newCards = 1;
	}

	//when deck is empty, then discard pile will be shuffled into deck
	if (pre.deckCount[p] < 1) {
		shuffled = pre.discardCount[p];
	}

	error += asserttrue(r, 0);
	error += asserttrue(post->handCount[p], pre.handCount[p] + newCards - discarded);
	error += asserttrue(post->deckCount[p], pre.deckCount[p] - newCards + shuffled);
	error += asserttrue(post->playedCardCount, pre.playedCardCount + discarded);
	error += asserttrue(post->numActions, pre.numActions + extraActions);
	if (error != 0) {
		if (DEBUG) {
			printf("handCount = %d, expected = %d\n", post->handCount[p], pre.handCount[p] + newCards - discarded);
			printf("deckCount = %d, expected = %d\n", post->deckCount[p], pre.deckCount[p] - newCards + shuffled);
			printf("playedCardCount = %d, expected = %d\n", post->playedCardCount, pre.playedCardCount + discarded);
			printf("numActions = %d, expected = %d\n", post->numActions, pre.numActions + extraActions);
			printf("TEST FAILED!\n");
		}
		return 1;
	}
	else
		return 0;
}