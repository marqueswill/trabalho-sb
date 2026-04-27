#include "assembler.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "auxiliar.h"

using namespace std;

void runAssembler(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Erro: Nao foi possivel abrir o arquivo " << filename << endl;
        return;
    }

    // Preparar os arquivos de saida (.obj e .pen)
    string baseFilename = filename.substr(0, filename.find_last_of('.'));
    string objFilename =getOutFileName(filename, ".obj");
    string penFilename = getOutFileName(filename, ".pen");

    ofstream objFile(objFilename);
    ofstream penFile(penFilename);

    map<string, SymbolInfo> symbolTable;  // Tabela de símbolos
    int locationCounter = 0;
    string line;

    // TODO: Implementar Tabela de Instrucoes (Opcodes e tamanhos)
    // TODO: Implementar Tabela de Diretivas (SPACE, CONST)

    while (getline(inputFile, line)) {
        // TODO: Analisar a linha (separar rotulo, instrucao, operandos)
        // TODO: Atualizar tabela de simbolos. Se necessário, atualizar a lista de pendências
        // TODO: Gerar codigo de maquina parcial.
        // TODO: Atualizar lista dependência no buffer do .pen e no buffer do .obj ->
        // Substituir lista de pendência pela definição só no segundo buffer
    }
    // TODO: Lógica symbol not defined

    // TODO: Escrever o codigo de maquina final em uma unica linha no .obj e no .pen

    cout << "Montagem concluida. arquivos gerados: " << baseFilename << ".obj e " << baseFilename << ".pen" << endl;

    inputFile.close();
    objFile.close();
    penFile.close();
}