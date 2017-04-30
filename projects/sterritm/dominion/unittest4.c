/*
Name: Michael Sterritt
Date: 4/29/17
Description: This file is a unit test for dominion.c's gainCard()
*/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "testHelper.h"


/*Tests gainCard() function*/

int checkGainCard(int supplyPos, struct gameState *post, int toFlag, int p);

int main() {
	int seed = 500;
	int numPlayer;
	int p, r, i;
	int k[10] = { adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall };
	struct gameState state;

	printf("Testing gainCard():\n");
	for (numPlayer = 2; numPlayer <= MAX_PLAYERS; numPlayer++) {			//test game with various player numbers
		for (p = 0; p < numPlayer; p++) {									//test for various current players
			printf("Test player %d with %d players in game.\n", p, numPlayer);
			for (i = 0; i < 3; i++) {
				/*Test 1-3: Supply pile of card is empty, card placed in discard, deck, or hand.*/
				memset(&state, 0, sizeof(struct gameState));	//clear game state
				r = initializeGame(numPlayer, k, seed, &state);	//initialize new game
				state.supplyCount[gold] = 0;
				checkGainCard(gold, &state, i, p);
			}
			for (i = 0; i < 3; i++) {
				/*Test 4-6: Supply pile of card is not empty, card placed in discard, deck, or hand.*/
				memset(&state, 0, sizeof(struct gameState));	//clear game state
				r = initializeGame(numPlayer, k, seed, &state);	//initialize new game
				state.supplyCount[gold] = 10;
				checkGainCard(gold, &state, i, p);
			}
		}
	}
	return 0;
}

/*Function checks if gainCard produces the appropriate post conditions and prints successful and failed tests to console.*/
int checkGainCard(int supplyPos, struct gameState *post, int toFlag, int p) {
	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));
	int r;
	r = gainCard(supplyPos, post, toFlag, p);
	int error = 0;


	if (r == -1) {
		error += asserttrue(r,  -1);
		error += asserttrue(memcmp(&pre, post, sizeof(struct gameState)),  0);
		return -1;
	}

	if (toFlag == 1) {
		pre.deck[p][pre.deckCount[p]] = supplyPos;
		pre.deckCount[p]++;
	}
	else if (toFlag == 2) {
		pre.hand[p][pre.handCount[p]] = supplyPos;
		pre.handCount[p]++;
	}
	else {
		pre.discard[p][pre.discardCount[p]] = supplyPos;
		pre.discardCount[p]++;
	}
	pre.supplyCount[supplyPos]--;

	error += asserttrue(r,  0);
	error += asserttrue(memcmp(&pre, post, sizeof(struct gameState)),  0);
	if (error != 0) {
		printf("discardCount = %d, expected = %d\n", post->discardCount[p], pre.discardCount[p]);
		printf("deckCount = %d, expected = %d\n", post->deckCount[p], pre.deckCount[p]);
		printf("handCount = %d, expected = %d\n", post->handCount[p], pre.handCount[p]);
		printf("TEST FAILED!\n");
	}
	else
		printf("TEST SUCCESSFUL!\n");
	printf("------------------------\n");

	return 0;
}