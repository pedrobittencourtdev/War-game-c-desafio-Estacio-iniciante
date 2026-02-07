//---Construir a base de dados dos territórios---//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



//---Definições de constantes---//
#define MAX_TERRITORIOS 5
#define TAM_STRING 10

// Definições de cores de texto (Foreground)
#define ANSI_COLOR_RED     "\x1b[1;31m"
#define ANSI_COLOR_GREEN   "\x1b[1;32m"
#define ANSI_COLOR_YELLOW  "\x1b[1;33m"
#define ANSI_COLOR_BLUE    "\x1b[1;34m"
#define ANSI_COLOR_MAGENTA "\x1b[1;35m"
#define ANSI_COLOR_CYAN    "\x1b[1;36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


//---Definição da estrutura Território---//

struct territorios {
    char nome[30];
    char cor_Exercito[10];
    int num_Tropas;
};

//---Função para limpar o buffer do teclado---//

void limparBuffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
};

//---Função principal---//

int main() {

    struct territorios lista_Territorios[MAX_TERRITORIOS];
    int totalTerritorios = 0;

    printf("======Cadastro do jogo WAR!======\n");

    while (totalTerritorios < MAX_TERRITORIOS){
        printf("\nDigite o nome do território: ");
        fgets(lista_Territorios[totalTerritorios].nome, TAM_STRING, stdin);

        printf("Digite a cor do exército: ");
        fgets(lista_Territorios[totalTerritorios].cor_Exercito, TAM_STRING, stdin);
       
        lista_Territorios[totalTerritorios].nome[strcspn(lista_Territorios[totalTerritorios].nome, "\n")] = 0;
        lista_Territorios[totalTerritorios].cor_Exercito[strcspn(lista_Territorios[totalTerritorios].cor_Exercito, "\n")] = 0;


        printf("Digite o número de tropas: ");
        scanf("%d", &lista_Territorios[totalTerritorios].num_Tropas);
        printf("\nTerritório cadastrado com sucesso!\n\n");
        limparBuffer();
        totalTerritorios++;
    }
    printf("======Lista de Territórios Cadastrados======\n");
    for (int i = 0; i < totalTerritorios; i++) {
        // Criamos uma cópia temporária da cor para converter para minúsculo
        char corTemp[20];
        strcpy(corTemp, lista_Territorios[i].cor_Exercito);
        
        for(int j = 0; corTemp[j]; j++) {
            corTemp[j] = tolower(corTemp[j]); // Transforma cada letra em minúscula
        }
        char *cor_formatacao = ANSI_COLOR_RESET;

        // Lógica de comparação para definir a cor
        if (strcmp(corTemp, "vermelho") == 0) cor_formatacao = ANSI_COLOR_RED;
        else if (strcmp(corTemp, "verde") == 0) cor_formatacao = ANSI_COLOR_GREEN;
        else if (strcmp(corTemp, "azul") == 0) cor_formatacao = ANSI_COLOR_BLUE;
        else if (strcmp(corTemp, "amarelo") == 0) cor_formatacao = ANSI_COLOR_YELLOW;
        else if (strcmp(corTemp, "magenta") == 0) cor_formatacao = ANSI_COLOR_MAGENTA;
        else if (strcmp(corTemp, "ciano") == 0) cor_formatacao = ANSI_COLOR_CYAN;
        else cor_formatacao = ANSI_COLOR_RESET;

        printf("\nTERRITÓRIO %d:\n", i + 1);
        printf("  -Nome: %s%s%s\n",cor_formatacao, lista_Territorios[i].nome, ANSI_COLOR_RESET);
        printf("  -Dominado por exército %s%s%s\n", cor_formatacao, lista_Territorios[i].cor_Exercito, ANSI_COLOR_RESET);
        printf("  -Tropas: %s%d%s\n", cor_formatacao, lista_Territorios[i].num_Tropas, ANSI_COLOR_RESET);
    }


return 0;

}




