Assignment 2 Part 3

Files
neuromancer.c - Task 1
average.c - Task 2
screenshots.pdf - output for Task 1 and 2
task3.pdf - Task 3 answers

To run
gcc neuromancer.c -lpthread -o neuromancer
./neuromancer
gcc average.c -lpthread -o average
./average

Task 1
Used a mutex and a condition variable. Each player waits until it is
their turn, then goes and wakes up the next player.

Task 2
Used a barrier so all the threads finish their sums before the average
is calculated.
