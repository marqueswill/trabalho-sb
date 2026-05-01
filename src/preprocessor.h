#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <string>

/**
 * Lê um arquivo .asm e gera um arquivo .pre processado.
 * Implementa: remoção de comentários, case-insensitivity e reordenação de seções.
 */
void runPreprocessor(const std::string& filename, string inputFolder = "", string outputFolder = "");
#endif