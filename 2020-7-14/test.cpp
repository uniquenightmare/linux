#include<stdio.h>
#include<iostream>

int main()
{
  union Data{
    int a;
    char b;
  }data;
  data.a = 1;
  if(data.b == 1)
  {
    printf("xiaoduan\n");
  }
  else{
    printf("daduan\n");
  }
  return 0;
}
