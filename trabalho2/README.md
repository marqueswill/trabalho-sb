# Trabalho 2 - Software Básico

## Integrantes
1. Gustavo Mena Barreto - 232026414
2. Willyan Marques de Melo - 221020940

## Requisitos 
- Sistema Operacional: Linux
- Compilador: Nasm
- Ferramenta de automação: Make

___

## Gerando e Executando o Projeto

### Método Automático (Recomendado)
1. Compilar e executar:
``` bash
    make run
```

2. Apenas compilar
A pasta `/build` receberá os arquivos objetos e a pasta `/bin` receberá o executável

``` bash
    make
```

``` bash
    ./bin/calculadora
```

3. Limpar arquivos objetos anteriores:

``` bash
    make clean
```

### Método Manual

Caso não possua o make instalado, utilize as instruções abaixo.

#### Comando Unificado
Execute a linha de comando abaixo para compilar todos os módulos, linkar e iniciar o programa automaticamente:

TODO:
``` bash
```

#### Passo a passo

1. Compilação dos módulos:

``` bash
    nasm -f elf32 src/add.asm -o src/add.o
    nasm -f elf32 src/calculadora.asm -o src/calculadora.o
    nasm -f elf32 src/div.asm -o src/div.o
    nasm -f elf32 src/exp.asm -o src/exp.o
    nasm -f elf32 src/mod.asm -o src/mod.o
    nasm -f elf32 src/mult.asm -o src/mult.o
    nasm -f elf32 src/sub.asm -o src/sub.o
```

2. Linkagem dos arquivos objeto:

``` bash
    ld -m elf_i386 src/*.o -o calculadora
```

3. Execução:


``` bash
./calculadora
```