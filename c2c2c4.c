#include "loi.h"


int loi(int a, int b)
{
	int a1, b1, a2, b2;
	a1= a/4;
	a2= a%4;
	b1= b/4;
	b2= b%4;
	a1 = (a1 ^ b1);
	a2 = (a2 + b2)%4;
	return (a1*4) + a2;
}

int ordre = 16;

int exposant = 4;
