#include "preprocessor.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "auxiliar.h"



using namespace std;

void runPreprocessor(const string& filename) {
    // 1. Abrir o arquivo .asm de entrada
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Erro: Nao foi possivel abrir o arquivo " << filename << endl;
        return;
    }

    // 2. Preparar o arquivo de saida (.pre)
    string outFilename = getOutFileName(filename, ".pre");
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

        // TODO: Análise léxica (lexer/tokenization): Toda linha vira um lista de tokens
        // ex: "L1:   ADD  DOIS  ; soma o valor" -> ["L1:", "ADD", "DOIS"]

        // TODO: Anáise sintática (parser):
        // Se len == 1:
        //   -> Só pode ser [opcode_sem_operando]. Ex: "STOP"
        // Se len == 2:
        //   Pode ser 4 coisas diferentes. Você olha para o PRIMEIRO token para descobrir:
        //   -> Se for LABEL:
        //        - O segundo é uma diretiva s/ valor? Ex: "OLD_DATA: SPACE"
        //        - O segundo é um opcode s/ operando? Ex: "L1: STOP"
        //   -> Se for diretiva "SECTION":
        //        - O segundo é o nome da seção. Ex: "SECTION TEXT"
        //   -> Se for um OPCODE:
        //        - O segundo é o simbolo/operando. Ex: "ADD DOIS" ou "COPY A,B"
        // Se len == 3:
        //   O primeiro TEM que ser uma LABEL. Você olha o SEGUNDO token:
        //   -> Se for diretiva CONST:
        //        - O terceiro é o valor. Ex: "DOIS: CONST 2"
        //   -> Se for Opcode:
        //        - O terceiro é o simbolo/endereço. Ex: "L1: ADD DOIS" ou "L1: COPY A,B"
        //        - Se o Opcode for copy, o símbolo é composto por dois simbolos separados por ','

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