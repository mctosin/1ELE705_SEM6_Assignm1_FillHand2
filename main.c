// Fig. 10.3: fig10_03.c
// Card shuffling and dealing program using structures
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CARDS 52
#define FACES 13
/* N�O MODIFIQUE O C�DIGO ACIMA */
#define COLUNAS 4           // Voc� pode mudar o valor das constantes para testar o programa
#define CARTAS_NA_MAO 4
/* N�O MODIFIQUE O C�DIGO ABAIXO */
//#define Gerador
//#define Teste

// card structure definition                  
typedef struct {
    const char* face; // define pointer face   
    const char* suit; // define pointer suit
    unsigned int face_number; // Define o n�mero da face da carta
    unsigned int suit_number; // Define o n�mero do naipe
} Card; // end struct card                         

//typedef struct card Card; // new type name for struct card   

// prototypes
void shuffle(Card* const wDeck);

void print_deck(Card* const wDeck, const int Ncolumns, const int printpar);

void fillDeckExt(Card* const wDeck, const char* wFace[], const char* wSuit[]);
void dealExt(const Card* const wDeck);
void print_deck_file(Card* const wDeck, const int Ncolumns);
unsigned int fillHandExt(Card* const wDeck, Card* const wHand, const unsigned int Ncards);

#ifndef Gerador

//argc[1] == "0" programa aluno
//        == "1" programa de testes
//argc[2] -> quantidade de colunas a serem impressas
//argc[3] -> quantidade de elementos a serem transferidos para a hand
//argc[4] -> arquivo .dat com as sequencia de cartas pr�-embaralhadas
//
int main(int argv, char* argc[])
{
    Card deck[CARDS + 1]; // define array of Cards
    Card hand[CARDS + 1]; // reserva espa�o para a m�o

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

    if (atoi(argc[1]) == 1) {
        // C�digo do servidor de testes
        static Card deck_test[CARDS + 1];
        FILE* cfPtr_t;
        int l;

        // L� os dados pr� embaralhados do arquivo em argc[4]
        // e reconstr�i em deck_test
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
            // Reconstr�i as strings dos elementos de deck_test
            for (l = 0; deck_test[l].face_number != 0; l++) {
                deck_test[l].face = face[deck_test[l].face_number - 1];
                deck_test[l].suit = suit[deck_test[l].suit_number - 1];
            }
        }
        //print_deck(deck_test, atoi(argc[2]), 1);
        //fprintf(stderr, "\n");
        fillHandExt(deck_test, hand, atoi(argc[3]));
        print_deck(hand, atoi(argc[2]), 0);
        print_deck(deck_test, atoi(argc[2]), 0);
    }
    else {
        // C�digo do aluno
        unsigned int colunas = COLUNAS, cartas_na_mao = CARTAS_NA_MAO;

        print_deck(deck, colunas, 1);

        fillHandExt(deck, hand, cartas_na_mao);

        fprintf(stderr, "\n");

        print_deck(hand, colunas, 0);

        fprintf(stderr, "\n");

        print_deck(deck, colunas, 0);
    }

    return 0;
} // end main

#else

