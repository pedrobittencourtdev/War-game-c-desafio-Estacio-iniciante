//---Construir a base de dados dos territórios---//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



//---Definições de constantes---//
#define MAX_TERRITORIOS 5
#define TAM_STRING 30

// Definições de cores de texto (Foreground)
#define ANSI_COLOR_RED     "\x1b[1;31m"
#define ANSI_COLOR_GREEN   "\x1b[1;32m"
#define ANSI_COLOR_YELLOW  "\x1b[1;33m"
#define ANSI_COLOR_BLUE    "\x1b[1;34m"
#define ANSI_COLOR_MAGENTA "\x1b[1;35m"
#define ANSI_COLOR_CYAN    "\x1b[1;36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


//---Definição da estrutura Território---//

typedef struct {
    char nome[30];
    char cor_Exercito[10];
    int num_Tropas;
} territorios;

// Struct para facilitar a busca de cores
typedef struct {
    char *nome;
    char *codigo;
} CorMapa;

//---Função para limpar o buffer do teclado---//

void limparBuffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
};

//---Função principal---//

int main() {

  
    territorios *t= NULL;
    territorios lista_Territorios[MAX_TERRITORIOS];
    int totalTerritorios = 0;

    // Tabela de referência para as cores
    CorMapa mapaCores[] = {
        {"vermelho", ANSI_COLOR_RED},
        {"verde",    ANSI_COLOR_GREEN},
        {"azul",     ANSI_COLOR_BLUE},
        {"amarelo",  ANSI_COLOR_YELLOW},
        {"magenta",  ANSI_COLOR_MAGENTA},
        {"ciano",    ANSI_COLOR_CYAN}
    };

    printf("======Cadastro do jogo WAR!======\n");

    while (totalTerritorios < MAX_TERRITORIOS){

        t = &lista_Territorios[totalTerritorios];
        
        printf("\nDigite o nome do território: ");
        fgets(t ->nome, TAM_STRING, stdin);

        printf("Digite a cor do exército: ");
        fgets(t ->cor_Exercito, TAM_STRING, stdin);
       
        t ->nome[strcspn(t ->nome, "\n")] = '\0'; // Remove o caractere de nova linha
        t ->cor_Exercito[strcspn(t ->cor_Exercito, "\n")] = '\0'; // Remove o caractere de nova linha

        printf("Digite o número de tropas: ");
        scanf("%d", &t ->num_Tropas);
        printf("\nTerritório cadastrado com sucesso!\n\n");
        limparBuffer();
        totalTerritorios++;
    }
    printf("======Lista de Territórios Cadastrados======\n");
    for (int i = 0; i < totalTerritorios; i++) {

        t = &lista_Territorios[i];
        // Criamos uma cópia temporária da cor para converter para minúsculo
        char corTemp[20];
        strcpy(corTemp, t->cor_Exercito);
        
        for(int j = 0; corTemp[j]; j++) {
            corTemp[j] = tolower(corTemp[j]); // Transforma cada letra em minúscula
        }
        // Busca simplificada da cor usando ponteiro
        char *cor_formatacao = ANSI_COLOR_RESET;
        for (int k = 0; k < 6; k++) {
            if (strcmp(corTemp, mapaCores[k].nome) == 0) {
                cor_formatacao = mapaCores[k].codigo;
                break;
            }
        }

        printf("\nTERRITÓRIO %d:\n", i + 1);
        printf("  -Nome: %s%s%s\n",cor_formatacao, t->nome, ANSI_COLOR_RESET);
        printf("  -Dominado por exército %s%s%s\n", cor_formatacao, t->cor_Exercito, ANSI_COLOR_RESET);
        printf("  -Tropas: %s%d%s\n", cor_formatacao, t->num_Tropas, ANSI_COLOR_RESET);
    }


return 0;

}




