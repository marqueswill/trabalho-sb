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
    string symbol = "";
    int address = -1;
    bool isDefined = false;
    vector<int> pendingReferences;
};

struct InstructionTokens {
    string label = "";
    string operation;
    string addr1 = "";
    string addr2 = "";
};

struct DataTokens {
    string label;
    string directive;
    int value = 0;  // CONST
};

using SymbolTable = map<string, SymbolInfo>;
using InstructionTable = map<string, InstructionInfo>;
using DirectiveTable = map<string, DirectiveInfo>;

InstructionTable getInstructionTable();
DirectiveTable getDirectiveTable();
SymbolTable getSymbolTable();
string getInstructionName(int opcode);

void addSymbol(SymbolTable& symbolTable, const string symbol, int address);  // Adiciona símbolo na tabela
void setSymbol(SymbolTable& symbolTable, const string symbol, int address);  // Define o enderço de um símbolo
bool isDefined(SymbolTable& symbolTable, const string symbol);
bool isDirective(DirectiveTable directiveTable, string operation);

InstructionTokens parseTextLine(const string&);
DataTokens parseDataLine(const string&);
constexpr int MAX_MEMORY = 65536;

#endif
