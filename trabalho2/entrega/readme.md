# Trabalho 2 - Software Básico

## Integrantes

1. Gustavo Mena Barreto - 232026414
2. Willyan Marques de Melo - 221020940

## Requisitos

- Sistema Operacional: Linux
- Compilador: Nasm
- Ferramenta de automação: Make (opcional)

---

## Gerando e Executando o Projeto

```
for f in *.asm; do nasm -f elf32 "$f"; done && ld -m elf_i386 -o calculadora *.o
```

```
./calculadora
```
