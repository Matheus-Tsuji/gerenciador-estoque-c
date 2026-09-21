# 📦 Gerenciador de Estoque em C

<p align="center">
  <img src="https://img.shields.io/badge/Linguagem-C-00599C?style=for-the-badge&logo=c&logoColor=white" alt="Linguagem C" />
  <img src="https://img.shields.io/badge/Plataforma-Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white" alt="Windows" />
  <img src="https://img.shields.io/badge/IDE-Code%3A%3ABlocks-2A7B9B?style=for-the-badge" alt="CodeBlocks" />
  <img src="https://img.shields.io/badge/Status-Conclu%C3%ADdo-brightgreen?style=for-the-badge" alt="Status" />
</p>

Um sistema completo, leve e intuitivo de **Gerenciador de Estoque** desenvolvido em **Linguagem C**. Criado para rodar direto no terminal com formatação rica via códigos de escape ANSI (cores vibrantes e destaques visuais), além de persistência de dados em arquivo de texto formatado (`produtos.txt`).

---

## 📑 Sumário

- [Visão Geral](#-visão-geral)
- [Funcionalidades](#-funcionalidades)
- [Estrutura dos Dados](#-estrutura-dos-dados)
- [Como Executar na sua Máquina](#-como-executar-na-sua-máquina)
  - [Opção 1: Executar Direto (Sem compilar nada)](#1-executar-direto-sem-compilar-nada)
  - [Opção 2: Compilar do Zero via Terminal (GCC)](#2-compilar-do-zero-via-terminal-gcc)
  - [Opção 3: Abrir e Compilar pelo Code::Blocks](#3-abrir-e-compilar-pelo-codeblocks)
- [Estrutura do Repositório](#-estrutura-do-repositório)
- [Destaques Técnicos do Código](#-destaques-técnicos-do-código)
- [Autor](#-autor)

---

## 🎯 Visão Geral

O projeto resolve o problema clássico de controle de estoque de produtos em um comércio ou empresa, oferecendo operações completas de entrada, listagem, busca, ajuste dinâmico de quantidades e consolidação financeira de patrimônio em estoque. 

Todo o fluxo é guiado por um **menu interativo no console**, com feedback visual colorido para indicar sucessos, avisos ou erros.

---

## ✨ Funcionalidades

| Opção | Ação | Descrição |
| :---: | :--- | :--- |
| **`[1]`** | **Cadastrar Produto** | Permite inserir código, nome, categoria, quantidade e preço unitário. Salva automaticamente no arquivo. |
| **`[2]`** | **Listar Produtos** | Exibe todos os itens cadastrados de forma organizada e limpa na tela. |
| **`[3]`** | **Buscar Produto** | Localiza rapidamente os dados de um item a partir do seu código único. |
| **`[4]`** | **Calcular Valor de Estoque** | Itera por todos os registros e calcula o valor total acumulado do estoque (`quantidade × preço`). |
| **`[5]`** | **Atualizar Estoque** | Permite ajustar a quantidade em estoque de um produto existente (adicionando ou subtraindo unidades). |
| **`[0]`** | **Sair** | Encerra a aplicação com segurança. |

---

## 💾 Estrutura dos Dados

Os dados cadastrados são persistidos no arquivo [`produtos.txt`](produtos.txt) utilizando o separador ponto e vírgula (`;`):

```text
codigo;nome;categoria;quantidade;preco
```

**Exemplo real armazenado:**
```text
101;Mouse;Periferico;13;130.869995
102;Teclado;Periferico;12;205.990005
103;Microfone;Eletronico;10;368.989990
```

> 📌 **Vantagem:** O formato é compatível com abertura direta em planilhas (como Microsoft Excel e LibreOffice Calc) através do formato CSV delimitado por `;`.

---

## 🚀 Como Executar na sua Máquina

Você pode rodar este projeto de três maneiras bem simples:

### 1. Executar Direto (Sem compilar nada)
O repositório já inclui o executável pronto para Windows: **`gerenciador_estoque.exe`**.

1. Clone ou baixe este repositório:
   ```bash
   git clone https://github.com/Matheus-Tsuji/gerenciador-estoque-c.git
   cd gerenciador-estoque-c
   ```
2. Execute no PowerShell ou Prompt de Comando (CMD):
   ```powershell
   .\gerenciador_estoque.exe
   ```
   *(Ou se preferir, basta dar um duplo clique no arquivo `gerenciador_estoque.exe` pelo Explorador de Arquivos do Windows).*

---

### 2. Compilar do Zero via Terminal (GCC)

Caso queira recompilar o código-fonte manualmente (no Windows com MinGW ou no Linux/macOS):

1. Verifique se possui o compilador `gcc` instalado:
   ```bash
   gcc --version
   ```
2. Compile o código:
   ```bash
   gcc -Wall -Wextra -O2 main.c -o gerenciador_estoque.exe
   ```
3. Execute o programa gerado:
   ```bash
   .\gerenciador_estoque.exe
   ```

---

### 3. Abrir e Compilar pelo Code::Blocks

Se você utiliza a IDE **Code::Blocks**:

1. Abra o arquivo de projeto [`trabalhoCCalin.cbp`](trabalhoCCalin.cbp) no Code::Blocks;
2. Pressione **F9** (ou clique no menu **Build** > **Build and run**);
3. O console abrirá pronto para receber os comandos.

---

## 📂 Estrutura do Repositório

```text
gerenciador-estoque-c/
├── .gitignore              # Configuração de arquivos ignorados pelo Git
├── README.md               # Documentação completa do projeto
├── gerenciador_estoque.exe # Executável pronto para uso no Windows
├── main.c                  # Código-fonte principal com comentários detalhados
├── produtos.txt            # Base de dados em formato de texto delimitado
└── trabalhoCCalin.cbp      # Arquivo de configuração de projeto do Code::Blocks
```

---

## 🛠️ Destaques Técnicos do Código

- **Atualização Segura de Dados com Arquivo Temporário**: A função `atualizarEstoque` utiliza o padrão seguro de leitura e reescrita através de `temporario.txt`, substituindo o original atomicamente com `remove()` e `rename()`, prevenindo corrupção de dados.
- **Tratamento de Buffers de Entrada**: Evita o clássico problema de pulo de leituras em C limpando o buffer com loops `while (getchar() != '\n')` e `getchar()` após o uso de `scanf`.
- **Sanitização de Strings com `strcspn`**: Substitui o caractere de quebra de linha (`\n`) capturado pelo `fgets` por `\0`, garantindo que nomes e categorias fiquem limpos na gravação e na exibição.
- **Leitura Formatada com Expressões de Conjunto no `fscanf`**:
  ```c
  fscanf(arquivo, "%d;%49[^;];%24[^;];%d;%f\n", &p.codigo, p.nome, p.categoria, &p.quantidade, &p.preco)
  ```
  Permite ler nomes e categorias com espaços sem quebrar o delimitador `;`.
- **Cores ANSI**: Macros customizadas que injetam códigos de escape ANSI diretamente nos `printf` (`VERMELHO`, `VERDE`, `CIANO`, `AMARELO`, `BOLD` e `RESET`), sem depender de bibliotecas externas complexas como `windows.h` ou `conio.h`.

---

## 👨‍💻 Autor

Desenvolvido por **[Matheus Tsuji Carneiro](https://github.com/Matheus-Tsuji)**.

Se este projeto te ajudou ou serviu de referência para estudos de C, sinta-se à vontade para deixar uma ⭐️ no repositório!
