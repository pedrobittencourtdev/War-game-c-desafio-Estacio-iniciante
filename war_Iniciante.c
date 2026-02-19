//---Construir a base de dados dos territórios---//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>




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

const char* obterCodigoCor(char* corExército, CorMapa* mapaCores) {
    char corTemp[20];
    strcpy(corTemp, corExército);

    // Converte para minúsculo
    for (int j = 0; corTemp[j]; j++) {
        corTemp[j] = tolower(corTemp[j]);
    }

    // Busca o código na sua struct mapaCores
    for (int k = 0; k < 6; k++) {
        if (strcmp(corTemp, mapaCores[k].nome) == 0) {
            return mapaCores[k].codigo;
        }
    }

    return ANSI_COLOR_RESET; // Cor padrão caso não encontre
}

 
//---Função para limpar o buffer do teclado---//

void limparBuffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void atacar(territorios* atacante, territorios* defensor){
    if (strcmp(atacante->cor_Exercito, defensor->cor_Exercito) == 0) {
        printf("Não é possível atacar um território do mesmo exército.\n");
        return;
    }

    //simulação de ataque usando dados
    int dadosAtacante = (rand() % 6) + 1; // Gera um número aleatório entre 1 e 6
    int dadosDefensor = (rand() % 6) + 1; // Gera um número aleatório entre 1 e 6

    printf("\nRESULTADO DA BATALHA:\n");

    printf("Atacante (%s) rolou: %d\n", atacante->nome, dadosAtacante);
    printf("Defensor (%s) rolou: %d\n", defensor->nome, dadosDefensor);

    if(dadosAtacante > dadosDefensor){
        printf("\nO atacante venceu a batalha!\n");
        defensor->num_Tropas -= 1; // O defensor perde uma tropa
        atacante->num_Tropas += 1; // O atacante ganha uma tropa
        if (defensor->num_Tropas <= 0) {
            printf("\nO território %s foi conquistado por %s!\n", defensor->nome, atacante->cor_Exercito);
        }
    } else if(dadosAtacante < dadosDefensor){
        printf("\nO defensor venceu a batalha!\n");
        atacante->num_Tropas -= 1; // O atacante perde uma tropa
        defensor->num_Tropas += 1; // O defensor ganha uma tropa
        if (atacante->num_Tropas <= 0) {
            printf("\nO território %s foi conquistado pelo exército %s!\n", atacante->nome, defensor->cor_Exercito);
        }
    } else {
        printf("\nEmpate! Nenhuma tropa é perdida.\n");
    }
}

void eliminarTerritorio(territorios* lista_Territorios, int* totalTerritorios, int indice) {
    printf("O território %s foi conquistado e removido do jogo!\n", lista_Territorios[indice].nome);
    // Remove o território do jogo
    for (int i = indice; i < (*totalTerritorios) - 1; i++) {
        lista_Territorios[i] = lista_Territorios[i + 1];
    }
    (*totalTerritorios)--; // Reduz o número total de territórios

    if (*totalTerritorios == 1) {
        printf("O território %s é o único restante e foi declarado vencedor!\n", lista_Territorios[0].nome);
        exit(0); // Encerra o programa
    }
}




void liberarMemoria(territorios* mapa) {
    free(mapa);
    printf("\nMemoria liberada. Ate a proxima!\n");
}



//---Função principal---//

int main() {

    srand(time(NULL)); // Inicializa a semente para números aleatórios

  
    territorios *t= NULL;
    territorios *lista_Territorios;

    int totalTerritorios;

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

    printf("Quantos territórios deseja cadastrar? ");
    scanf("%d", &totalTerritorios);
    limparBuffer(); // Limpa o buffer após ler o número

    // Aloca memória para a lista de territórios

    lista_Territorios = (territorios*)calloc(totalTerritorios, sizeof(territorios));

    // Verifica se a alocação foi bem-sucedida
    if (lista_Territorios == NULL) {
        printf("Erro ao alocar memória para os territórios.\n");
        return 1;
    }

    int contador = 0;

    while (contador < totalTerritorios){

        t = &lista_Territorios[contador];
        
        printf("\nDigite o nome do território: ");
        fgets(t ->nome, 30, stdin);

        printf("Digite a cor do exército: ");
        fgets(t ->cor_Exercito, 10, stdin);
       
        t ->nome[strcspn(t ->nome, "\n")] = '\0'; // Remove o caractere de nova linha
        t ->cor_Exercito[strcspn(t ->cor_Exercito, "\n")] = '\0'; // Remove o caractere de nova linha

        printf("Digite o número de tropas: ");
        scanf("%d", &t ->num_Tropas);
        printf("\nTerritório cadastrado com sucesso!\n\n");
        limparBuffer();
        contador++;
    }

        int opcao;

        do {

        

        // Exibe o mapa do mundo atual
        printf("========================================\n");
        printf("          MAPA DO MUNDO ATUAL     \n");
        printf("========================================\n");

        // Exibe os territórios cadastrados
        for (int i = 0; i < totalTerritorios; i++) {
        t = &lista_Territorios[i];

        
        const char*cor_formatacao = obterCodigoCor(t->cor_Exercito, mapaCores); // Obtém o código de formatação para a cor do exército

            printf("%d - %s (Exercito %s%s%s, Tropas: %d)\n", i + 1,  
                t->nome, cor_formatacao, 
                t->cor_Exercito, ANSI_COLOR_RESET, t->num_Tropas);
        }

        printf("\n======TELA DE BATALHA======\n");
        printf("\n1 - Realizar ataque\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        

         //ESCOLHA DE EQUIPES PARA O JOGO
        if (opcao == 1) {

        printf("\nEscolha os territórios para o jogo:\n");
        int escolha1, escolha2;
        printf("Escolha o território do jogador 1 (1 a %d): ", totalTerritorios);
        scanf("%d", &escolha1);
        printf("Escolha o território do jogador 2 (1 a %d): ", totalTerritorios);
        scanf("%d", &escolha2);
        limparBuffer();

        //validação das escolhas
         if (escolha1 < 1 || escolha1 > totalTerritorios || escolha2 < 1 || escolha2 > totalTerritorios) {
            printf("Escolha inválida. Encerrando o jogo.\n");
            free(lista_Territorios); // Libera a memória alocada para os territórios
            return 1;
        }
        

        atacar(&lista_Territorios[escolha1 - 1], &lista_Territorios[escolha2 - 1]);

        // --- NOVO: Verifica quem morreu após o ataque e elimina ---
        // Verifica o defensor
        if (lista_Territorios[escolha2 - 1].num_Tropas <= 0) {
            eliminarTerritorio(lista_Territorios, &totalTerritorios, escolha2 - 1);
        } 
        // Verifica o atacante
        else if (lista_Territorios[escolha1 - 1].num_Tropas <= 0) {
            eliminarTerritorio(lista_Territorios, &totalTerritorios, escolha1 - 1);
        }

    } // Fim do if (opcao == 1)
         

    } while (opcao != 0); // Loop infinito para permitir múltiplas batalhas

    liberarMemoria(lista_Territorios);

return 0;
}
