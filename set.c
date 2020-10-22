#include <stdio.h>
#include <stdlib.h>

#include "set.h"

uint32_t __nb_nodes;
node_t __node_list;

// tree data
uint_t		*lo_bound;
uint_t		*up_bound;
char		*node_ht;
uint32_t	*left_child;
uint32_t	*right_child;



#define NEXT(I)	left_child[(I)]


element_t min_element, max_element;

void set_min_max(element_t x, element_t y)
{
	min_element=x;
	max_element=y;
}

void prologue(uint32_t nb_nodes)
{
	uint32_t i;
	
	__nb_nodes=nb_nodes;
	// memory allocation
	lo_bound=malloc((size_t)nb_nodes*(size_t)sizeof(uint_t));
	if (lo_bound==NULL)
		goto fatal;
	up_bound=malloc((size_t)nb_nodes*(size_t)sizeof(uint_t));
	if (up_bound==NULL)
		goto fatal;
	node_ht=malloc((size_t)nb_nodes*(size_t)sizeof(char));
	if (node_ht==NULL)
		goto fatal;
	left_child=malloc((size_t)nb_nodes*(size_t)sizeof(uint32_t));
	if (left_child==NULL)
		goto fatal;
	right_child=malloc((size_t)nb_nodes*(size_t)sizeof(uint32_t));
	if (right_child==NULL)
		goto fatal;
	// memory initialisation : nodes are linked through left_child
	left_child[0]=nb_nodes;
	for (i=0;i<nb_nodes;i++)
	{
		NEXT(i)=i-1;
	}
	__node_list=i-1;
	min_element=0;
	max_element=UINT64_MAX;
	return;
	
fatal:
	fatal_error("allocation mémoire impossible",1);
}


// for debugh purpose
// return nuber of remaining free nodes
uint32_t nb_free_nodes()
{
	uint32_t i,n;
	n=0;
	for (i=__node_list; i<__nb_nodes; i=NEXT(i) )
	{
		n++;
	}
	return n;
}

// allocate a new node
node_t new_node()
{
	uint32_t nn;
	if (__node_list>=__nb_nodes)
	{
		fatal_error("out of memory",2);
	}
	nn=__node_list;
	__node_list=NEXT(nn);
	return nn;
}


// free a given node
void free_node(node_t n)
{
	NEXT(n)=__node_list;
	__node_list=n;
}

// create a new interval
node_t new_interval(uint_t a, uint_t b)
{
	node_t nn;
	nn=new_node();
	lo_bound[nn]=a;
	up_bound[nn]=b;
	//node_ht[nn]=1;
	left_child[nn]=NIL;
	right_child[nn]=NIL;
	return nn;
}


int height(node_t n)
{
	int lh,rh;
	if ( n == NIL )
		return 0;
	
	if ( left_child[n] == NIL )
		lh=0;
	else
		lh=1+node_ht[left_child[n]];
		
	if ( right_child[n] == NIL )
		rh=0;
	else
		rh=1+node_ht[right_child[n]];
	
	if (lh > rh)
		return lh;
	
	return rh;
}

int balance(node_t n)
{
	int lh,rh;
	if ( n == NIL )
		return 0;
 
	if ( left_child[n] == NIL )
		lh=0;
	else
		lh=1+node_ht[left_child[n]];
 
	if ( right_child[n] == NIL )
		rh=0;
	else
		rh=1+node_ht[right_child[n]];
 
	return lh - rh;
}

node_t rotate_right(node_t x)
{
	node_t y;
	y=left_child[x];
	left_child[x]=right_child[y];
	right_child[y]=x;
	node_ht[x]=height(x);
	node_ht[y]=height(y);
	return y;
}
 
node_t rotate_left(node_t x)
{
	node_t y;
	y=right_child[x];
	right_child[x]=left_child[y];
	left_child[y]=x;
	node_ht[x]=height(x);
	node_ht[y]=height(y);
	return  y;
}
 
 
node_t rotate_left_right(node_t T)
{
	left_child[T]=rotate_left(left_child[T]);
	return rotate_right(T);
}
 
