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
#### Compilação automática
Na pasta raíz do projeto, execute:
```mingw32-make all```

#### Execução de testes
```mingw32-make test```

#### Compilação e simulação automática:
Para arquivo exemplo default:
 ```mingw32-make run```
Para arquivo específico:
 ```mingw32-make run FILE="caminho_arquivo/nome_arquivo.asm"```

#### Compilação manual

Ou então, se deseja compilar manualmente, primeiro compile o arquivo main:

```
g++ -Wall -std=c++11 src/main.cpp src/preprocessor.cpp src/assembler.cpp src/simulator.cpp -o ./build/montador
```
E então execute o arquivo gerado:
```
./build/montador examples/ex1.asm
```




