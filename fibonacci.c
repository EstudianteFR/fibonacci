#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <stdbool.h>
#include <math.h>

int factorsArray[10];
int potArray[10];
int currentId = 0;

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

void primesFactors(int number) {
    if (number == 0) return;

    for (int i = 1; i <= number; ++i) {
        if (number % i == 0) {
            if (isPrime(i)) {
                factorsArray[currentId] = i;
                currentId++;
            }
        }
    }
    int current = 1;
    for (int i = 0; i < currentId; ++i) {
        current *= factorsArray[i];
    }

    if (current == number) {
        for (int i = 0; i < currentId; ++i) {
            if (i == 0) {
                printf("%d", factorsArray[i]);
            }else{
                printf(" X %d", factorsArray[i]);

            }
        }
    } else {
        int aux = number / current;

        int idaux = 0;
        int aux2 = 1;
        bool p = false;
        while (true) {
            if (aux == aux2) {
                break;
            }
            aux2 *= factorsArray[idaux];
            potArray[idaux] += 1;
            if (aux2 > aux) {
                aux2 = aux2 / factorsArray[idaux];
                if (potArray[idaux] - 2 < 0 ) {
                    current *= pow(factorsArray[idaux], 0);
                    potArray[idaux] = 0;
                }else{

                    current *= pow(factorsArray[idaux], potArray[idaux] - 2);
                }
                aux = number / current;
                aux2 = 1;
                idaux++;
                p = true;
            }
        }
        if (p) {
            potArray[0] -= 2;
        }
        for (int i = 0; i < currentId; ++i) {
            if (i == 0) {
                printf("%d", factorsArray[i]);
            }else{
                printf(" X %d", factorsArray[i]);

            }
            if (potArray[i] > 0) {
                printf("^%d", potArray[i] + 1);
            }
        }
    }

    currentId = 0;
    for (int i = 0; i < 10; ++i) {
        factorsArray[i] = 0;
        potArray[i] = 0;
    }
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
            printf(" -> ");
            primesFactors(result);
            printf("\n");
        }
    }
}
