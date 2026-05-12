#include "preprocessor.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

static string trim(const string& s) {
    size_t start = 0;
    while (start < s.size() && isspace(static_cast<unsigned char>(s[start]))) {
        start++;
    }

    size_t end = s.size();
    while (end > start && isspace(static_cast<unsigned char>(s[end - 1]))) {
        end--;
    }

    return s.substr(start, end - start);
}

static string toUpperCase(string s) {
    for (char& c : s) {
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    }
    return s;
}

static string removeComment(const string& s) {
    size_t pos = s.find(';');
    if (pos == string::npos) {
        return s;
    }
    return s.substr(0, pos);
}

static string normalizeSpaces(const string& s) {
    string temp;
    temp.reserve(s.size());

    // troca TAB e \r por espaço normal
    for (char c : s) {
        if (c == '\t' || c == '\r') {
            temp += ' ';
        } else {
            temp += c;
        }
    }

    // colapsa múltiplos espaços em um só
    string collapsed;
    collapsed.reserve(temp.size());
    bool lastWasSpace = false;

    for (char c : temp) {
        if (isspace(static_cast<unsigned char>(c))) {
            if (!lastWasSpace) {
                collapsed += ' ';
                lastWasSpace = true;
            }
        } else {
            collapsed += c;
            lastWasSpace = false;
        }
    }

    collapsed = trim(collapsed);

    // normaliza COPY A, B -> COPY A,B
    string fixed;
    fixed.reserve(collapsed.size());

    for (size_t i = 0; i < collapsed.size(); i++) {
        if (collapsed[i] == ',') {
            while (!fixed.empty() && fixed.back() == ' ') {
                fixed.pop_back();
            }
            fixed += ',';
            while (i + 1 < collapsed.size() && collapsed[i + 1] == ' ') {
                i++;
            }
        } else {
            fixed += collapsed[i];
        }
    }

    return fixed;
}

static vector<string> splitTokens(const string& line) {
    vector<string> tokens;
    string token;
    stringstream ss(line);

    while (ss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

static bool isLabelOnly(const string& line) {
    return !line.empty() && line.back() == ':';
}

static string replaceEquSymbols(const string& line, const unordered_map<string, string>& equTable) {
    vector<string> tokens = splitTokens(line);

    for (string& token : tokens) {
        auto it = equTable.find(token);
        if (it != equTable.end()) {
            token = it->second;
        }
    }

    string rebuilt;
    for (size_t i = 0; i < tokens.size(); i++) {
        if (i > 0) {
            rebuilt += ' ';
        }
        rebuilt += tokens[i];
    }

    return rebuilt;
}

void runPreprocessor(const string& filename, string inputFolder, string outputFolder) {
    // 1. Abrir arquivo .asm
    string asmFilePath = inputFolder + filename + ".asm";
    ifstream inputFile(asmFilePath);

    if (!inputFile.is_open()) {
        cerr << "Erro: Nao foi possivel abrir o arquivo " << asmFilePath << endl;
        return;
    }

    // 2. Preparar saída .pre
    string preFilePath = outputFolder + filename + ".pre";
    ofstream outputFile(preFilePath);

    if (!outputFile.is_open()) {
        cerr << "Erro: Nao foi possivel criar o arquivo " << preFilePath << endl;
        inputFile.close();
        return;
    }

    string line;
    vector<string> cleanedLines;

    // 3. Limpeza básica
    while (getline(inputFile, line)) {
        line = removeComment(line);
        line = toUpperCase(line);
        line = normalizeSpaces(line);
        line = trim(line);

        if (!line.empty()) {
            cleanedLines.push_back(line);
        }
    }

    inputFile.close();

    // 4. Juntar rótulo isolado com a linha seguinte
    vector<string> joinedLines;
    string pendingLabel;

    for (const string& current : cleanedLines) {
        if (isLabelOnly(current)) {
            pendingLabel = current;
            continue;
        }

        if (!pendingLabel.empty()) {
            joinedLines.push_back(pendingLabel + " " + current);
            pendingLabel.clear();
        } else {
            joinedLines.push_back(current);
        }
    }

    if (!pendingLabel.empty()) {
        joinedLines.push_back(pendingLabel);
    }

    // 5. Coletar EQU
    unordered_map<string, string> equTable;
    vector<string> withoutEqu;

    for (const string& current : joinedLines) {
        vector<string> tokens = splitTokens(current);

        if (tokens.size() == 3 && tokens[0].back() == ':' && tokens[1] == "EQU") {
            string label = tokens[0].substr(0, tokens[0].size() - 1);
            equTable[label] = tokens[2];
        } else {
            withoutEqu.push_back(current);
        }
    }

    // 6. Substituir símbolos definidos por EQU
    vector<string> replacedEqu;
    for (const string& current : withoutEqu) {
        replacedEqu.push_back(replaceEquSymbols(current, equTable));
    }

    // 7. Aplicar IF
    vector<string> afterIf;

    for (size_t i = 0; i < replacedEqu.size(); i++) {
        vector<string> tokens = splitTokens(replacedEqu[i]);

        if (tokens.size() == 2 && tokens[0] == "IF") {
            if (tokens[1] == "0") {
                i++;  // pula a próxima linha
            }
            continue;  // IF não vai para o .pre
        }

        afterIf.push_back(replacedEqu[i]);
    }

    // 8. Separar SECTION TEXT e SECTION DATA
    vector<string> sectionText;
    vector<string> sectionData;

    enum SectionMode { NONE,
                       TEXT,
                       DATA };
    SectionMode mode = NONE;

    for (const string& current : afterIf) {
        if (current == "SECTION TEXT") {
            mode = TEXT;
            continue;
        }

        if (current == "SECTION DATA") {
            mode = DATA;
            continue;
        }

        if (mode == TEXT) {
            sectionText.push_back(current);
        } else if (mode == DATA) {
            sectionData.push_back(current);
        }
    }

    // 9. Escrever sempre TEXT antes de DATA
    outputFile << "SECTION TEXT" << '\n';
    for (const string& s : sectionText) {
        outputFile << s << '\n';
    }

    outputFile << "SECTION DATA";
    for (const string& s : sectionData) {
        outputFile << '\n'
                   << s;
    }

    outputFile.close();

    cout << "   Pre-processamento concluido. arquivo gerado: " << preFilePath << endl;
}