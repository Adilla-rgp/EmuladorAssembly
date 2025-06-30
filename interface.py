
import subprocess
import os

def menu():
    print("""\n=== SimCPU Emulator Interface ===
1. Montar programa (.asm → .bin)
2. Executar programa (.bin)
3. Testes automáticos
4. Sair""")
    return input("Escolha uma opção: ").strip()

def montar_programa():
    entrada = input("Nome do arquivo .asm: ").strip()
    saida = input("Nome do arquivo .bin de saída: ").strip()
    subprocess.run(["python", "assembler.py", entrada, saida], check=True)
    print("Programa montado com sucesso.")

def executar_programa():
    binario = input("Nome do programa binário: ").strip()
    subprocess.run(["./emulador", binario])

def testes_automaticos():
    from environment import Environment
    with open("template.asm") as f:
        template = f.read()
    env = Environment()
    env.run_test("assembler.py", "./emulador", template)

def main():
    while True:
        opcao = menu()
        if opcao == "1": montar_programa()
        elif opcao == "2": executar_programa()
        elif opcao == "3": testes_automaticos()
        elif opcao == "4": break
        else: print("Opção inválida.")

if __name__ == "__main__":
    main()
