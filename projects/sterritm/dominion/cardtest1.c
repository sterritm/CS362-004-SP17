/*
Name: Michael Sterritt
Date: 4/29/17
Description: This file is a unit test for dominion.c's smithy card effect
*/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "testHelper.h"


/*Tests smithy card*/
int testSmithy(int p, struct gameState *post);

int main() {
	int seed = 500;
	int numPlayer;
	int p, r;
	int k[10] = { adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall };
	struct gameState state;
	printf("----------------------Test Card: smithy----------------------\n");
	for (numPlayer = 2; numPlayer <= MAX_PLAYERS; numPlayer++) {			//test game with various player numbers
		for (p = 0; p < numPlayer; p++) {									//test for various current players
			printf("Test player %d with %d players in game.\n", p, numPlayer);

			/*Test 1: More than 3 cards in deck*/
			memset(&state, 0, sizeof(struct gameState));	//clear game state
			r = initializeGame(numPlayer, k, seed, &state);	//initialize new game
			state.whoseTurn = p;
			state.handCount[p] = 5;
			state.deckCount[p] = 5;
			testSmithy(p, &state);

			/*Test 2: No cards left in deck, at least 3 in discard*/
			memset(&state, 0, sizeof(struct gameState));	//clear game state
			r = initializeGame(numPlayer, k, seed, &state);	//initialize new game
			state.whoseTurn = p;
			state.handCount[p] = 5;
			state.deckCount[p] = 0;
			state.discardCount[p] = 5;
			testSmithy(p, &state);

			/*Test 3: No cards in deck or discard pile*/
			memset(&state, 0, sizeof(struct gameState));	//clear game state
			r = initializeGame(numPlayer, k, seed, &state);	//initialize new game
			state.whoseTurn = p;
			state.handCount[p] = 5;
			state.deckCount[p] = 0;
			state.discardCount[p] = 0;
			testSmithy(p, &state);

			/*Test 4: Only 1 card in deck and none in discard*/
			memset(&state, 0, sizeof(struct gameState));	//clear game state
			r = initializeGame(numPlayer, k, seed, &state);	//initialize new game
			state.whoseTurn = p;
			state.handCount[p] = 5;
			state.deckCount[p] = 1;
			state.discardCount[p] = 0;
			testSmithy(p, &state);

			/*Test 5: Only 1 card in deck and 1 in discard*/
			memset(&state, 0, sizeof(struct gameState));	//clear game state
			r = initializeGame(numPlayer, k, seed, &state);	//initialize new game
			state.whoseTurn = p;
			state.handCount[p] = 5;
			state.deckCount[p] = 1;
			state.discardCount[p] = 1;
			testSmithy(p, &state);
		}
	}
	return 0;
}

/*function tests if smithy draws the appropriate number of cards and prints test successful or failed to console.*/
int testSmithy(int p, struct gameState *post) {
	int choice1 = 0;
	int choice2 = 0;
	int choice3 = 0;
	int handpos = 0;
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
		printf("handCount = %d, expected = %d\n", post->handCount[p], pre.handCount[p] + newCards - discarded);
		printf("deckCount = %d, expected = %d\n", post->deckCount[p], pre.deckCount[p] - newCards + shuffled);
		printf("playedCardCount = %d, expected = %d\n", post->playedCardCount, pre.playedCardCount + discarded);
		printf("TEST FAILED!\n");
	}
	else
		printf("TEST SUCCESSFUL!\n");
	printf("------------------------\n");
	return 0;
}