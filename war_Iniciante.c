//---Construir a base de dados dos territórios---//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>
#include <unistd.h>
#include <locale.h>

// Definições de cores de texto (Foreground)
#define ANSI_COLOR_RED     "\x1b[1;31m"
#define ANSI_COLOR_GREEN   "\x1b[1;32m"
#define ANSI_COLOR_YELLOW  "\x1b[1;33m"
#define ANSI_COLOR_BLUE    "\x1b[1;34m"
#define ANSI_COLOR_MAGENTA "\x1b[1;35m"
#define ANSI_COLOR_CYAN    "\x1b[1;36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//DEfinição de cores de fundo (Background)
#define ANSI_BG_RED     "\x1b[41m"
#define ANSI_BG_GREEN   "\x1b[42m"
#define ANSI_BG_YELLOW  "\x1b[43m"
#define ANSI_BG_BLUE    "\x1b[44m"
#define ANSI_BG_MAGENTA "\x1b[45m"
#define ANSI_BG_CYAN    "\x1b[46m"
#define ANSI_BG_RESET   "\x1b[0m"


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

 CorMapa mapaCores[] = {
        {"vermelho", ANSI_COLOR_RED},
        {"verde",    ANSI_COLOR_GREEN},
        {"azul",     ANSI_COLOR_BLUE},
        {"amarelo",  ANSI_COLOR_YELLOW},
        {"magenta",  ANSI_COLOR_MAGENTA},
        {"ciano",    ANSI_COLOR_CYAN}
    };// Tabela de referência para as cores

void obterCodigoCor(char* corExército, CorMapa* mapaCores, char* saida);
void limparBuffer();
void quantidadeTerritorios(int* totalTerritorios);
void cadastrarTerritorios(territorios* lista_Territorios, int totalTerritorios);
void exibirMapa(territorios* lista_Territorios, int totalTerritorios);
void menuBatalha( territorios* lista_Territorios, int totalTerritorios);
void lerEscolhas(int* escolha1, int* escolha2, int totalTerritorios);
void atacar(territorios* atacante, territorios* defensor);
void eliminarTerritorio(territorios* lista_Territorios, int* totalTerritorios, int indice);
void liberarMemoria(territorios* mapa);


//---Função principal---//

int main() {

    setlocale(LC_ALL, "Portuguese"); // Configura a localidade para Português
    SetConsoleOutputCP(CP_UTF8); // Configura a saída do console para UTF-8 para suportar caracteres acentuados
    srand(time(NULL)); // Inicializa a semente para números aleatórios
    int escolha1, escolha2; // Variáveis para armazenar as escolhas dos jogadores


    territorios *t= NULL; 
    territorios *lista_Territorios;
    int totalTerritorios;

    quantidadeTerritorios(&totalTerritorios);

    // Aloca memória para a lista de territórios
    lista_Territorios = (territorios*)calloc(totalTerritorios, sizeof(territorios));
    // Verifica se a alocação foi bem-sucedida
    if (lista_Territorios == NULL) {
        printf(ANSI_COLOR_RED"Erro ao alocar memória para os territórios.\n" ANSI_COLOR_RESET);
        return 1;
    }
 
    // Loop para cadastrar os territórios
    cadastrarTerritorios(lista_Territorios, totalTerritorios);
    

    // Loop para exibir o mapa do mundo e iniciar as batalhas
    int opcao;

    do {
        exibirMapa(lista_Territorios, totalTerritorios);
        menuBatalha(lista_Territorios, totalTerritorios);
        lerEscolhas(&escolha1, &escolha2, totalTerritorios);
        atacar(&lista_Territorios[ escolha1 - 1], &lista_Territorios[escolha2 - 1]);

        // --- NOVO: Verifica quem morreu após o ataque e elimina ---
        // Verifica o defensor
        if (lista_Territorios[escolha2 - 1].num_Tropas <= 0) {
            eliminarTerritorio(lista_Territorios, &totalTerritorios, escolha2 - 1);
        } 
        // Verifica o atacante
        else if (lista_Territorios[escolha1 - 1].num_Tropas <= 0) {
            eliminarTerritorio(lista_Territorios, &totalTerritorios, escolha1 - 1);
        }
    } while (opcao != 0); // Loop infinito para permitir múltiplas batalhas

liberarMemoria(lista_Territorios);

return 0;
}


//---FUNÇÕES DO JOGO---//

void limparBuffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void quantidadeTerritorios(int* totalTerritorios) {

    do {
        printf("\nQuantos territórios deseja cadastrar? (Minimo de 2): ");
        scanf("%d", totalTerritorios);
        limparBuffer(); // Limpa o buffer após ler o número
            if (*totalTerritorios < 2) {
                printf(ANSI_COLOR_RED "\nPor favor, cadastre pelo menos 2 territórios para iniciar o jogo.\n" ANSI_COLOR_RESET);
                printf("\n");
            }
    } while (*totalTerritorios < 2);
}

void cadastrarTerritorios(territorios* lista_Territorios, int totalTerritorios){
    int contador = 0;

    while (contador < totalTerritorios){

        territorios* t = &lista_Territorios[contador];
        
        printf("\nDigite o nome do território: ");
        fgets(t ->nome, 30, stdin);

        printf("Digite a cor do exército: ");
        fgets(t ->cor_Exercito, 10, stdin);
       
        t ->nome[strcspn(t ->nome, "\n")] = '\0'; // Remove o caractere de nova linha
        t ->cor_Exercito[strcspn(t ->cor_Exercito, "\n")] = '\0'; // Remove o caractere de nova linha

        printf("Digite o número de tropas: ");
        scanf("%d", &t ->num_Tropas);
        printf("\n");
        printf(ANSI_COLOR_YELLOW"Aguarde um momento enquanto processamos o cadastro...\n" ANSI_COLOR_RESET);
        Sleep(700); // Pausa de 0.7 segundos para criar uma sensação de processamento
        printf(ANSI_COLOR_GREEN"\nTerritório cadastrado com sucesso!\n\n" ANSI_COLOR_RESET);
        limparBuffer();
        contador++;
    }

}

void exibirMapa(territorios* lista_Territorios, int totalTerritorios){
    printf("\n");
        printf(ANSI_COLOR_YELLOW"Processando o mapa do mundo atual...\n" ANSI_COLOR_RESET);
            Sleep(1000); // Pausa de 1 segundo para criar uma sensação de processamento
        // Exibe o mapa do mundo atual
        printf(ANSI_COLOR_BLUE"========================================\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BLUE"          MAPA DO MUNDO ATUAL     \n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BLUE"========================================\n" ANSI_COLOR_RESET);

        // Exibe os territórios cadastrados
        for (int i = 0; i < totalTerritorios; i++) {
        territorios* t = &lista_Territorios[i];

        char cor_formatacao[100]; // Buffer para armazenar o código de formatação da cor
        obterCodigoCor(t->cor_Exercito, mapaCores, cor_formatacao); // Obtém o código de formatação para a cor do exército

            // Exibe o território com a formatação de cor
            printf("%d - %s (Exército %s%s%s, Tropas: %d)\n", i + 1,  
                t->nome, cor_formatacao, t->cor_Exercito, ANSI_COLOR_RESET, t->num_Tropas);
        }

    
}

