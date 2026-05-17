#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <string>

#include "assembler.h"
#include "preprocessor.h"
#include "simulator.h"

using namespace std;

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    if (argc != 2) {
        cerr << "Uso incorreto. Formato esperado: ./montador <arquivo>" << endl;
        return 1;
    }

    string filename = argv[1];
    string extension = "";

    size_t dotPos = filename.find_last_of('.');
    if (dotPos != string::npos) {
        extension = filename.substr(dotPos);
        filename = filename.substr(0, dotPos);
    } else {
        cerr << "Erro: O arquivo precisa ter uma extensao (.asm, .pre ou .obj)" << endl;
        return 1;
    }

    if (extension == ".asm") {
        runPreprocessor(filename);
    } else if (extension == ".pre") {
        runAssembler(filename);

    } else if (extension == ".obj") {
        runSimulator(filename);

    } else {
        cerr << "Erro: Extensao nao reconhecida (" << extension << ")." << endl;
        cerr << "Extensoes validas para este programa: .asm, .pre, .obj" << endl;
        return 1;
    }

    return 0;
}