#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include<stdbool.h>

void reset () {
    printf("\033[0m");
}

void red (int n) {
    printf("\033[1;31m%d", n);
    reset();
}

void green(int n) {
    printf("\033[0;32m%d", n);
    reset();
}

int fib(int n) {
	int i, j;
	if (n<2) return n;
	else {

		#pragma omp task shared(i)
		i=fib(n-1);
		#pragma omp task shared(j)
		j=fib(n-2);
		#pragma omp taskwait
		return i + j;
	}
}

bool isPrime(int n) {

    if (n <= 0) return false;
    if (n == 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2) {
        if ((n % i) == 0) {
            return false;
        }
    }
    return true;
}


int main(int argc, char **argv){
	int n, result;
	char *a = argv[1];
    n = atoi(a);
	#pragma omp parallel
    {
		#pragma omp single
        for (int i = 0; i <= n; ++i) {
            result = fib(i);

            if (isPrime(i)) {
                red(i);
            }else {
                printf("%d",i);
            }
            printf(": ");
            if (isPrime(result)) {
                green(result);
            }
            else {
                printf("%d", result);
            }
            printf("\n");
        }
    }
}
