#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

#define tamanhoMatriz 3
#define maximoDeJogadas 5
#define quantidadeJogadasVencedoras 8

typedef enum SELECIONADO_POR
{
    HUMANO = -1,
    NINGUEM = 0,
    ROBO = 1
} SelecionadoPor;

typedef enum NUMERO_JOGADA
{
    PRIMEIRA = 0,
    SEGUNDA = 2,
    TERCEIRA = 4,
    QUARTA = 6,
    QUINTA = 8
} NumeroJogada;

typedef enum POSICAO_NO_TABULEIRO
{
    CANTO,
    MEIO,
    BORDA
} PosicaoNoTabuleiro;

struct Jogada
{
    int linha;
    int coluna;
    PosicaoNoTabuleiro posicaoNoTabuleiro;
};

bool menu();
void iniciaTabuleiro(int tabuleiro[][tamanhoMatriz]);
void exibeTabuleiro(int tabuleiro[][tamanhoMatriz]);
void jogar(int tabuleiro[][tamanhoMatriz], int *quantidadeVitoriasRobo, int *quantidadeEmpates);
bool checaPosicao(int tabuleiro[][tamanhoMatriz], int linha, int coluna);
bool checaLinha(int tabuleiro[][tamanhoMatriz]);
bool checaColuna(int tabuleiro[][tamanhoMatriz]);
bool checaDiagonal(int tabuleiro[][tamanhoMatriz]);
bool checaEmpate(int tabuleiro[][tamanhoMatriz]);
bool checaTermino(int tabuleiro[][tamanhoMatriz], int roboJogando, int *quantidadeVitoriasRobo, int *quantidadeEmpates);
int zeroOuUm();
int zeroOuDois();
int posicaoOposta(int posicao);
PosicaoNoTabuleiro posicaoNoTabuleiroDaJogada(int linha, int coluna);
void primeiraJogadaRobo(int *linha, int *coluna);
void ultimasJogadasRobo(int *linha, int *coluna, int tabuleiro[][tamanhoMatriz]);
void jogadaHumano(int tabuleiro[][tamanhoMatriz], int numeroJogadas, struct Jogada jogadasHumano[]);
void proximaJogada(int tabuleiro[][tamanhoMatriz], int numeroJogadas, int *roboJogando,
                   struct Jogada jogadasRobo[], struct Jogada jogadasHumano[]);
void segundaJogadaRobo(int *linha, int *coluna, int numeroJogadas,
                       struct Jogada jogadasRobo[], struct Jogada jogadasHumano[]);
void terceiraJogadaRobo(int *linha, int *coluna, int numeroJogadas,
                        struct Jogada jogadasRobo[], struct Jogada jogadasHumano[]);
void jogadaRobo(int tabuleiro[][tamanhoMatriz], int numeroJogadas,
                struct Jogada jogadasRobo[], struct Jogada jogadasHumano[]);
bool jogadaVencedora(int *linhaVencedora, int *colunaVencedora,
                     int tabuleiro[][tamanhoMatriz], SelecionadoPor vencedor);

static struct Jogada jogadasVencedoras[quantidadeJogadasVencedoras][tamanhoMatriz] = {
    {{.linha = 0, .coluna = 0},
     {.linha = 0, .coluna = 1},
     {.linha = 0, .coluna = 2}},

    {{.linha = 1, .coluna = 0},
     {.linha = 1, .coluna = 1},
     {.linha = 1, .coluna = 2}},

    {{.linha = 2, .coluna = 0},
     {.linha = 2, .coluna = 1},
     {.linha = 2, .coluna = 2}},

    {{.linha = 0, .coluna = 0},
     {.linha = 1, .coluna = 0},
     {.linha = 2, .coluna = 0}},

    {{.linha = 0, .coluna = 1},
     {.linha = 1, .coluna = 1},
     {.linha = 2, .coluna = 1}},

    {{.linha = 0, .coluna = 2},
     {.linha = 1, .coluna = 2},
     {.linha = 2, .coluna = 2}},

    {{.linha = 0, .coluna = 0},
     {.linha = 1, .coluna = 1},
     {.linha = 2, .coluna = 2}},

    {{.linha = 2, .coluna = 0},
     {.linha = 1, .coluna = 1},
     {.linha = 0, .coluna = 2}},
};

