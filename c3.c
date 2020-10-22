#include "loi.h"


int loi(int a, int b)
{
	return (a+b) %3;
}

int opp(int a)
{
	if (a==0) return 0;
	return 3-a;
}

int group_sub(int a, int b)
{
	return loi(a, opp(b));
}
int ordre = 3;

int exposant = 3;
