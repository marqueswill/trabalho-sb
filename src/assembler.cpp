#include "assembler.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "auxiliar.h"

using namespace std;

void updateBuffer(vector<int> buffer,
                  SymbolTable& symbolTable,
                  InstructionInfo instInfo,
                  int locationCounter) {

};

void resolvePendencies(vector<int> buffer, SymbolTable& symbolTable, string& symbol) {}

void runAssembler(const string& filename,
                  string inputFolder,
                  string outputFolder) {
    string preFilePath = inputFolder + filename + ".pre";
    ifstream inputFile(preFilePath);
    if (!inputFile.is_open()) {
        cerr << "Erro: Nao foi possivel abrir o arquivo " << preFilePath << endl;
        return;
    }

    // Preparar os arquivos de saida (.obj e .pen)
    string objFilename = outputFolder + filename + ".obj";
    string penFilename = outputFolder + filename + ".pen";

    ofstream objFile(objFilename);
    ofstream penFile(penFilename);

    // Preparar tabelas
    InstructionTable instructionTable = getInstructionTable();
    DirectiveTable directiveTable = getDirectiveTable();
    SymbolTable symbolTable = getSymbolTable();

    int locationCounter = 0;
    bool dataSection = false;
    vector<int> buffer;
    string line;

    // TEXT SECTION
    while (getline(inputFile, line)) {
        if (line == "DATA SECTION") break;

        InstructionTokens tokens = parseTextLine(line);  // Assumo que não há erro sintáticos ou léxicos

        if (!tokens.label.empty()) {
            setSymbol(symbolTable, tokens.label, locationCounter);
        }

        InstructionInfo instInfo = instructionTable[tokens.operation];
        switch (instInfo.opcode) {
            case 9:  // COPY -> Adiciona dois símbolos
                addSymbol(symbolTable, tokens.addr1, locationCounter);
                addSymbol(symbolTable, tokens.addr2, locationCounter + 1);
                break;
            case 14:  // STOP -> Não adiciona símbolos
                break;
            default:  // Adiciona um símbolo
                addSymbol(symbolTable, tokens.addr1, locationCounter);
                break;
        }

        updateBuffer(buffer, symbolTable, instInfo, locationCounter);

        locationCounter += instInfo.size;
    }

    // DATA SECTION -> assume que o pré processador colocou o DATA por último
    while (getline(inputFile, line)) {
        DataTokens tokens = parseDataLine(line);

        string directive = tokens.directive;
        if (directive == "SPACE") {
            setSymbol(symbolTable, tokens.label, 0);
        } else if (directive == "CONST") {
            setSymbol(symbolTable, tokens.label, tokens.value);
        }

        // Atualiza o buffer substituindo valor definido
        // TODO?: passar para dentro do SET
        resolvePendencies(buffer, symbolTable, tokens.label);
        locationCounter++;
    }

    // TODO: Lógica symbol not defined -> Se depois de percorrer todas linhas
    // existir na tabela de simbolo alguem que não foi definido

    // TODO: Escrever o codigo de maquina final em uma unica linha no .obj e no .pen

    cout << "   Montagem concluida. arquivos gerados: " << objFilename << " e " << penFilename << endl;

    inputFile.close();
    objFile.close();
    penFile.close();
}