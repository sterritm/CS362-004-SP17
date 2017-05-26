/*
Name: Michael Sterritt
Date: 5/11/17
Description: This file is a random tester for dominion.c's smithy card effect
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

/*Random tests smithy card*/
int testSmithy(int p, int handpos, struct gameState *post);

int main() {
	srand(time(NULL));
	int i, n, p, handpos;
	int error = 0;
	struct gameState G;
	printf("----------------------Test Card: smithy----------------------\n");
	for (n = 0; n < 200000; n++) {
		for (i = 0; i < sizeof(struct gameState); i++) {
			((char*)&G)[i] = floor(Random() * 256);				//create gamestate G filled with random bytes - code from CS 362 lesson 11 lecture
		}
		p = floor(Random() * MAX_PLAYERS);
		G.whoseTurn = p;
		G.deckCount[p] = floor(Random() * MAX_DECK);
		G.discardCount[p] = floor(Random() * MAX_DECK);
		G.handCount[p] = floor(Random() * MAX_HAND);
		handpos = floor(Random() * G.handCount[p]);
		G.playedCardCount = floor(Random() * MAX_DECK);
		if (DEBUG)
			printf("player: %d, deck: %d, discard: %d, hand: %d, played: %d\n", p, G.deckCount[p], G.discardCount[p], G.handCount[p], G.playedCardCount);
		error += testSmithy(p, handpos, &G);
	}
	if (error != 0) {
		printf("%d of %d tests failed.\n", error, n);
	}
	else
		printf("All tests successful!\n");
	return 0;
}

/*
Descriptions: function tests if smithy draws the appropriate number of cards for the player. Returns boolean value stating if test was successful or not.
Parameters: player, handPos, random gameState
Returns: 0 if no errors, 1 if error
*/
int testSmithy(int p, int handpos, struct gameState *post) {
	int choice1 = 0;
	int choice2 = 0;
	int choice3 = 0;
	int bonus = 0;
	int newCards;
	int discarded = 1;
	int shuffled = 0;
	int error = 0;

	post->hand[p][handpos] = smithy;
	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));
	int r;
	r = cardEffect(smithy, choice1, choice2, choice3, post, handpos, &bonus);
	//change values for number of cards possible to draw
	if (pre.deckCount[p] + pre.discardCount[p] < 3) {
		newCards = pre.deckCount[p] + pre.discardCount[p];
	}
	else {
		newCards = 3;
	}
	//when deck size is less than 3, then discard pile will be shuffled into deck
	if (pre.deckCount[p] < 3) {
		shuffled = pre.discardCount[p];
	}
	error += asserttrue(r, 0);
	error += asserttrue(post->handCount[p], pre.handCount[p] + newCards - discarded);
	error += asserttrue(post->deckCount[p], pre.deckCount[p] - newCards + shuffled);
	error += asserttrue(post->playedCardCount, pre.playedCardCount + discarded);
	if (error != 0) {
		if (DEBUG) {
			printf("handCount = %d, expected = %d\n", post->handCount[p], pre.handCount[p] + newCards - discarded);
			printf("deckCount = %d, expected = %d\n", post->deckCount[p], pre.deckCount[p] - newCards + shuffled);
			printf("playedCardCount = %d, expected = %d\n", post->playedCardCount, pre.playedCardCount + discarded);
			printf("TEST FAILED!\n");
		}
		return 1;
	}
	else {
		if (DEBUG)
			printf("TEST SUCCESSFUL!\n");
		return 0;
	}
}