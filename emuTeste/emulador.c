#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definições de constantes
#define MEM_SIZE 4096       // Tamanho da memória
#define REG_COUNT 8         // Número de registradores
#define TRUE 1
#define FALSE 0

// Estrutura da CPU - estado do processador
typedef struct {
    unsigned short PC;                      
    unsigned short SP;                     
    unsigned short registers[REG_COUNT];    
    unsigned char memory[MEM_SIZE];         
    unsigned char io_ports[4];             
    unsigned char zero_flag;               
    unsigned char negative_flag;           
    unsigned char carry_flag;             
    unsigned char halt;                     
} CPU;

CPU cpu;

// Variáveis auxiliares para mostrar a operação final
unsigned short A = 0;
unsigned short B = 0;

// INICIALIZAÇÃO E CONFIGURAÇÃO

void reset_cpu() {
    cpu.PC = 0x100;     // PC inicia em 0x100
    cpu.SP = 0xEFF;     // Stack cresce para baixo
    cpu.zero_flag = 0;
    cpu.negative_flag = 0;
    cpu.carry_flag = 0;
    cpu.halt = 0;
    // Zera registradores, memória e portas I/O
    memset(cpu.registers, 0, sizeof(cpu.registers));
    memset(cpu.memory, 0, sizeof(cpu.memory));
    memset(cpu.io_ports, 0, sizeof(cpu.io_ports));
}


// FUNÇÕES DE ATUALIZAÇÃO DE FLAGS

void update_zero_flag(unsigned short value) {
    cpu.zero_flag = (value == 0);
}

void update_negative_flag(short value) {
    cpu.negative_flag = (value < 0);
}

void update_carry_flag(unsigned int result) {
    cpu.carry_flag = (result > 0xFFFF);
}


// ACESSO À MEMÓRIA

// Lê 16 bits da memória (big-endian)
unsigned short read_memory(unsigned short address) {
    return (cpu.memory[address] << 8) | cpu.memory[address + 1];
}

// Escreve 16 bits na memória (big-endian)
void write_memory(unsigned short address, unsigned short value) {
    cpu.memory[address] = (value >> 8) & 0xFF;
    cpu.memory[address + 1] = value & 0xFF;
}

// DECODIFICAÇÃO E EXECUÇÃO

// Decodifica instrução de 16 bits
void decode(unsigned short instruction, unsigned char *opcode, unsigned char *rd, unsigned char *rs, unsigned short *imm) {
    *opcode = (instruction >> 12) & 0xF;    //  Bits 15-12: opcode
    *rd = (instruction >> 9) & 0x7;         // Bits 11-9: reg  destino
    *rs = (instruction >> 6) & 0x7;         // Bits 8-6: reg fonte  
    *imm = instruction & 0x1FF;             // Bits  8-0: imediato
}

// Executa instrução decodificada
void execute(unsigned short instruction) {
    unsigned char opcode, rd, rs;
    unsigned short imm;
    decode(instruction, &opcode, &rd, &rs, &imm);

    switch (opcode) {
        case 0x0: break; // NOP - não faz nada

        case 0x1: // LOAD - carrega imediato no registrador
            cpu.registers[rd] = imm;
            if (rd == 0) A = imm;  // Armazena A para mostrar resultado
            if (rd == 1) B = imm;  // Armazena B para mostrar resultado
            update_zero_flag(cpu.registers[rd]);
            break;

        case 0x2: // STORE - armazena registrador na memória
            write_memory(imm, cpu.registers[rd]);
            break;

        case 0x3: // LOADM - carrega da memória para registrador
            cpu.registers[rd] = read_memory(imm);
            break;

        case 0x4: { // ADD - soma registradores
            unsigned int res = cpu.registers[rd] + cpu.registers[rs];
            update_carry_flag(res);
            cpu.registers[rd] = res & 0xFFFF;
            update_zero_flag(cpu.registers[rd]);
            break;
        }

        case 0x5: { // SUB - subtrai registradores
            short res = cpu.registers[rd] - cpu.registers[rs];
            update_negative_flag(res);
            cpu.registers[rd] = res & 0xFFFF;
            update_zero_flag(cpu.registers[rd]);
            break;
        }

        case 0x6: { // CMP - compara registradores (só atualiza flags)
            short res = cpu.registers[rd] - cpu.registers[rs];
            update_zero_flag(res);
            update_negative_flag(res);
            break;
        }

        case 0x7: // JMP - salto incondicional
            cpu.PC = imm;
            return; // Não incrementa PC

        case 0x8: // JZ - salto condicional se zero
            if (cpu.zero_flag) {
                cpu.PC = imm;
                return; // Não incrementa PC
            }
            break;

        case 0x9: // PUSH - empilha valor
            cpu.SP -= 2;
            write_memory(cpu.SP, cpu.registers[rs]);
            break;

        case 0xA: // POP - desempilha valor
            cpu.registers[rd] = read_memory(cpu.SP);
            cpu.SP += 2;
            break;

        case 0xB: // IN - lê da porta de entrada
            cpu.registers[rd] = cpu.io_ports[imm];
            update_zero_flag(cpu.registers[rd]);
            break;

        case 0xC: // OUT - escreve na porta de saída
            cpu.io_ports[imm] = cpu.registers[rd];
            break;

        case 0xF: // HALT - para execução
            cpu.halt = TRUE;
            break;

        default: // Instrução inválida
            printf("Instrucao invalida: 0x%04X\n", instruction);
            cpu.halt = TRUE;
            break;
    }

    cpu.PC += 2; // Avança para próxima instrução
}


// Carrega programa binário na memória
void load_binary(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Erro ao abrir arquivo binario");
        exit(1);
    }
    fread(&cpu.memory[cpu.PC], 1, MEM_SIZE - cpu.PC, f);
    fclose(f);
}

// Mostra estado atual da CPU
void dump_state() {
    printf("\n[REGISTROS]\n");
    for (int i = 0; i < REG_COUNT; i++)
        printf("R%d = %04X\n", i, cpu.registers[i]);
    printf("PC = %04X | SP = %04X\n", cpu.PC, cpu.SP);
    printf("Flags: Z=%d N=%d C=%d\n", cpu.zero_flag, cpu.negative_flag, cpu.carry_flag);
    printf("PORT1 = %02X\n", cpu.io_ports[1]);
}

// Loop principal de execução
void run() {
    while (!cpu.halt) {
        unsigned short instr = read_memory(cpu.PC);  // Busca instrução
        execute(instr);                              // Executa instrução
        dump_state();                                // Mostra estado
    }

    printf("\n[Programa finalizado]\n");
    printf("\nResultado: %d + %d = %d (0x%02X)\n", A, B, A + B, A + B);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s programa.bin\n", argv[0]);
        return 1;
    }
    reset_cpu();           // Inicializa CPU
    load_binary(argv[1]);  // Carrega programa
    run();                 // Executa
    return 0;
}