/* -----------------------------------------------------------------------
* Demonstration of how to write unit tests for dominion-base
* Include the following lines in your makefile:
*
* unittest1: unittest1.c dominion.o rngs.o
*      gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
* -----------------------------------------------------------------------
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

/*Tests updateCoin() function*/

int main() {
	int seed = 500;
	int numPlayer = 2;
	int maxBonus = 10;
	int p, r, handCount, cT, sT, gT;
	int bonus;
	int pos;
	int k[10] = { adventurer, smithy, gardens, witch, militia, thief, cellar, village, market, laboratory };
	struct gameState state;
	int maxHandCount = 5;	//possible to have more than 5 cards in hand from drawing during turn

	printf("Testing updateCoins():\n");
	for (p = 0; p < numPlayer; p++) {
		for (handCount = 1; handCount <= maxHandCount; handCount++) {
			for (bonus = 0; bonus <= maxBonus; bonus++) {
				memset(&state, 0, sizeof(struct gameState));	//clear game state
				r = initializeGame(numPlayer, k, seed, &state);	//initialize new game
				state.handCount[p] = handCount;
				for (gT = 0; gT < handCount; gT++) {
					for (sT = 0; sT < handCount - gT; sT++) {
						cT = populateHand(&state, p, sT, gT);
						updateCoins(p, &state, bonus);

						printf("state.coins = %d, expected = %d\n", state.coins, cT + sT * 2 + gT * 3 + bonus);
						assert(state.coins == cT + sT * 2 + gT * 3 + bonus);
					}
				}
			}
		}
	}
}

int populateHand(struct gameState *state, int p, int sT, int gT) {
	int i;
	int cT = 0;
	for (i = 0; i < state->handCount[p]; i++) {
		if (gT > 0) {
			state->hand[i] = gold;
		}
		else if (sT > 0) {
			state->hand[i] = silver;
		}
		else {
			state->hand[i] = copper;		//fill rest of hand with copper Treasure
			cT++;
		}
	}
	return cT;
}