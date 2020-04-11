#ifndef NODE_H
#define NODE_H

typedef struct node
{
  char name[1000];
  int x, y;
  int mevcut_PP, max_PP;
  struct node *onceki;
  int adj_size;
  int ziyare_edildi;
  struct node **adj_list; /*This points to the players adjacent list*/
  int healing;
} oyuncu;

#endif
