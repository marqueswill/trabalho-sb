# trabalho-sb

## Integrantes
1. Gustavo Mena Barreto - 232026414
2. Willyan Marques de Melo - 221020940

## Requisitos 
- Sistema Operacional: Linux
- Compilador: GNU Compiler Collection (GCC)
- Ferramenta de automação: Make

## Como executar o montador

---
### Terminal

#### Compilação automática
Na pasta raiz do projeto, execute:
`make all`

#### Execução de testes
`make test`

#### Compilação e simulação automática:
Para o arquivo exemplo padrão:
`make run`

Para um arquivo específico (informe o caminho sem a extensão):
`make run FILE="caminho_arquivo/nome_arquivo"`

#### Compilação manual
Se deseja compilar manualmente, primeiro compile os arquivos:

```bash
g++ -Wall -std=c++11 src/main.cpp src/preprocessor.cpp src/assembler.cpp src/simulator.cpp src/auxiliar.cpp -o ./montador