node_t rotate_right_left(node_t T)
{
	right_child[T]=rotate_right(right_child[T]);
	return rotate_left(T);
}
 


node_t insert_intvl(uint_t a, uint_t b, node_t n)
{
	if(n==NIL)
	{
		n=new_interval(a,b);
		node_ht[n]=height(n);
		return n;
	}
	if(a > lo_bound[n])		// insert in right subtree
	{
		right_child[n]=insert_intvl(a,b,right_child[n]);
		if ( balance(n) == -2)
		{
			if( a > lo_bound[right_child[n]] )
				n=rotate_left(n);
			else
				n=rotate_right_left(n);
		}
	}
	else if( a < lo_bound[n] )
	{
		left_child[n]=insert_intvl(a,b,left_child[n]);
		if ( balance(n) == 2 )
		{
			if( a < lo_bound[left_child[n]] )
				n=rotate_right(n);
			else
				n=rotate_left_right(n);
		}
	}

	node_ht[n]=height(n);
	
	return n;
}

// remove existing interval [a,b] from node n
node_t remove_intvl(uint_t a,  node_t n)
{
	node_t p;
	
	if(n==NIL)
	{
		return NIL;
	}
	if(a > lo_bound[n])		// insert in right subtree
	{
		right_child[n]=remove_intvl(a,right_child[n]);
		if(balance(n)==2)
		{
			if(balance(right_child[n])>=0)
				n=rotate_right(n);
			else
				n=rotate_left_right(n);
		}
	}
	else if ( a < lo_bound[n] )
	{
		left_child[n]=remove_intvl(a,left_child[n]);
		if(balance(n)==-2)
		{	//Rebalance during windup
			if( balance(right_child[n]) <= 0 )
				n=rotate_left(n);
			else
				n=rotate_right_left(n);
		}
	}
	else
	{
		//data to be deleted is found
		if (right_child[n] != NIL)
		{	//delete its inorder succesor
			p=right_child[n];
			
			while( left_child[p] != NIL)
				p=left_child[p];
			
			lo_bound[n]=lo_bound[p];
			up_bound[n]=up_bound[p];
			right_child[n]=remove_intvl(lo_bound[p], right_child[n]);
			
			if(balance(n)==2)
			{	//Rebalance during windup
				if( balance(left_child[n]) >= 0 )
					n=rotate_right(n);
				else
					n=rotate_left_right(n);
			}
		}
		else
		{
			p=left_child[n];
			free_node(n);
			return p;
		}
	}
	node_ht[n]=height(n);
	return n;
}

uint32_t size_avl(node_t a)
{
	if (a==NIL)
		return 0;
	return 1+size_avl(left_child[a])+size_avl(right_child[a]);
}

void print_avl(node_t a)
{	
	if (a == NIL) 
	{
		printf("* ");
		return;
	}
	printf("( %d [%ld,%ld] ", node_ht[a], lo_bound[a], up_bound[a]);
	print_avl(left_child[a]);
	print_avl(right_child[a]);
	printf(")");
}


//----------------------------------------------------------------------------------

// set operation
// ------------


// make the set empty and free nodes it used
void empty_out(set_t *n)
{
	if (*n==NIL)
		return;
	empty_out(left_child+*n);
	empty_out(right_child+*n);
	free_node(*n);
	*n=NIL;
}


// return is set is [min_element, max_element]
int is_full(set_t n)
{
	if (n==NIL)	// check non empty
		return 0;
	if ( (left_child[n]!=NIL) || (right_child[n]!=NIL) )	// check single node
		return 0;
	return ( lo_bound[n]==min_element ) && ( up_bound[n]==max_element );
}

