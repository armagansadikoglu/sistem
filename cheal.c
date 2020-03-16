#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lib/fields.h"
#include "./lib/node.h"
#include "./lib/jval.h"
int main(int argc, char **argv)
{
  IS is;
  
  int satirSayisi=0;
  int initial_range;
  int jump_range;
  int num_jumps;
  int initial_power;
  double power_reduction;
  
  
  if (argc != 7) { fprintf(stderr, "usage: printwords filename\n"); exit(1); }
  else {
    initial_range = atoi(argv[1]);
    jump_range = atoi(argv[2]);
    num_jumps = atoi(argv[3]);
    initial_power = atoi(argv[4]);
    power_reduction = atof(argv[5]);
    
  }
 
  is = new_inputstruct(argv[6]);
  if (is == NULL) {
    perror(argv[6]);
    exit(1);
  }

  while(get_line(is) > 0) {
    //printf("%s %s %s %s %s \n", is->fields[0],is->fields[1],is->fields[2],is->fields[3],is->fields[4]);
    satirSayisi++; // satir sayisini öğrenip bellekte yer ayıracağız
  }
  jettison_inputstruct(is); // dosyayı tekrar okumak için kapattık
  
  
  Node* nodes[satirSayisi];
  for (int i = 0; i < satirSayisi; i++)
  {
    nodes[i] = malloc(sizeof(Node));
  }

   is = new_inputstruct(argv[6]);// tekrar okumak için açma
   int i =0; // indexte dönmek için


  /* 
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
  
  jettison_inputstruct(is);
  exit(0);
}
