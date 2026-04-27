
#include "auxiliar.h"

#include <map>
#include <string>
#include <vector>
using namespace std;

map<string, InstructionInfo> getInstructionTable() {
    return {
        {"ADD", {1, 2}},      // ACC <- ACC + mem(OP)
        {"SUB", {2, 2}},      // ACC <- ACC - mem(OP)
        {"MULT", {3, 2}},     // ACC <- ACC × mem(OP)
        {"DIV", {4, 2}},      // ACC <- ACC ÷ mem(OP)
        {"JMP", {5, 2}},      // PC <- OP
        {"JMPN", {6, 2}},     // Se ACC<0 então PC <- OP
        {"JMPP", {7, 2}},     // Se ACC>0 então PC <- OP
        {"JMPZ", {8, 2}},     // Se ACC=0 então PC <- OP
        {"COPY", {9, 3}},     // mem(OP2) <- mem(OP1)
        {"LOAD", {10, 2}},    // ACC <- mem(OP)
        {"STORE", {11, 2}},   // mem(OP) <- ACC
        {"INPUT", {12, 2}},   // em(OP) <- entrada
        {"OUTPUT", {13, 2}},  // saída <- mem(OP)
        {"STOP", {14, 1}}     // Suspende a execução
    };
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
    string outFilename = "outputs/" + baseName + extension;

    return outFilename;
};