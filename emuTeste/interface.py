import subprocess

# INTERFACE DO USUÁRIO

def menu():
    #Exibe menu principal e retorna escolha do usuário"""

    print("""\n___ Emulador--Interface ___
1. Montar programa (.asm → .bin)
2. Executar programa (.bin)
3. Sair""")
    return input("Escolha uma opcao: ").strip()


# OPERAÇÕES PRINCIPAIS

def montar_programa():
    """Chama o assembler para converter .asm em .bin"""
    entrada = input("Nome do arquivo .asm: ").strip()
    saida = input("Nome do arquivo .bin de saida: ").strip()
    
    # Executa o assembler via subprocess
    subprocess.run(["python", "assembler.py", entrada, saida], check=True)
    print("[Programa montado com sucesso]")

def executar_programa():
    """Chama o emulador para executar arquivo .bin"""
    binario = input("Nome do programa binario: ").strip()
    
    # Executa o emulador via subprocess
    subprocess.run(["./emulador", binario])

#loop principal

def main():
    """Loop principal da interface"""
    while True:
        opcao = menu()
        
        if opcao == "1":
            montar_programa()
        elif opcao == "2":
            executar_programa()
        elif opcao == "3":
            break
        else:
            print("Opção invalida.")

# ponto de entrada

if __name__ == "__main__":
    main()