#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Gera float randomizado
float randomFloat(float min, float max) {
    return min + ((float) rand() / RAND_MAX) * (max - min);
}

int main(int argc, char *argv[]) {

    //Verifica se foram passados o numero correto de argumentos para o programa
    if (argc <= 2)
        return -1;

    char *filename;

    //Le nome do arquivo
    if (argv[2] != NULL) {
        filename = argv[2];
    }

    FILE *file;

    //Prepara arquivo para escrita
    file = fopen(filename, "w");

    //Inicia seed para gerar pseudo aleatório
    srand(time(NULL));

    //Define intervalo maximo dos numeros
    float min = -1000.0f;
    float max = 1000.0f;

    int n = 0;

    //Le quantidade de linhas
    if (argv[1] != NULL) {
        n = atoi(argv[1]);
    }

    //Popula o arquivo
    for (int i = 0; i < n; i++) {
        float f = randomFloat(min, max);

        char c[40];
        sprintf(c, "%g", f);
        fprintf(file, "%s", c);

        if (i < n - 1) {
            fprintf(file, "\n");
        }
    }

    //Fecha arquivo terminando escrita
    fclose(file);

    return 0;
}