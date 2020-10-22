#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <setjmp.h>
#include <inttypes.h>

#include "set.h"
#include "loi.h"


typedef uint64_t u64;
typedef __uint128_t duint_t;


// tab_num: transforme un bitmap ens en table croissante des bits mis à 1 entre 0 et max-1
// ens	notre bitmap
// max	taille de notre bitmap 
// tab	le tableau contenant les indices 
void tab_num(u64 ens, int *tab, int max)
{  
	int i,j;
	i=0;
	j=0;
	u64 m_max; // m_max est le masque
	u64 m; 
	m_max=1l<<max;
	for(m=1;m!=m_max;m<<=1)
	{
		if((m&ens)!=0)
		{
			tab[i++]=j;
		}
		j++;
	}
}

// rend le numéro d'un sous-ensemble d'un ensemble de n éléments
// le sous-ensemble est donné comme un bitmap des éléments

u64 numsubset(u64 w,int n)
{
   	int i,j; // index du parcours du triangle
   	u64 a;   // valeurs du triangle de Pascal modifié
   	u64 p;   // valeurs du triangle de Pascal
   	u64 x;  // cumul de tous les sommets parcourus
   	// initialisation
   	i=-1;
   	j=-1;
   	p=0;
   	a=0;
   	x=0;
   	while (n>0)
   	{
      	// selon la valeur du chiffre
     		switch (w&1)
      		{
        	case 0:
          		++i;           // en bas seulement
          		a=2*a-p;       // mise à jour coef de Pascal modifié 
          		p=(p*i)/(i-j); // mise à jour du coef de Pascal
          		break;
        	case 1:
          		if (j==-1) p=1; else p=(p*(i-j))/(j+1);  // p(i-1,j) en fonction de p(i-1,j-1)
          		a=2*a+p;        // mise à jour de a
          		++i; ++j; // en bas à droite 
          		if (i==j) p=1; else p=(p*i)/(i-j);  // mise à jour p(i,j) en fonction de p(i-1,j)
          		break;
      		}
      		// décaler w pour positionner le chiffre suivant
      		w>>=1;
      		// cumul de la valeur du sommet
      		x+=a;
      		--n; // compteur de boucle
   	}
   	return x;
}


// rend le sous-ensemble d'un numéro donné
u64 subsetnum(u64 x,int i) // le paramètre i sert de ligne courante
{
  	int	j; // colonne courante
  	u64	a; // Coefficients du triangle de Pascal modifié a(n,j)
  	u64	p; // Coefficients du triangle de Pascal
  	u64	w; // mot resultat

  	// initialisation
  	w=0;
  	p=1;
  	a=0;
  	j=0;
  	// recherche de la colonne j telle que a(i,j) <= "x" < a(i,j+1)
  	// et initialisation de a(i,j)
  	while (a+p<=x)
  	{
    		a+=p;
    		j++;
    		p=(p*(i-j+1))/j;
  	}
  	p=(p*j)/i;
  	a=(a+p)/2;
  	--j;
  	x-=a;
  	--i;
  	// boucle nominale
  	while ( (j>=0) && (j<i) )
  	{ // 'algorithme s'arrête dès qu'on est arrivé sur 'un des côté
    	// soit le côté j=0, alors on compléte avec des zéros
    	// soit le côté i=j, alors on complète avec des uns
    		if (x>=a) // peut-on soustraire a(i-1,j)
    		{
      			p=(p*(i-j))/i;
      			a=(a+p)/2; // mise à jour de a
      			--i; // en haut seulement
    		}
    		else
    		{
      			w|=(1ll<<i); // se souvenir que le chiffre est 1
      			a=(a-(p*(i-j))/i)/2; // mise à jour de a
      			p=(p*j)/i;  // mise à jour de p
      			--i; // en haut 
     	 		--j; // a gauche 
    		}
    		x-=a; // on soustrait ce qu'on a pu
  	}
  	// remplissage des derniers caracteres avec des zéro ou des 1
  	if (j!=-1) // selon de cote du triangle ou on arrive
  	{
    		w|=(1ll<<(i+1))-1ll;
  	}
  	return w;
}

