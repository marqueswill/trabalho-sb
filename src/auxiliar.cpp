
#include "auxiliar.h"

#include <iostream>
#include <map>
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
    // Array estático: definido apenas uma vez na memória
    static const string names[] = {
        "", "ADD", "SUB", "MULT", "DIV", "JMP", "JMPN",
        "JMPP", "JMPZ", "COPY", "LOAD", "STORE", "INPUT",
        "OUTPUT", "STOP"};

    if (opcode >= 1 && opcode <= 14) {
        return names[opcode];
    }
    return "UNKNOWN";
}

void addSymbol(SymbolTable& symbolTable, string symbol, int address) {
    // Se símbolo existe
    //// Se ele foi definido -> Faço nada
    //// Se não foi definido -> Adiciono pendência

    // Se símbolo não existe
    //// Se não foi definido -> Adiciono pendência
}

void setSymbol(SymbolTable& symbolTable, string symbol, int address) {
    // Erro redefinição de símbolo
};

bool isDefined(SymbolTable& symbolTable, string symbol) {
    return symbolTable[symbol].isDefined;
}

bool isDirective(DirectiveTable directiveTable, string operation) {
    return directiveTable.count(operation);
}

InstructionTokens parseTextLine(const string& line) {
    InstructionTokens tokens;
    // ... seu código de quebra de linha ...
    return tokens;
}

DataTokens parseDataLine(const string& line) {
    DataTokens tokens;
    // ... seu código de quebra de linha ...
    return tokens;
}