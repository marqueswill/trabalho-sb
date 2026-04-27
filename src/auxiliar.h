#ifndef AUXILIAR_H
#define AUXILIAR_H
#include <map>
#include <string>
#include <vector>
struct InstructionInfo {
    int opcode;
    int size;
};
struct DirectiveInfo {
    int size;
};
struct SymbolInfo {
    int address = -1;
    bool isDefined = false;
    std::vector<int> pendingReferences;
};

std::map<std::string, InstructionInfo> getInstructionTable();
std::map<std::string, DirectiveInfo> getDirectiveTable();
std::string getOutFileName(const std::string& filename, const std::string& extension);

constexpr int MAX_MEMORY = 65536;

#endif
