#include "stdio.h"

//Declara estrutura para poder representar float em ponto flutuante
typedef union {
    float f;
    struct {
        unsigned int mantissa : 23;
        unsigned int exponent : 8;
        unsigned int sign : 1;
    } parts;
} FloatPoint;

//Metodo que converte ponto flutuante para float
float convertToFloat(FloatPoint fCast) {
    unsigned int n = (fCast.parts.sign << 31) | (fCast.parts.exponent << 23) | fCast.parts.mantissa;

    float f = *((float*) &n);

    return f;
}

//Printa um numero em ponto flutuante
void printFloatPoint(FloatPoint f) {
    printf("\nFloat -> %f\n", f.f);
    printf("Mantissa -> %d\n", f.parts.mantissa);
    printf("Expoente -> %d\n", f.parts.exponent);
    printf("Sinal -> %d\n", f.parts.sign);
}

//Realiza operação de soma com ponto flutuante
FloatPoint sumFloatPoint(FloatPoint f1, FloatPoint f2) {

    //Verifica se um dos numeros é igual a zero
    if (f1.parts.exponent == 0 && f1.parts.mantissa == 0)
        return f2;

    if (f2.parts.exponent == 0 && f2.parts.mantissa == 0)
        return f1;

    FloatPoint result;
    FloatPoint aux;

    //Verifica qual possui maior expoente
    if (f1.parts.exponent <= f2.parts.exponent) {
        result = (FloatPoint) { .f = f1.f };
        aux = (FloatPoint) { .f = f2.f };
    } else {
        result = (FloatPoint) { .f = f2.f };
        aux = (FloatPoint) { .f = f1.f };
    }

    //Recupera diferença entre expoentes
    int diff = aux.parts.exponent - result.parts.exponent;

    //Realiza operação de shift a direita caso necessário
    if (diff > 0) {
        result.parts.mantissa >>= 1;
        //Move 1. invisivel para o bit mais significativo após o shift
        result.parts.mantissa |= (1u << 22);
        result.parts.mantissa >>= (diff - 1);
        result.parts.exponent += diff;
    }

    //Verifica se algum dos valores é negativo
    int sign = result.parts.sign == 1 ? -1 : 1;
    int auxSign = aux.parts.sign == 1 ? -1 : 1;

    //Reliza soma das mantissas
    unsigned int sumMantissa = (result.parts.mantissa * sign) + (aux.parts.mantissa * auxSign);

    printf("%d\n", result.parts.mantissa);

    printf("%d\n", aux.parts.mantissa);

    //Verifica qual deve ser o sinal do resultado final de acordo com o tamanho absoluto da mantissa
    if (result.parts.mantissa <= aux.parts.mantissa) {
        result.parts.sign = aux.parts.sign;
    }

    //Verifica se ocorre overflow
    if (sumMantissa > (1 << 23)) {
        //Caso ocorra realiza shift para direita
        result.parts.mantissa = sumMantissa >> 1;
        //Remove bit mais significativo por conta do overflow
        result.parts.mantissa &= ~(1u << 22);
        //Soma o expoente
        result.parts.exponent += 1;
    } else {
        result.parts.mantissa = sumMantissa;
    }

    return result;
}


int main() {
    FloatPoint fPoint1 = { .f = -3.0 };

    printFloatPoint(fPoint1);

    float originalFloat1 = convertToFloat(fPoint1);

    printf("Reconstructed float: %f\n", originalFloat1);

    FloatPoint fPoint2 = { .f = 13.36 };

    printFloatPoint(fPoint2);

    float originalFloat2 = convertToFloat(fPoint2);

    printf("Reconstructed float2: %f\n", originalFloat2);

    FloatPoint sum = sumFloatPoint(fPoint1, fPoint2);

    printFloatPoint(sum);
}
