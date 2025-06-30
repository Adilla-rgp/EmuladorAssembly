# 🧠 SimCPU - Emulador Assembly

Este projeto é um emulador de uma arquitetura fictícia de processador desenvolvido em C, com suporte a programas em Assembly, montador automático, interface interativa e testes automatizados.

---

## ✅ Requisitos

- Python 3
- GCC (compilador C)
- Terminal de linha de comando (Linux/Mac/Windows com Bash ou PowerShell)

---

## 🗂️ Estrutura do Projeto

| Arquivo          | Função                                           |
|------------------|--------------------------------------------------|
| `emulador.c`     | Emulador do processador fictício em C           |
| `assembler.py`   | Traduz `.asm` para `.bin`                        |
| `environment.py` | Executa testes automáticos                       |
| `interface.py`   | Interface interativa via terminal                |
| `soma.asm`       | Exemplo de programa em Assembly                  |
| `template.asm`   | Template para gerar testes dinâmicos             |

---

## ⚙️ Compilar o Emulador

No terminal, execute:

```bash
gcc emulador.c -o emulador
```

No Windows, será criado `emulador.exe`.

---

## 💻 Interface Interativa

Rode o menu interativo com:

```bash
python interface.py
```

Você verá:

```
=== SimCPU Emulator Interface ===
1. Montar programa (.asm → .bin)
2. Executar programa (.bin)
3. Testes automáticos
4. Sair
```

---

## 🛠️ Montar Programa `.asm`

1. Escolha a opção `1` no menu da interface.
2. Digite o nome do arquivo `.asm` (ex: `soma.asm`)
3. Digite o nome do arquivo `.bin` de saída (ex: `soma.bin`)

---

## ▶️ Executar Arquivo `.bin`

Escolha a opção `2` na interface e insira o nome do arquivo binário gerado:

```bash
soma.bin
```

A execução exibirá o estado dos registradores, flags e saída em PORT1.

---

## 🧪 Testes Automáticos

Para rodar todos os testes disponíveis, escolha a opção `3` na interface ou rode diretamente:

```bash
python environment.py
```

Exemplo de saída:

```
Iniciando testes automáticos...
Teste: 5 + 7 = 12 -> OK
Teste: 10 + 20 = 30 -> OK
```

---

## 💾 Exemplo `.asm` - Soma

```asm
; soma.asm
LOAD R0, 5
LOAD R1, 7
ADD R0, R1
OUT R0, 1 ; envia para PORT1
HALT
```

---

## 🧠 Arquitetura Fictícia

- **Registradores:** R0–R7
- **Memória:** 4096 bytes
- **Instruções:**
  - `LOAD`, `STORE`, `LOADM`
  - `ADD`, `SUB`, `CMP`
  - `JMP`, `JZ`, `PUSH`, `POP`
  - `IN`, `OUT`, `NOP`, `HALT`
- **Flags:** Zero, Negativo, Carry

---

## 📦 Como Rodar do Zero (Resumo)

```bash
# Compilar
gcc emulador.c -o emulador

# Rodar interface
python interface.py

# Ou testar diretamente
python environment.py
```

---

## 🚀 Créditos

Desenvolvido para o projeto "Arquiteturas de Processadores". 
