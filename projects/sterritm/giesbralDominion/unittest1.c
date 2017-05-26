#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "testHelper.h"

/*Tests updateCoins() function*/

int populateHand(struct gameState *state, int p, int sT, int gT);

int main() {
	int seed = 500;
	int numPlayer;
	int maxBonus = 10;
	int p, r, handCount, cT, sT, gT;
	int bonus;
	int k[10] = { adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall };
	struct gameState state;
	int maxHandCount = 5;	//possible to have up to 600 cards in hand, but for testing purposes, we are only checking for 5 treasures in hand
	int error = 0;

	printf("Testing updateCoins():\n");
	for (numPlayer = 2; numPlayer <= MAX_PLAYERS; numPlayer++) {			//test game with various player numbers
		for (p = 0; p < numPlayer; p++) {									//test for various current players
			for (handCount = 1; handCount <= maxHandCount; handCount++) {	//test for various hand sizes
				for (bonus = 0; bonus <= maxBonus; bonus++) {				//test for various bonuses

					printf("Test player %d with %d treasure card(s) and %d bonus.\n", p, handCount, bonus);

					memset(&state, 0, sizeof(struct gameState));	//clear game state
					r = initializeGame(numPlayer, k, seed, &state);	//initialize new game
					state.handCount[p] = handCount;
					for (gT = 0; gT <= handCount; gT++) {
						for (sT = 0; sT <= handCount - gT; sT++) {
							cT = populateHand(&state, p, sT, gT);
							updateCoins(p, &state, bonus);

							if (asserttrue(state.coins, cT + sT * 2 + gT * 3 + bonus) == 0) {
								//printf("TEST SUCCESSFUL!\n");
								error += 0;
							}
							else {
								printf("state.coins = %d, expected = %d (copper = %d, silver = %d, gold = %d)\n", state.coins, cT + sT * 2 + gT * 3 + bonus, cT, sT, gT);
								printf("TEST FAILED!\n");
								error += -1;
							}
							//printf("-------------------\n");
						}
					}
				}
			}
		}
	}
	if (error == 0) {
		printf("All tests successful.\n");
	}
	return 0;
}

int populateHand(struct gameState *state, int p, int sT, int gT) {
	int i;
	int cT = 0;
	for (i = 0; i < state->handCount[p]; i++) {
		if (gT > 0) {
			state->hand[p][i] = gold;
			gT--;
		}
		else if (sT > 0) {
			state->hand[p][i] = silver;
			sT--;
		}
		else {
			state->hand[p][i] = copper;		//fill rest of hand with copper Treasure
			cT++;
		}
	}
	return cT;
}