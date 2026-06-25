void m(void)
{
  time_t tVar1;
  
  tVar1 = time((time_t *)0x0);
  printf("%s - %d\n",c,tVar1);
  return;
}

void main(int ac, char **av)

{
  char *puVar1;
  void *pvVar2;
  char *puVar3;
  FILE *__stream;
  
  puVar1 = malloc(8);
  *puVar1 = 1;
  pvVar2 = malloc(8);
  puVar1[1] = pvVar2;
  puVar3 = malloc(8);
  *puVar3 = 2;
  pvVar2 = malloc(8);
  puVar3[1] = pvVar2;
  strcpy((char *)puVar1[1], av[1]);
  strcpy((char *)puVar3[1], av[2];
  __stream = fopen("/home/user/level8/.pass","r");
  fgets(c,0x44,__stream);  //0x44 = 68
  puts("~~");
  return 0;
}
