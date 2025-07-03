import sys


# BLOCO 1: MAPEAMENTO DE INSTRUÇÕES

# Dicionário que mapeia mnemônicos das instruções para seus opcodes
instr_map = {
    'NOP': 0x0,      
    'LOAD': 0x1,     
    'STORE': 0x2,    
    'LOADM': 0x3,    
    'ADD': 0x4,     
    'SUB': 0x5,     
    'CMP': 0x6,      
    'JMP': 0x7,      
    'JZ': 0x8,       
    'PUSH': 0x9,     
    'POP': 0xA,      
    'IN': 0xB,       
    'OUT': 0xC,     
    'HALT': 0xF      
}


# CODIFICAÇÃO DE INSTRUÇÕES

def encode_instruction(parts):
    
    # Extrai o mnemônico e converte para maiúsculo
    instr = parts[0].upper()
    
    # Coloca o opcode nos 4 bits mais significativos (bits 15-12)
    opcode = instr_map[instr] << 12
    
    # Instruções que não precisam de operandos
    if instr in ['HALT', 'NOP']:
        return opcode
    
    # Extrai o número do registrador destino
    rd = int(parts[1][1])
    
    # Coloca o registrador destino nos bits 11-9
    opcode |= (rd << 9)
    
    # Instruções que usam valor imediato ou endereço
    if instr in ['LOAD', 'STORE', 'LOADM', 'JMP', 'JZ', 'IN', 'OUT']:
        # Converte o valor imediato/endereço para inteiro
        imm = int(parts[2])
        opcode |= imm & 0x1FF
        
    # Instruções que usam dois registradores
    elif instr in ['ADD', 'SUB', 'CMP']:
        # Extrai o número do registrador fonte
        rs = int(parts[2][1])
        # Coloca o registrador fonte nos bits 8-6
        opcode |= (rs << 6)
    
    return opcode

# BLOCO 3: FUNÇÃO PRINCIPAL DE MONTAGEM
def assemble(input_file, output_file):
    
    # Lê todas as linhas do arquivo assembly
    with open(input_file, 'r', encoding='utf-8') as f:
        lines = f.readlines()
    
    # Array de bytes para armazenar o código de máquina
    binary = bytearray()
    
    # Processa cada linha do arquivo
    for line in lines:
        # Remove comentários (tudo após ';') e espaços em branco
        line = line.split(';')[0].strip()
        
        # Pula linhas vazias
        if not line:
            continue
        
        # Divide a linha em partes, removendo vírgulas
    
        parts = line.replace(',', '').split()
        
        # Codifica a instrução em 16 bits
        instr = encode_instruction(parts)
        
        # Converte a instrução de 16 bits em 2 bytes 
        binary.append((instr >> 8) & 0xFF)  # 8 bits superiores
        binary.append(instr & 0xFF)         # 8 bits inferiores
    
    # Escreve o código de máquina no arquivo binário
    with open(output_file, 'wb') as f:
        f.write(binary)

# PONTO DE ENTRADA DO PROGRAMA
if __name__ == "__main__":
    # Verifica se foram fornecidos os argumentos corretos
    if len(sys.argv) != 3:
        print("Uso: python assembler.py entrada.asm saida.bin")
        sys.exit(1)
    
    # Chama a função de montagem com os arquivos especificados
    assemble(sys.argv[1], sys.argv[2])