// Fonction de soustraction.
// une table croissante en liste d'éléments
// Cette fonction sert de transformer le sous-ensemble en une suite. 
// tab: contient le sous-ensemble. 
// n: la taille du tableau tab.
void sous_tab_num(int *tab,int n)
{
	int i;
	for(i=0;i<n;i++)
	{
		tab[i]=tab[i]-i;
	}
}
//
// insère un 1 dans x à la position donnée par m = 0..010..0 où m est le masque 
u64 insere_un_bis(u64 x, u64 m)
{
    u64 mm;
    mm=m-1;
    return (x&mm) + ((x&~mm)<<1) + m;
}

// returns binomoial coefficient {n\choose p}
// not robust : suppose that n and p are nonnegative and that p <= n
uint_t binome(int n, int p)
{
	uint_t r=1;
	int i;
	int q;
	q=n-p;
	if (q<p)
	{	// {n choose p} = {n choose (n-p)}, compute the faster
		p=q;
	}
	for (i=1;i<=p;i++,n--)
	{
		r=((duint_t)r*(duint_t)n)/(duint_t)i;
	}
	return r;
}


//int_t MINI;
// Schemaenz est la dernière version qui marche 
// m=n+k tel que n est la taille de l'alphabet. et k est le poids.  
// nbr: Le numéro dont on cherche ses successeurs. 
// Le parcours du nbr commence par l'indice 0 
// ptails est la table des queues
// renvoie la dimension du carré dans lequel se trouve nbr
int schemaenz(uint_t*tab_sc,uint_t*ptails,uint_t nbr, int m, int k)
{	
    	//***************** Les variables ***************** 
	int n; // taille de l'alphabet. 
	int j; // sert à parcourir les sucesseurs qui sont à l'intérieur du schéma en Z. 
	// Les variables pour parcourir notre arbre. 
	uint_t new_min;	// La borne inférieure de l'intervale. 
	uint_t b;		// binome(m,k)
	uint_t new_max;	// La borne supérieure de l'intervalle. 
	int i;	// parcours la table des successeurs. (le i va de 0 à n-1).
	uint_t c;	// représente les coins, c'-à-d le premier successeur de chaque grand carré, représente aussi coefficients_{i}
	int dim;	// dimension du carré dans lequel se trouve nbr
	uint_t tail;

	i=0;
	new_min=0;
	n=m-k+1;	// taille de l'alphabet
	c=0; //min_successeurs_bis(n,k-1);
	//nbr -=	MINI;//numsubset(((1<<(k-1))-1)<<n,m-1);	
    	dim=n;
	tab_sc[0]=c+nbr; // initialiser tab_sc[0] dans notre cas tab_sc[0]=113 avec c=93 et p=0

	
	//***************** Le parcours dans l'arbre ***************** l
	b= binome(m,k);
	//printf("m vaut %d\n",m);
	//printf("k vaut %d\n",k);
	//printf("b vaut %d\n",b);
	tail=b;
	new_max=b;		// binome(m-1,k-1)
	while(nbr>new_min)	// Le test d'arrêt: tant qu'on n'a pas atteind le nbr.  
	{
		if( nbr < new_max )
		{
			// si le nombre est dans l'intervalle [min, max[, descendre dans l'arbre
			b=((duint_t)b*(duint_t)k)/(duint_t)m;	// (m-1 choose k-1 ) = (m choose k) * k / m
		//	printf("b desc vaut %d\n",b);

			m--;
			k--;		// b == binome(m,k)
			tail=new_max;
			new_max=new_min+b;
			continue;
		}
		
		while( nbr >= new_max ) 
		{	// si nbr est hors de l'intervalle, parcourir les fils vers la gauche
			c += (duint_t)b*(duint_t)(m+1)/(duint_t)(k+1); //binome(m+1,k+1);
			b=((duint_t)b*(duint_t)(m-k))/(duint_t)m;	// (m-1 choose k) = (m choose k) * (m-k) / m
			m--;
			new_min=new_max;
			new_max=new_min+b;
			i++;
			dim--;
			tab_sc[i]=c + nbr - new_min;
			if (ptails!=NULL)
			{
			       ptails[i]=tail;
			}
		}
	}
	for(j=i+1;j<n;j++)
	{	// les autres successeurs sont obtenus en ajoutant 1
		tab_sc[j] = tab_sc[j-1] + 1;
	}
	return dim;
}


#define MAX_N 60
// rend l'ensemble des successeurs de x
node_t succ_elt(uint_t x,int n, int k)
{
	node_t p;
	int i;
	uint_t t[MAX_N];

	p=NIL;
	schemaenz(t,NULL,x,n+k,k+1);
	for (i=0;i<n;i++)
	{
		include_elt(t[i],&p);
	}
	return p;

}


