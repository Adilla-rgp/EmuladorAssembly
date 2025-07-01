import subprocess

def menu():
    print("""\n=== SimCPU Emulator Interface ===
1. Montar programa (.asm → .bin)
2. Executar programa (.bin)
3. Sair""")
    return input("Escolha uma opcao: ").strip()

def montar_programa():
    entrada = input("Nome do arquivo .asm: ").strip()
    saida = input("Nome do arquivo .bin de saida: ").strip()
    subprocess.run(["python", "assembler.py", entrada, saida], check=True)
    print("[Programa montado com sucesso]")

def executar_programa():
    binario = input("Nome do programa binario: ").strip()
    subprocess.run(["./emulador", binario])

def main():
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

if __name__ == "__main__":
    main()
