#include <iostream>
#include <string>

// Inclusão dos cabeçalhos dos módulos independentes
#include "assembler.h"
#include "preprocessor.h"
#include "simulator.h"

using namespace std;

int main(int argc, char* argv[]) {
    // 1. Validação básica de argumentos da linha de comando
    if (argc != 2) {
        cerr << "Uso incorreto. Formato esperado: ./montador <arquivo>" << endl;
        return 1;
    }

    string filename = argv[1];
    string extension = "";

    // 2. Extrair a extensão do arquivo fornecido
    size_t dotPos = filename.find_last_of('.');
    if (dotPos != string::npos) {
        extension = filename.substr(dotPos);
    } else {
        cerr << "Erro: O arquivo precisa ter uma extensão (.asm, .pre ou .obj)" << endl;
        return 1;
    }

    // 3. Roteamento baseado na extensão
    if (extension == ".asm") {
        cout << "Iniciando Pré-processamento..." << endl;
        // Chama a função principal do módulo de pré-processamento
        // Ex: 
        runPreprocessor(filename);
    } else if (extension == ".pre") {
        cout << "Iniciando Montagem (Passagem Única)..." << endl;
        // Chama a função principal do módulo montador
        // Ex: 
        runAssembler(filename);
    } else if (extension == ".obj") {
        cout << "Iniciando Simulação..." << endl;
        // Chama a função principal do módulo simulador
        // Ex: 
        runSimulator(filename);
    } else {
        cerr << "Erro: Extensão não reconhecida (" << extension << ")." << endl;
        cerr << "Extensões válidas para este programa: .asm, .pre, .obj" << endl;
        return 1;
    }

    return 0;
}