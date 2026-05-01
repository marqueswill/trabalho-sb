
#include "auxiliar.h"

#include <map>
#include <string>
#include <vector>
using namespace std;

map<string, InstructionInfo> getInstructionTable() {
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

map<string, DirectiveInfo> getDirectiveTable() {
    return {
        {"SPACE", {1}},    // Reserva 1 espaço de memória (inicializado com "00" no .obj)
        {"CONST", {1}},    // Reserva 1 espaço de memória (preenchido com o valor passado)
        {"SECTION", {0}},  // Não vai para a memória, serve apenas para organizar o código
        {"EQU", {0}},      // Resolvido no pré-processador, não ocupa espaço na montagem
        {"IF", {0}}        // Resolvido no pré-processador, não ocupa espaço na montagem
    };
}

string getOutFileName(const string& filename, const string& extension) {
    size_t lastSlashPos = filename.find_last_of("/\\");
    size_t startPos = (lastSlashPos == string::npos) ? 0 : lastSlashPos + 1;

    // Pega o nome do arquivo ignorando os 4 últimos caracteres (ex: ".asm")
    string baseName = filename.substr(startPos, filename.length() - startPos - 4);

    // Constrói o novo caminho forçando a pasta "outputs/"
    string outFilename = "tests/outputs/" + baseName + extension;

    return outFilename;
};