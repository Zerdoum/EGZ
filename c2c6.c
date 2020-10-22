#include "loi.h"


int loi(int a, int b)
{
	int a1, b1, a2, b2;
	a1= a/2;
	a2= a%2;
	b1= b/2;
	b2= b%2;
	a1 = (a1 + b1)%2;
	a2 = (a2 ^ b2);
	return (a1*2) + a2;
}

int ordre = 4;

int exposant = 2;