// rend l'ensemble des successeurs de x avec l'algorithme naif
node_t succ_elt_1(uint_t x, int n, int k)
{
	node_t p;
	int i;
	u64 m;
	u64 ens;
	i=0;
	m=1;
	p=NIL;
	ens=subsetnum(x,n+k-1); // L'ensemble contient le bitmap associé au numéro x
	while (i<n)
	{
		if ((ens&m)==0)
		{
			include_elt(numsubset(insere_un_bis(ens,m),n+k),&p);
			i++;
		}
		m<<=1;
	}
	return p;
}

node_t succ_intvl(uint_t a, uint_t b, int n, int k)
{
	int m;
	uint_t ta[MAX_N];
	uint_t tb[MAX_N];
	int i;
	int dim_a;
	uint_t tailsa[MAX_N];

	node_t p;

	m=n+k-1;
	if (a==b)
	{
		return succ_elt(a,n,k);
	}

	p=NIL;
	// successeurs de a
	dim_a=schemaenz(ta,tailsa,a,m+1,k+1);
	// successeurs de b
	schemaenz(tb,NULL,b,m+1,k+1);
	
	// inclure le schéma en z
	include_intvl(ta[n-dim_a],tb[n-1],&p);
	// inclure les autres colonnes
	for (i=0;i<n-dim_a;i++)
	{
		if ( tb[i]-ta[i] == b-a )
			include_intvl(ta[i],tb[i],&p);
		else
			include_intvl(ta[i],ta[i]+tailsa[i]-a-1,&p);
	}
	return p;
}




// successeur d'un interval avec l'algorithme naif
node_t succ_intvl_1(uint_t a, uint_t b, int n, int k)
{
	node_t p, q;
	uint_t i;
	p=NIL;
	for (i=a; i<=b;i++)
	{	// parcours tout l'intervalle et ajoute tous les successeurs des éléments
		q=succ_elt_1(i,n,k);
		include_set(q,&p);
		empty_out(&q);
	}

	return p;
}

//int affiche_succ_ele(int nbr, int)+

#define K_MAX	100

node_t init(int n, int k)
{
	node_t q;
	uint_t min, max;
	int i;
	int j;
	int m;
	int tabens[K_MAX];
	u64 x;
	int res;
	m=n+k-1;
	min = numsubset(((1l<<k)-1)<<(n-1), m);
	max = numsubset((1l<<k)-1,m);
	q=NIL;
	for (i=min; i<=max ; i++)
	{
		// calcul du bitmapl
		x=subsetnum(i,m);
		tab_num(x,tabens,m);	// transforme le bitmap en table d'indices strict croi
		sous_tab_num(tabens,k);	// change en indice de suite 0..n-1
		res=0;
		for (j=0;j<k;j++)
		{
			res=loi(res,tabens[j]);

		}
		if (res==0)
		{
			include_elt(i-min,&q);
		}
	}
	//printf("nb de suites de somme nulle = %ld\n", cardinal(q));l
	return q;
}

void successeurs(node_t*pp,int n, int k)
{
	node_t e;
	set_t p1,p2;
	int m;

	m=n+k-1;
	set_min_max(0,binome(m+1,k+1)-1); //max1-min1);
	e=start_node(*pp);
	p2=NIL;
	while (e!=NIL)
	{
		p1=succ_intvl(lo_bound[e],up_bound[e],n,k);
		include_set(p1,&p2);
		e=next_node();
	}
	*pp=p2;
}

node_t init_to(int kk)
{
	node_t q;
	node_t p;
	uint_t min, max;
	int i;
	int j;
	int m;
	int tabens[K_MAX];
	u64 x;
	int res;
	int n;
	int k;
	n=ordre;
	k=exposant;
	m=n+k-1;
	min = numsubset(((1l<<k)-1)<<(n-1), m);
	max = numsubset((1l<<k)-1,m);
	p=NIL;
	for (i=min; i<=max ; i++)
	{
		// calcul du bitmapl
		q=NIL;
		set_min_max(0,binome(m+1,k+1)-1); //max1-min1);

		x=subsetnum(i,m);
		tab_num(x,tabens,m);	// transforme le bitmap en table d'indices strict croi
		sous_tab_num(tabens,k);	// change en indice de suite 0..n-1
		res=0;
		for (j=0;j<k;j++)
		{
			res=loi(res,tabens[j]);
		}
		if (res==0)
		{
			include_elt(i-min,&q);
		}
		//print_set(q);
		for (j=k;j<kk;j++)
		{
			successeurs(&q,n,j);
		}
		include_set(q,&p);
		printf("**[%d : %d]**",i,size_avl(p));

	}
	//printf("nb de suites de somme nulle = %ld\n", cardinal(q));l
	return p;
}

