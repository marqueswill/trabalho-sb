#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "assembler.h"
#include "auxiliar.h"
#include "preprocessor.h"

using namespace std;

// Lê dois arquivos inteiros para a memória e verifica se são idênticos.
bool compareFiles(const string& generatedFile, const string& expectedFile) {
    // cout << "   Comparando: " << generatedFile << " com " << expectedFile << endl;
    ifstream fGen(generatedFile);
    ifstream fExp(expectedFile);

    if (!fGen.is_open() || !fExp.is_open()) {
        return false;
    }

    // Lendo o conteúdo inteiro dos arquivos
    stringstream bufferGen, bufferExp;
    bufferGen << fGen.rdbuf();
    bufferExp << fExp.rdbuf();

    return bufferGen.str() == bufferExp.str();
}

// Executa um teste completo (Pré-processador + Montador) e retorna se passou (true) ou falhou (false)
bool runTestCase(const string& testName,
                 const string& inputAsm,
                 const string& expectedPre,
                 const string& expectedObj,
                 const string& expectedPen) {
    cout << "[TESTE] Executando: " << testName << "..." << endl;

    // 1. Executar Pré-processador
    // runPreprocessor(inputAsm);
    string generatedPre = getOutFileName(inputAsm, ".pre");

    // 2. Executar Montador
    // runAssembler(generatedPre);
    string generatedObj = getOutFileName(inputAsm, ".obj");
    string generatedPen = getOutFileName(inputAsm, ".pen");

    // 3. Comparar todos os resultados
    bool passedPre = compareFiles(generatedPre, expectedPre);
    bool passedObj = compareFiles(generatedObj, expectedObj);
    bool passedPen = compareFiles(generatedPen, expectedPen);

    if (passedPre && passedObj && passedPen) {
        cout << "  -> \033[1;32mPASSOU\033[0m" << endl
             << endl;  // Imprime verde
        return true;
    } else {
        cout << "  -> \033[1;31mFALHOU\033[0m. Divergencias ou arquivos ausentes:" << endl;  // Imprime vermelho
        if (!passedPre) cout << "     [x] Diferenca no arquivo PRE-PROCESSADO: " << generatedPre << endl;
        if (!passedObj) cout << "     [x] Diferenca no arquivo OBJETO: " << generatedObj << endl;
        if (!passedPen) cout << "     [x] Diferenca no arquivo PENDENCIAS: " << generatedPen << endl;
        cout << endl;
        return false;
    }
}

// =====================================================================
// CASOS DE TESTE
// =====================================================================
// Retornam o resultado de runTestCase para contabilizar na main

bool testFibo() {
    return runTestCase("Formatacao Extrema",
                       "../examples/fibo.asm",
                       "../expected/fibo.pre",
                       "../expected/fibo.obj",
                       "../expected/fibo.pen");
}

bool testFat() {
    return runTestCase("Inversao de Secoes",
                       "../examples/fat.asm",
                       "../expected/fat.pre",
                       "../expected/fat.obj",
                       "../expected/fat.pen");
}

bool testEx1() {
    return runTestCase("Exemplo Base (ex1.asm)",
                       "tests/examples/ex1.asm",
                       "tests/expected/ex1.pre",
                       "tests/expected/ex1.obj",
                       "tests/expected/ex1.pen");
}

// =====================================================================
// MAIN DO TESTADOR
// =====================================================================
int main() {
    cout << "========================================" << endl;
    cout << " INICIANDO SUITE DE TESTES DO MONTADOR" << endl;
    cout << "========================================" << endl
         << endl;

    vector<function<bool()>> tests = {
        testEx1,
        testFat,
        testFibo,
    };

    int totalTests = tests.size();
    int passedTests = 0;

    // Executar bateria de testes (usando as funções encapsuladas)
    for (int i = 0; i < totalTests; i++) {
        if (tests[i]()) {
            passedTests++;
        };
    }

    cout << endl
         << "========================================" << endl;
    cout << " RESULTADO: " << passedTests << "/" << totalTests << " testes passaram." << endl;
    cout << "========================================" << endl;

    // Retorna 0 se tudo passou, ou 1 se algo falhou
    return (passedTests != totalTests);
}