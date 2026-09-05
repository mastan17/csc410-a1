#include <stdio.h>

#define N 100000 // intervals

double f(double x) {
    return 4.0 / (1.0 + x * x); // Function to integrate
}

double trapezoidalRule() 
{
    double a = 0.0, b = 1.0; 
    double h = (b - a) / N;  // width of each interval
    double sum = (f(a) + f(b)) / 2.0; 

    for (int i = 1; i < N; i++) {
        double x = a + i * h;
        sum += f(x);
    }

    return sum * h;
}

int main() {
    double pi = trapezoidalRule();
    printf("Estimated value of π: %f\n", pi);
    return 0;
}
