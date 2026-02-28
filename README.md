# 🌍 WAR! - Gerenciamento de Territórios

Este é um projeto desenvolvido em **Linguagem C** para o gerenciamento simplificado de territórios do jogo WAR. O sistema permite o cadastro técnico e a listagem visual dos dados através de uma interface via terminal colorida.

## 🚀 Objetivo

O projeto foi criado para consolidar conceitos avançados de programação estruturada, tais como:

* **Manipulação de Structs** para modelagem de dados dos territórios.
* **Uso de Ponteiros** para navegação eficiente em arrays de estruturas.
* **Gerenciamento de entrada/saída** e tratamento de **buffer de teclado**.
* **Uso de funções da biblioteca string.h** (como `strcspn` para limpeza de strings e `strcmp` para lógica de cores).
* **Alocação dinâmica de memória** utilização de malloc e calloc.
* **Modularização**



## 🛠️ Funcionalidades

* **Cadastrar Território:** Registra nome, cor do exército e quantidade de tropas.
* **Listar Territórios:** Exibe os dados cadastrados com formatação ANSI dinâmica (cores no terminal).
* **Tratamento de Dados:** Remoção automática do `\n` e limpeza de buffer para evitar erros de leitura.
* **Sistema de ataque:** Simula um ataque, utilizando números aleatórios, como se fosses dados de uma batalha.
* **Atualização de dados:** o território que vencer a rodada recebe uma tropa a mais, o perdedor perde uma tropa, caso chegue a 0 tropas o território é eliminado, o último território restante vence o jogo.
* **Exibição pós-ataque:** o sistema deve exibir os dados atualizados dos territórios após cada ataque.
* **INTERFACE INTUITIVA**
* **Missões**

## 💻 Como executar:

* Para rodar este projeto, você precisará de um compilador de C (GCC, Clang ou o próprio VS Code configurado).

## 📥 Como baixar e testar o projeto

Se você quiser testar este sistema na sua máquina, siga os passos abaixo:

1. **Clone o repositório:**
   ```bash
   git clone https://github.com/pedrobittencourtdev/War-game-c-desafio-Estacio-iniciante.git

2. **Entre no terminal e na pasta:**
   ```bash
   cd War-game-c-desafio-Estacio-iniciante

3. **Compile o código:**
   ```bash
   gcc war.c -o war

4. **Execute:**
   ```bash
   .\war.exe


## 👨‍💻 Autor
Desenvolvido por Pedro Bittencourt como parte da jornada de aprendizado em Engenharia de Software.
Sinta-se à vontade para clonar, testar e sugerir melhorias!


