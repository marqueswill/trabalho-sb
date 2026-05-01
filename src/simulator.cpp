#include "simulator.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "auxiliar.h"

using namespace std;

static bool validAddress(int addr, const vector<int>& memory) {
    return addr >= 0 && addr < static_cast<int>(memory.size());
}

void loadToMemory(const string& filename, vector<int>& memory) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Erro: Nao foi possivel abrir o arquivo " << filename << endl;
        return;
    }

    int value;
    while (inputFile >> value) {
        memory.push_back(value);
    }

    inputFile.close();

    if (memory.empty()) {
        cerr << "Erro: arquivo objeto vazio." << endl;
        return;
    }

    if (memory.size() > MAX_MEMORY) {
        cerr << "Erro: Arquivo objeto excede a memoria de 16 bits." << endl;
        return;
    }
}

// TODO: Fazer um getter
bool getOperandAddr(int pc, const vector<int>& memory, int& outAddr) {
    int opcode = memory[pc];
    string instructionName = getInstructionName(opcode);
    cout << "Realizando uma operação de " << instructionName << endl;
    if (!validAddress(pc + 1, memory)) {  // Verifica se o endereço passado é válido
        cerr << "Erro: operando ausente em " << instructionName << "." << endl;
        return false;
    }

    outAddr = memory[pc + 1]; // Altera a variável original

    if (!validAddress(outAddr, memory)) {  // Verifica se o endereço lido é valido
        cerr << "Erro: endereco invalido em " << instructionName << ": " << outAddr << endl;
        return false;
    }

    return true;
}

