#!/bin/bash
# Times seq_sum and parallel_sums (1-4 processes) for N = 100,000,000.
# Runs each config RUNS times and prints the average wall-clock time.
# Usage: ./benchmark.sh [N] [RUNS]
N=${1:-100000000}
RUNS=${2:-5}

gcc -O2 -o seq_sum seq_sum.c || exit 1
gcc -O2 -o parallel_sums parallel_sums.c || exit 1

avg_time() {   # avg_time <command...>
    local total=0 s e
    for ((r = 0; r < RUNS; r++)); do
        s=$(date +%s.%N)
        "$@" > /dev/null
        e=$(date +%s.%N)
        total=$(echo "$total + ($e - $s)" | bc -l)
    done
    printf "%.3f" "$(echo "$total / $RUNS" | bc -l)"
}

echo "N = $N, averaged over $RUNS runs (wall-clock seconds)"
printf "%-12s %-12s %-12s %-12s %-12s\n" "Sequential" "1 process" "2 processes" "3 processes" "4 processes"
seq_t=$(avg_time ./seq_sum $N)
t1=$(avg_time ./parallel_sums $N 1)
t2=$(avg_time ./parallel_sums $N 2)
t3=$(avg_time ./parallel_sums $N 3)
t4=$(avg_time ./parallel_sums $N 4)
printf "%-12s %-12s %-12s %-12s %-12s\n" "${seq_t}s" "${t1}s" "${t2}s" "${t3}s" "${t4}s"
