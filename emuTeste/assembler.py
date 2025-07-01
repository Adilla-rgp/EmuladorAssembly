import sys

instr_map = {
    'NOP': 0x0, 'LOAD': 0x1, 'STORE': 0x2, 'LOADM': 0x3,
    'ADD': 0x4, 'SUB': 0x5, 'CMP': 0x6, 'JMP': 0x7,
    'JZ': 0x8, 'PUSH': 0x9, 'POP': 0xA, 'IN': 0xB,
    'OUT': 0xC, 'HALT': 0xF
}

def encode_instruction(parts):
    instr = parts[0].upper()
    opcode = instr_map[instr] << 12
    if instr in ['HALT', 'NOP']:
        return opcode
    rd = int(parts[1][1])
    opcode |= (rd << 9)
    if instr in ['LOAD', 'STORE', 'LOADM', 'JMP', 'JZ', 'IN', 'OUT']:
        imm = int(parts[2])
        opcode |= imm & 0x1FF
    elif instr in ['ADD', 'SUB', 'CMP']:
        rs = int(parts[2][1])
        opcode |= (rs << 6)
    return opcode

def assemble(input_file, output_file):
    with open(input_file, 'r', encoding='utf-8') as f:
        lines = f.readlines()
    binary = bytearray()
    for line in lines:
        line = line.split(';')[0].strip()
        if not line:
            continue
        parts = line.replace(',', '').split()
        instr = encode_instruction(parts)
        binary.append((instr >> 8) & 0xFF)
        binary.append(instr & 0xFF)
    with open(output_file, 'wb') as f:
        f.write(binary)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Uso: python assembler.py entrada.asm saida.bin")
        sys.exit(1)
    assemble(sys.argv[1], sys.argv[2])
