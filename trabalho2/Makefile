NASM = nasm
LD = ld
NASMFLAGS = -f elf32
LDFLAGS = -m elf_i386

SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.asm)
OBJS = $(SRCS:.asm=.o)
TARGET = calculadora

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET)

%.o: %.asm
	$(NASM) $(NASMFLAGS) $< -o $@


clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

