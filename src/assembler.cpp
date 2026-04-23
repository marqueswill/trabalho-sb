#include "assembler.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

// Estrutura basica para a Tabela de Simbolos
struct SymbolInfo {
    int address = -1;
    bool isDefined = false;
    vector<int> pendingReferences; // Lista de pendencias para passagem unica
};

void runAssembler(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Erro: Nao foi possivel abrir o ficheiro " << filename << endl;
        return;
    }

    // Preparar os ficheiros de saida (.obj e .pen)
    string baseFilename = filename.substr(0, filename.find_last_of('.'));
    ofstream objFile(baseFilename + ".obj");
    ofstream penFile(baseFilename + ".pen");

    map<string, SymbolInfo> symbolTable; // Tabela de símbolos
    int locationCounter = 0;
    string line;

    // TODO: Implementar Tabela de Instrucoes (Opcodes e tamanhos)
    // TODO: Implementar Tabela de Diretivas (SPACE, CONST)

    while (getline(inputFile, line)) {
        // TODO: Analisar a linha (separar rotulo, instrucao, operandos)
        // TODO: Atualizar tabela de simbolos
        // TODO: Gerar codigo de maquina parcial
        // TODO: Guardar as pendencias no ficheiro .pen conforme as regras
    }

    // TODO: Escrever o codigo de maquina final em uma unica linha no .obj

    cout << "Montagem concluida. Ficheiros gerados: " << baseFilename << ".obj e " << baseFilename << ".pen" << endl;

    inputFile.close();
    objFile.close();
    penFile.close();
}