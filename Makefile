# Variáveis de compilador e flags
CXX = g++
CXXFLAGS = -Wall -std=c++11 -I./src

# Pastas e Arquivos
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests
EXAMPLES_DIR = examples

# Fontes comuns (lógica do sistema)
COMMON_SRCS = $(SRC_DIR)/preprocessor.cpp $(SRC_DIR)/assembler.cpp $(SRC_DIR)/simulator.cpp

# Alvos principais
MAIN_EXEC = $(BUILD_DIR)/montador
TEST_EXEC = $(BUILD_DIR)/run_tests

# Regra padrão: compila tudo
all: 
	prepare $(MAIN_EXEC) $(TEST_EXEC)

# Cria a pasta build se não existir
prepare:
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

# Compila o montador principal
$(MAIN_EXEC): $(SRC_DIR)/main.cpp $(COMMON_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compila o executador de testes
$(TEST_EXEC): $(TEST_DIR)/test.cpp $(COMMON_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Atalho para rodar o programa principal com ex1.asm
run: $(MAIN_EXEC)
	./$(MAIN_EXEC) $(EXAMPLES_DIR)/ex1.asm

# Atalho para rodar os testes
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# Limpeza dos binários
clean:
	@if exist $(BUILD_DIR) rd /s /q $(BUILD_DIR)