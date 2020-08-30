# Little Man Computer
## Yet another [little man computer](https://en.wikipedia.org/wiki/Little_man_computer) implementation in C.

### Usage
```
$ make
$ ./main examples/fib.asm
10
0
1
1
2
3
5
8
```

### Tools
Tested on GNU+Linux with GCC 10.1.0, GNU Bison 3.6.4, flex 2.6.4 and GNU [libjit](http://git.savannah.gnu.org/cgit/libjit.git/commit/?id=942c988db170d98061a9e934fb3d7b618b7d5137).  
Tested on Microsoft Windows with GCC 8.1.0, GNU Bison 3.5.0 and flex 2.6.4. JIT not tested on this platform.  

### JIT
Experimental JIT assembly compiler is enabled by compiling with `make JIT=1` and by passing any value to the program second argument.  
It should work fine, but it won't definetily works with programs that use self-writing code (i.e. `examples/quine.asm`).  
Only tested on GNU+Linux.
