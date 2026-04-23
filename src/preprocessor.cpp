#include "preprocessor.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

void runPreprocessor(const string& filename) {
    // 1. Abrir o arquivo .asm de entrada
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Erro: Nao foi possivel abrir o arquivo " << filename << endl;
        return;
    }

    // 2. Preparar o nome do arquivo de saida (.pre)
    string outFilename = filename;
    size_t dotPos = outFilename.find_last_of('.');
    if (dotPos != string::npos) {
        outFilename = outFilename.substr(0, dotPos) + ".pre";
    }

    ofstream outputFile(outFilename);
    if (!outputFile.is_open()) {
        cerr << "Erro: Nao foi possivel criar o arquivo " << outFilename << endl;
        inputFile.close();
        return;
    }

    string line;
    vector<string> sectionText;
    vector<string> sectionData;
    bool inDataSection = false;

    // 3. Ler o arquivo linha a linha
    while (getline(inputFile, line)) {
        // TODO: Implementar a logica de limpeza:
        // - Converter tudo para maiusculas
        // - Remover comentarios (tudo apos ';')
        // - Remover espacos e tabulacoes redundantes
        // - Lidar com rotulos isolados (rotulo seguido de ENTER)
        
        // TODO: Separar as linhas entre SECTION TEXT e SECTION DATA
        // para garantir que a SECTION TEXT seja escrita primeiro.
    }

    // 4. Escrever as secoes na ordem correta no arquivo .pre
    // TODO: Escrever sectionText
    // TODO: Escrever sectionData

    cout << "Pre-processamento concluido. arquivo gerado: " << outFilename << endl;

    inputFile.close();
    outputFile.close();
}