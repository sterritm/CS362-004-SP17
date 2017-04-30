/*
Name: Michael Sterritt
Date: 4/29/17
Description: This file is a unit test for dominion.c's village effect
*/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "testHelper.h"

/*Tests village card*/
int testVillage(int p, struct gameState *post);

int main() {
	int seed = 500;
	int numPlayer;
	int p, r;
	int k[10] = { adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall };
	struct gameState state;
	printf("----------------------Test Card: village----------------------\n");
	for (numPlayer = 2; numPlayer <= MAX_PLAYERS; numPlayer++) {			//test game with various player numbers
		for (p = 0; p < numPlayer; p++) {									//test for various current players
			printf("-----Test player %d with %d players in game.-----\n", p, numPlayer);

			/*Test 1: More than 3 cards in deck*/
			memset(&state, 0, sizeof(struct gameState));	//clear game state
			r = initializeGame(numPlayer, k, seed, &state);	//initialize new game
			state.whoseTurn = p;
			state.handCount[p] = 5;
			state.deckCount[p] = 5;
			testVillage(p, &state);

		}
	}
	return 0;
}

/*function tests if village draws the appropriate number of cards for the player, increases the number of actions, and prints test successful or failed to console depending on results.*/
int testVillage(int p, struct gameState *post) {
	int choice1 = 0;
	int choice2 = 0;
	int choice3 = 0;
	int handpos = 0;
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

	//when deck size is less than 3, then discard pile will be shuffled into deck
	if (pre.deckCount[p] < 1) {
		shuffled = pre.discardCount[p];
	}

	error += asserttrue(r, 0);
	error += asserttrue(post->handCount[p], pre.handCount[p] + newCards - discarded);
	error += asserttrue(post->deckCount[p], pre.deckCount[p] - newCards + shuffled);
	error += asserttrue(post->playedCardCount, pre.playedCardCount + discarded);
	error += asserttrue(post->numActions, pre.numActions + extraActions);
	if (error != 0) {
		printf("handCount = %d, expected = %d\n", post->handCount[p], pre.handCount[p] + newCards - discarded);
		printf("deckCount = %d, expected = %d\n", post->deckCount[p], pre.deckCount[p] - newCards + shuffled);
		printf("playedCardCount = %d, expected = %d\n", post->playedCardCount, pre.playedCardCount + discarded);
		printf("numActions = %d, expected = %d\n", post->numActions, pre.numActions + extraActions);
		printf("TEST FAILED!\n");
	}
	else
		printf("TEST SUCCESSFUL!\n");
	printf("------------------------\n");

	return 0;
}