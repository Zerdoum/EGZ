#include "loi.h"


int loi(int a, int b)
{
    int a1, b1, a0, b0;
    
    a0= a%3;
    a1= a/3;

    b0=b%3;
    b1= b/3;

    a0 = (a0 + b0)%3;
    a1 = (a1 + b1)%3;
    return a0+(a1*3);
}


int group_sub(int a,int b)
{
    int a1, b1, a0, b0;
    
    a0= a%3;
    a1= a/3;

    b0=b%3;
    b1= b/3;

    a0 = (a0+3-b0)%3;
    a1 = (a1+3-b1)%3;
    return a0+(a1*3);
}

int ordre = 9;

int exposant = 3;

