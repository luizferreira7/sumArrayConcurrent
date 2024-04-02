#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FLOAT_SIZE 1000000

typedef struct {
    char *string;
    float floatValue;
    double doubleValue;
    long long int intValue;
} Real;

//Gera float randomizado
float randomFloat(float min, float max) {
    return min + ((float) rand() / RAND_MAX) * (max - min);
}

// Metodo que imprime o vetor
void printArray(Real *array, int size) {
    for (int i = 0; i < size; i++) {
        printf("%s", array[i].string);

        if (i > 0 && i < size - 1) {
            printf(", ");
        }
    }

    printf("\n");
}

//Metodo que soma os valores do array de uma determinada posição inicial ate uma final
Real sumArray(Real *array, int start, int end) {
    Real sum;
    sum.floatValue = 0.0f;
    sum.intValue = 0;
    sum.doubleValue = 0.0;

    for (int i = start; i < end; i++) {
        sum.intValue += array[i].intValue;
        sum.doubleValue += array[i].doubleValue;
        sum.floatValue += array[i].floatValue;
    }

    return sum;
}


//Le array de float a partir de um arquivo
void readArray(Real **array, int *size) {
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

        Real real;
        real.string = (char *) malloc(strlen(line) + 1);
        int index = 0;
        while (index <= strlen(line)) {
            real.string[index] = line[index];
            index++;
        }

        real.floatValue = f;
        real.intValue = c;
        real.doubleValue = atof(line);

        *array = realloc(*array, (*size + 1) * sizeof(Real));
        (*array)[*size] = real;
        (*size)++;
    }
}

float convertLongIntToFloat(long long int value) {
    return (float) value / FLOAT_SIZE;
}