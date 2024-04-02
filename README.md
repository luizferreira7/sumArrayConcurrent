# sumArrayConcurrent

Atividade 2 do Laboratorio 2 da disciplina de programação concorrente.

Objetivo: Projetar, implementar e avaliar uma versão concorrente para o problema de somar todos os elementos de um vetor de números reais.

Roteiro:

Roteiro:
1. Implemente um programa auxiliar para gerar os vetores de entrada (quantidade de elementos e elementos do vetor) para os testes da aplicação e o resultado esperado (soma de todos os elementos do vetor).
2. Implemente o programa concorrente para somar os elementos de um vetor de floats, considerando que a entrada será lida dos arquivos gerados.
3. Verifique se o resultado gerado está correto. Lembre-se de repetir a execução do programa várias vezes, para a mesma entrada de dados.

Requisistos de implementação: 
1. A quantidade de threads deverá ser informada pelo usuário na linha de comando (argumento para o programa). 
2. Todas as threads deverão executar a mesma função.

## Instruções do Programa

Rode os comandos a seguir para compilar as classes necessárias

``` bash
gcc generateArray.c -o generateArray
```

``` bash
gcc lab2.c -o lab2
```

1. Para cria o vetor e armazenar em um arquivo utilize o seguinte comando:

``` bash
./generateArray <numero_de_elementos> <nome_do_arquivo>
```

Exemplo:
``` bash
./generateArray 10000 array.txt
```


2. Após gerar o arquivo rode o seguinte comenado para executar o programa:
``` bash
./lab2 <numero_de_threads> < <nome_do_arquivo> 
```

Exemplo:
``` bash
./lab2 5 < array.txt
```
