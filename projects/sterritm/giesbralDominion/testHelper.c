#include "testHelper.h"

int asserttrue(int x, int y) {
	if (x == y)
		return 0;
	else
		return -1;
}