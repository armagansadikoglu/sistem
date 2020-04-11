#include "node.h"

#ifndef BILGI_H
#define BILGI_H

typedef struct bilgi
{
	int sicrama_sayisi;         // yapılacak sıçrama sayısı
	double c_pp;		        // ??? the current amount of pp healed per current jump
	int amnt_heal;		        // ??? the current amount of healing done in this dfs 
	int toplam_iyilesme;		// toplam iyileşme miktarı
	double guc_azalma_katsayisi;    // güç azalma katsayısı (power_reduction)
	int * iyilesme;		        // iyileşme, dizi gibi kullanılıyor
	oyuncu **en_iyi_yol;         // en iyi iyileştirme sırasında oyuncular
	int best_path_length;
} bg;

#endif