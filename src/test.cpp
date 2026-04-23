#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Incluímos os módulos do nosso projeto
#include "assembler.h"
#include "preprocessor.h"

using namespace std;

// Lê dois arquivos inteiros para a memória e verifica se são idênticos.
bool compareFiles(const string& generatedFile, const string& expectedFile) {
    ifstream fGen(generatedFile);
    ifstream fExp(expectedFile);

    if (!fGen.is_open() || !fExp.is_open()) {
        // Reduzimos o log de erro aqui para não poluir o terminal; o testador principal avisa qual falhou.
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

    // O preprocessor.cpp gera um arquivo com o mesmo nome, mas extensão .pre
    string baseName = inputAsm.substr(0, inputAsm.find_last_of('.'));
    string generatedPre = baseName + ".pre";

    // 2. Executar Montador
    // Passamos o .pre gerado para o assembler
    runAssembler(generatedPre);

    // O assembler.cpp gera os arquivos .obj e .pen com base no .pre fornecido
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
//                        "../examples/expected_test_format.pre",
//                        "../examples/expected_test_format.obj",
//                        "../examples/expected_test_format.pen");
// }

// bool testSectionOrder() {
//     // Cenário: SECTION DATA vem antes da SECTION TEXT no .asm
//     return runTestCase("Inversao de Secoes",
//                        "../examples/test_sections.asm",
//                        "../examples/expected_test_sections.pre",
//                        "../examples/expected_test_sections.obj",
//                        "../examples/expected_test_sections.pen");
// }

bool testLogicPass1() {
    // Cenário: Teste usando o arquivo de exemplo completo dado na especificação (ex1.asm)
    return runTestCase("Exemplo Base (ex1.asm)",
                       "../examples/ex1.asm",
                       "../examples/expected_ex1.pre",
                       "../examples/expected_ex1.obj",
                       "../examples/expected_ex1.pen");
}

// =====================================================================
// MAIN DO TESTADOR
// =====================================================================
int main() {
    cout << "========================================" << endl;
    cout << " INICIANDO SUITE DE TESTES DO MONTADOR" << endl;
    cout << "========================================" << endl
         << endl;

    int totalTests = 1;  // Atualize esse número quando adicionar novos testes
    int passedTests = 0;

    // Executar bateria de testes (usando as funções encapsuladas)
    // if (testFormatting()) passedTests++;
    // if (testSectionOrder()) passedTests++;
    if (testLogicPass1()) passedTests++;

    cout << endl
         << "========================================" << endl;
    cout << " RESULTADO: " << passedTests << "/" << totalTests << " testes passaram." << endl;
    cout << "========================================" << endl;

    // Retorna 0 se tudo passou, ou 1 se algo falhou
    return (passedTests == totalTests) ? 0 : 1;
}