void runSimulator(const string& filename) {
    vector<int> memory;
    loadToMemory(filename, memory);

    int pc = 0;
    int acc = 0;
    int addr;
    bool isRunning = true;

    // cout << "--- Iniciando Simulacao ---" << endl;

    while (isRunning) {
        if (!validAddress(pc, memory)) {
            cerr << "Erro: PC fora da memoria: " << pc << endl;
            return;
        }

        int opcode = memory[pc];

        switch (opcode) {
            case 1: {  // ADD
                if (!validAddress(pc + 1, memory)) {
                    cerr << "Erro: operando ausente em ADD." << endl;
                    return;
                }
                int addr = memory[pc + 1];
                if (!validAddress(addr, memory)) {
                    cerr << "Erro: endereco invalido em ADD: " << addr << endl;
                    return;
                }
                acc += memory[addr];
                pc += 2;
                break;
            }

            case 2: {  // SUB
                if (!validAddress(pc + 1, memory)) {
                    cerr << "Erro: operando ausente em SUB." << endl;
                    return;
                }
                int addr = memory[pc + 1];
                if (!validAddress(addr, memory)) {
                    cerr << "Erro: endereco invalido em SUB: " << addr << endl;
                    return;
                }
                acc -= memory[addr];
                pc += 2;
                break;
            }

            case 3: {  // MUL
                if (!validAddress(pc + 1, memory)) {
                    cerr << "Erro: operando ausente em MUL." << endl;
                    return;
                }
                int addr = memory[pc + 1];
                if (!validAddress(addr, memory)) {
                    cerr << "Erro: endereco invalido em MUL: " << addr << endl;
                    return;
                }
                acc *= memory[addr];
                pc += 2;
                break;
            }

            case 4: {  // DIV
                if (!validAddress(pc + 1, memory)) {
                    cerr << "Erro: operando ausente em DIV." << endl;
                    return;
                }
                int addr = memory[pc + 1];
                if (!validAddress(addr, memory)) {
                    cerr << "Erro: endereco invalido em DIV: " << addr << endl;
                    return;
                }

                if (memory[addr] == 0) {
                    cerr << "Erro: divisao por zero." << endl;
                    return;
                }
                acc /= memory[addr];
                pc += 2;
                break;
            }

            case 5: {  // JMP
                if (!validAddress(pc + 1, memory)) {
                    cerr << "Erro: operando ausente em JMP." << endl;
                    return;
                }
                int addr = memory[pc + 1];
                if (!validAddress(addr, memory)) {
                    cerr << "Erro: endereco invalido em JMP: " << addr << endl;
                    return;
                }
                pc = addr;
                break;
            }

            case 6: {  // JMPN
                if (!validAddress(pc + 1, memory)) {
                    cerr << "Erro: operando ausente em JMPN." << endl;
                    return;
                }
                int addr = memory[pc + 1];
                if (!validAddress(addr, memory)) {
                    cerr << "Erro: endereco invalido em JMPN: " << addr << endl;
                    return;
                }
                if (acc < 0) {
                    pc = addr;
                } else {
                    pc += 2;
                }
                break;
            }

            case 7: {  // JMPP
                if (!validAddress(pc + 1, memory)) {
                    cerr << "Erro: operando ausente em JMPP." << endl;
                    return;
                }
                int addr = memory[pc + 1];
                if (!validAddress(addr, memory)) {
                    cerr << "Erro: endereco invalido em JMPP: " << addr << endl;
                    return;
                }
                if (acc > 0) {
                    pc = addr;
                } else {
                    pc += 2;
                }
                break;
            }

            case 8: {  // JMPZ
                if (!validAddress(pc + 1, memory)) {
                    cerr << "Erro: operando ausente em JMPZ." << endl;
                    return;
                }
                int addr = memory[pc + 1];
                if (!validAddress(addr, memory)) {
                    cerr << "Erro: endereco invalido em JMPZ: " << addr << endl;
                    return;
                }
                if (acc == 0) {
                    pc = addr;
                } else {
                    pc += 2;
                }
                break;
            }

            case 9: {  // COPY
                if (!validAddress(pc + 1, memory) || !validAddress(pc + 2, memory)) {
                    cerr << "Erro: operandos ausentes em COPY." << endl;
                    return;
                }
                int addr1 = memory[pc + 1];
                int addr2 = memory[pc + 2];

                if (!validAddress(addr1, memory) || !validAddress(addr2, memory)) {
                    cerr << "Erro: endereco invalido em COPY." << endl;
                    return;
                }

                memory[addr2] = memory[addr1];
                pc += 3;
                break;
            }

            case 10: {  // LOAD
                if (!validAddress(pc + 1, memory)) {
                    cerr << "Erro: operando ausente em LOAD." << endl;
                    return;
                }
                int addr = memory[pc + 1];
                if (!validAddress(addr, memory)) {
                    cerr << "Erro: endereco invalido em LOAD: " << addr << endl;
                    return;
                }
                acc = memory[addr];
                pc += 2;
                break;
            }

            case 11: {  // STORE
                if (!validAddress(pc + 1, memory)) {
                    cerr << "Erro: operando ausente em STORE." << endl;
                    return;
                }
                int addr = memory[pc + 1];
                if (!validAddress(addr, memory)) {
                    cerr << "Erro: endereco invalido em STORE: " << addr << endl;
                    return;
                }
                memory[addr] = acc;
                pc += 2;
                break;
            }

            case 12: {  // INPUT
                if (!validAddress(pc + 1, memory)) {
                    cerr << "Erro: operando ausente em INPUT." << endl;
                    return;
                }
                int addr = memory[pc + 1];
                if (!validAddress(addr, memory)) {
                    cerr << "Erro: endereco invalido em INPUT: " << addr << endl;
                    return;
                }

                int inValue;
                cin >> inValue;
                memory[addr] = inValue;
                pc += 2;
                break;
            }

            case 13: {  // OUTPUT
                if (!validAddress(pc + 1, memory)) {
                    cerr << "Erro: operando ausente em OUTPUT." << endl;
                    return;
                }
                int addr = memory[pc + 1];
                if (!validAddress(addr, memory)) {
                    cerr << "Erro: endereco invalido em OUTPUT: " << addr << endl;
                    return;
                }
                cout << memory[addr];
                pc += 2;
                break;
            }

            case 14: {  // STOP
                isRunning = false;
                break;
            }

            default:
                cerr << "Erro: Opcode desconhecido " << opcode
                     << " na posicao " << pc << endl;
                return;
        }
    }

    // cout << "--- Simulacao Finalizada ---" << endl;
}
