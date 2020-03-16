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

  while(get_line(is) > 0) {
    satirSayisi++;
  }

  jettison_inputstruct(is);


  /*

  Node* nodes[satirSayisi];
  for (int i = 0; i < satirSayisi; i++)
  {
    nodes[i] = malloc(sizeof(Node));
  }

   is = new_inputstruct(argv[6]);// tekrar okumak için açma
   int i =0; // indexte dönmek için

  while(get_line(is) > 0) {
     
      nodes[i]->x = atoi(is->fields[0]);
      nodes[i]->y = atoi(is->fields[1]);
      nodes[i]->cur_PP = atoi(is->fields[2]);
      nodes[i]->max_PP = atoi(is->fields[3]);
      strcpy(nodes[i]->name,is->fields[4]);
      //node[i]->name = &(is->fields[4]);
      printf("%s \n",nodes[i]->name);
      i++;
      printf("%d",i);
    
  }

  */

  exit(0);
}