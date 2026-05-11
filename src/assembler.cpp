#include "assembler.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "auxiliar.h"

using namespace std;

void runAssembler(const string& filename, string inputFolder, string outputFolder) {
    ////////////////////////////////////
    // ENTRADA

    // Abrir arquivo
    string preFilePath = inputFolder + filename + ".pre";
    ifstream inputFile(preFilePath);
    if (!inputFile.is_open()) {
        cerr << "Erro: Nao foi possivel abrir o arquivo " << preFilePath << endl;
        return;
    }

    ////////////////////////////////////
    // ASSEMBLER

    // Preparar tabelas
    InstructionTable instructionTable = getInstructionTable();
    DirectiveTable directiveTable = getDirectiveTable();
    SymbolTable symbolTable = getSymbolTable();

    // cout << "\n[DEBUG] Iniciando Montagem de: " << filename << ".pre" << endl;

    // Variáveis de controle
    int locationCounter = 0;
    bool hasStop = false;
    bool hasTextSection = false;
    bool hasDataSection = false;
    bool inTextSection = false;
    bool inDataSection = false;

    vector<int> buffer;
    vector<int> penBuffer;

    string line;

    // Percorre arquivo
    while (getline(inputFile, line)) {
        if (line == "SECTION TEXT") {
            // cout << "\n\n[DEBUG] Entrou na SECTION TEXT" << endl;
            hasTextSection = true;
            inTextSection = true;
            inDataSection = false;
            continue;
        }

        if (line == "SECTION DATA") {
            // cout << "\n\n[DEBUG] Entrou na SECTION DATA" << endl;
            hasDataSection = true;
            inTextSection = false;
            inDataSection = true;
            continue;
        }

        if (inTextSection) {
            InstructionTokens tokens = splitTextLine(line);  // Assumo que não há erro sintáticos ou léxicos
            // cout << "\n[DEBUG] L" << locationCounter << " | " << line << endl;

            if (!tokens.label.empty()) {                                 // Se encontrar uma label
                setSymbol(symbolTable, tokens.label, locationCounter);   // Define a label
                resolveDependencies(buffer, symbolTable, tokens.label);  // Resolve as dependências
            }

            InstructionInfo instInfo = instructionTable[tokens.operation];
            buffer.push_back(instInfo.opcode);  // Escreve opcode
            penBuffer.push_back(instInfo.opcode);

            // cout << "        Instrução: " << tokens.operation << " (Opcode: " << instInfo.opcode << ")" << endl;
            switch (instInfo.opcode) {
                case 9: {  // COPY -> Adiciona dois símbolos e escreve o endereço no buffer

                    // cout << "        Operandos: " << tokens.addr1 << ", " << tokens.addr2 << endl;
                    int val1 = addSymbol(symbolTable, tokens.addr1, locationCounter + 1);
                    int val2 = addSymbol(symbolTable, tokens.addr2, locationCounter + 2);
                    buffer.push_back(val1);     // Escreve endereço
                    buffer.push_back(val2);     // Escreve endereço
                    penBuffer.push_back(val1);  // Escreve endereço
                    penBuffer.push_back(val2);  // Escreve endereço
                    break;
                }
                case 14: {  // STOP -> Não adiciona símbolos
                    hasStop = true;
                    break;
                }
                // Adiciona um símbolo e escreve o endereço no buffer
                default: {
                    // cout << "        Operando: " << tokens.addr1 << endl;
                    int val1 = addSymbol(symbolTable, tokens.addr1, locationCounter + 1);
                    buffer.push_back(val1);  // Escreve endereço
                    penBuffer.push_back(val1);
                    break;
                }
            }

            locationCounter += instInfo.size;
            continue;
        }

        // DATA SECTION -> assume que o pré processador colocou o DATA por último
        if (inDataSection) {
            DataTokens tokens = splitDataLine(line);
            string directive = tokens.directive;
            DirectiveInfo dirInfo = directiveTable[directive];
            // cout << "\n[DEBUG] L" << locationCounter << " | " << tokens.label << " " << tokens.directive << endl;
            if (directive == "SPACE") {
                setSymbol(symbolTable, tokens.label, locationCounter);  // Define na tabela
                buffer.push_back(0);                                    // Escreve endereço no buffer
                penBuffer.push_back(0);
            } else if (directive == "CONST") {
                setSymbol(symbolTable, tokens.label, locationCounter);  // Define na tabela
                buffer.push_back(tokens.value);                         // Escreve endereço no buffer
                penBuffer.push_back(tokens.value);
                // cout << "        Valor CONST: " << tokens.value << endl;
            }

            // Atualiza o buffer em retrocesso usando valor definido
            resolveDependencies(buffer, symbolTable, tokens.label);
            locationCounter += dirInfo.size;
            continue;
        }

        break;
    }

    ////////////////////////////////////
    // VERIFICAÇÕES
    if (!hasTextSection) {
        cerr << "Erro: A seção TEXT ausente." << endl;
        return;
    }

    if (!hasStop) {
        cerr << "Erro: A seção TEXT não possui a instrução STOP." << endl;
        return;
    }
    inputFile.close();

    // Lógica symbol not defined
    for (auto const& pair : symbolTable) {
        // pair.first é a chave (nome do símbolo), pair.second é o SymbolInfo
        if (!pair.second.isDefined) {
            cerr << "Erro Semântico: O símbolo '" << pair.first << "' foi usado, mas nunca foi definido." << endl;
            return;
        }
    }

    ////////////////////////////////////
    // ESCRITA

    // Preparar os arquivos de saida (.obj e .pen)
    string objFilename = outputFolder + filename + ".obj";
    string penFilename = outputFolder + filename + ".pen";

    ofstream objFile(objFilename);
    ofstream penFile(penFilename);

    // Escrever o codigo de maquina final em uma unica linha no .obj e no .pen
    for (size_t i = 0; i < buffer.size(); i++) {
        objFile << buffer[i];
        penFile << penBuffer[i];

        // Coloca um espaço depois do número, exceto se for o último
        if (i != buffer.size() - 1) {
            objFile << " ";
            penFile << " ";
        }
    }
    objFile.close();
    penFile.close();

    // cout << "   Montagem concluida. arquivos gerados: " << objFilename << " e " << penFilename << endl;
}