typedef struct node
{
  char *name;
  int x, y;
  int cur_PP, max_PP;
  struct node *prev;
} Node;