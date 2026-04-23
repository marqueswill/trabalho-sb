#include "simulator.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void runSimulator(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Erro: Nao foi possivel abrir o arquivo " << filename << endl;
        return;
    }

    vector<int> memory;
    string opcodeStr;

    // 1. Carregar o codigo de maquina (linha unica) para a memoria
    while (inputFile >> opcodeStr) {
        try {
            memory.push_back(stoi(opcodeStr));
        } catch (const invalid_argument& e) {
            cerr << "Erro: Codigo de maquina invalido detetado." << endl;
            return;
        }
    }
    inputFile.close();

    // 2. Inicializar os registadores da Maquina Virtual
    int pc = 0;   // Program Counter
    int acc = 0;  // Accumulator
    bool isRunning = true;

    cout << "--- Iniciando Simulacao ---" << endl;

    // 3. Ciclo de execucao (Fetch-Decode-Execute)
    while (isRunning && pc < memory.size()) {
        int opcode = memory[pc];

        switch (opcode) {
            case 1:  // ADD
                // acc += memory[memory[pc + 1]];
                // pc += 2;
                break;
            case 2: // SUB
                break;
            case 3: // MUL
                break;
            case 4: // DIV
                break;
            case 5: // JMP
                break;
            case 6: // JMPN
                break;
            case 7: // JMPP
                break;
            case 8: // JMPZ
                break;
            case 9: // COPY
                break;
            case 10: // LOAD
                break;
            case 11: // STORE
                break;
            case 12:  // INPUT
                // TODO: Ler valor do teclado e guardar no endereco memory[pc+1]
                break;
            case 13:  // OUTPUT
                // TODO: Imprimir no monitor o valor no endereco memory[pc+1]
                break;
            case 14:  // STOP
                isRunning = false;
                break;
            default:
                cerr << "Erro: Opcode desconhecido " << opcode << " na posicao " << pc << endl;
                isRunning = false;
                break;
        }
    }

    cout << "--- Simulacao Finalizada ---" << endl;
}