#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "./lib/fields.h"
#include "./lib/node.h"
#include "./lib/jval.h"


/*This struct will be used to hold global information*/
typedef struct information
{
	int c_j;			//cap on jump number
	double c_pp;		//the current amount of pp healed per current jump	
	int amnt_heal;		//the current amount of healing done in this dfs 
	int best_heal;		//the best amount of healing done on a DFS
	double c_p_r;		//current power reduction
	int * healing;		//points to the healing values 
	player **best_path;	//an array of pointers to players in the order of the best path
	int best_path_length;
}info;


/* işlemdeki oyuncunun tüm çocukları arasında DFS araması yapılacak */
void DFS( player * pp, int hop, info * Ip, int ttl_heal, player * from)
{
	int i, j, rslt, dfs_heal, old_pp, cnt;
	
	player * ptr;

	double prv_pp, num;

	cnt = 0;

	/*if the node has already been visited in this dfs just return*/
	if( (*pp).visited != -1) return;
	
	/*Check to make sure that the hop limit is not exceeded if so just return*/
	if( hop > Ip->c_j)
	{
		return;
	}

	(*pp).visited = 1;

	(*pp).prev = from;					/*set the previous field to the node that came before*/
	

	/*Save the state of the player and heal amounts before the DFS*/
	old_pp = (*pp).cur_PP;			

	prv_pp = Ip->c_pp;					

	dfs_heal = Ip->amnt_heal;			
	
	
	
	/*get how much healing would occur if the max is not exceeded*/
	rslt = Ip->c_pp + pp->cur_PP;	


	/*the below will make sure the players max pp is not exceeded, and calculate
	  the appropriate amount of healing done one the current node and thus far.
	  It also sets the currents nodes new pp*/
	if( rslt > (*pp).max_PP)
	{
		Ip->amnt_heal += (*pp).max_PP - (*pp).cur_PP;	
		pp->healing  = (*pp).max_PP - (*pp).cur_PP;
		ttl_heal += (*pp).max_PP - (*pp).cur_PP;	
		(*pp).cur_PP = (*pp).max_PP;			

	}
	else
	{
		Ip->amnt_heal += rint( Ip->c_pp );					
		ttl_heal += rint( Ip->c_pp );					
		(*pp).cur_PP = rslt;						

		pp->healing  = rint(Ip->c_pp);
		
	}

	
	/*now check this new total heal amount against your best heal and if it beats it replace the old with the new
	  and store the path that got you here*/
	if( ttl_heal > Ip->best_heal)
	{

		Ip->best_heal = ttl_heal;


		Ip->best_path_length = hop;


		ptr = pp;

		for( i = Ip->best_path_length - 1 ; i >= 0 ; i--)
		{
			(Ip->best_path[i]) = ptr;

			(Ip->healing[i]) = ptr->healing;

			ptr = ptr->prev;
		}

	}
	
	//adjust the next jumps pp healed using the power reduction
	Ip->c_pp =  Ip->c_pp * (1 - (Ip->c_p_r)) ;


	for(i = 0; i < (*pp).adj_size ; i++)
	{
		ptr = (*pp).adj_list[i];		

		DFS( ptr, hop+1, Ip, ttl_heal ,pp);

	}



	//set everything back to the way it was

	(*pp).visited = -1;

	Ip->c_pp = prv_pp;

	Ip->amnt_heal = dfs_heal;

	(*pp).cur_PP = old_pp;

	return;
}



