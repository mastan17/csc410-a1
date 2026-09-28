Assignment 2 - Part 2: Multithreading with pthreads

Files:
- sumT.c: parallel array sum (100,000,000 elements)
- matrixT.h and matrixT.c: parallel matrix multiplication (1000 x 1000)

How to compile (change NUM_THREADS to 1, 2, 3 or 4):
  gcc -pthread -DNUM_THREADS=4 -o sumT sumT.c
  gcc -pthread -DNUM_THREADS=4 -o matrixT matrixT.c

How to run:
  ./sumT
  ./matrixT

What it does:
- sumT.c splits the array equally between the threads. Each thread adds
  up its part and saves it in its own spot in partialSums. Main adds
  those up at the end.
- matrixT.c splits the rows of the matrix between the threads. Each
  thread computes its own rows of C.

Output:
- sumT prints Total Sum (should be 5000000050000000) and the time.
- matrixT prints "Result check: CORRECT" if every entry is 1000, and the time.

Notes:
- No locks were needed because each thread only writes to its own data.
- Tested in WSL Ubuntu.
