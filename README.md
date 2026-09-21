# Gerenciador de Estoque em C

Sistema simples e eficiente em linguagem C para gerenciamento de estoque em terminal, com persistência de dados em arquivo de texto.

## Funcionalidades

- **Cadastrar Produto**: Registro de novos itens (código, nome, categoria, quantidade e preço).
- **Listar Produtos**: Visualização de todos os itens cadastrados no estoque.
- **Buscar Produto**: Localização de um produto específico através de seu código.
- **Calcular Valor de Estoque**: Cálculo do montante financeiro total investido no estoque.
- **Interface Colorida**: Destaques visuais no terminal utilizando sequências de escape ANSI.
- **Persistência em Arquivo**: Gravação e leitura automática dos dados no arquivo `produtos.txt`.

## Como compilar e executar

### Via GCC (Linha de Comando)
```bash
gcc -o gerenciador main.c
./gerenciador
```

### Via Code::Blocks
Abra o arquivo de projeto `trabalhoCCalin.cbp` diretamente no Code::Blocks e clique em **Build and Run** (F9).
