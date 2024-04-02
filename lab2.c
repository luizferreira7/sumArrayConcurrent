#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "util.c"

#define TEST
//#define PRINT

typedef struct {
    Real *array;
    int start;
    int end;
    int index;
} ThreadArgs;

// Metodo usado para passar os argumentos da Thread e invocar a função que soma os elementos do array.
void *sumArrayThread(void *args) {
    ThreadArgs *thread_args = (ThreadArgs *)args;

    //Declara ponteiro usado para retornar calculo realizado na Thread
    Real *t_sum_ptr = malloc(sizeof(Real));
    *t_sum_ptr = sumArray(thread_args -> array, thread_args -> start, thread_args -> end);

#ifdef PRINT
    printf("--Thread %d terminou a execução.\n", thread_args -> index);
#endif

    free(args);
    pthread_exit(t_sum_ptr);
}

int testSum(Real *array, int size, Real sumConcurrent) {
    Real sumSequential = sumArray(array, 0, size);

    printf("\n");
    printf("A soma sequential em double é: %lf\n",  sumSequential.doubleValue);
    printf("A soma sequential em float é: %f\n",  sumSequential.floatValue);
    printf("A soma sequential com tratamento é: %f\n", convertLongIntToFloat(sumSequential.intValue));

    printf("\n");
    printf("A diferença de precisao em double foi: %lf\n", sumSequential.doubleValue - sumConcurrent.doubleValue);
    printf("A diferença de precisao em float foi: %f\n", sumSequential.floatValue - sumConcurrent.floatValue);
    printf("A diferença de precisao com tratamento foi: %f\n", convertLongIntToFloat(sumSequential.intValue) - convertLongIntToFloat(sumConcurrent.intValue));

    if (sumSequential.floatValue - sumConcurrent.floatValue > 0.01) {
        return -1;
    }

    return 0;
}


//Função main que contem a logica principal
int main(int argc, char *argv[]) {

    if (argc == 1)
        return -1;

    int M = atoi(argv[1]);

    int size = 0;
    Real *array = NULL;

    readArray(&array, &size);

#ifdef PRINT
    printArray(array, size);
#endif

    pthread_t tid_sistema[M];

    // Calcula o numero de elementos que cada Thread vai processar
    int threadArraySize = size / M;
    int remainder = size % M;

    for(int i = 0; i < M; i++) {
#ifdef PRINT
        printf("--Cria a thread %d\n", i);
#endif

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

    Real sumConcurrent;
    sumConcurrent.floatValue = 0.0f;
    sumConcurrent.doubleValue = 0.0;
    sumConcurrent.intValue = 0;

    for (int i = 0; i < M; i++) {
        Real *tempSumP;
        if (pthread_join(tid_sistema[i], (void*) &tempSumP)) {
            printf("--ERRO: pthread_join() \n");
            exit(-1);
        }
#ifdef PRINT
        printf("T[%i]: %f\n", i, convertLongIntToFloat(tempSumP -> intValue));
#endif
        sumConcurrent.floatValue += tempSumP -> floatValue;
        sumConcurrent.doubleValue += tempSumP -> doubleValue;
        sumConcurrent.intValue += tempSumP -> intValue;
    }

    printf("A soma em double é: %lf\n", sumConcurrent.doubleValue);
    printf("A soma em float é: %f\n", sumConcurrent.floatValue);
    printf("A soma com tratamento é: %f\n", convertLongIntToFloat(sumConcurrent.intValue));

    free(array);

#ifdef TEST
    if (testSum(array, size, sumConcurrent) < 0) {
        printf("Erro de precisão muito grande!\n");
    }
#endif

    return 0;
}