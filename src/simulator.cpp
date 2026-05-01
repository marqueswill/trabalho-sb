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
    if (!validAddress(pc + 1, memory)) {  // Verifica se o endereço passado é válido
        cerr << "Erro: operando ausente em " << instructionName << "." << endl;
        return false;
    }

    outAddr = memory[pc + 1];  // Altera a variável original

    cout << instructionName << " " << outAddr << endl;
    cout << "memory[" << outAddr <<"] = " << memory[outAddr] << endl;

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

        cout << "___________________________________" << endl;
        cout << "\nACC = " << acc << endl;

        int opcode = memory[pc];

        switch (opcode) {
            case 1: {  // ADD
                getOperandAddr(pc, memory, addr);
                acc += memory[addr];
                pc += 2;
                break;
            }

            case 2: {  // SUB
                getOperandAddr(pc, memory, addr);
                acc -= memory[addr];
                pc += 2;
                break;
            }

            case 3: {  // MUL
                getOperandAddr(pc, memory, addr);
                acc *= memory[addr];
                pc += 2;
                break;
            }

            case 4: {  // DIV
                getOperandAddr(pc, memory, addr);
                acc /= memory[addr];
                pc += 2;
                break;
            }

            case 5: {  // JMP
                getOperandAddr(pc, memory, addr);
                pc = addr;
                break;
            }

            case 6: {  // JMPN
                getOperandAddr(pc, memory, addr);
                pc = (acc < 0) ? addr : pc += 2;
                break;
            }

            case 7: {  // JMPP
                getOperandAddr(pc, memory, addr);
                pc = (acc > 0) ? addr : pc += 2;
                break;
            }

            case 8: {  // JMPZ
                getOperandAddr(pc, memory, addr);
                pc = (acc == 0) ? addr : pc += 2; 
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
                getOperandAddr(pc, memory, addr);
                acc = memory[addr];
                pc += 2;
                break;
            }

            case 11: {  // STORE
                getOperandAddr(pc, memory, addr);

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

                cout << "INPUT " << addr <<endl; 
                cout << "memory[" << addr << "] = " << memory[addr] << endl;

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

                cout << "OUTPUT " << addr << endl;
                cout << "memory[" << addr << "] = " << memory[addr] << endl << endl;

                cout << memory[addr] << endl;
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
