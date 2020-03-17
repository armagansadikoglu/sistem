#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lib/fields.h"
#include "./lib/node.h"
#include "./lib/jval.h"

int main(int argc, char **argv)
{
  /* Program içerisinde ihtiyaç duyulan değişkenler */
  IS is;

  int satirSayisi = 0;
  int initial_range;
  int jump_range;
  int num_jumps;
  int initial_power;
  double power_reduction;

  /* Argüman sayısının 6 olduğu kontrol ediliyor */

  if (argc != 6)
  {
    fprintf(stderr, "usage: cheal initial_range jump_range num_jumps initial_power power_reduction < input_file\n");
    exit(1);
  }

  else
  {
    initial_range = atoi(argv[1]);
    jump_range = atoi(argv[2]);
    num_jumps = atoi(argv[3]);
    initial_power = atoi(argv[4]);
    power_reduction = atof(argv[5]);
  }

  
  /* DOSYA OKUMA İŞLEMLERİ */

  is = new_inputstruct(NULL);

  satirSayisi = get_line(is); // text dosyasındaki satır sayısı
  int satirSayac = 0;

  Node nodes[satirSayisi];
  
  while(get_line(is) >= 0) {  // satır sayısı kadar dönüyor
    nodes[satirSayac].x = atoi(is->fields[0]);
    nodes[satirSayac].y = atoi(is->fields[1]);
    nodes[satirSayac].cur_PP = atoi(is->fields[2]);
    nodes[satirSayac].max_PP = atoi(is->fields[3]);
    nodes[satirSayac].name = is->fields[4];

    // kontrol için
    printf("%d %d %d %d %s", nodes[satirSayac].x, nodes[satirSayac].y, nodes[satirSayac].cur_PP, nodes[satirSayac].max_PP, nodes[satirSayac].name);
    printf("\n");

    satirSayac++;
  }

  jettison_inputstruct(is);

  exit(0);
}