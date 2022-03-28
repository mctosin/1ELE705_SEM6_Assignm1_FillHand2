// Fig. 10.3: fig10_03.c
// Card shuffling and dealing program using structures
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CARDS 52
#define FACES 13
/* NÃO MODIFIQUE O CÓDIGO ACIMA */
#define COLUNAS 4           // Você pode mudar o valor das constantes para testar o programa
#define CARTAS_NA_MAO 8
/* NÃO MODIFIQUE O CÓDIGO ABAIXO */
//#define Gerador
//#define Teste

// card structure definition                  
typedef struct {
    const char* face; // define pointer face   
    const char* suit; // define pointer suit
    unsigned int face_number; // Define o número da face da carta
    unsigned int suit_number; // Define o número do naipe
} Card; // end struct card                         

//typedef struct card Card; // new type name for struct card   

// prototypes
void shuffle(Card* const wDeck);

void print_deck(Card* const wDeck, const int Ncolumns, const int printpar);

void fillDeckExt(Card* const wDeck, const char* wFace[], const char* wSuit[]);
void dealExt(const Card* const wDeck);
void print_deck_file(Card* const wDeck, const int Ncolumns);
unsigned int fillHandExt(Card* const wDeck, Card* const wHand, const unsigned int Ncards);

#ifndef Gerador // Se a constante Gerador não estiver definida, compila o código até o #else (linha 119)

//argc[1] == "0" programa aluno
//        == "1" programa de testes
//argc[2] -> quantidade de colunas a serem impressas
//argc[3] -> quantidade de elementos a serem transferidos para a hand
//argc[4] -> arquivo .dat com as sequencia de cartas pré-embaralhadas
//
int main(int argv, char* argc[])
{
    Card deck[CARDS + 1]; // define array of Cards
    Card hand[CARDS + 1]; // reserva espaço para a mão

    // initialize array of pointers
    const char* face[] = { "Ace", "Deuce", "Three", "Four", "Five",
       "Six", "Seven", "Eight", "Nine", "Ten",
       "Jack", "Queen", "King" };

    // initialize array of pointers
    const char* suit[] = { "Hearts", "Diamonds", "Clubs", "Spades" };

    srand(time(NULL)); // randomize

    //fillDeck(deck, face, suit); // load the deck with Cards
    fillDeckExt(deck, face, suit);
    shuffle(deck); // put Cards in random order

    if (atoi(argc[1]) == 0) {
        // Código do aluno
        unsigned int colunas = COLUNAS, cartas_na_mao = CARTAS_NA_MAO;

        fprintf(stderr, "Deck de cartas original\n");
        print_deck(deck, colunas, 1);
        fprintf(stderr, "\n");

        fillHandExt(deck, hand, cartas_na_mao);
        fprintf(stderr, "Mao do jogador preenchida com %d cartas\n", CARTAS_NA_MAO);
        print_deck(hand, colunas, 1);
        fprintf(stderr, "\n");

        fprintf(stderr, "Deck de cartas apos retirar as %d cartas\n", CARTAS_NA_MAO);
        print_deck(deck, colunas, 1);
        fprintf(stderr, "\n");
    }
    else {
        // Código do servidor de testes
        static Card deck_test[CARDS + 1];
        FILE* cfPtr_t;
        int l;

        // Lê os dados pré embaralhados do arquivo em argc[4]
        // e reconstrói em deck_test
        if ((cfPtr_t = fopen(argc[4], "rb")) == NULL) {
            fprintf(stderr, "File could not be opened.\n");
        } // end if
        else {
            // read all records from file (until eof)
            for (l = 0; !feof(cfPtr_t); l++) {
                fread(&deck_test[l], sizeof(Card), 1, cfPtr_t);
                if (deck_test[l].face == 0) {
                    break;
                }
            }
            fclose(cfPtr_t);
            // Reconstrói as strings dos elementos de deck_test
            for (l = 0; deck_test[l].face_number != 0; l++) {
                deck_test[l].face = face[deck_test[l].face_number - 1];
                deck_test[l].suit = suit[deck_test[l].suit_number - 1];
            }
        }
        //print_deck(deck_test, atoi(argc[2]), 1);
        //fprintf(stderr, "\n");
        fillHandExt(deck_test, hand, atoi(argc[3]));
        printf("Hand\n");
        print_deck(hand, atoi(argc[2]), 0);
        printf("Deck\n");
        print_deck(deck_test, atoi(argc[2]), 0);
    }

    return 0;
} // end main

