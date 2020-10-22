#ifndef SET

#define SET

#include <inttypes.h>
#include <setjmp.h>

typedef uint64_t uint_t;
typedef uint32_t node_t;
typedef node_t set_t;
typedef uint_t element_t;

extern uint32_t __nb_nodes;
extern 
#define NIL	__nb_nodes

void fatal_error(char*, int);
void bye();

void prologue(uint32_t nb_nodes);

void set_min_max(element_t, element_t);

void empty_out(set_t *n);
int is_full(set_t n);
void print_set(set_t n);
void include_elt(element_t x, set_t *pp);
void include_intvl(element_t a, element_t b, set_t*pp);
void include_set(set_t q, set_t*pp);
void exclude_elt(element_t x, set_t*pp);
void exclude_intvl(element_t a, element_t b, node_t* pp);
void exclude_set(set_t q, set_t*pp);

extern jmp_buf env;

node_t start_node(node_t);
node_t next_node();

uint32_t nb_free_nodes();
// tree data
extern uint_t		*lo_bound;
extern uint_t		*up_bound;
//char		*node_ht;
//uint32_t	*left_child;
//uint32_t	*right_child;
uint32_t size_avl(node_t a);


#endif
