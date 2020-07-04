#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
  char *lp = (char *)malloc(10);
  memset(lp,'\0',10);
  memcpy(lp,"linux-61",8);
  printf("%s\n",lp);
  free(lp);

  //free(lp);
  lp = NULL;
  free(lp);
  return 0;
}