int main()
{
    time_t t;
    srand((unsigned)time(&t));

    bool continuar;
    int tabuleiro[tamanhoMatriz][tamanhoMatriz], quantidadeVitoriasRobo = 0, quantidadeEmpates = 0;

    do
    {
        continuar = menu();

        if (continuar)
        {
            jogar(tabuleiro, &quantidadeVitoriasRobo, &quantidadeEmpates);
        }

    } while (continuar);

    printf("\t===SCOREBOARD===\n\n");
    printf("\tQuantidade de jogos: %d\n", quantidadeVitoriasRobo + quantidadeEmpates);
    printf("\tNumero de vitorias: %d\n", 0);
    printf("\tNumero de derrotas: %d\n", quantidadeVitoriasRobo);
    printf("\tNumero de empates: %d\n", quantidadeEmpates);

    return 0;
}

bool menu()
{
    int opcao;

    printf("\n\n\tJogo da Velha");
    printf("\n1 - Jogar 0 - Sair");
    printf("\nSelecione: ");

    scanf("%d", &opcao);

    system("cls");
    if (opcao != 1 && opcao != 0)
    {
        printf("\nSelecione alguma opcao valida!");
    }

    return opcao;
}

void iniciaTabuleiro(int tabuleiro[][tamanhoMatriz])
{
    int linha, coluna;
    for (linha = 0; linha < tamanhoMatriz; linha++)
    {
        for (coluna = 0; coluna < tamanhoMatriz; coluna++)
        {
            tabuleiro[linha][coluna] = NINGUEM;
        }
    }
}