int main( int argc, char** argv)
{
	/* argüman sayısı kontrol ediliyor */
	if (argc != 6) { fprintf(stderr, "usage: printwords filename\n"); exit(1); }

	char hldr[100];

	double dist;

	int player_cnt, x, y, c_pp, mx_pp, i, j;

    player * pp;				

	player * prv = NULL;				/* geçici olarak kullanılacak*/

	player * UP = NULL;					/* Lokman Hekimi gösteren player işaretçisi */

	player_cnt = 0;
	
	// process command line arguments and store in global information struct
	
	info *Ip = malloc(sizeof(info));	/* bilgiler için yer ayırılıyor */

	Ip->c_j = atoi(argv[3]);	/* num_jumps atanıyor */
	Ip->c_pp = atoi(argv[4]);	/* initial_power atanıyor */
	sscanf(argv[5],"%lf",&Ip->c_p_r);	/* power_reduction atanıyor */
	Ip->amnt_heal = 0;
	Ip->best_heal = 0;


	Ip->best_path = (player **) malloc( sizeof(player *) * atoi(argv[3]) );

	Ip->healing = (int *) malloc( sizeof( int * ) * atoi(argv[3]) );
	

	/* TEXT DOSYASINDAKİ SATIRLAR DÖNGÜSEL OLARAK OKUNUYOR */

	while(scanf("%d %d %d %d %s", &x, &y, &c_pp, &mx_pp, hldr) > 0 )
	{
		pp = (player *)malloc(sizeof(player));	// oyuncu için yer ayırılıyor

		pp->x = x;	// x değeri atanıyor
		pp->y = y;	// y değeri atanıyor
		pp->cur_PP = c_pp;	// mevcut gücü atanıyor
		pp->max_PP = mx_pp;	// maksimum gücü atanıyor
		pp->visited = -1;	// ziyaret edildi mi(-1 ise edilmedi)
		pp->healing = 0;	// iyileştirme başlangıçta 0 atanıyor
		pp->adj_size = 0;	// bitişik boyutu 0 atanıyor
		strcpy(pp->name, hldr);	// adı kopyalanıyor
		pp->prev = prv;	// önceki düğüm (ilk düğümde NULL)

		/* lokman hekim okunuyorsa kayıt ediliyor */
		if( strcmp(hldr, "Lokman_Hekim") == 0) UP = pp; 
		
		prv = pp;	// önceki oyuncu olarak atanıyor

		player_cnt++;	// oyuncu sayısı artılıyor
	}


	player *nodes[player_cnt];				/*tüm oyuncu düğümleri tutulacak*/
	player *edges[player_cnt];				/*will be used as a temporary player pointer contianer*/

	// tüm düğümler taranıyor ve adresleri nodes içerisine atanıyor
	for( i=0 ; i < player_cnt ; i++)
	{
		nodes[i] = prv;
		prv = prv->prev;
	}

	// tüm düğümler taranıyor ve bitişik olanları atanıyor
	for( i = 0; i < player_cnt; i++)
	{
		// check the needed size of adjacent array for current node
		// by going through and counting how many are possible based on the distance from current node
		
		for(j = 0; j < player_cnt; j++)
		{
			// düğümün kendisini de saymaması gerekiyor
			if(j != i)
			{
				// uzaklık hesaplanıyor
				// pow(üzerini almada kullanılıyor)
				x = pow((double) ( (*nodes[i]).x - (*nodes[j]).x) ,2);
				y = pow( (double) ( (*nodes[i]).y - (*nodes[j]).y) ,2);
				dist = ( sqrt( x + y) );

				/*check for those players that are within jump range if so add to edges temporarily and increment cnt*/
				
				// eğer jump_range değerinden kısaysa veya kenarlara atanıyor
				if(dist <= atoi(argv[2])) 
				{
					edges[ nodes[i]->adj_size] = nodes[j];	
					
					nodes[i]->adj_size++;
				}
			}


		}


		nodes[i]->adj_list = (player **)malloc(sizeof(player *)* (nodes[i]->adj_size) );

		for(j = 0; j < nodes[i]->adj_size  ; j++)
		{
			nodes[i]->adj_list[j] = edges[j];
		}

	}

	/*check the distance of the each node from Urgosa and if it is 
	  within the initial cast range perform DFS on it to enumerate all 
	  possible paths of healing and find the one with the most pp healed*/


	for(i = 0; i < player_cnt ; i++)
	{
		
		x = pow( ( (double) ( (*UP).x - (*nodes[i]).x ) ), 2 );
		y = pow( ( (double) ( (*UP).y - (*nodes[i]).y ) ), 2 );

		dist = ( sqrt(x + y) );

		if(dist <= atoi(argv[1])) 
		{
			
			DFS( nodes[i] , 1 , Ip, 0 ,NULL); 
		
		}
	}

	/*Now print the best path with the name of the player and the amount they were healed. 
	  Then print out the total healing done*/
	for(i = 0; i < Ip->best_path_length ;i++)
	{
		printf("%s %d\n", (Ip->best_path[i])->name, Ip->healing[i] );
	}

	printf("Toplam Iyilesme %d\n", Ip->best_heal); 
	
	return 0;
}