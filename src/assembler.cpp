#include "assembler.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "auxiliar.h"

using namespace std;

void resolvePendencies(vector<int> buffer, SymbolTable& symbolTable, const string& symbol) {
    if (symbolTable.find(symbol) == symbolTable.end()) {
        // Símbolo não definido
        cerr << "Erro: Não foi possível definir o símbolo: " << symbol << "." << endl;
        return;
    }

    SymbolInfo info = symbolTable[symbol];
    if (info.isDefined) {
        // Percorrer lista de pendência para o símbolo
        for (int i = 0; i < info.pendingReferences.size(); i++) {
            // Acessar o buffer e atualiza com valor definido
            int indexToUpdate = info.pendingReferences[i];
            buffer[indexToUpdate] = info.address;
        }

        info.pendingReferences.clear();
    }
}

void runAssembler(const string& filename, string inputFolder, string outputFolder) {
    // Abrir arquivo
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

    // Variáveis de controle
    int locationCounter = 0;
    bool hasStop = false;
    bool hasTextSection = false;
    bool hasDataSection = false;
    bool inTextSection = false;
    bool inDataSection = false;

    vector<int> buffer;
    string line;

    // Assembler
    while (getline(inputFile, line)) {
        if (line == "SECTION TEXT") {
            hasTextSection = true;
            inTextSection = true;
            inDataSection = false;
            continue;
        }

        if (line == "SECTION DATA") {
            hasDataSection = true;
            inTextSection = false;
            inDataSection = true;
            continue;
        }

        if (inTextSection) {
            InstructionTokens tokens = splitTextLine(line);  // Assumo que não há erro sintáticos ou léxicos

            if (!tokens.label.empty()) {                                // Se encontrar uma label
                setSymbol(symbolTable, tokens.label, locationCounter);  // Define a label
                resolvePendencies(buffer, symbolTable, tokens.label);   // Resolve as dependências
            }

            InstructionInfo instInfo = instructionTable[tokens.operation];
            buffer[locationCounter] = instInfo.opcode;  // Escreve opcode
            switch (instInfo.opcode) {
                case 9:  // COPY -> Adiciona dois símbolos e escreve o endereço no buffer
                    addSymbol(symbolTable, tokens.addr1, locationCounter + 1);
                    addSymbol(symbolTable, tokens.addr2, locationCounter + 2);
                    buffer[locationCounter + 1] = symbolTable[tokens.addr1].address;  // Se for pendência, será 0
                    buffer[locationCounter + 2] = symbolTable[tokens.addr2].address;  // Se for pendência, será 0
                    break;
                case 14:  // STOP -> Não adiciona símbolos
                    hasStop = true;
                    break;
                default:  // Adiciona um símbolo e escreve o endereço no buffer
                    addSymbol(symbolTable, tokens.addr1, locationCounter + 1);
                    buffer[locationCounter + 1] = symbolTable[tokens.addr1].address;  // Se for pendência, será 0
                    break;
            }

            locationCounter += instInfo.size;
        }

        // DATA SECTION -> assume que o pré processador colocou o DATA por último
        if (inDataSection) {
            DataTokens tokens = splitDataLine(line);

            DirectiveInfo dirInfo = directiveTable[tokens.label];
            string directive = tokens.directive;
            if (directive == "SPACE") {
                setSymbol(symbolTable, tokens.label, locationCounter);  // Salva na tabela
                buffer.push_back(0);                                    // Escreve endereço
            } else if (directive == "CONST") {
                setSymbol(symbolTable, tokens.label, locationCounter);  // Salva na tabela
                buffer.push_back(tokens.value);                         // Escreve endereço
            }

            // Atualiza o buffer substituindo valor definido
            resolvePendencies(buffer, symbolTable, tokens.label);
            locationCounter += dirInfo.size;
        }

        break;
    }

    if (!hasTextSection) {
        cerr << "Erro: A seção TEXT ausente." << endl;
        return;
    }

    if (!hasStop) {
        cerr << "Erro: A seção TEXT não possui a instrução STOP." << endl;
        return;
    }

    // TODO: Lógica symbol not defined -> Se depois de percorrer todas linhas
    // existir na tabela de simbolo alguem que não foi definido

    // TODO: Escrever o codigo de maquina final em uma unica linha no .obj e no .pen

    cout << "   Montagem concluida. arquivos gerados: " << objFilename << " e " << penFilename << endl;

    inputFile.close();
    objFile.close();
    penFile.close();
}