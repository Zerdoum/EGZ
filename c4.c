#include "loi.h"


int loi(int a, int b)
{
	return (a+b) %4;
}

int opp(int a)
{
	if (a==0) return 0;
	return 4-a;
}


int ordre = 4;

int exposant = 4;
