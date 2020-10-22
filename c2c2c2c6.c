#include "loi.h"


int loi(int a, int b)
{
	int a1, b1, a2, b2;
	a1= a/8;
	a2= a%8;
	b1= b/8;
	b2= b%8;
	a2 = (a2 ^ b2);
	a1 = (a1 + b1)%6;
	return (a1*8) + a2;
}

int ordre = 48;

int exposant = 6; 


