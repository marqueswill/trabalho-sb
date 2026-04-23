#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>

/**
 * Lê um arquivo .pre e gera os arquivos .obj (código de máquina) e .pen (pendências).
 * Implementa a lógica de montagem e tradução de opcodes/diretivas.
 */
void runAssembler(const std::string& filename);

#endif