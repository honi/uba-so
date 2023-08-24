Write a C program that forks a child process that ultimately becomes a zombie process. This zombie process must remain in the system for at least 10 seconds.

```sh
make
./bin/main &
ps -l | grep -E "PPID|main| Z "
```

Expected output:

```
F S   UID     PID    PPID  C PRI  NI ADDR SZ WCHAN  TTY          TIME CMD
0 S     0    1677    1101  0  80   0 -   589 hrtime pts/0    00:00:00 main
1 Z     0    1678    1677  0  80   0 -     0 -      pts/0    00:00:00 main <defunct>
```
