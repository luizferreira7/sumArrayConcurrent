#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

#define M 8

typedef struct {
    float *array;
    int start;
    int end;
    int index;
} ThreadArgs;


// Metodo que imprime o vetor
void printArray(float array[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%f", array[i]);

        if ( (i + 1) != size) {
            printf(", ");
        }
    }

    printf("\n");
}

//Le array de float a partir de um arquivo
void readArray(char *filename, float **array, int *size, float *sum) {

    FILE *file;
    char line[100];

    file = fopen(filename, "r");

    //Converte cada linha do arquivo para um float do array
    while (fgets(line, sizeof(line), file) != NULL) {
        float f = atof(line);
        *array = realloc(*array, (*size + 1)  * sizeof(float));
        (*array)[*size] = f;
        (*size)++;
    }

    //Remove a soma dos valores do array e atribui para variavel
    *sum = (*array)[*size - 1];
    *array = realloc(*array, (*size - 1)  * sizeof(float));
    (*size)--;

    fclose(file);
}

//Metodo que soma os valores do array de uma determinada posição inicial ate uma final
float sumArray(float *array, int start, int end) {
    float sum = 0;
    for (int i = start; i < end; i++) {
        sum += array[i];
    }

    return sum;
}

// Metodo usado para passar os argumentos da Thread e invocar a função que soma os elementos do array.
void *sumArrayThread(void *args) {
    ThreadArgs *thread_args = (ThreadArgs *)args;

    //Declara ponteiro usado para retornar calculo realizado na Thread
    float *t_sum_ptr = malloc(sizeof(float));
    *t_sum_ptr = sumArray(thread_args -> array, thread_args -> start, thread_args -> end);

    printf("--Thread %d terminou a execução.\n", thread_args -> index);

    free(args);
    pthread_exit(t_sum_ptr);
}

//Função main que contem a logica principal
int main(int argc, char *argv[]) {

    char *filename = argv[1];
    float *array = NULL;
    float originalSum = 0;
    float sum = 0;
    int size = 0;

    readArray(filename, &array, &size, &originalSum);

    printArray(array, size);

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

    for (int i = 0; i < M; i++) {
        float *t_sum_p;
        if (pthread_join(tid_sistema[i], (void*) &t_sum_p)) {
            printf("--ERRO: pthread_join() \n"); exit(-1);
        }

        printf("T[%i]: %f\n", i, *t_sum_p);
        sum += *t_sum_p;
    }

    printf("A soma original é: %f\n", originalSum);
    printf("A soma é: %f", sum);

    free(array);

    return 0;
}