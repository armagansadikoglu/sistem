#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "./lib/fields.h"
#include "./lib/bilgi.h"
#include "./lib/node.h"
#include "./lib/jval.h"


/*This struct will be used to hold global information*/

/* işlemdeki oyuncunun tüm çocukları arasında DFS araması yapılacak */

void DFS( oyuncu * pp, int hop, bg * bilgi, int ttl_heal, oyuncu * from);

int main( int argc, char** argv)
{
	/* argüman sayısı kontrol ediliyor */
	if (argc != 6) { fprintf(stderr, "usage: printwords filename\n"); exit(1); }

	/*
	 * TEXT DOSYASINDAKİ SATIRLAR DÖNGÜSEL OLARAK OKUNUYOR
	 */

	char hldr[100];

	double dist;

	int oyuncu_sayisi, x, y, c_pp, mx_pp, i, j;

    oyuncu * pp;				

	oyuncu * onceki_temp = NULL;						/* geçici olarak kullanılacak*/

	oyuncu * Lokman_Hekim = NULL;				/* Lokman Hekimi gösteren oyuncu işaretçisi */

	oyuncu_sayisi = 0;
	
	bg *bilgi = malloc(sizeof(bg));				/* bilgiler için yer ayırılıyor */

	bilgi->sicrama_sayisi = atoi(argv[3]);		/* yapılacak sıçrama sayısı (num_jumps) atanıyor */
	bilgi->c_pp = atoi(argv[4]);				/* initial_power atanıyor */
	sscanf(argv[5],"%lf",&bilgi->guc_azalma_katsayisi);		/* güç azalma katsayısı atanıyor */
	bilgi->amnt_heal = 0;						/* ??? */
	bilgi->toplam_iyilesme = 0;					/* toplam iyileşme (hedef) */

	// sıçrama sayısı kadar oyuncu yeri ayrılıyor
	bilgi->en_iyi_yol = (oyuncu **) malloc(sizeof(oyuncu *) * atoi(argv[3]));

	// sıçrama sayısı kadar iyileşme miktarı yeri ayrılıyor
	bilgi->iyilesme = (int *) malloc(sizeof(int *) * atoi(argv[3]));

	/*
	 * TEXT DOSYASINDAKİ SATIRLAR DÖNGÜSEL OLARAK OKUNUYOR
	 */

	while(scanf("%d %d %d %d %s", &x, &y, &c_pp, &mx_pp, hldr) > 0 )
	{
		pp = (oyuncu *)malloc(sizeof(oyuncu));	// oyuncu için yer ayırılıyor

		pp->x = x;	// x değeri atanıyor
		pp->y = y;	// y değeri atanıyor
		pp->mevcut_PP = c_pp;	// mevcut gücü atanıyor
		pp->max_PP = mx_pp;	// maksimum gücü atanıyor
		pp->ziyare_edildi = -1;	// ziyaret edildi mi(-1 ise edilmedi)
		pp->healing = 0;	// iyileştirme başlangıçta 0 atanıyor
		pp->adj_size = 0;	// bitişik boyutu 0 atanıyor
		strcpy(pp->name, hldr);	// adı kopyalanıyor
		pp->onceki = onceki_temp;	// önceki düğüm (ilk düğümde NULL)

		/* lokman hekim okunuyorsa kayıt ediliyor */
		if( strcmp(hldr, "Lokman_Hekim") == 0) Lokman_Hekim = pp; 
		
		onceki_temp = pp;	// önceki oyuncu olarak atanıyor

		oyuncu_sayisi++;	// oyuncu sayısı artılıyor
	}



	oyuncu *nodes[oyuncu_sayisi];				/*tüm oyuncu düğümleri tutulacak*/
	oyuncu *edges[oyuncu_sayisi];				/*will be used as a temporary player pointer contianer*/

	/*
	 * TÜM DÜĞÜMLER TARANIYOR VE ADRESLERİ NODES İÇERİSİNE ATANIYOR
	 */

	for( i=0 ; i < oyuncu_sayisi ; i++)
	{
		nodes[i] = onceki_temp;
		onceki_temp = onceki_temp->onceki;
	}

	/*
	 * TÜM DÜĞÜMLER TARANIYOR VE KENARLARI EDGES İÇERİSİNE ATANIYOR
	 */
	
	for( i = 0; i < oyuncu_sayisi; i++)
	{
		// check the needed size of adjacent array for current node
		// by going through and counting how many are possible based on the distance from current node
		
		for(j = 0; j < oyuncu_sayisi; j++)
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
				
				// eğer jump_range değerinden kısaysa veya eşitse kenarlara atanıyor
				if(dist <= atoi(argv[2])) 
				{
					edges[ nodes[i]->adj_size] = nodes[j];	
					
					nodes[i]->adj_size++;
				}
			}
		}

		// kenarlar adj_list'e ekleniyor

		nodes[i]->adj_list = (oyuncu **)malloc(sizeof(oyuncu *)* (nodes[i]->adj_size) );

		for(j = 0; j < nodes[i]->adj_size  ; j++)
		{
			nodes[i]->adj_list[j] = edges[j];
		}

	}

	/*check the distance of the each node from Urgosa and if it is 
	  within the initial cast range perform DFS on it to enumerate all 
	  possible paths of healing and find the one with the most pp healed*/


	for(i = 0; i < oyuncu_sayisi ; i++)
	{
		
		x = pow( ( (double) ( (*Lokman_Hekim).x - (*nodes[i]).x ) ), 2 );
		y = pow( ( (double) ( (*Lokman_Hekim).y - (*nodes[i]).y ) ), 2 );

		dist = ( sqrt(x + y) );

		if(dist <= atoi(argv[1])) 
		{
			
			DFS( nodes[i] , 1 , bilgi, 0 ,NULL); 
		
		}
	}

	/*Now print the best path with the name of the player and the amount they were healed. 
	  Then print out the total healing done*/
	for(i = 0; i < bilgi->best_path_length ;i++)
	{
		printf("%s %d\n", (bilgi->en_iyi_yol[i])->name, bilgi->iyilesme[i] );
	}

	printf("Toplam Iyilesme %d\n", bilgi->toplam_iyilesme); 
	
	return 0;
}



