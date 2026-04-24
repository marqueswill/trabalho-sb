#include <map>
#include <string>
#include <vector>

using namespace std;

// 1. TABELA DE INSTRUÇÕES
struct InstructionInfo {
    int opcode;
    int size;
};

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

// 2. TABELA DE DIRETIVAS
struct DirectiveInfo {
    int size;
};

map<string, DirectiveInfo> getDirectiveTable() {
    return {
        {"SPACE", {1}},    // Reserva 1 espaço de memória (inicializado com "00" no .obj)
        {"CONST", {1}},    // Reserva 1 espaço de memória (preenchido com o valor passado)
        {"SECTION", {0}},  // Não vai para a memória, serve apenas para organizar o código
        {"EQU", {0}},      // Resolvido no pré-processador, não ocupa espaço na montagem
        {"IF", {0}}        // Resolvido no pré-processador, não ocupa espaço na montagem
    };
}


// 3. TABELA DE SÍMBOLOS
struct SymbolInfo {
    int address = -1;        // Endereço do símbolo (se já foi definido)
    bool isDefined = false;  // Indica se o rótulo já foi encontrado

    // Lista encadeada (aqui, um vetor) de endereços onde o símbolo foi chamado
    // antes de ser definido. Estes são os valores que irão para o arquivo .pen!
    vector<int> pendingReferences;
};


// 4. CONSTANTES DO SISTEMA
// Define o tamanho máximo da memória do simulador.
// Memória de 16-bits ~ 64K palavras
constexpr int MAX_MEMORY = 65536;