typedef struct node
{
  char name[1000];
  int x, y;
  int cur_PP, max_PP;
  struct node *prev;
} Node;
