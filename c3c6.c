#include "loi.h"


int loi(int a, int b)
{
	int a1, b1, a2, b2;
	a1= a/3;
	a2= a%3;
	b1= b/3;
	b2= b%3;
	a1 = (a1 + b1)%6;
	a2 = (a2 + b2)%3;
	return (a1*3) + a2;
}

int ordre = 18;

int exposant = 6;
