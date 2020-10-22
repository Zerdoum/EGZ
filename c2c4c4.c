#include "loi.h"


int loi(int a, int b)
{
	int a0,a1,a2,b0,b1,b2;
    a0=a%2;
    a1=(a/2)%4;
    a2=a/8;

    b0=b%2;
    b1=(b/2)%4;
    b2=b/4;
    // calculer les trois composantes de la somme 
    a0=(a0+b0)%2;
    a1=(a1+b1)%4;
    a2=(a2+b2)%4;

    return a0+2*a1+8*a2;
}

int group_sub(int a, int b)
{
	int a0,a1,a2,b0,b1,b2;
    a0=a%2;
    a1=(a/2)%4;
    a2=a/8;

    b0=b%2;
    b1=(b/2)%4;
    b2=b/8;
    // calculer les trois composantes de la somme 
    a0=(a0+2-b0)%2;
    a1=(a1+4-b1)%4;
    a2=(a2+4-b2)%4;

    return a0+2*a1+8*a2;
}

int ordre = 32;

int exposant = 4; 


