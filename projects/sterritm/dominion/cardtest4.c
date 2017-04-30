/*
Name: Michael Sterritt
Date: 4/29/17
Description: This file is a unit test for dominion.c's adventurer effect
*/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "testHelper.h"

/*Tests adventurer card*/
int testAdventurer(int p, struct gameState *pre, struct gameState *post, int r, int newTreasure, int nonTreasure, int discarded, int expectedValue, int shuffled);
int countTreasures(int p, struct gameState *state);

int main() {
	int seed = 500;
	int numPlayer;
	int p, r, bonus;
	int k[10] = { adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall };
	struct gameState pre;
	struct gameState post;
	bonus = 0;

	/*create decks and discard piles to test with*/
	int testHand[] = { adventurer, smithy, gardens, mine, copper };
	int test1Deck[] = {adventurer, smithy, feast, gardens, copper, copper};
	int test2Deck[] = {adventurer, smithy, feast, gardens, silver, silver};
	int test3Deck[] = {adventurer, smithy, feast, gardens, gold, gold};
	int test4Deck[] = {adventurer, copper, smithy, feast, gardens, copper};
	int test5Deck[] = {silver, adventurer, smithy, gold, feast, gardens};
	int test6Deck[] = {adventurer, smithy, feast, gardens, copper};
	int test6Discard[] = {adventurer, silver, feast, gardens};
	int test7Deck[] = {adventurer, smithy, feast, gardens};
	int test7Discard[] = {adventurer, smithy, feast, gardens};

	printf("----------------------Test Card: adventurer----------------------\n");
	for (numPlayer = 2; numPlayer <= MAX_PLAYERS; numPlayer++) {			//test game with various player numbers
		for (p = 0; p < numPlayer; p++) {									//test for various current players
			printf("-----Test player %d with %d players in game.-----\n", p, numPlayer);

			memset(&post, 0, sizeof(struct gameState));	//clear game state
			r = initializeGame(numPlayer, k, seed, &post);	//initialize new game
			memcpy(post.hand[p], testHand, 5 * sizeof(int));
			post.handCount[p] = 5;
			post.whoseTurn = p;

			/*Test 1: Copper top two cards of deck*/
			memcpy(post.deck[p], test1Deck, 6 * sizeof(int));
			post.deckCount[p] = 6;
			memcpy(&pre, &post, sizeof(struct gameState));
			r = cardEffect(adventurer, 0, 0, 0, &post, 0, &bonus);
			testAdventurer(p, &pre, &post, r, 2, 0, 1, 2, 0);

			/*Test 2: Silver top two cards of deck*/
			memcpy(&post, &pre, sizeof(struct gameState));			//return to pre condition
			memcpy(post.deck[p], test2Deck, 6 * sizeof(int));
			post.deckCount[p] = 6;
			memcpy(&pre, &post, sizeof(struct gameState));
			r = cardEffect(adventurer, 0, 0, 0, &post, 0, &bonus);
			testAdventurer(p, &pre, &post, r, 2, 0, 1, 4, 0);

			/*Test 3: Gold top two cards of deck*/
			memcpy(&post, &pre, sizeof(struct gameState));			//return to pre condition
			memcpy(post.deck[p], test3Deck, 6 * sizeof(int));
			post.deckCount[p] = 6;
			memcpy(&pre, &post, sizeof(struct gameState));
			r = cardEffect(adventurer, 0, 0, 0, &post, 0, &bonus);
			testAdventurer(p, &pre, &post, r, 2, 0, 1, 6, 0);

			/*Test 4: Only one treasure at top of deck*/
			memcpy(&post, &pre, sizeof(struct gameState));			//return to pre condition
			memcpy(post.deck[p], test4Deck, 6 * sizeof(int));
			post.deckCount[p] = 6;
			memcpy(&pre, &post, sizeof(struct gameState));
			r = cardEffect(adventurer, 0, 0, 0, &post, 0, &bonus);
			testAdventurer(p, &pre, &post, r, 2, 3, 1, 2, 0);

			/*Test 5: No treasures at top of deck*/
			memcpy(&post, &pre, sizeof(struct gameState));			//return to pre condition
			memcpy(post.deck[p], test5Deck, 6 * sizeof(int));
			post.deckCount[p] = 6;
			memcpy(&pre, &post, sizeof(struct gameState));
			r = cardEffect(adventurer, 0, 0, 0, &post, 0, &bonus);
			testAdventurer(p, &pre, &post, r, 2, 4, 1, 5, 0);

			/*Test 6: One treasure in deck and one in discard*/
			memcpy(&post, &pre, sizeof(struct gameState));			//return to pre condition
			memcpy(post.deck[p], test6Deck, 5 * sizeof(int));
			post.deckCount[p] = 5;
			memcpy(post.discard[p], test6Discard, 4 * sizeof(int));
			post.discardCount[p] = 4;
			memcpy(&pre, &post, sizeof(struct gameState));
			r = cardEffect(adventurer, 0, 0, 0, &post, 0, &bonus);
			testAdventurer(p, &pre, &post, r, 2, 0, 1, 3, 1);

			/*Test 7: No treasures in deck or discard*/
			memcpy(&post, &pre, sizeof(struct gameState));			//return to pre condition
			memcpy(post.deck[p], test7Deck, 4 * sizeof(int));
			post.deckCount[p] = 4;
			memcpy(post.discard[p], test7Discard, 4 * sizeof(int));
			post.discardCount[p] = 4;
			memcpy(&pre, &post, sizeof(struct gameState));
			r = cardEffect(adventurer, 0, 0, 0, &post, 0, &bonus);
			testAdventurer(p, &pre, &post, r, 0, 0, 1, 0, 1);
		}
	}
	return 0;
}

