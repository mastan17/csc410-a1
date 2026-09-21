Assignment 2 - Part 1
Mason Stanley

Files:
  starwars.c           Task 1 (4 child processes adjusting the shield)
  parallel_sums.c      Task 3 (parallel sum with fork + pipe)
  seq_sum.c            sequential sum (given)
  benchmark.sh         times sequential and 1-4 processes for N = 100,000,000
  A2_part1_report.pdf  Task 2, Task 4, and screenshots

How to run:
  gcc -Wall -o starwars starwars.c
  ./starwars

  gcc -O2 -o parallel_sums parallel_sums.c
  ./parallel_sums 100000000 4      (second number = how many processes, default is 4)

  gcc -O2 -o seq_sum seq_sum.c
  ./seq_sum 100000000

  ./benchmark.sh                   (needs bc)

Notes:
  - parallel_sums splits the array into equal-ish chunks, each child sums one chunk and sends
    the result to the parent through its own pipe. The parent closes the write ends, the children
    close the read ends they don't use, and the parent waits for everyone before reading and adding
    up the results.
  - I added the optional second argument so I could time it with 1, 2, 3 and 4 processes.