#else // Caso contrário (a constante Gerador está definida) compila o código até o #endif (linha 228)

//argc[1] == "0" programa aluno
//        == "1" programa de testes
//argc[2] -> quantidade de colunas a serem impressas
//argc[3] -> quantidade de elementos a serem transferidos para a hand
//argc[4] -> arquivo .dat com as sequencia de cartas pré-embaralhadas

int main(int argv, char* argc[])
{
    Card deck[CARDS + 1]; // define array of Cards
    Card hand[CARDS + 1]; // reserva espaço para a mão

    // initialize array of pointers
    const char* face[] = { "Ace", "Deuce", "Three", "Four", "Five",
       "Six", "Seven", "Eight", "Nine", "Ten",
       "Jack", "Queen", "King" };

    // initialize array of pointers
    const char* suit[] = { "Hearts", "Diamonds", "Clubs", "Spades" };

    srand(time(NULL)); // randomize

    fillDeckExt(deck, face, suit);
    shuffle(deck); // put Cards in random order
    //dealExt(deck); // deal all 52 Cards
    //print_deck(deck, atoi(argc[1])); // argc[1] é a quantidade de colunas

    // Cria um arquivo binário com o contéudo das structs que definem cada carta no deck,
    // incluindo o elemento terminador.

    FILE* cfPtr;
    size_t i;
    if ((cfPtr = fopen("cardseq.dat", "wb")) == NULL) {
        fprintf(stderr, "File could not be opened.\n");
    }
    else {
                               
        for (i = 0; deck[i].face_number != 0; ++i) {
            fwrite(&deck[i], sizeof(Card), 1, cfPtr);
        }
        fwrite(&deck[i], sizeof(Card), 1, cfPtr);

        fclose(cfPtr); 
    } 

    // Cria um arquivo texto com a mesma saída de print_deck, mas em um arquivo.

    fillHandExt(deck, hand, atoi(argc[3]));
    

    if ((cfPtr = fopen("cardprint4.txt", "w")) == NULL) {
        fprintf(stderr, "File could not be opened.\n");
    }
    else {

        fprintf(cfPtr, "Hand\n");
        if (hand[0].face_number != 0) {
            for (i = 0; hand[i + 1].face_number != 0; ++i) {
                fprintf(cfPtr, "%5s of %-8s%s", hand[i].face, hand[i].suit,
                    (i + 1) % atoi(argc[2]) ? "  " : "\n");
            } // end for
            fprintf(cfPtr, "%5s of %-8s%s", hand[i].face, hand[i].suit, "\n");
        }
        fprintf(cfPtr, "Deck\n");
        if (deck[0].face_number != 0) {
            for (i = 0; deck[i + 1].face_number != 0; ++i) {
                fprintf(cfPtr, "%5s of %-8s%s", deck[i].face, deck[i].suit,
                    (i + 1) % atoi(argc[2]) ? "  " : "\n");
            } // end for
            fprintf(cfPtr, "%5s of %-8s%s", deck[i].face, deck[i].suit, "\n");
        }

        fclose(cfPtr);
    }


    //print_deck_file(deck, atoi(argc[2]));
        
#ifdef Teste
    // Testa se o arquivo contém as informações corretas
    
    static Card deck_test[CARDS + 1];

    if ((cfPtr = fopen("cardseq.dat", "rb")) == NULL) {
        fprintf(stderr, "File could not be opened.\n");
    } // end if
    else {
        // read all records from file (until eof)
        for (i = 0; !feof(cfPtr); i++) {
            fread(&deck_test[i], sizeof(Card), 1, cfPtr);
            if (deck_test[i].face == 0) {
                break;
            }
        }
        fclose(cfPtr);
        // Reconstrói as strings dos elementos de deck_test
        for (i = 0; deck_test[i].face_number != 0; i++) {
            deck_test[i].face = face[deck_test[i].face_number - 1];
            deck_test[i].suit = suit[deck_test[i].suit_number - 1];
        }
    }

    print_deck_file(deck_test, atoi(argc[2])); // argc[2] é a quantidade de colunas
#endif

    return 0;
} // end main

#endif

