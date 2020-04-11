typedef struct node
{
  char name[1000];
  int x, y;
  int cur_PP, max_PP;
  struct node *prev;
  int adj_size;
  int visited;
  struct node **adj_list; /*This points to the players adjacent list*/
  int healing;
} player;