//n: L'alphabet
//x: Le numéro dont nous souhaiterons trouver ses succésseurs. 
//k: Le poids de Hamming de x
//t: La table des succésseurs 
int successeurs_bis(u64*t, u64 x, int n,int k)
{
	int i;
	u64 m;
	u64 ens;
	i=0;
	m=1;
    ens=subsetnum(x,n+k-1); // L'ensemble contient le bitmap associé au numéro x
	while (i<n)
	{
		if ((ens&m)==0)
		{
			t[i++]=numsubset(insere_un_bis(ens,m),n+k);
//insere_un_bis(x,m);   
		}
		m<<=1;
	}
   
   /* for(i=0;i<n;i++){
        printf("les succésseurs sont: tab[%d]=%llx\n",i,t[i]);
    }*/
	return i;
}





extern jmp_buf env;

int main()
{

	printf("Hello egz!\n");
	if ((setjmp(env))!=0)
	{
		return 0;
	}

	prologue(4000000000);

	int n;
	int k;
	

	n=ordre;
	printf("ordre vaut %d\n",n);
	k=exposant;
	printf("exposant vaut %d\n",k);

	/*node_t p;
	p=succ_elt(4,5,3);
	print_set(p);
	return 0;*/

	int ff;
	node_t p;
//	p=init_to(10);

//	bye();

//	empty_out(&p);

	clock_t h;
	double dh;
	h=clock();


	p=init(n,k);
	printf("-%d-",k);
	printf("n=%d - taille = %d\n",n,size_avl(p));
	clock_t h0=clock()-h;
	dh=((double)h0)/CLOCKS_PER_SEC;
	printf("[%lf sec.]\n",dh);
	
//	print_set(p);

	printf("\n");
	do
	{
		printf("-%d-",k+1);
		fflush(stdout);
		successeurs(&p,n,k);
	//	print_set(p);
		ff=is_full(p);
		k++;
		clock_t h1=clock()-h;
		dh=((double)h1)/CLOCKS_PER_SEC;
		printf("[%lf sec.]",dh);
		printf("**[%d]**",size_avl(p));
	}
	while (ff==0);
	printf("egz=%d\n",k);
	clock_t h2=clock()-h;
	dh=((double)h2)/CLOCKS_PER_SEC;
	printf("[%lf sec.]\n",dh);
	fflush(stdout);
	
	bye();
/*
u64 tab_succ[1000000]; // Comme ca quand on appelle la fonction succ_sing dans le main avec succ_sing(tab,8,4) tab sera déjà déclarée en variable globale.
    //int t[1000000]; 
	int t_tab_succ; // le nombre d'éléments dans la table. 
    //int premier;
   // int dernier;
    int n;
    int k;
    int m;
    int i;
    int j;
    n=4;
    k=3;
    m=n+k-1;
    
    
    int min;
    int max; 
    min=numsubset(((1<<k)-1)<<(n-1),m);	// 1...1 0...0  (le plus petit numéro)
	max=numsubset((1<<k)-1,m);		// 0...0 1...1 (le plus grand numéro)
    printf("min vaut %d\n",min);
    printf("max vaut %d\n",max);
    
	for(i=min;i<=max;i++){
         t_tab_succ=successeurs_bis(tab_succ,i,n,k);
        // printf("la valeur de la dernière case du tableau vaut %d\n",tab_succ[0]);
            printf("[%d] : ",i-22);
            for(j=0;j<t_tab_succ;j++){
                printf("%d ", tab_succ[j]-64);
                
            }
            printf("\n");

	}

   printf("*****************\n");
    for(i=0;i<max-min+1;i++){
     schemaenz(i,n+k,k+1);
       fprintf(fichier,"[%d] : ",i);
     for(j=0;j<n;j++){
        t[j]=tab_sc[j];
        fprintf(fichier,"%d  ", t[j]);
    }
     fprintf(fichier,"\n");

    }*/

 	return 0; 
 
}