//argc[1] == "0" programa aluno
//        == "1" programa de testes
//argc[2] -> quantidade de colunas a serem impressas
//argc[3] -> quantidade de elementos a serem transferidos para a hand
//argc[4] -> arquivo .dat com as sequencia de cartas pr�-embaralhadas
// Nelson -  43 984871043
int main(int argv, char* argc[])
{
    Card deck[CARDS + 1]; // define array of Cards
    Card hand[CARDS + 1]; // reserva espa�o para a m�o

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
    //print_deck(deck, atoi(argc[1])); // argc[1] � a quantidade de colunas

    // Cria um arquivo bin�rio com o cont�udo das structs que definem cada carta no deck,
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

    // Cria um arquivo texto com a mesma sa�da de print_deck, mas em um arquivo.
    print_deck_file(deck, atoi(argc[2]));
        
#ifdef Teste
    // Testa se o arquivo cont�m as informa��es corretas
    
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
        // Reconstr�i as strings dos elementos de deck_test
        for (i = 0; deck_test[i].face_number != 0; i++) {
            deck_test[i].face = face[deck_test[i].face_number - 1];
            deck_test[i].suit = suit[deck_test[i].suit_number - 1];
        }
    }

    print_deck_file(deck_test, atoi(argc[2])); // argc[2] � a quantidade de colunas
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
    // Imprime as cartas do ma�o apontado por wDeck
    // O terminador n�o � impresso, obviamente
    // Se printpar == 0 imprime na stdio; caso contr�rio imprime na stderr

    size_t i; // counter

    if (printpar == 0) {
        if (wDeck[0].face_number != 0) { //Testa se a primeira carta � o terminador
            for (i = 0; wDeck[i + 1].face_number != 0; ++i) { //Testa se a pr�xima carta n�o � o terminador
                printf("%5s of %-8s%s", wDeck[i].face, wDeck[i].suit,
                    (i + 1) % Ncolumns ? "  " : "\n");
            }
            //Ap�s encontrar o terminador, imprime a carta que falta
            printf("%5s of %-8s%s", wDeck[i].face, wDeck[i].suit, "\n");
        }
    }
    else {
        if (wDeck[0].face_number != 0) { //Testa se a primeira carta � o terminador
            for (i = 0; wDeck[i + 1].face_number != 0; ++i) { //Testa se a pr�xima carta n�o � o terminador
                fprintf(stderr, "%5s of %-8s%s", wDeck[i].face, wDeck[i].suit,
                    (i + 1) % Ncolumns ? "  " : "\n");
            }
            //Ap�s encontrar o terminador, imprime a carta que falta
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
    /* N�O MODIFIQUE O C�DIGO ACIMA */
    // Apague o c�digo abaixo e fa�a aqui o c�digo da fun��o
    unsigned int i = 0;
    wHand[i] = wDeck[10];
    i++;
    wHand[i].face_number = 0;
    wHand[i].suit_number = 0;
    wHand[i].face = NULL;
    wHand[i].suit = NULL;
    return i;

    /* N�O MODIFIQUE O C�DIGO ABAIXO */
}

/*  Descri��o do Assignment  */
/*
    Neste assignment voc� deve entender a parte do c�digo necess�ria para realizar a tarefa e deve
codificar a fun��o fillHandExt segundo as especifica��es abaixo
    Neste programa, a struct Card que define quantidade de informac�o que cada carta armazena foi modificada.
    Agora, para cada carta armazena-se tamb�m o n�mero da face e o n�mero do naipe.
    Assim, a fun��o fillDeckExt preenche as 52 cartas do baralho � partir do endere�o de um deck de cartas passado
para a vari�vel interna wDeck. Esta fun��o � uma modifica��o da fun��o fillDeck da aula.
    Adicionalmente � introduzido o conceito do terminador de cartas, que � uma carta cujos campos de face e nome 
apontam para NULL e os n�meros da carta e do naipe s�o zero. Isso pode ser visto na ao final da fun��o.
    A fun��o dealExt usa o terminador para imprimir as cartas de um certo deck de cartas e determinar a carta final
deste. Assim, qualquer que seja o tamanho do deck de cartas, esta fun��o � capaz de imprim�-lo na tela.
    A fun��o dealExt n�o � usada no programa, pois foi criada uma fun��o com mais funcionalidades denomiada
print_deck.
    A fun��o print_deck tem por par�metros endere�o de um dado deck ou ma�o de cartas com quantidade de cartas qualquer, 
o n�mero de colunas no qual se deseja imprimir uma carta, entre 1 e 4 colunas, e o par�metro de impress�o (printpar).
Se o par�metro de impress�o for zero, a fun��o imprime na stdio e se for qualquer outro, imprime na stderr. A fun��o
print_deck imprime as cartas de um determinado ma�o de cartas at� encontrar o terminador.
    O c�digo principal que vai ser executado est� no corpo do else a partir da linha 96. Tamb�m faz parte do c�digo
a vari�vel Card hand, definida na linha 47 e as constantes COLUNAS e CARTAS_NA_MAO, definidas nas linhas 10 e 11.
    A vari�vel hand ir� armazenar uma certa quantidade de cartas que um certo jogador possui, tamb�m chamada de m�o.
Dependendo do jogo, uma m�o de cartas pode ter uma quantidade diferente e pode variar no decorrer do jogo.
    Assim, a fun��o fillHandExt foi criada como forma de preencher uma m�o de cartas � partir de um deck de cartas
existente. A fun��o fillHandExt recebe como par�metros o endere�o do deck, o endere�o da hand (m�o de cartas) e a 
quantidade de cartas que ser� transferida do deck para a hand.
    Ap�s a �ltima carta transferida para a hand, um terminador deve ser inserido na mesma, indicando o fim desta 
sequencia armazenada.
    Cada carta deve ser transferida para a hand � partir do in�cio do deck (posi��o zero do vetor). Assim, ap�s a
transfer�ncia de cada carta, todas as cartas remanecentes do deck devem ser deslocadas de uma posi��o em dire��o ao
in�cio do deck, incluindo o terminador que ser� o �ltimo elemento do vetor deslocado. Assim, ap�s cada transfer�ncia,
o deck conter� uma carta a menos.
    Todas as informa��es necess�rias para codificar a fun��o fillHandExt est�o dispon�veis pelas suas vari�veis de
passagem de par�metros. Assim, � proibido utilizar qualque constante, vari�vel ou elemento externo � fun��o.
    N�o modifique o c�digo do programa, a n�o ser nas �reas especificadas nos coment�rios.

*/