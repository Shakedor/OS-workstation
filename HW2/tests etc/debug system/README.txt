All the info you need is written in the 'debug_system.c' file

you DONT need to copy this file into your system!
it only contains code and instructions on where to put these pieces of code and how to use the system

the only file you'll need is 'main.c' and it will be used as a runnable file not a kernel file.

the only changes in kernel you'll need to do is adding new system calls for debuging in 'entry.S' and
a few pieces of code in 'sched.c' as explaind thoroughly in 'debug_system.c'

GOOD LUCK!