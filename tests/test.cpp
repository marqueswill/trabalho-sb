#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "assembler.h"
#include "auxiliar.h"
#include "preprocessor.h"

using namespace std;
struct TestCase {
    string title;
    string baseName;  // ex: "fibo", "fat", "ex1"
};

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
bool runTestCase(const string& testName) {
    cout << "[TESTE] Executando: " << testName << "..." << endl;

    string exampleFolder = "tests/examples";
    string expectedFolder = "tests/expected";
    string outputFolder = "tests/outputs/";

    // 1. Executar Pré-processador
    runPreprocessor(testName, exampleFolder, outputFolder);

    // 2. Executar Montador
    runAssembler(testName, outputFolder, outputFolder);

    // 3. Comparar todos os resultados
    string generatedPre = outputFolder + testName + ".pre";
    string generatedObj = outputFolder + testName + ".obj";
    string generatedPen = outputFolder + testName + ".pen";

    string expectedPre = expectedFolder + testName + ".pre";
    string expectedObj = expectedFolder + testName + ".obj";
    string expectedPen = expectedFolder + testName + ".pen";

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
// MAIN DO TESTADOR
// =====================================================================
int main() {
    cout << "========================================" << endl;
    cout << " INICIANDO SUITE DE TESTES DO MONTADOR" << endl;
    cout << "========================================" << endl
         << endl;

    vector<TestCase> tests = {
        {"Exemplo Base", "ex1"},
        {"Inversao de Secoes", "fat"},
        {"Formatacao Extrema", "fibo"}};

    int totalTests = tests.size();
    int passedTests = 0;

    // Executar bateria de testes
    for (const auto& tc : tests) {
        bool result = runTestCase(tc.baseName);
        if (result) passedTests++;
    }

    cout << endl
         << "========================================" << endl;
    cout << " RESULTADO: " << passedTests << "/" << totalTests << " testes passaram." << endl;
    cout << "========================================" << endl;

    // Retorna 0 se tudo passou, ou 1 se algo falhou
    return (passedTests != totalTests);
}