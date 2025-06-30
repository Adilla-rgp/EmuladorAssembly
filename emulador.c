
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SIZE 4096
#define REG_COUNT 8
#define TRUE 1
#define FALSE 0

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

void reset_cpu() {
    cpu.PC = 0x100;
    cpu.SP = 0xEFF;
    cpu.zero_flag = 0;
    cpu.negative_flag = 0;
    cpu.carry_flag = 0;
    cpu.halt = 0;
    memset(cpu.registers, 0, sizeof(cpu.registers));
    memset(cpu.memory, 0, sizeof(cpu.memory));
    memset(cpu.io_ports, 0, sizeof(cpu.io_ports));
}

void update_zero_flag(unsigned short value) {
    cpu.zero_flag = (value == 0);
}

void update_negative_flag(short value) {
    cpu.negative_flag = (value < 0);
}

void update_carry_flag(unsigned int result) {
    cpu.carry_flag = (result > 0xFFFF);
}

unsigned short read_memory(unsigned short address) {
    return (cpu.memory[address] << 8) | cpu.memory[address + 1];
}

void write_memory(unsigned short address, unsigned short value) {
    cpu.memory[address] = (value >> 8) & 0xFF;
    cpu.memory[address + 1] = value & 0xFF;
}

void decode(unsigned short instruction, unsigned char *opcode, unsigned char *rd, unsigned char *rs, unsigned short *imm) {
    *opcode = (instruction >> 12) & 0xF;
    *rd = (instruction >> 9) & 0x7;
    *rs = (instruction >> 6) & 0x7;
    *imm = instruction & 0x1FF; // usa os 9 bits finais
}

void execute(unsigned short instruction) {
    unsigned char opcode, rd, rs;
    unsigned short imm;
    decode(instruction, &opcode, &rd, &rs, &imm);

    switch (opcode) {
        case 0x0: break; // NOP
        case 0x1: cpu.registers[rd] = imm; update_zero_flag(cpu.registers[rd]); break;
        case 0x2: write_memory(imm, cpu.registers[rd]); break;
        case 0x3: cpu.registers[rd] = read_memory(imm); break;
        case 0x4: { unsigned int res = cpu.registers[rd] + cpu.registers[rs]; update_carry_flag(res); cpu.registers[rd] = res & 0xFFFF; update_zero_flag(cpu.registers[rd]); } break;
        case 0x5: { short res = cpu.registers[rd] - cpu.registers[rs]; update_negative_flag(res); cpu.registers[rd] = res & 0xFFFF; update_zero_flag(cpu.registers[rd]); } break;
        case 0x6: { short res = cpu.registers[rd] - cpu.registers[rs]; update_zero_flag(res); update_negative_flag(res); } break;
        case 0x7: cpu.PC = imm; return;
        case 0x8: if (cpu.zero_flag) { cpu.PC = imm; return; } break;
        case 0x9: cpu.SP -= 2; write_memory(cpu.SP, cpu.registers[rs]); break;
        case 0xA: cpu.registers[rd] = read_memory(cpu.SP); cpu.SP += 2; break;
        case 0xB: cpu.registers[rd] = cpu.io_ports[imm]; update_zero_flag(cpu.registers[rd]); break;
        case 0xC: cpu.io_ports[imm] = cpu.registers[rd]; break;
        case 0xF: cpu.halt = TRUE; break;
        default: printf("Instrução inválida: 0x%04X\n", instruction); cpu.halt = TRUE; break;
    }

    cpu.PC += 2;
}

void load_binary(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Erro ao abrir arquivo binário");
        exit(1);
    }
    fread(&cpu.memory[cpu.PC], 1, MEM_SIZE - cpu.PC, f);
    fclose(f);
}

void dump_state() {
    printf("\n[REGISTROS]\n");
    for (int i = 0; i < REG_COUNT; i++)
        printf("R%d = %04X\n", i, cpu.registers[i]);
    printf("PC = %04X | SP = %04X\n", cpu.PC, cpu.SP);
    printf("Flags: Z=%d N=%d C=%d\n", cpu.zero_flag, cpu.negative_flag, cpu.carry_flag);
    printf("PORT1 = %02X\n", cpu.io_ports[1]);
}

void run() {
    while (!cpu.halt) {
        unsigned short instr = read_memory(cpu.PC);
        execute(instr);
        dump_state();
    }
    printf("\nPrograma finalizado.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s programa.bin\n", argv[0]);
        return 1;
    }
    reset_cpu();
    load_binary(argv[1]);
    run();
    return 0;
}
