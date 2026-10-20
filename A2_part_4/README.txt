A2 Part 4 - Mason Stanley

p_integrate.c - Task 1
p_merge.h, main_merge.c - merge sort header/main
p_merge_1.c, p_merge_2.c, p_merge_3.c - Approaches 1, 2, 3

Compile/run (NUM_THREADS = 1-4):
gcc -DNUM_THREADS=4 p_integrate.c -o integ -pthread && ./integ
gcc -DNUM_THREADS=4 main_merge.c p_merge_1.c -o m1 -pthread && ./m1
(same for p_merge_2.c and p_merge_3.c)
