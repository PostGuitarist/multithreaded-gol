# multithreaded-gol

A multithreaded implementation of Conway's Game of Life using pthreads

This splits the grid into 4 regions and each thread is responsible for updating a region of the grid.

In a school project I was tasked with implementing multithreading to complete a specific task. Using pthreads I was able to multi-thread the game.


## Usage

### Compiling / Running

>I don't believe that pthreads is supported on Windows, so i would be aware it may only run if you are on MacOS/Linux

This make file compiles and runs the program.
Just run the following:
```
make
```
