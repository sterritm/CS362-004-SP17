/*
Name: Michael Sterritt
Date: 4/29/17
Description: This file is a unit test for dominion.c's getCost()
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "testHelper.h"

/*Tests getCost() function*/

int checkGetCost(char* name, enum CARD card, int expected);

/*Price of cards can be found at http://wiki.dominionstrategy.com/index.php/Main_Page */
int main() {
	printf("----------------------------Testing getCost():----------------------------\n");
	/*Test all known cards in dominion.c*/
	checkGetCost("curse", curse, 0);
	checkGetCost("estate", estate, 2);
	checkGetCost("duchy", duchy, 5);
	checkGetCost("province", province, 8);
	checkGetCost("copper", copper, 0);
	checkGetCost("silver", silver, 3);
	checkGetCost("gold", gold, 6);
	checkGetCost("adventurer", adventurer, 6);
	checkGetCost("council_room", council_room, 5);
	checkGetCost("feast", feast, 4);
	checkGetCost("gardens", gardens, 4);
	checkGetCost("mine", mine, 5);
	checkGetCost("remodel", remodel, 4);
	checkGetCost("smithy", smithy, 4);
	checkGetCost("village", village, 3);
	checkGetCost("baron", baron, 4);
	checkGetCost("great_hall", great_hall, 3);
	checkGetCost("minion", minion, 5);
	checkGetCost("steward", steward, 3);
	checkGetCost("tribute", tribute, 5);
	checkGetCost("ambassador", ambassador, 3);
	checkGetCost("cutpurse", cutpurse, 4);
	checkGetCost("embargo", embargo, 2);
	checkGetCost("outpost", outpost, 5);
	checkGetCost("salvager", salvager, 4);
	checkGetCost("sea_hag", sea_hag, 4);
	checkGetCost("treasure_map", treasure_map, 4);

	/*Test cards that do not exist. Note, enum must be random int not in enum CARD.*/
	checkGetCost("billy_bob", 999, -2);
	checkGetCost("foo", 1000, 3);

	return 0;
}

/*checkGetCost determines whether the card cost is correct or wrong.*/
int checkGetCost(char *name, enum CARD card, int expected) {
	int error = 0;
	if (getCost(card) != -1) {
		error += asserttrue(getCost(card), expected);
	}
	else {
		printf("%s is not a card.\n", name);
		error += asserttrue(getCost(card), -1);
	}
	if (error != 0) {
		printf("%s = %d, expected = %d\n", name, getCost(card), expected);
		printf("TEST FAILED!\n");
	}
	else
		printf("TEST SUCCESSFUL!\n");
	printf("------------------------\n");
	return 0;
}