void menuBatalha (territorios* lista_Territorios, int totalTerritorios){
        int opcao;
    // Exibe as opções de batalha
        printf("\n");
        printf(ANSI_BG_BLUE"INICIAR CONFRONTO" ANSI_BG_RESET);
        printf("\n");
        printf(ANSI_COLOR_BLUE"\n1 - Realizar ataque\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_RED"0 - Sair\n"ANSI_COLOR_RESET);
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer();
}

void lerEscolhas(int* escolha1, int* escolha2, int totalTerritorios) {
    printf("\nEscolha os territórios para o confronto:\n");
    printf("Escolha o território do jogador 1 (1 a %d): ", totalTerritorios);
    scanf("%d", escolha1);
    printf("Escolha o território do jogador 2 (1 a %d): ", totalTerritorios);
    scanf("%d", escolha2);
    limparBuffer();

    // Validação das escolhas
    if (*escolha1 < 1 || *escolha1 > totalTerritorios || *escolha2 < 1 || *escolha2 > totalTerritorios) {
        printf(ANSI_COLOR_RED"Escolha inválida. Encerrando o jogo.\n" ANSI_COLOR_RESET);
        exit(1); // Encerra o programa com código de erro
    }
}

void atacar(territorios* atacante, territorios* defensor){
    if (strcmp(atacante->cor_Exercito, defensor->cor_Exercito) == 0) {
        printf(ANSI_COLOR_RED"Não é possível atacar um território do mesmo exército.\n" ANSI_COLOR_RESET);
        return;
    }

    //simulação de ataque usando dados
    int dadosAtacante = (rand() % 6) + 1; // Gera um número aleatório entre 1 e 6
    int dadosDefensor = (rand() % 6) + 1; // Gera um número aleatório entre 1 e 6

    printf("\nRESULTADO DA BATALHA:\n");
// Loop para criar a animação rápida
for (int i = 0; i < 15; i++) {
    int simulaAtaque = (rand() % 6) + 1;
    int simulaDefesa = (rand() % 6) + 1;
    
    // O \r no começo faz o texto sobrescrever a si mesmo na mesma linha
    printf("\rGirando... Atacante: [%d] | Defensor: [%d]", simulaAtaque, simulaDefesa);
    
    // Força o terminal a mostrar o texto imediatamente
    fflush(stdout); 
    
    // Pausa bem rápida de 100 milissegundos (0.2 segundo)
    Sleep(200); 
}

// Quando o loop acaba, limpamos a linha da animação com espaços e imprimimos o resultado real
printf("\r                                                  \r");

    printf("Atacante (%s) rolou: %d\n", atacante->nome, dadosAtacante);
    printf("Defensor (%s) rolou: %d\n", defensor->nome, dadosDefensor);

    if(dadosAtacante > dadosDefensor){
        printf(ANSI_COLOR_GREEN"\nO atacante venceu a batalha!\n" ANSI_COLOR_RESET);
        defensor->num_Tropas -= 1; // O defensor perde uma tropa
        atacante->num_Tropas += 1; // O atacante ganha uma tropa
        if (defensor->num_Tropas <= 0) {
            printf(ANSI_COLOR_GREEN"\nO território %s (%s) foi conquistado por %s (%s)!\n" ANSI_COLOR_RESET, defensor->nome, defensor->cor_Exercito, atacante->cor_Exercito, atacante->nome);
        }
    } else if(dadosAtacante < dadosDefensor){
        printf(ANSI_COLOR_GREEN"\nO defensor venceu a batalha!\n" ANSI_COLOR_RESET);
        atacante->num_Tropas -= 1; // O atacante perde uma tropa
        defensor->num_Tropas += 1; // O defensor ganha uma tropa
        if (atacante->num_Tropas <= 0) {
            printf(ANSI_COLOR_GREEN"\nO território %s (%s) foi conquistado pelo exército %s (%s)!\n" ANSI_COLOR_RESET, atacante->nome, atacante->cor_Exercito, defensor->cor_Exercito, defensor->nome);
        }
    } else {
        printf(ANSI_COLOR_YELLOW"\nEmpate! Nenhuma tropa é perdida.\n" ANSI_COLOR_RESET);
    }
}

void eliminarTerritorio(territorios* lista_Territorios, int* totalTerritorios, int indice) {
    printf(ANSI_COLOR_RED"\nO território %s (%s) perdeu todas tropas e foi removido do jogo!\n" ANSI_COLOR_RESET, lista_Territorios[indice].nome, lista_Territorios[indice].cor_Exercito);
    // Remove o território do jogo
    for (int i = indice; i < (*totalTerritorios) - 1; i++) {
        lista_Territorios[i] = lista_Territorios[i + 1];
    }
    (*totalTerritorios)--; // Reduz o número total de territórios

    if (*totalTerritorios == 1) {
        printf(ANSI_COLOR_GREEN"O território %s (%s) é o único restante e foi declarado vencedor!\n" ANSI_COLOR_RESET, lista_Territorios[0].nome, lista_Territorios[0].cor_Exercito);
        printf("\n");
        printf("=======================================");
        printf("\n");
        printf(ANSI_BG_GREEN"     Exército %s (%s) WINNER!    " ANSI_BG_RESET, lista_Territorios[0].cor_Exercito, lista_Territorios[0].nome);
        printf("\n");
        printf("=======================================");
            exit(0); // Encerra o programa
    }
}

void liberarMemoria(territorios* mapa) {
    free(mapa);
    printf(ANSI_COLOR_GREEN"\nMemoria liberada. Ate a proxima!\n" ANSI_COLOR_RESET);
}

void obterCodigoCor(char* corExército, CorMapa* mapaCores, char* saida) {
    char corTemp[20];
    strcpy(corTemp, corExército);

    // Converte para minúsculo
    for (int j = 0; corTemp[j]; j++) {
        corTemp[j] = tolower(corTemp[j]);
    }

    // Busca o código na sua struct mapaCores
    for (int k = 0; k < 6; k++) {
        if (strcmp(corTemp, mapaCores[k].nome) == 0) {
            strcpy(saida, mapaCores[k].codigo); // Copia o código de formatação para a variável de saída
        }
    }

    strcpy(saida, ANSI_COLOR_RESET); // Cor padrão caso não encontre
}









