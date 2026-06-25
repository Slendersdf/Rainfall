void n(void)
{
  system("/bin/cat /home/user/level7/.pass");
  return;
}

void m(void *param_1,int param_2,char *param_3,int param_4, int param_5)
{
  puts("Nope");
  return;
}

void main(int ac, char **av)
{
  char *__dest;
  void *puVar1;
  
  __dest = malloc(0x40); //64
  puVar1 = malloc(4); //4
  *puVar1 = m;
  strcpy(__dest,*(char **)(param_2 + 4)); //copy av[1] (cause +4 = next argument,) to __dest
  (*(code *)*puVar1)(); //m function call
  return;
}
