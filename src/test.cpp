#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "assembler.h"
#include "preprocessor.h"

using namespace std;

// Lê dois arquivos inteiros para a memória e verifica se são idênticos.
bool compareFiles(const string& generatedFile, const string& expectedFile) {
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
    runPreprocessor(inputAsm);
    string baseName = inputAsm.substr(0, inputAsm.find_last_of('.'));
    string generatedPre = baseName + ".pre";

    // 2. Executar Montador
    runAssembler(generatedPre);
    string generatedObj = baseName + ".obj";
    string generatedPen = baseName + ".pen";

    // 3. Comparar todos os resultados
    bool passedPre = compareFiles(generatedPre, expectedPre);
    bool passedObj = compareFiles(generatedObj, expectedObj);
    bool passedPen = compareFiles(generatedPen, expectedPen);

    if (passedPre && passedObj && passedPen) {
        cout << "  -> \033[1;32mPASSOU\033[0m" << endl;  // Imprime verde
        return true;
    } else {
        cout << "  -> \033[1;31mFALHOU\033[0m. Divergencias ou arquivos ausentes:" << endl;  // Imprime vermelho
        if (!passedPre) cout << "     [x] Diferenca no arquivo PRE-PROCESSADO: " << generatedPre << endl;
        if (!passedObj) cout << "     [x] Diferenca no arquivo OBJETO: " << generatedObj << endl;
        if (!passedPen) cout << "     [x] Diferenca no arquivo PENDENCIAS: " << generatedPen << endl;
        return false;
    }
}

// =====================================================================
// CASOS DE TESTE
// =====================================================================
// Retornam o resultado de runTestCase para contabilizar na main

// bool testFormatting() {
//     // Cenário: Arquivo cheio de espaços duplos, tabs e minúsculas.
//     return runTestCase("Formatacao Extrema",
//                        "../examples/test_format.asm",
//                        "../examples/test_format.pre",
//                        "../examples/test_format.obj",
//                        "../examples/test_format.pen");
// }

// bool testSectionOrder() {
//     // Cenário: SECTION DATA vem antes da SECTION TEXT no .asm
//     return runTestCase("Inversao de Secoes",
//                        "../examples/test_sections.asm",
//                        "../examples/test_sections.pre",
//                        "../examples/test_sections.obj",
//                        "../examples/test_sections.pen");
// }

bool testEx1() {
    return runTestCase("Exemplo Base (ex1.asm)",
                       "../examples/ex1.asm",
                       "../examples/ex1.pre",
                       "../examples/ex1.obj",
                       "../examples/ex1.pen");
}

// =====================================================================
// MAIN DO TESTADOR
// =====================================================================
int main() {
    cout << "========================================" << endl;
    cout << " INICIANDO SUITE DE TESTES DO MONTADOR" << endl;
    cout << "========================================" << endl
         << endl;

    int totalTests = 1;  // ATUALIZAR MANUALMENTE!!
    int passedTests = 0;

    // Executar bateria de testes (usando as funções encapsuladas)
    if (testEx1()) passedTests++;
    // if (testFormatting()) passedTests++;
    // if (testSectionOrder()) passedTests++;

    cout << endl
         << "========================================" << endl;
    cout << " RESULTADO: " << passedTests << "/" << totalTests << " testes passaram." << endl;
    cout << "========================================" << endl;

    // Retorna 0 se tudo passou, ou 1 se algo falhou
    return (passedTests != totalTests);
}