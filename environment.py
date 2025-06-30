class Environment:
    def __init__(self):
        self.input_buffer = []
        self.output_buffer = []
        self.test_cases = [(5, 7), (10, 20), (3, 8)]

    def generate_test_case(self):
        return self.test_cases.pop(0) if self.test_cases else (0, 0)

    def validate_response(self, expected, actual):
        return expected == actual

    def run_test(self, assembler, emulator, asm_template):
        import subprocess
        print("Iniciando testes automáticos...")

        total = 0
        passed = 0
        failed_cases = []

        for a, b in self.test_cases:
            total += 1
            asm_code = asm_template.replace("{A}", str(a)).replace("{B}", str(b))
            with open("temp.asm", "w") as f:
                f.write(asm_code)

            try:
                subprocess.run(["python", assembler, "temp.asm", "temp.bin"], check=True)
            except subprocess.CalledProcessError:
                print(f"Erro ao montar o programa para {a} + {b}")
                failed_cases.append((a, b, "Erro montagem"))
                continue

            try:
                result = subprocess.run([emulator, "temp.bin"], capture_output=True, text=True)
                output = result.stdout
            except Exception as e:
                print(f"Erro ao executar o emulador: {e}")
                failed_cases.append((a, b, "Erro execução"))
                continue

            val = None
            for line in output.splitlines():
                if "PORT1" in line:
                    val_str = line.split("=")[-1].strip()
                    try:
                        val = int(val_str, 16)
                    except ValueError:
                        try:
                            val = int(val_str)
                        except ValueError:
                            val = None
                    break

            expected = a + b
            if val is None:
                print(f"Teste: {a} + {b} = ? -> FALHA (PORT1 não encontrado)")
                failed_cases.append((a, b, "PORT1 não encontrado"))
            elif self.validate_response(expected, val):
                print(f"Teste: {a} + {b} = {val} -> OK")
                passed += 1
            else:
                print(f"Teste: {a} + {b} = {val} -> FALHA (esperado {expected})")
                failed_cases.append((a, b, val))

        # Estatísticas finais
        print("\n=== RESUMO DOS TESTES ===")
        print(f"Total de testes: {total}")
        print(f"Passaram: {passed}")
        print(f"Falharam: {total - passed}")

        if failed_cases:
            print("\nCasos com falha:")
            for a, b, motivo in failed_cases:
                print(f"  - Entrada: {a} + {b} → {motivo}")