// shuffle cards
void shuffle(Card* const wDeck)
{
    size_t i; // counter
    size_t j; // variable to hold random value between 0 - 51
    Card temp; // define temporary structure for swapping Cards

    // loop through wDeck randomly swapping Cards
    for (i = 0; i < CARDS; ++i) {
        j = rand() % CARDS;
        temp = wDeck[i];
        wDeck[i] = wDeck[j];
        wDeck[j] = temp;
    } // end for
} // end function shuffle

void fillDeckExt(Card* const wDeck, const char* wFace[], const char* wSuit[]) {
    size_t i; // counter

   // loop through wDeck
    for (i = 0; i < CARDS; ++i) {
        wDeck[i].face = wFace[i % FACES];
        wDeck[i].suit = wSuit[i / FACES];
        wDeck[i].face_number = i % FACES + 1;
        wDeck[i].suit_number = i / FACES + 1;
    } // end for
    wDeck[i].face = NULL;
    wDeck[i].suit = NULL;
    wDeck[i].face_number = 0;
    wDeck[i].suit_number = 0;
}

void dealExt(const Card* const wDeck)
{
    size_t i; // counter

    // loop through wDeck
    for (i = 0; wDeck[i].face_number != 0; ++i) {
        printf("%5s of %-8s%s", wDeck[i].face, wDeck[i].suit,
            (i + 1) % 4 ? "  " : "\n");
    } // end for
} // end function deal

void print_deck(Card* const wDeck, const int Ncolumns, const int printpar) {
    // Imprime as cartas do maço apontado por wDeck
    // O terminador não é impresso, obviamente
    // Se printpar == 0 imprime na stdio; caso contrário imprime na stderr

    size_t i; // counter

    if (printpar == 0) {
        if (wDeck[0].face_number != 0) { //Testa se a primeira carta é o terminador
            for (i = 0; wDeck[i + 1].face_number != 0; ++i) { //Testa se a próxima carta não é o terminador
                printf("%5s of %-8s%s", wDeck[i].face, wDeck[i].suit,
                    (i + 1) % Ncolumns ? "  " : "\n");
            }
            //Após encontrar o terminador, imprime a carta que falta
            printf("%5s of %-8s%s", wDeck[i].face, wDeck[i].suit, "\n");
        }
    }
    else {
        if (wDeck[0].face_number != 0) { //Testa se a primeira carta é o terminador
            for (i = 0; wDeck[i + 1].face_number != 0; ++i) { //Testa se a próxima carta não é o terminador
                fprintf(stderr, "%5s of %-8s%s", wDeck[i].face, wDeck[i].suit,
                    (i + 1) % Ncolumns ? "  " : "\n");
            }
            //Após encontrar o terminador, imprime a carta que falta
            fprintf(stderr, "%5s of %-8s%s", wDeck[i].face, wDeck[i].suit, "\n");
        }
    }
    
}

void print_deck_file(Card* const wDeck, const int Ncolumns) {

    FILE* cfPtr;
    size_t i;
    char filename[20];

#ifdef Teste
    static int num = 0;
    num++;
    sprintf(filename, "cardprint%d_%d.txt", Ncolumns, num);
#else
    sprintf(filename, "cardprint%d.txt", Ncolumns);
#endif

    if ((cfPtr = fopen(filename, "w")) == NULL) {
        fprintf(stderr, "File could not be opened.\n");
    }
    else {
        if (wDeck[0].face_number != 0) {
            for (i = 0; wDeck[i + 1].face_number != 0; ++i) {
                fprintf(cfPtr, "%5s of %-8s%s", wDeck[i].face, wDeck[i].suit,
                    (i + 1) % Ncolumns ? "  " : "\n");
            } // end for
            fprintf(cfPtr, "%5s of %-8s%s", wDeck[i].face, wDeck[i].suit, "\n");
        }
        fclose(cfPtr);
    }
}


unsigned int fillHandExt(Card* const wDeck, Card* const wHand, const unsigned int Ncards)
{
    /* NÃO MODIFIQUE O CÓDIGO ACIMA */
    // Apague o código abaixo e faça aqui o código da função
    
    unsigned int i = 0;
   
    wHand[i].face_number = 0;
    wHand[i].suit_number = 0;
    wHand[i].face = NULL;
    wHand[i].suit = NULL;
    
    return i;
    
    /* NÃO MODIFIQUE O CÓDIGO ABAIXO */
}