/*function tests if adventurer effect produces the expected end state after it is played. There are numerous parameters used for determining end state, and prints success or failure depending on results of test.*/
int testAdventurer(int p, struct gameState *pre, struct gameState *post, int r, int newTreasure, int nonTreasure, int discarded, int expectedValue, int shuffled) {
	int error = 0;
	error += asserttrue(r, 0);
	error += asserttrue(post->handCount[p], pre->handCount[p] + newTreasure - discarded);
	error += asserttrue(post->playedCardCount, pre->playedCardCount + discarded);
	if (!shuffled) {																									//can only know discard count if shuffling does not occur
		error += asserttrue(post->discardCount[p], pre->discardCount[p] + nonTreasure);
	}
	error += asserttrue(countTreasures(p, post), countTreasures(p, pre) + newTreasure);
	updateCoins(p, pre, 0);
	updateCoins(p, post, 0);
	error += asserttrue(post->coins, pre->coins + expectedValue);
	if (error != 0) {
		printf("handCount = %d, expected = %d\n", post->handCount[p], pre->handCount[p] + newTreasure - discarded);
		printf("playedCardCount = %d, expected = %d\n", post->playedCardCount, pre->playedCardCount + discarded);
		if (!shuffled)
			printf("discardCount = %d, expected = %d\n", post->discardCount[p], pre->discardCount[p] + nonTreasure);
		printf("Treasures in hand = %d, expected = %d\n", countTreasures(p, post), countTreasures(p, pre) + newTreasure);
		printf("Value = %d, expected = %d\n", post->coins, pre->coins + expectedValue);
		printf("TEST FAILED!\n");
	}
	else
		printf("TEST SUCCESSFUL!\n");
	printf("------------------------\n");
	return 0;
}

/*countTreasures counts the number of treasure cards in the selected player's hand.*/
int countTreasures(int p, struct gameState *state) {
	int i;
	int treasure = 0;
	enum CARD c;
	for (i = 0; i < state->handCount[p]; i++) {
		c = state->hand[p][i];
		if (c == copper || c == silver || c == gold) {
			treasure++;
		}
	}
	return treasure;
}