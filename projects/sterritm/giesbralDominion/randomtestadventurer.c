/*
Name: Michael Sterritt
Date: 5/11/17
Description: This file is a random tester for dominion.c's adventurer effect
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

/*Tests adventurer card*/
int testAdventurer(int p, int handpos, struct gameState *post, int deckTreasures, int discardTreasures);
int generateRandomDeck(int p, struct gameState *state);
int generateRandomDiscard(int p, struct gameState *state);
int countDiscarded(int p, struct gameState *state, int limit);
int countTreasures(int p, struct gameState *state);

int main() {
	srand(time(NULL));
	int i, n, p, handpos, deckTreasures, discardTreasures;
	struct gameState G;
	int error = 0;
	printf("----------------------Test Card: adventurer----------------------\n");
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
		deckTreasures = generateRandomDeck(p, &G);
		discardTreasures = generateRandomDiscard(p, &G);
		if (DEBUG) {
			printf("player: %d, deck: %d, discard: %d, hand: %d, played: %d\n", p, G.deckCount[p], G.discardCount[p], G.handCount[p], G.playedCardCount);
			printf("Treasures: %d(deck), %d(discard)\n", deckTreasures, discardTreasures);
		}
		testAdventurer(p, handpos, &G, deckTreasures, discardTreasures);
	}
	if (error != 0) {
		printf("%d of %d tests failed.\n", error, n);
	}
	else
		printf("All tests successful!\n");
	return 0;
}

/*
Description: function tests if adventurer effect produces the expected end state after it is played.
Parameters: player, handPos, random gameState, number of treasures in deck, number of treasures in discard
Returns: 0 if no error, 1 if error
*/
int testAdventurer(int p, int handpos, struct gameState *post, int deckTreasures, int discardTreasures) {
	int choice1 = 0;
	int choice2 = 0;
	int choice3 = 0;
	int bonus = 0;
	int newCards = 2;
	int discarded = 1;
	int nonTreasures;
	int error = 0;

	post->hand[p][handpos] = adventurer;

	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));

	int r;
	r = cardEffect(adventurer, choice1, choice2, choice3, post, handpos, &bonus);
	//check if there are enough treasure cards to draw
	if (deckTreasures + discardTreasures < 2) {
		newCards = deckTreasures + discardTreasures;
	}
	//enough treasures in deck, no shuffling
	if (deckTreasures >= 2) {
		nonTreasures = countDiscarded(p, &pre, 2);		//number of cards discarded from deck
	}
	else if (deckTreasures + discardTreasures >= 2) {		//need to shuffle discard into deck and shuffle
		if (deckTreasures == 1)
			nonTreasures = pre.deckCount[p] - 1;
		else
			nonTreasures = pre.deckCount[p];
		//copy deck and discard piles from post state to account for shuffling
		memcpy(pre.deck[p], post->deck[p], sizeof(int) * pre.discardCount[p]);
		memcpy(pre.discard[p], post->discard[p], sizeof(int) * pre.discardCount[p]);
		pre.deckCount[p] = pre.discardCount[p];
		pre.discardCount[p] = 0;
		//search shuffled deck for number of cards drawn
		nonTreasures += countDiscarded(p, &pre, 2 - deckTreasures);
	}
	else {				//not enough treasures in deck and discard, draw and discard all cards
		nonTreasures = pre.deckCount[p] + pre.discardCount[p] - (deckTreasures + discardTreasures);
	}


	error += asserttrue(r, 0);
	error += asserttrue(post->handCount[p], pre.handCount[p] + newCards - discarded);
	error += asserttrue(post->playedCardCount, pre.playedCardCount + discarded);																									//can only know discard count if shuffling does not occur
	error += asserttrue(post->discardCount[p], pre.discardCount[p] + nonTreasures);
	error += asserttrue(countTreasures(p, post), countTreasures(p, &pre) + newCards);
	if (error != 0) {
		if (DEBUG) {
			printf("handCount = %d, expected = %d\n", post->handCount[p], pre.handCount[p] + newCards - discarded);
			printf("playedCardCount = %d, expected = %d\n", post->playedCardCount, pre.playedCardCount + discarded);
			printf("discardCount = %d, expected = %d\n", post->discardCount[p], pre.discardCount[p] + nonTreasures);
			printf("Treasures in hand = %d, expected = %d\n", countTreasures(p, post), countTreasures(p, &pre) + newCards);
			printf("TEST FAILED!\n");
		}
		return 1;
	}
	else
		return 0;
}

/*
Description: fills deck of current player with random cards, counting the number of treasures placed in deck
Parameters: player, gameState
Returns: number of treasures in deck
*/
int generateRandomDeck(int p, struct gameState *state) {
	int i;
	int treasures = 0;
	enum CARD c;
	//fill deck with random cards
	for (i = 0; i < state->deckCount[p]; i++) {
		c = (Random() * (treasure_map + 1));		//treasure_map is last value in CARD structure
		state->deck[p][i] = c;
		if (c == copper || c == silver || c == gold)
			treasures++;

	}
	return treasures;
}

/*
Description: fills discard pile of current player with random cards, counting the number of treasures placed in discard
Parameters: player, gameState
Returns: number of treasures in discard
*/
int generateRandomDiscard(int p, struct gameState *state) {
	int i;
	int treasures = 0;
	enum CARD c;
	//fill discard with random cards
	for (i = 0; i < state->discardCount[p]; i++) {
		c = (Random() * (treasure_map + 1));		//treasure_map is last value in CARD structure
		state->discard[p][i] = c;
		if (c == copper || c == silver || c == gold)
			treasures++;

	}
	return treasures;
}

/*
Description: checks the order of cards in the deck and counts the number of non-treasure cards viewed until a certain amount of treasure cards are iterated over
Parameters: player, gameState, limit of treasures cards
Returns: number of non-treasure cards iterated over
*/
int countDiscarded(int p, struct gameState *state, int limit) {
	int i = state->deckCount[p] - 1;
	int treasures = 0;
	int discarded = 0;
	enum CARD c;
	while (i >= 0 && treasures < limit) {
		c = state->deck[p][i];
		if (c == copper || c == silver || c == gold)
			treasures++;
		else
			discarded++;
		i--;
	}
	return discarded;
}

/*
Description: countTreasures counts the number of treasure cards in the selected player's hand.
Parameters: player, gameState
Returns: Number of treasure cards in player's hand.
*/
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