/*  Descrição do Assignment  */
/*
    Neste assignment você deve entender a parte do código necessária para realizar a tarefa e deve
codificar a função fillHandExt segundo as especificações abaixo
    Neste programa, a struct Card que define quantidade de informacão que cada carta armazena foi modificada.
    Agora, em cada carta armazena-se também o número da face e o número do naipe.
    Assim, a função fillDeckExt preenche as 52 cartas do baralho à partir do endereço de um deck de cartas passado
para a variável interna wDeck. Esta função é uma modificação da função fillDeck da aula. Adicionalmente a função 
introduz uma carta de terminação ou terminador ao final do vetor do deck de cartas. O terminador é uma carta cujos
campos face e name apontam para NULL e os números da carta e do naipe são zero. Isso pode ser visto quando fillDeckExt
insere o terminador ao final de seu código, a partir da linha 256. O terminador tem por finalidade indicar o final 
de um dado vetor de cartas.A vantagem do terminador é que não é preciso de antemão o tamanho do deck de cartas. 
Basta percorrer o Deck de cartas até encontrar o terminador. 
    A função dealExt usa o terminador para imprimir as cartas de um certo deck de cartas e determinar a carta final
deste. Assim, qualquer que seja o tamanho do deck de cartas, esta função é capaz de imprimí-lo na tela.
    A função dealExt não é usada no programa, pois foi criada uma função com mais funcionalidades denominada print_deck.
    A função print_deck tem por parâmetros o endereço de um dado deck ou maço de cartas com quantidade de cartas qualquer, 
o número de colunas no qual se deseja imprimir uma carta, entre 1 e 4 colunas, e o parâmetro de impressão (printpar).
Se o parâmetro de impressão for zero, a função imprime na stdio e se for qualquer outro, imprime na stderr. A função
print_deck imprime as cartas de um determinado maço de cartas até encontrar o terminador.
    O código principal que vai ser executado no Visual Studio está no corpo do if a partir da linha 64. Também faz 
parte do código a variável Card hand, definida na linha 47 e as constantes COLUNAS e CARTAS_NA_MAO, definidas nas
linhas 10 e 11.
    O vetor hand é um vetor do tipo Card que irá armazenar uma certa quantidade de cartas que um certo jogador possui,
também chamada de mão. Dependendo do jogo, uma mão de cartas pode ter uma quantidade diferente e pode variar no decorrer
do jogo.
    Assim, a função fillHandExt foi criada como forma de preencher uma mão de cartas à partir de um deck de cartas
existente. A função fillHandExt recebe como parâmetros o endereço do deck, o endereço da hand (mão de cartas) e a 
quantidade de cartas que será transferida do deck para a hand.
    Após a última carta transferida para a hand, um terminador deve ser inserido na mesma, indicando o fim desta 
sequencia armazenada.
    A função fillHandExt considera que a mão de cartas ainda não existe, existe apenas um vetor do tipo Card cujo 
endereço foi passado para a função através de wHand.
    Cada carta deve ser transferida para a hand à partir do início do deck (posição zero do vetor). Assim, após a
transferência de cada carta, todas as cartas remanecentes no deck devem ser deslocadas de uma posição em direção ao
início do deck, incluindo o terminador que será o último elemento do vetor deslocado. Assim, após cada transferência,
o deck conterá uma carta a menos.
    Adicionalmente a função fillHandExt retorna através do retorno da função (comando return) a quantidade de cartas
efetivamente transferidas do deck para a hand. Por exemplo, a função pode ter sido chamada para transferir 15 cartas
do deck para uma hand. Se o deck contiver apenas 7 cartas, somente estas 7 cartas serão transferidas e a função
retornara o inteiro 7. Neste caso o deck de cartas ficará sem carta alguma, restando apenas o terminador na posição
zero do vetor de cartas que define o deck.
    A função fillHandDeck pode ser utilizada para inicializar uma dada hand vazia. Se a quantidade de cartas passada
passada para a função for zero, então ela somente insere o terminador na hand cujo endereço foi passado para wHand.
O deck neste caso permanece inalterado, obviamente.
    Todas as informações necessárias para codificar a função fillHandExt estão disponíveis pelas suas variáveis de
passagem de parâmetros. Assim, é proibido utilizar qualque constante, variável ou elemento externo à função.
    Não modifique o código do programa, a não ser nas áreas especificadas nos comentários.

*/