static void print_set_rec(node_t n)
{
	while(n!=NIL)
	{
		print_set_rec(left_child[n]);
		if (lo_bound[n]==up_bound[n])
			printf("[%lu]",lo_bound[n]);
		else
			printf("[%lu,%lu]",lo_bound[n],up_bound[n]);
		n=right_child[n];	// tail recursion
	}
}

void print_set(set_t n)
{
	printf("(");
	print_set_rec(n);
	printf(")\n");
	fflush(stdout);
}

uint_t cardinal(set_t n)
{
	if (n==NIL)
		return 0;
	return cardinal(left_child[n])
		+ cardinal(right_child[n])
		+ up_bound[n] - lo_bound[n] + 1;
}

// rend NIL ou l'intervalle auquel appartient x
uint32_t belong(element_t x, set_t n)
{
	while (n!=NIL)
	{
		if (x<lo_bound[n])
		{
			n=left_child[n];
			continue;
		}
		if (x>up_bound[n])
		{
			n=right_child[n];
			continue;
		}
		return n;
	}
	return NIL;
}

void include_elt(element_t x, set_t *pp)
{
	node_t n,p;
	node_t na,nb;
	element_t y;

	p=*pp;
	n=belong(x,p);
	if (n!=NIL)	// if x already belong to *pp, do nothing
		return;
	// does it increase existing interval by left
	if ( x < max_element )
		na = belong(x+1,p);
	else
		na=NIL;
	if ( x > min_element )
		nb = belong(x-1,p);
	else
		nb=NIL;
	if ( (na!=NIL) && (nb!=NIL) )
	{	// join two intervals
		y=lo_bound[nb];
		*pp=remove_intvl(y,p);
		na=belong(x+1,*pp);	// structure may have changed
		lo_bound[na]=y;
		return;
	}
	if (na != NIL)
	{	// increase interval from right
		lo_bound[na]--;
		return;
	}
	if (nb != NIL)
	{	// increase interval from right
		up_bound[nb]++;
		return;
	}
	// insert a new singleton
	*pp=insert_intvl(x,x,p);
	return;
}


// search for interval in set included in [a,b]
node_t search_included(element_t a, element_t b, set_t n)
{
	node_t m;
	if (n==NIL)
		return NIL;
	if (a<lo_bound[n])
	{
		m=search_included(a,b,left_child[n]);
		if (m!=NIL)
			return m;
	}
	if ( (a<=lo_bound[n]) && (b>=up_bound[n]) )
			return n;
	if ( a > lo_bound[n] )
		return search_included(a,b,right_child[n]);
	return NIL;
}
//
// remove all intervals of pp included in [a,b]
void suppr_included(element_t a, element_t b, set_t*pp)
{
	node_t n;
	do
	{
		n=search_included(a,b,*pp);
		if (n!=NIL)
			*pp=remove_intvl(lo_bound[n],*pp);
	}
	while (n!=NIL);
}

void include_intvl(element_t a, element_t b, set_t*pp)
{
	element_t	x;
	node_t		n;

	if ( b < a )	// 
		return;
	if ( a == b )
	{
		include_elt(a,pp);
		return;
	}
	x=min_element;
	if (a > min_element)
		x=a-1;
	// search if a-1 belong to an interval of pp
	n=belong(x,*pp);
	if (n!=NIL)	// if so, replace a by lower bound of the interval
		a=lo_bound[n];
	x=max_element;
	if (b < max_element)
		x=b+1;
	n=belong(x,*pp);
	if (n!=NIL)
		b=up_bound[n];
	suppr_included(a,b,pp);
	*pp=insert_intvl(a,b,*pp);
}

// include set q in set pp
// and free q
void include_set(set_t q, set_t*pp)
{
//	element_t a,b;
	if (q==NIL)	// noting to include
		return;
	include_set(left_child[q],pp);
	include_set(right_child[q],pp);
//	a=lo_bound[q];
//	b=up_bound[q];
	free_node(q);
	include_intvl(lo_bound[q],up_bound[q],pp);
}