void exibeTabuleiro(int tabuleiro[][tamanhoMatriz])
{
    int linha, coluna;
    printf("\n");

    for (linha = 0; linha < tamanhoMatriz; linha++)
    {
        for (coluna = 0; coluna < tamanhoMatriz; coluna++)
        {
            if (tabuleiro[linha][coluna] == NINGUEM)
            {
                printf("    ");
            }
            else if (tabuleiro[linha][coluna] == ROBO)
            {
                printf("  X ");
            }
            else
            {
                printf("  O ");
            }

            if (coluna != (tamanhoMatriz - 1))
            {
                printf("|");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void jogar(int tabuleiro[][tamanhoMatriz], int *quantidadeVitoriasRobo, int *quantidadeEmpates)
{
    int numeroJogadas = 0, roboJogando = 1;
    struct Jogada jogadasRobo[maximoDeJogadas], jogadasHumano[maximoDeJogadas];
    iniciaTabuleiro(tabuleiro);

    do
    {
        system("cls");
        exibeTabuleiro(tabuleiro);
        proximaJogada(tabuleiro, numeroJogadas, &roboJogando, jogadasRobo, jogadasHumano);
        numeroJogadas++;
    } while (!checaTermino(tabuleiro, roboJogando, quantidadeVitoriasRobo, quantidadeEmpates));
}

bool checaPosicao(int tabuleiro[][tamanhoMatriz], int linha, int coluna)
{
    if (linha < 0 || linha > (tamanhoMatriz - 1) || coluna < 0 || coluna > (tamanhoMatriz - 1) || tabuleiro[linha][coluna] != 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool checaLinha(int tabuleiro[][tamanhoMatriz])
{
    int linha, coluna, soma;

    for (linha = 0; linha < tamanhoMatriz; linha++)
    {
        soma = 0;

        for (coluna = 0; coluna < tamanhoMatriz; coluna++)
        {
            soma += tabuleiro[linha][coluna];
        }

        if (soma == tamanhoMatriz || soma == (-1) * tamanhoMatriz)
        {
            return true;
        }
    }

    return false;
}

bool checaColuna(int tabuleiro[][tamanhoMatriz])
{
    int linha, coluna, soma;

    for (coluna = 0; coluna < tamanhoMatriz; coluna++)
    {
        soma = 0;

        for (linha = 0; linha < tamanhoMatriz; linha++)
        {
            soma += tabuleiro[linha][coluna];
        }

        if (soma == tamanhoMatriz || soma == (-1) * tamanhoMatriz)
        {
            return true;
        }
    }

    return false;
}

bool checaDiagonal(int tabuleiro[][tamanhoMatriz])
{
    int linha, diagonalPrincipal = 0, diagonalSecundaria = 0;

    for (linha = 0; linha < tamanhoMatriz; linha++)
    {
        diagonalPrincipal += tabuleiro[linha][linha];
        diagonalSecundaria += tabuleiro[linha][tamanhoMatriz - linha - 1];
    }

    if (diagonalPrincipal == tamanhoMatriz || diagonalPrincipal == (-1) * tamanhoMatriz ||
        diagonalSecundaria == tamanhoMatriz || diagonalSecundaria == (-1) * tamanhoMatriz)
    {
        return true;
    }

    return false;
}

bool checaEmpate(int tabuleiro[][tamanhoMatriz])
{
    int linha, coluna;

    for (linha = 0; linha < tamanhoMatriz; linha++)
    {
        for (coluna = 0; coluna < tamanhoMatriz; coluna++)
        {
            if (tabuleiro[linha][coluna] == 0)
            {
                return false;
            }
        }
    }

    return true;
}

bool checaTermino(int tabuleiro[][tamanhoMatriz], int roboJogando, int *quantidadeVitoriasRobo, int *quantidadeEmpates)
{
    if (checaLinha(tabuleiro) || checaColuna(tabuleiro) || checaDiagonal(tabuleiro))
    {
        printf("Jogo encerrado. ");
        if (roboJogando == 0)
        {
            printf("Voce perdeu!\n\n");
            *quantidadeVitoriasRobo = *quantidadeVitoriasRobo + 1;
        }
        else
        {
            printf("Voce venceu!\n\n");
        }

        exibeTabuleiro(tabuleiro);
        return true;
    }

    if (checaEmpate(tabuleiro))
    {
        *quantidadeEmpates = *quantidadeEmpates + 1;
        printf("Jogo encerrado. Deu velha!\n\n");
        exibeTabuleiro(tabuleiro);
        return true;
    }

    return false;
}

int zeroOuUm()
{
    return rand() % 2;
}

int zeroOuDois()
{
    return zeroOuUm() * 2;
}

int posicaoOposta(int posicao)
{
    if (posicao == 1)
    {
        return posicao;
    }

    return (-1) * (posicao - 2);
}

PosicaoNoTabuleiro posicaoNoTabuleiroDaJogada(int linha, int coluna)
{
    if (linha == 1 && coluna == 1)
    {
        return MEIO;
    }
    else if (linha == 0 && (coluna == 0 || coluna == 2) || linha == 2 && (coluna == 0 || coluna == 2))
    {
        return CANTO;
    }
    else
    {
        return BORDA;
    }
}

// Verifica se a entidade recebida por parametro tem alguma jogada vencedora
bool jogadaVencedora(int *linhaVencedora, int *colunaVencedora,
                     int tabuleiro[][tamanhoMatriz], SelecionadoPor vencedor)
{
    int linhaAtual, colunaAtual, soma = 0;

    for (int i = 0; i < quantidadeJogadasVencedoras; i++)
    {
        for (int j = 0; j < tamanhoMatriz; j++)
        {
            linhaAtual = jogadasVencedoras[i][j].linha;
            colunaAtual = jogadasVencedoras[i][j].coluna;

            if (tabuleiro[linhaAtual][colunaAtual] == vencedor)
            {
                soma += 2;
            }

            if (tabuleiro[linhaAtual][colunaAtual] == NINGUEM)
            {
                soma += 1;
                *linhaVencedora = linhaAtual;
                *colunaVencedora = colunaAtual;
            }
        }

        if (soma == 5)
        {
            return true;
        }

        soma = 0;
    }

    return false;
}

// O robo sempre começa em um canto aleatório
void primeiraJogadaRobo(int *linha, int *coluna)
{
    *linha = zeroOuDois();
    *coluna = zeroOuDois();
}

// O robo segue sua estratégia dependendo da primeira jogada do humano
void segundaJogadaRobo(int *linha, int *coluna, int numeroJogadas,
                       struct Jogada jogadasRobo[], struct Jogada jogadasHumano[])
{
    int numeroJogadaRobo = numeroJogadas / 2, numeroUltimaJogadaHumano = (numeroJogadas - 2) / 2;

    struct Jogada ultimaJogadaRobo = jogadasRobo[numeroJogadaRobo - 1],
                  ultimaJogadaHumano = jogadasHumano[numeroUltimaJogadaHumano];

    switch (ultimaJogadaHumano.posicaoNoTabuleiro)
    {
    case CANTO:
        if (posicaoOposta(ultimaJogadaRobo.linha) == ultimaJogadaHumano.linha &&
            posicaoOposta(ultimaJogadaRobo.coluna) == ultimaJogadaHumano.coluna)
        {
            int escolherLinha = zeroOuUm();
            if (escolherLinha)
            {
                *linha = ultimaJogadaRobo.linha;
                *coluna = ultimaJogadaHumano.coluna;
            }
            else
            {
                *linha = ultimaJogadaHumano.linha;
                *coluna = ultimaJogadaRobo.coluna;
            }
        }
        else
        {
            *linha = posicaoOposta(ultimaJogadaRobo.linha);
            *coluna = posicaoOposta(ultimaJogadaRobo.coluna);
        }
        break;
    case MEIO:
        *linha = posicaoOposta(ultimaJogadaRobo.linha);
        *coluna = posicaoOposta(ultimaJogadaRobo.coluna);
        break;
    case BORDA:
        if (ultimaJogadaHumano.coluna == 1)
        {
            *linha = posicaoOposta(ultimaJogadaHumano.linha);
            if (ultimaJogadaHumano.linha == posicaoOposta(ultimaJogadaRobo.linha))
            {
                *coluna = posicaoOposta(ultimaJogadaRobo.coluna);
            }
            else
            {
                *coluna = ultimaJogadaRobo.coluna;
            }
        }
        else if (ultimaJogadaHumano.linha == 1)
        {
            if (ultimaJogadaHumano.coluna == posicaoOposta(ultimaJogadaRobo.coluna))
            {
                *linha = posicaoOposta(ultimaJogadaRobo.linha);
            }
            else
            {
                *linha = ultimaJogadaRobo.linha;
            }
            *coluna = posicaoOposta(ultimaJogadaHumano.coluna);
        }
        break;
    }
}

// O robo segue sua estratégia dependendo da primeira e segunda jogada do humano
void terceiraJogadaRobo(int *linha, int *coluna, int numeroJogadas,
                        struct Jogada jogadasRobo[], struct Jogada jogadasHumano[])
{
    int numeroJogadaRobo = numeroJogadas / 2, numeroUltimaJogadaHumano = (numeroJogadas - 2) / 2;

    struct Jogada ultimaJogadaRobo = jogadasRobo[numeroJogadaRobo - 1],
                  penultimaJogadaRobo = jogadasRobo[numeroJogadaRobo - 2],
                  ultimaJogadaHumano = jogadasHumano[numeroUltimaJogadaHumano],
                  penultimaJogadaHumano = jogadasHumano[numeroUltimaJogadaHumano - 1];

    switch (penultimaJogadaHumano.posicaoNoTabuleiro)
    {
    case CANTO:
        if (ultimaJogadaRobo.linha == posicaoOposta(penultimaJogadaRobo.linha) &&
            ultimaJogadaRobo.coluna == posicaoOposta(penultimaJogadaRobo.coluna))
        {
            if (ultimaJogadaHumano.posicaoNoTabuleiro == MEIO)
            {
                *linha = posicaoOposta(penultimaJogadaHumano.linha);
                *coluna = posicaoOposta(penultimaJogadaHumano.coluna);
            }
            else
            {
                *linha = 1;
                *coluna = 1;
            }
        }
        else
        {
            if (ultimaJogadaHumano.posicaoNoTabuleiro == BORDA)
            {
                if (ultimaJogadaHumano.linha == ultimaJogadaRobo.linha &&
                        ultimaJogadaHumano.linha == penultimaJogadaRobo.linha ||
                    ultimaJogadaHumano.coluna == ultimaJogadaRobo.coluna &&
                        ultimaJogadaHumano.coluna == penultimaJogadaRobo.coluna)
                {
                    *linha = posicaoOposta(ultimaJogadaRobo.linha);
                    *coluna = posicaoOposta(ultimaJogadaRobo.coluna);
                }
                else
                {
                    if (ultimaJogadaRobo.coluna == penultimaJogadaRobo.coluna)
                    {
                        *linha = 1;
                        *coluna = ultimaJogadaRobo.coluna;
                    }
                    else
                    {
                        *linha = ultimaJogadaRobo.linha;
                        *coluna = 1;
                    }
                }
            }
            else
            {
                if (ultimaJogadaRobo.linha == penultimaJogadaRobo.linha)
                {
                    *linha = ultimaJogadaRobo.linha;
                    *coluna = 1;
                }
                else
                {
                    *linha = 1;
                    *coluna = ultimaJogadaRobo.coluna;
                }
            }
        }
        break;
    case MEIO:
        if (ultimaJogadaHumano.posicaoNoTabuleiro == CANTO)
        {
            *linha = posicaoOposta(ultimaJogadaHumano.linha);
            *coluna = posicaoOposta(ultimaJogadaHumano.coluna);
        }
        else
        {
            if (ultimaJogadaHumano.coluna == 1)
            {
                *linha = posicaoOposta(ultimaJogadaHumano.linha);
                *coluna = ultimaJogadaHumano.coluna;
            }
            else if (ultimaJogadaHumano.linha == 1)
            {
                *coluna = posicaoOposta(ultimaJogadaHumano.coluna);
                *linha = ultimaJogadaHumano.linha;
            }
        }
        break;
    case BORDA:
        if ((ultimaJogadaHumano.linha == 1 && ultimaJogadaHumano.coluna == ultimaJogadaRobo.coluna) ||
            (ultimaJogadaHumano.coluna == 1 && ultimaJogadaHumano.linha == ultimaJogadaRobo.linha))
        {
            *linha = 1;
            *coluna = 1;
        }
        else
        {
            if (ultimaJogadaRobo.coluna == penultimaJogadaRobo.coluna)
            {
                *linha = 1;
                *coluna = ultimaJogadaRobo.coluna;
            }
            else
            {
                *linha = ultimaJogadaRobo.linha;
                *coluna = 1;
            }
        }
        break;
    }
}

// Nas ultimas jogadas do robo ele ganha o jogo ou bloqueia o humano de vencer
void ultimasJogadasRobo(int *linha, int *coluna, int tabuleiro[][tamanhoMatriz])
{
    int roboPodeGanhar = jogadaVencedora(linha, coluna, tabuleiro, ROBO);

    if (roboPodeGanhar == 0)
    {
        jogadaVencedora(linha, coluna, tabuleiro, HUMANO);
    }
}

void jogadaRobo(int tabuleiro[][tamanhoMatriz], int numeroJogadas,
                struct Jogada jogadasRobo[], struct Jogada jogadasHumano[])
{
    int linha, coluna, numeroJogadaRobo = numeroJogadas / 2;

    switch (numeroJogadas)
    {
    case PRIMEIRA:
        primeiraJogadaRobo(&linha, &coluna);
        break;
    case SEGUNDA:
        segundaJogadaRobo(&linha, &coluna, numeroJogadas, jogadasRobo, jogadasHumano);
        break;
    case TERCEIRA:
        terceiraJogadaRobo(&linha, &coluna, numeroJogadas, jogadasRobo, jogadasHumano);
        break;
    case QUARTA:
    case QUINTA:
        ultimasJogadasRobo(&linha, &coluna, tabuleiro);
        break;
    }

    tabuleiro[linha][coluna] = ROBO;

    jogadasRobo[numeroJogadaRobo].linha = linha;
    jogadasRobo[numeroJogadaRobo].coluna = coluna;
    jogadasRobo[numeroJogadaRobo].posicaoNoTabuleiro = posicaoNoTabuleiroDaJogada(linha, coluna);
}

void jogadaHumano(int tabuleiro[][tamanhoMatriz], int numeroJogadas, struct Jogada jogadasHumano[])
{
    int linha, coluna;
    do
    {
        printf("Linha: ");
        scanf("%d", &linha);
        linha--;

        printf("Coluna: ");
        scanf("%d", &coluna);
        coluna--;

        if (!checaPosicao(tabuleiro, linha, coluna))
        {
            printf("Posicao ocupada ou inexistente, escolha outra.\n");
        }

    } while (!checaPosicao(tabuleiro, linha, coluna));

    tabuleiro[linha][coluna] = HUMANO;

    int numeroJogadaHumano = (numeroJogadas - 1) / 2;
    jogadasHumano[numeroJogadaHumano].linha = linha;
    jogadasHumano[numeroJogadaHumano].coluna = coluna;
    jogadasHumano[numeroJogadaHumano].posicaoNoTabuleiro = posicaoNoTabuleiroDaJogada(linha, coluna);
}

void proximaJogada(int tabuleiro[][tamanhoMatriz], int numeroJogadas, int *roboJogando,
                   struct Jogada jogadasRobo[], struct Jogada jogadasHumano[])
{
    if (*roboJogando == 1)
    {
        jogadaRobo(tabuleiro, numeroJogadas, jogadasRobo, jogadasHumano);
        *roboJogando = 0;
    }
    else
    {
        jogadaHumano(tabuleiro, numeroJogadas, jogadasHumano);
        *roboJogando = 1;
    }
}