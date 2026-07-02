NASM = nasm
LD = ld
NASMFLAGS = -f elf32
LDFLAGS = -m elf_i386

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.asm)
OBJS = $(patsubst $(SRC_DIR)/%.asm, $(BUILD_DIR)/%.o, $(SRCS))
TARGET = $(BIN_DIR)/calculadora

all: $(TARGET)

# Executável final dentro de bin/
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET)

# Arquivos objeto (.o) dentro de build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(BUILD_DIR)
	$(NASM) $(NASMFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)