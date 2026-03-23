On this level, we got a binary named **level0**, when we execute it alone, it segfaults and when we give to it one or more argument it says "No !"

So to check what's going on with this binary, we analyzes it with Ghidra.
Then, we have obtained a decompiled readable **main** function (source file)

In that code, we can see that the binary compares the first arg to 0x1a7 (hexa format) which in decimal is 423.
Then we recover the ID of the effectives group and user (level1) and we set them to "become" level1, which grants us during the following execution of the shell, the privileges of level1.
Therefore, such as said in the subject, we look for the .pass file (/home/user/level1/.pass) and we see its content:

```zsh
$ cat /home/user/level1/.pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
```
