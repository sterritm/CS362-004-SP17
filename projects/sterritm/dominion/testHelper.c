/*
Name: Michael Sterritt
Date: 4/29/17
Description: Implements custom assert statement that does not cause the program exit upon encountering an error.
*/

#include "testHelper.h"

int asserttrue(int x, int y) {
	if (x == y)
		return 0;
	else
		return -1;
}