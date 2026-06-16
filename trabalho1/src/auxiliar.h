#ifndef AUXILIAR_H
#define AUXILIAR_H
#include <map>
#include <string>
#include <vector>

using namespace std;
struct InstructionInfo {
    int opcode;
    int size;
};
struct DirectiveInfo {
    int size;
};
struct SymbolInfo {
    int address;
    bool isDefined = false;
    // vector<int> pendingReferences = {};
    int listHead = -1;
};

struct InstructionTokens {
    string label;
    string operation;
    string addr1;
    string addr2;
};

struct DataTokens {
    string label;
    string directive;
    int value;
};

using SymbolTable = map<string, SymbolInfo>;
using InstructionTable = map<string, InstructionInfo>;
using DirectiveTable = map<string, DirectiveInfo>;

// Tabelas
InstructionTable getInstructionTable();
DirectiveTable getDirectiveTable();
SymbolTable getSymbolTable();

// Tokenização
InstructionTokens splitTextLine(const string&);
DataTokens splitDataLine(const string&);

string getInstructionName(int opcode);
int addSymbol(SymbolTable& symbolTable, const string symbol, int address);   // Adiciona símbolo na tabela
void setSymbol(SymbolTable& symbolTable, const string symbol, int address);  // Define o enderço de um símbolo
bool isDefined(SymbolTable& symbolTable, const string symbol);
bool isDirective(DirectiveTable directiveTable, string operation);
void resolveDependencies(vector<int>& buffer, SymbolTable& symbolTable, const string& symbol);
constexpr int MAX_MEMORY = 65536;

#endif
