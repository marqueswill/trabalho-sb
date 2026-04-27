# Variáveis de compilador e flags
CXX = g++
CXXFLAGS = -Wall -std=c++11 -I./src

# Pastas e Arquivos
SRC_DIR = src
TEST_DIR = tests
EXAMPLES_DIR = $(TEST_DIR)/examples
OUTPUT_DIR = $(TEST_DIR)/outputs

# Fontes comuns (lógica do sistema)
COMMON_SRCS = $(SRC_DIR)/preprocessor.cpp $(SRC_DIR)/assembler.cpp $(SRC_DIR)/simulator.cpp $(SRC_DIR)/auxiliar.cpp

# Alvos principais
MAIN_EXEC = montador
TEST_EXEC = run_tests

# Regra padrão: compila tudo
all: $(MAIN_EXEC) $(TEST_EXEC)


# Atalho para rodar o programa principal com ex1.asm
FILE ?= ex1

run: $(MAIN_EXEC)
	./$(MAIN_EXEC) $(EXAMPLES_DIR)/$(FILE).asm
	./$(MAIN_EXEC) $(OUTPUT_DIR)/$(FILE).pre
	./$(MAIN_EXEC) $(OUTPUT_DIR)/$(FILE).obj

# Atalho para rodar os testes
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# Compila o montador principal
$(MAIN_EXEC): $(SRC_DIR)/main.cpp $(COMMON_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compila o executador de testes
$(TEST_EXEC): $(TEST_DIR)/test.cpp $(COMMON_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@