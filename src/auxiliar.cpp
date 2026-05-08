
#include "auxiliar.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

InstructionTable getInstructionTable() {
    return {
        {"ADD", {1, 2}},      // ACC <- ACC + mem(ADDR)
        {"SUB", {2, 2}},      // ACC <- ACC - mem(ADDR)
        {"MULT", {3, 2}},     // ACC <- ACC × mem(ADDR)
        {"DIV", {4, 2}},      // ACC <- ACC ÷ mem(ADDR)
        {"JMP", {5, 2}},      // PC <- ADDR
        {"JMPN", {6, 2}},     // Se ACC<0 então PC <- ADDR
        {"JMPP", {7, 2}},     // Se ACC>0 então PC <- ADDR
        {"JMPZ", {8, 2}},     // Se ACC=0 então PC <- ADDR
        {"COPY", {9, 3}},     // mem(ADDR2) <- mem(ADDR1)
        {"LOAD", {10, 2}},    // ACC <- mem(ADDR)
        {"STORE", {11, 2}},   // mem(ADDR) <- ACC
        {"INPUT", {12, 2}},   // em(ADDR) <- entrada
        {"OUTPUT", {13, 2}},  // saída <- mem(ADDR)
        {"STOP", {14, 1}}     // Suspende a execução
    };
}

DirectiveTable getDirectiveTable() {
    return {
        {"SPACE", {1}},    // Reserva 1 espaço de memória (inicializado com "00" no .obj)
        {"CONST", {1}},    // Reserva 1 espaço de memória (preenchido com o valor passado)
        {"SECTION", {0}},  // Não vai para a memória, serve apenas para organizar o código
        {"EQU", {0}},      // Resolvido no pré-processador, não ocupa espaço na montagem
        {"IF", {0}}        // Resolvido no pré-processador, não ocupa espaço na montagem
    };
}

SymbolTable getSymbolTable() {
    map<string, SymbolInfo> symbolTable;
    return symbolTable;
}

string getInstructionName(int opcode) {
    static const string names[] = {
        "", "ADD", "SUB", "MULT", "DIV", "JMP", "JMPN",
        "JMPP", "JMPZ", "COPY", "LOAD", "STORE", "INPUT",
        "OUTPUT", "STOP"};

    if (opcode >= 1 && opcode <= 14) {
        return names[opcode];
    }

    return "UNKNOWN";
}

/**
 * Define o endereço real de um símbolo na SymbolTable.
 * É chamada quando uma label é encontrada no código (definição do símbolo).
 */
void setSymbol(SymbolTable& symbolTable, string symbol, int address) {
    // Erro redefinição de símbolo

    if (symbolTable[symbol].isDefined) {
        cerr << "Erro: Redefinição de símbolo." << endl;
        return;
    }
    cout << "[DEBUG] Definindo símbolo: " << symbol << " no endereço: " << address << endl;
    // Atualiza o endereço para o valor atual do locationCounter e marca como definido
    symbolTable[symbol].address = address;
    symbolTable[symbol].isDefined = true;
};

/**
 * Adiciona um símbolo à tabela ou regista uma nova referência pendente.
 */
void addSymbol(SymbolTable& symbolTable, string symbol, int address) {
    auto info = symbolTable.find(symbol);

    // O símbolo já existe na tabela (foi definido ou usado antes)
    if (info != symbolTable.end()) {
        if (!info->second.isDefined) {  // Símbolo existe, mas não foi definido -> Adiciono pendência
            cout << "[DEBUG] Símbolo '" << symbol << "' já possui pendências. " << "Adicionando nova referência no endereço: " << address << endl;
            info->second.pendingReferences.push_back(address);
        }
    }

    // Primeira vez que o símbolo aparece no código
    else {
        cout << "[DEBUG] Primeira aparição de '" << symbol << "'. " << "Criando entrada com pendência no endereço: " << address << endl;
        symbolTable[symbol].address = 0;
        symbolTable[symbol].isDefined = false;  // Definição ocorre só quando encontra label
        symbolTable[symbol].pendingReferences.push_back(address);
    }
}

bool isDefined(SymbolTable& symbolTable, string symbol) {
    return symbolTable[symbol].isDefined;
}

bool isDirective(DirectiveTable directiveTable, string operation) {
    return directiveTable.count(operation);
}

vector<string> splitBySpace(const string& line) {
    vector<string> substrings;
    istringstream stream(line);
    string crrntWord;

    while (stream >> crrntWord) {
        substrings.push_back(crrntWord);
    }

    return substrings;
}

InstructionTokens splitTextLine(const string& line) {
    InstructionTokens tokens = {"", "", "", ""};
    vector<string> splitLine = splitBySpace(line);

    if (splitLine.empty()) return tokens;

    int currentIdx = 0;
    // Identifica Label
    if (splitLine[0].back() == ':') {
        tokens.label = splitLine[0].substr(0, splitLine[0].size() - 1);
        currentIdx++;
    }

    // Identifica Operação
    if (currentIdx < splitLine.size()) {
        tokens.operation = splitLine[currentIdx++];
    }

    // Identifica Operandos
    if (currentIdx < splitLine.size()) {
        string operandos = splitLine[currentIdx];

        // Se for COPY, tenta separar por vírgula
        size_t commaPos = operandos.find(',');
        if (tokens.operation == "COPY" && commaPos != string::npos) {
            tokens.addr1 = operandos.substr(0, commaPos);
            tokens.addr2 = operandos.substr(commaPos + 1);
        } else {
            // Caso padrão (outras instruções ou COPY mal formatado)
            tokens.addr1 = operandos;
            if (currentIdx + 1 < splitLine.size()) {
                tokens.addr2 = splitLine[currentIdx + 1];
            }
        }
    }

    return tokens;
}

DataTokens splitDataLine(const string& line) {
    DataTokens tokens = {"", "", 0};
    vector<string> splitLine = splitBySpace(line);

    if (splitLine.empty()) return tokens;

    if (!splitLine[0].empty() && splitLine[0].back() == ':') {
        tokens.label = splitLine[0].substr(0, splitLine[0].size() - 1);
    } else {
        tokens.label = splitLine[0];
    }

    if (splitLine.size() > 1) {
        tokens.directive = splitLine[1];
    }

    if (splitLine.size() > 2) {
        try {
            tokens.value = stoi(splitLine[2]);
        } catch (...) {
            tokens.value = 0;
        }
    } else {
        tokens.value = 0;
    }

    return tokens;
}