void DFS( oyuncu * pp, int hop, bg * bilgi, int ttl_heal, oyuncu * from)
{
	int i, j, rslt, dfs_heal, old_pp, cnt;
	
	oyuncu * ptr;

	double prv_pp, num;

	cnt = 0;

	/*if the node has already been visited in this dfs just return*/
	if( (*pp).ziyare_edildi != -1) return;
	
	/*Check to make sure that the hop limit is not exceeded if so just return*/
	if( hop > bilgi->sicrama_sayisi)
	{
		return;
	}

	(*pp).ziyare_edildi = 1;

	(*pp).onceki = from;					/*set the previous field to the node that came before*/
	

	/*Save the state of the player and heal amounts before the DFS*/
	old_pp = (*pp).mevcut_PP;			

	prv_pp = bilgi->c_pp;					

	dfs_heal = bilgi->amnt_heal;			
	
	
	
	/*get how much healing would occur if the max is not exceeded*/
	rslt = bilgi->c_pp + pp->mevcut_PP;	


	/*the below will make sure the players max pp is not exceeded, and calculate
	  the appropriate amount of healing done one the current node and thus far.
	  It also sets the currents nodes new pp*/
	if( rslt > (*pp).max_PP)
	{
		bilgi->amnt_heal += (*pp).max_PP - (*pp).mevcut_PP;	
		pp->healing  = (*pp).max_PP - (*pp).mevcut_PP;
		ttl_heal += (*pp).max_PP - (*pp).mevcut_PP;	
		(*pp).mevcut_PP = (*pp).max_PP;			

	}
	else
	{
		bilgi->amnt_heal += rint( bilgi->c_pp );					
		ttl_heal += rint( bilgi->c_pp );					
		(*pp).mevcut_PP = rslt;						

		pp->healing  = rint(bilgi->c_pp);
		
	}

	
	/*now check this new total heal amount against your best heal and if it beats it replace the old with the new
	  and store the path that got you here*/
	if( ttl_heal > bilgi->toplam_iyilesme)
	{

		bilgi->toplam_iyilesme = ttl_heal;


		bilgi->best_path_length = hop;


		ptr = pp;

		for( i = bilgi->best_path_length - 1 ; i >= 0 ; i--)
		{
			(bilgi->en_iyi_yol[i]) = ptr;

			(bilgi->iyilesme[i]) = ptr->healing;

			ptr = ptr->onceki;
		}

	}
	
	//adjust the next jumps pp healed using the power reduction
	bilgi->c_pp =  bilgi->c_pp * (1 - (bilgi->guc_azalma_katsayisi)) ;


	for(i = 0; i < (*pp).adj_size ; i++)
	{
		ptr = (*pp).adj_list[i];		

		DFS( ptr, hop+1, bilgi, ttl_heal ,pp);

	}



	//set everything back to the way it was

	(*pp).ziyare_edildi = -1;

	bilgi->c_pp = prv_pp;

	bilgi->amnt_heal = dfs_heal;

	(*pp).mevcut_PP = old_pp;

	return;
}