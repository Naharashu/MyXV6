# MyXV6

MyXV6 is a toy os based on xv6.

> [!WARNING]
> Some parts may be vibe coded because i not really low-level coder, but i tried to learn something

## Difference between MyXV6 and xv6

1. Max file size is now ~500 MBs
2. FS size now ~500 MBs
3. added /dev with /dev/null, /dev/random, /dev/zero, /dev/urandom
4. head tool, size, time and rename
5. 16KB stack for programs
6. ^C (ctrl + c) kills program
7. support of reboot
8. Currently working on RTL8139 driver
9. 8KB kernel stack size
10. 8MB of ram mapped for virtual memory(instead of 4MB for xv6)