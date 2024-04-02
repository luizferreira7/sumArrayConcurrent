#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FLOAT_SIZE 1000000

typedef struct {
    char *floatChar;
    float value;
    long long int intValue;
} Float;


//Gera float randomizado
float randomFloat(float min, float max) {
    return min + ((float) rand() / RAND_MAX) * (max - min);
}

// Metodo que imprime o vetor
void printArray(Float *array, int size) {
    for (int i = 0; i < size; i++) {
        printf("%s", array[i].floatChar);

        if (i > 0 && i < size - 1) {
            printf(", ");
        }
    }

    printf("\n");
}

//Metodo que soma os valores do array de uma determinada posição inicial ate uma final
long long int sumArray(Float *array, int start, int end) {
    long long int sum = 0;
    for (int i = start; i < end; i++) {
        sum += array[i].intValue;
    }

    return sum;
}


//Le array de float a partir de um arquivo
void readArray(Float **array, int *size) {
    char line[100];

    while (scanf("%s", line) != EOF) {
        int nDigits = 0;
        int flag = 0;

        for (int i = 0; i < 100; i++) {
            if (line[i] == '.') {
                flag = 1;
                continue;
            }

            if (line[i] == '\0') {
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

        Float customFloat;
        customFloat.floatChar = (char *) malloc(strlen(line) + 1);
        int index = 0;
        while (index <= strlen(line)) {
            customFloat.floatChar[index] = line[index];
            index++;
        }

        customFloat.value = f;
        customFloat.intValue = c;

        *array = realloc(*array, (*size + 1) * sizeof(Float));
        (*array)[*size] = customFloat;
        (*size)++;
    }
}

float convertLongIntToFloat(long long int value) {
    return (float) value / FLOAT_SIZE;
}