//----------------------------------------------------------------------------------

void exclude_elt(element_t x, set_t*pp)
{
	node_t		n;
	element_t	y;

	n=belong(x,*pp);
	if (n==NIL)	// if x in not in pp, do noting
		return;
	if ( (lo_bound[n]==x)&&(up_bound[n]==x) )
	{	// if interval is [x,x], just remove the interval
		*pp=remove_intvl(x,*pp);
		return;
	}
	if  (lo_bound[n]==x)
	{	// if x is one of the bound, just change the bound
		lo_bound[n]=x+1;
		return;
	}
	if (up_bound[n]==x)
	{
		up_bound[n]=x-1;
		return;
	}
	// if x is strictly in interval, cut the interval in two
	y=lo_bound[n];
	lo_bound[n]=x+1;
	*pp=insert_intvl(y,x-1,*pp);
}


void exclude_intvl(element_t a, element_t b, node_t* pp)
{
	node_t na,nb;
	element_t x,y;
	if (a==b)
	{
		exclude_elt(a,pp);
		return;
	}
	na=belong(a,*pp);
	nb=belong(b,*pp);
	if (na!=NIL)
	{
		x=lo_bound[na];
		if (na==nb)
		{	// if a and b are in the same intervall, split 
			// [x a-1] U [b+1,y]
			y=up_bound[nb];
			if (x<a)
			{
				up_bound[na]=a-1;
				if (y>b)
					include_intvl(b+1,y,pp);
				return;
			}
			if (y>b)	// [b+1, y]
				lo_bound[na]=b+1;
			else	// remove all interval
				*pp=remove_intvl(a,*pp);
			return;
		}
		// if a and b are in distinct intervals
		if (x<a)	// [x, a-1]
			up_bound[na]=a-1;
		// else, ie x==a, it will be removed by suppr_included
	}
	if (nb!=NIL)
	{
		if (up_bound[nb]>b)	// [b+1,y]
			lo_bound[nb]=b+1;
	}
	suppr_included(a,b,pp);
}


void exclude_set(set_t q, set_t*pp)
{
	if (q==NIL)
		return;
	exclude_set(left_child[q],pp);
	exclude_set(right_child[q],pp);
	free_node(q);
	exclude_intvl(lo_bound[q],up_bound[q],pp);
}

//----------------------------------------------------------------------------------

// function for sweeping intervals of a set

// stack size
#define S_SIZE	256

static struct { enum {
	__cont_left,
	__cont_node,
	__cont_right
	} cont;
	node_t node;
} __stack[S_SIZE];

// stack pointer
static int __sp;

node_t next_node();

static void __push(node_t n)
{
	__sp++;
	__stack[__sp].cont=__cont_left;
	__stack[__sp].node=n;
}

node_t start_node(node_t n)
{
	__sp=-1;
	if (n!=NIL)
		__push(n);
	return next_node();
}

node_t next_node()
{
	node_t n;
	while(__sp>=0)
	{
		n=__stack[__sp].node;
		if (n==NIL)
		{
			__sp--;
			continue;
		}
		switch(__stack[__sp].cont)
		{
			case __cont_left:
				__stack[__sp].cont=__cont_node;
				__push(left_child[n]);
				break;
			case __cont_node:
				__stack[__sp].cont=__cont_right;
				return n;
			case __cont_right:	// tail recursion, no push
				__stack[__sp].cont=__cont_left;
				__stack[__sp].node=right_child[n];
				free_node(n);
				break;
			default:	// should not occur
				return NIL;
		}
	}
	return NIL;
}

//----------------------------------------------------------------------------------
jmp_buf env;

void fatal_error(char*msg, int no)
{
	printf("** Fatal Error \"%s\" !\n",msg);
	longjmp(env,no);
}

void bye()
{
	longjmp(env,1);
}



