# trabalho-sb
TODO: introdução do trabalho

## Integrantes
1. Gustavo Mena Barreto - 232026414
2. Willyan Marques de Melo - 221020940


## Requisitos 
- Sistema Operacional: Windows
- Compilador: GNU Compiler Collection (GCC)

## Como executar o montador

---
### CodeBlocks

---
### Powershell
#### Execução automática
Na pasta raíz do projeto, execute:
```mingw32-make test```

#### Execução manual

Ou então, se deseja compilar manualmente, primeiro compile o arquivo main:

```
g++ -Wall -std=c++11 src/main.cpp src/preprocessor.cpp src/assembler.cpp src/simulator.cpp -o ./build/montador
```
E então execute o arquivo gerado:
```
./build/montador examples/ex1.asm
```
---


## Como executar os testes
### Powershell

#### Execução automática
Na pasta raíz do projeto, execute:
```mingw32-make test```

#### Execução manual

Ou então, se deseja compilar manualmente, primeiro compile o arquivo de testes:
```
g++ -Wall -std=c++11 tests/test.cpp src/preprocessor.cpp src/assembler.cpp src/simulator.cpp -I./src -o ./build/run_tests
```
E então execute o arquivo gerado:

```
./build/run_tests

```




