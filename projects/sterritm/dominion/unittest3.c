#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "testHelper.h"

/*Tests buyCards() function*/

int checkBuyCard(struct gameState *post, int p);

int main() {
	int seed = 500;
	int numPlayer;
	int p, r;
	int k[10] = { adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall };
	struct gameState state;
	struct gameState pre;

	printf("Testing buyCard():\n");
	for (numPlayer = 2; numPlayer <= MAX_PLAYERS; numPlayer++) {			//test game with various player numbers
		for (p = 0; p < numPlayer; p++) {									//test for various current players
			printf("Test player %d with %d players in game.\n", p, numPlayer);

			memset(&state, 0, sizeof(struct gameState));	//clear game state
			r = initializeGame(numPlayer, k, seed, &state);	//initialize new game
			state.whoseTurn = p;
			memcpy(&pre, &state, sizeof(struct gameState));

			/*Test 1: No buy actions left for player.*/
			state.numBuys = 0;
			state.coins = 100;
			state.supplyCount[gold] = 10;
			checkBuyCard(&state, p);

			/*Test 2: Player does not have enough coins to purchase card.*/
			memcpy(&state, &pre, sizeof(struct gameState));
			state.numBuys = 1;
			state.coins = 0;
			state.supplyCount[gold] = 10;
			checkBuyCard(&state, p);

			/*Test 3: Player has sufficient buys and coins, but there are no more of that card in the kingdom.*/
			memcpy(&state, &pre, sizeof(struct gameState));
			state.numBuys = 1;
			state.coins = 100;
			state.supplyCount[gold] = 0;
			checkBuyCard(&state, p);

			/*Test 4: Player has sufficient buys and coins, and there is at least 1 card left in kingdom.*/
			memcpy(&state, &pre, sizeof(struct gameState));
			state.numBuys = 1;
			state.coins = 100;
			state.supplyCount[gold] = 10;
			checkBuyCard(&state, p);
		}
	}
	return 0;
}

int checkBuyCard(struct gameState *post, int p) {
	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));
	int r;
	r = buyCard(gold, post);
	if (r == 0) {
		pre.numBuys--;
		pre.coins = pre.coins - 6;
		pre.discard[p][pre.discardCount[p]] = gold;
		pre.discardCount[p]++;
		pre.supplyCount[gold]--;
	}
	else {
		assert(r == -1);
	}

	int error = 0;
	error += asserttrue(post->numBuys, pre.numBuys);
	error += asserttrue(post->coins, pre.coins);
	error += asserttrue(post->supplyCount[gold], pre.supplyCount[gold]);
	error += asserttrue(post->discardCount[p], pre.discardCount[p]);
	error += asserttrue(post->discard[p][post->discardCount[p] - 1], pre.discard[p][pre.discardCount[p] - 1]);

	if (error != 0) {
		printf("state.numBuys = %d, expected = %d\n", post->numBuys, pre.numBuys);
		printf("state.coins = %d, expected = %d\n", post->coins, pre.coins);
		printf("state.supplyCount[gold] = %d, expected = %d\n", post->supplyCount[gold], pre.supplyCount[gold]);
		printf("state.discardCount = %d, expected = %d\n", post->discardCount[p], pre.discardCount[p]);
		printf("Top of discard = %d, expected = %d\n", post->discard[p][post->discardCount[p] - 1], pre.discard[p][pre.discardCount[p] - 1]);
		printf("TEST FAILED!\n");
	}
	else
		printf("TEST SUCCESSFUL!\n");
	printf("------------------------\n");
	
	return 0;
}