#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "./include/fields.h"
#include "./include/bilgi.h"
#include "./include/node.h"
#include "./include/jval.h"



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

	double mesafe;

	int oyuncu_sayisi, x, y, c_pp, mx_pp, i, j;

    oyuncu * pp;				

	oyuncu * onceki_temp = NULL;						/* geçici olarak kullanılacak*/

	oyuncu * Lokman_Hekim = malloc(sizeof(oyuncu));				/* Lokman Hekimi gösteren oyuncu işaretçisi */

	oyuncu_sayisi = 0;
	
	bg *bilgi = malloc(sizeof(bg));				/* bilgiler için yer ayırılıyor */

	bilgi->sicrama_sayisi = atoi(argv[3]);		/* yapılacak sıçrama sayısı (num_jumps) atanıyor */
	bilgi->ziplamadaki_iyilestirme = atoi(argv[4]);				/* initial_power atanıyor */
	sscanf(argv[5],"%lf",&bilgi->guc_azalma_katsayisi);		/* güç azalma katsayısı atanıyor */
	bilgi->dfs_basilan_can = 0;						/* ??? */
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
		pp->ziyaret_edildi = -1;	// ziyaret edildi mi(-1 ise edilmedi)
		pp->iyilestirme = 0;	// iyileştirme başlangıçta 0 atanıyor
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
		
		// Mesafeye bağlı olarak mevcut node'dan atlamanın yapılabileceği nodelar bulunuyor
		// Array için boyut hesaplanıyor
		
		for(j = 0; j < oyuncu_sayisi; j++)
		{
			// düğümün kendisini de saymaması gerekiyor
			if(j != i)
			{
				// uzaklık hesaplanıyor
				// pow(üzerini almada kullanılıyor)
				x = pow((double) ( (*nodes[i]).x - (*nodes[j]).x) ,2);
				y = pow( (double) ( (*nodes[i]).y - (*nodes[j]).y) ,2);
				mesafe = ( sqrt( x + y) );

				
				
				// eğer jump_range değerinden kısaysa veya eşitse kenarlara atanıyor
				// dizi için gereken boyut değişkeni arttırılıyor
				if(mesafe <= atoi(argv[2])) 
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


	// Her bir node'un Urgosa'ya olan mesafesine bakılıp initial değerinde olanlar için DFS'i çalıştır
	// En yüksek iyileştirmeli olanı bul
	for(i = 0; i < oyuncu_sayisi ; i++)
	{
		// x ve y uzaklıklarının karesi alınıyor
		x = pow( ( (double) ( (*Lokman_Hekim).x - (*nodes[i]).x ) ), 2 );
		y = pow( ( (double) ( (*Lokman_Hekim).y - (*nodes[i]).y ) ), 2 );

		// karekök ile mesafe ölçülüyor
		mesafe = ( sqrt(x + y) );

		if(mesafe <= atoi(argv[1])) 
		{
			
			DFS( nodes[i] , 1 , bilgi, 0 ,NULL); 
		
		}
	}
	

	  // En iyi yola ait iyileştirme
	for(i = 0; i < bilgi->en_iyi_yol_uzunluk ;i++)
	{
		printf("%s %d\n", (bilgi->en_iyi_yol[i])->name, bilgi->iyilesme[i] );
	}

	printf("Toplam Iyilesme %d\n", bilgi->toplam_iyilesme); 
	
	return 0;
}

/*
 * DEPTH FIRST SEARCH (DERİN ÖNCELİKLİ ARAMA) yapılıyor
 */

void DFS( oyuncu * pp, int mevcut_sicrama, bg * bilgi, int toplam_iyilestirme, oyuncu * onceki_oyuncu)
{
	int i, j, sonuc, dfs_iyilestirme, eski_pp, cnt;
	
	oyuncu * ptr;

	double onceki_pp, num;

	cnt = 0;

	
	// Node (oyuncu) daha önce ziyaret edildiyse atla
	if( (*pp).ziyaret_edildi != -1) return;
	
	// Sıçrama sayısı aşmadığından emin olunuyor
	if( mevcut_sicrama > bilgi->sicrama_sayisi)
	{
		return;
	}

	(*pp).ziyaret_edildi = 1;

	(*pp).onceki = onceki_oyuncu;					//Atlanılan nodeun adresini tut
	

	
	// playerın bilgilerini ve yapılan iyileşirmesi kayıt ediliyor
	eski_pp = (*pp).mevcut_PP;			

	onceki_pp = bilgi->ziplamadaki_iyilestirme;					

	dfs_iyilestirme = bilgi->dfs_basilan_can;			
	
	
	
	// iyileştrime yapılıyor
	sonuc = bilgi->ziplamadaki_iyilestirme + pp->mevcut_PP;	




	  // iyileştirmede maksimum değeri aşmadığının kontrolü
	if( sonuc > (*pp).max_PP)
	{
		bilgi->dfs_basilan_can += (*pp).max_PP - (*pp).mevcut_PP;	
		pp->iyilestirme  = (*pp).max_PP - (*pp).mevcut_PP;
		toplam_iyilestirme += (*pp).max_PP - (*pp).mevcut_PP;	
		(*pp).mevcut_PP = (*pp).max_PP;			

	}
	else
	{
		bilgi->dfs_basilan_can += rint( bilgi->ziplamadaki_iyilestirme );					
		toplam_iyilestirme += rint( bilgi->ziplamadaki_iyilestirme );					
		(*pp).mevcut_PP = sonuc;						

		pp->iyilestirme  = rint(bilgi->ziplamadaki_iyilestirme);
		
	}

	// yolları kıyaslama
	if( toplam_iyilestirme > bilgi->toplam_iyilesme)
	{

		bilgi->toplam_iyilesme = toplam_iyilestirme;


		bilgi->en_iyi_yol_uzunluk = mevcut_sicrama;


		ptr = pp;

		for( i = bilgi->en_iyi_yol_uzunluk - 1 ; i >= 0 ; i--)
		{
			(bilgi->en_iyi_yol[i]) = ptr;

			(bilgi->iyilesme[i]) = ptr->iyilestirme;

			ptr = ptr->onceki;
		}

	}
	
	//power reduction kat sayısına göre bir sonraki iyiliştirmede kullanılabilecek puan
	bilgi->ziplamadaki_iyilestirme =  bilgi->ziplamadaki_iyilestirme * (1 - (bilgi->guc_azalma_katsayisi)) ;


	for(i = 0; i < (*pp).adj_size ; i++)
	{
		ptr = (*pp).adj_list[i];		

		DFS( ptr, mevcut_sicrama+1, bilgi, toplam_iyilestirme ,pp);

	}
	
	// sonraki çalıştırma için sıfırlama

	(*pp).ziyaret_edildi = -1;

	bilgi->ziplamadaki_iyilestirme = onceki_pp;

	bilgi->dfs_basilan_can = dfs_iyilestirme;

	(*pp).mevcut_PP = eski_pp;

	return;
}
