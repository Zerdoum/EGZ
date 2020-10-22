#include "loi.h"


int loi(int a, int b)
{
	int a0,a1,a2,b0,b1,b2;
    a0=a%2;
    a1=(a/2)%2;
    a2=a/4;

    b0=b%2;
    b1=(b/2)%2;
    b2=b/4;
    // calculer les trois composantes de la somme 
    a0=(a0+b0)%2;
    a1=(a1+b1)%2;
    a2=(a2+b2)%6;

    return a0+2*a1+4*a2;
}

int group_sub(int a, int b)
{
	int a0,a1,a2,b0,b1,b2;
    a0=a%2;
    a1=(a/2)%2;
    a2=a/4;

    b0=b%2;
    b1=(b/2)%2;
    b2=b/4;
    // calculer les trois composantes de la somme 
    a0=(a0+2-b0)%2;
    a1=(a1+2-b1)%2;
    a2=(a2+6-b2)%6;

    return a0+2*a1+4*a2;
}

int ordre = 24;

int exposant = 6; 


