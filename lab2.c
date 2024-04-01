#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "math.h"
#include "string.h"

#define M 3

typedef struct {
    char *floatChar;
    int intValue;
} CustomFloat;

typedef struct {
    CustomFloat *array;
    int start;
    int end;
    int index;
} ThreadArgs;

// Metodo que imprime o vetor
void printArray(CustomFloat *array, int size) {
    for (int i = 0; i < size; i++) {
        printf("%s", array[i].floatChar);
    }

    printf("\n");
}

//Le array de float a partir de um arquivo
void readArray(char *filename, CustomFloat **array, int *size, CustomFloat *sum) {

    FILE *file;
    char line[100];

    file = fopen(filename, "r");

    //Converte cada linha do arquivo para um float do array
    while (fgets(line, sizeof(line), file) != NULL) {
        int nDigits = 0;
        int flag = 0;

        for (int i = 0; i < 100; i++) {
            if (line[i] == '.') {
                flag = 1;
                continue;
            }

            if (line[i] == '\n') {
                break;
            }

            if (flag == 1) {
                nDigits += 1;
            }
        }

        char *p;
        float f = strtof(line, &p);

        float temp = f * pow(10, nDigits);
        int c = (int) temp;
        c *= (int) pow(10, 6 - nDigits);

        CustomFloat customFloat;
        customFloat.floatChar = (char *) malloc(strlen(line)+1);
        int index=0;
        while(index <= strlen(line))
        {
            customFloat.floatChar[index] = line[index];
            index++;
        }

        customFloat.intValue = c;

        *array = realloc(*array, (*size + 1)  * sizeof(CustomFloat));
        (*array)[*size] = customFloat;
        (*size)++;
    }

    //Remove a soma dos valores do array e atribui para variavel
    *sum = (*array)[*size - 1];
    *array = realloc(*array, (*size - 1)  * sizeof(CustomFloat));
    (*size)--;

    fclose(file);
}

//Metodo que soma os valores do array de uma determinada posição inicial ate uma final
int sumArray(CustomFloat *array, int start, int end) {
    int sum = 0;
    for (int i = start; i < end; i++) {
        sum += array[i].intValue;
    }

    return sum;
}

// Metodo usado para passar os argumentos da Thread e invocar a função que soma os elementos do array.
void *sumArrayThread(void *args) {
    ThreadArgs *thread_args = (ThreadArgs *)args;

    //Declara ponteiro usado para retornar calculo realizado na Thread
    int *t_sum_ptr = malloc(sizeof(int));
    *t_sum_ptr = sumArray(thread_args -> array, thread_args -> start, thread_args -> end);

    printf("--Thread %d terminou a execução.\n", thread_args -> index);

    free(args);
    pthread_exit(t_sum_ptr);
}

//Função main que contem a logica principal
int main(int argc, char *argv[]) {

    char *filename = argv[1];
    int sum = 0;
    int size = 0;

    CustomFloat *array = NULL;
    CustomFloat originalSum;

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
        int *t_sum_p;
        if (pthread_join(tid_sistema[i], (void*) &t_sum_p)) {
            printf("--ERRO: pthread_join() \n"); exit(-1);
        }

        printf("T[%i]: %f\n", i, (float) *t_sum_p / 1000000);
        sum += *t_sum_p;
    }

    printf("A soma original é: %s\n", originalSum.floatChar);
    printf("A soma é: %f", (float) sum / 1000000);

    free(array);

    return 0;
}