#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "util.c"

#define TEST

typedef struct {
    Float *array;
    int start;
    int end;
    int index;
} ThreadArgs;

// Metodo usado para passar os argumentos da Thread e invocar a função que soma os elementos do array.
void *sumArrayThread(void *args) {
    ThreadArgs *thread_args = (ThreadArgs *)args;

    //Declara ponteiro usado para retornar calculo realizado na Thread
    long long int *t_sum_ptr = malloc(sizeof(long int));
    *t_sum_ptr = sumArray(thread_args -> array, thread_args -> start, thread_args -> end);

    printf("--Thread %d terminou a execução.\n", thread_args -> index);

    free(args);
    pthread_exit(t_sum_ptr);
}

void test(Float *array, int size, float sumConcurrent) {
    float sumSequential = (float) sumArray(array, 0, size) / FLOAT_SIZE;

    printf("A soma sequential é: %f\n",  sumSequential);

    printf("A diferença de precisao foi: %f\n", sumSequential - sumConcurrent);
}

//Função main que contem a logica principal
int main(int argc, char *argv[]) {

    if (argc == 1)
        return -1;

    int M = atoi(argv[1]);

    int size = 0;
    Float *array = NULL;

    readArray(&array, &size);

#ifdef TEST
    printArray(array, size);
#endif

    pthread_t tid_sistema[M];

    // Calcula o numero de elementos que cada Thread vai processar
    int threadArraySize = size / M;
    int remainder = size % M;

    for(int i = 0; i < M; i++) {
        printf("--Cria a thread %d\n", i);

        // Prepara os argumentos da Thread
        ThreadArgs *args = malloc(sizeof(ThreadArgs));
        args -> array = array;
        args -> start = i * threadArraySize;
        args -> end = (i + 1) * threadArraySize;
        args -> index = i;

        // Garante que a ultima Thread executa os elementos que sobram da divisam de size/M caso não seja exata
        if ( (i + 1) == M) {
            args -> end += remainder;
        }

        if (pthread_create(&tid_sistema[i], NULL, sumArrayThread, args)) {
            printf("--ERRO: pthread_create()\n"); exit(-2);
        }
    }

    long long int sumConcurrentInt = 0;
    for (int i = 0; i < M; i++) {
        long long int *t_sum_p;
        if (pthread_join(tid_sistema[i], (void*) &t_sum_p)) {
            printf("--ERRO: pthread_join() \n");
            exit(-1);
        }
#ifdef TEST
        printf("T[%i]: %f\n", i, convertLongIntToFloat(*t_sum_p));
#endif
        sumConcurrentInt += *t_sum_p;
    }

    float sumConcurrent = convertLongIntToFloat(sumConcurrentInt);
    printf("A soma é: %f\n", sumConcurrent);

#ifdef TEST
    test(array, size, sumConcurrent);
#endif

    free(array);

    return 0;
}