#include "node.h"

#ifndef BILGI_H
#define BILGI_H

typedef struct bilgi
{
	int sicrama_sayisi;         // yapılacak sıçrama sayısı
	double ziplamadaki_iyilestirme; // zıplamada yapilan iyileştirme
	int dfs_basilan_can;		        // DFS'de yapılan  iyileştirme
	int toplam_iyilesme;		// toplam iyileşme miktarı
	double guc_azalma_katsayisi;    // güç azalma katsayısı (power_reduction)
	int * iyilesme;		        // iyileşme, dizi gibi kullanılıyor
	oyuncu **en_iyi_yol;         // en iyi iyileştirme sırasında oyuncular
	int en_iyi_yol_uzunluk;
} bg;

#endif
