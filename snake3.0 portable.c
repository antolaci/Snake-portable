#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef SISTEMA
#define SISTEMA 1 // 1 -> Linux, 0 -> Windows,  scegliere il sistema operativo usato
#endif

#if SISTEMA == 0
#include <conio.h>
#include <windows.h>
#define usleep(x) Sleep(x)
#elif SISTEMA == 1
#include <termios.h>
char kbhit(void);
#else
printf("ERROR! - Sistema operativo sconosciuto\n");
#endif

#define dx 'd'
#define sx 'a'
#define up 'w'
#define down 's'
#define lose 'l'

char** initializeCampo( const int , const int );  /* inizializza la matrice dello snake (campo di gioco)*/
int** creaSpazioCoordinate( const int ,const int );  /* creao la matrice per le coordinate degli elementi di snake e inizializza lo snake di 3 elementi*/
int aggiornaSnake( char**, char , int , int** , const int , const int,int*,int*,int*); /*aggiorna il campo con la nuova posizione dello snake*/
void generaFrutta( char** , const int , const int ); /* genera la frutta nel campo*/
void inserisciBordo( char , int* , int* , int** , const int , const int); /* dirzione , elementi presenti, indirizzo inserimento,matrice, coordinate a, b*/
void inserisci( char , int* , int* , int** , const int , const int); /* dirzione , elementi presenti, indirizzo inserimento,matrice coordinate a, b*/
void estrai( int* , int* , int** , const int , const int); /* elementi presenti, indirizzo estrazione ,matrice coordinate , a, b*/
void stampa(  char**, const int , const int); /* stamapa a schermo il campo*/
void stampaInt( int** , const int , const int); /* funzione di controllo per stampare la matrice coordinate*/
void haiPerso(char**,const int,const int,int); /*stampa a schermo "hai perso" e chiude il programma*/
void pausa(void); /* ferma il programma finchè non viene premuto un tasto*/
void istruzioni(void); /* Istruzioni gioco*/
void clean(void); /*pulisce lo schermo*/

int main( int argc, char *argv[])
{
    
    int Pi = 3,Pe = 0,presenti = 3; // puntatori: ingresso, uscita, elementi presenti
    unsigned long int t = 0;
    unsigned int a, b , diffic; // dimensioni campo e difficolà
    char c; // char per il carattere da tastiera
    char lastDirection = dx ;
    char **campo;
    int **coordinateSnake;

if ( argc != 4){
    istruzioni();
    return 0;
    }

else{
    a = atoi(argv[1]);
    b = atoi(argv[2]);
    diffic = atoi(argv[3]);

    if( a>30 || a<10 || b>30 || b<10 || diffic<1 || diffic>3 ){
        istruzioni();
        return 0;
        }
    }

   /* inizializzo campo , matrice coordinate e frutta */
   campo = initializeCampo(a,b);
   coordinateSnake = creaSpazioCoordinate(a,b);
   generaFrutta (campo,a,b);

    while(1){  /* loop infinito*/

             if (lastDirection == lose)
                 return 0;

             if(t%2 == 0)
             lastDirection = aggiornaSnake( campo , lastDirection , lastDirection , coordinateSnake ,a ,b,&Pi,&Pe,&presenti);

             if (diffic == 1)
             usleep(200);
             if (diffic == 2)
             usleep(80);
             if (diffic == 3)
             usleep(40);

             #if SISTEMA == 0
             if (kbhit()){   // funzione definita in conio.h che rileva la pressione di un tasto
                c =(char)getch();
             #else
             c = kbhit();
             #endif
                if((c==dx)||(c==sx)||(c==up)||(c==down)){
                   lastDirection = aggiornaSnake( campo , c , lastDirection , coordinateSnake ,a ,b,&Pi,&Pe,&presenti);
                   clean();
                }
                #if SISTEMA == 0
             }
             #endif

             clean();
             stampa ( campo , a , b );
             srand(t);
             t++;
      }
return 0;
}

//---------------------------------------------------------------------
//Aggiorna lo snake nel campo
//---------------------------------------------------------------------
int aggiornaSnake( char **campo, char direction , int lastDirection , int **coordinateSnake , const int a , const int b , int* Pi , int* Pe ,int* presenti){
        int H,W,h,w;
                 if ( *Pi > 0){
                      H = coordinateSnake[*Pi-1][0]; // coordinate elemento di testa
                      W = coordinateSnake[*Pi-1][1]; // coordinate elemento di testa
                      h = coordinateSnake[*Pe][0]; // coordinate elemento di coda
                      w = coordinateSnake[*Pe][1]; // coordinate elemento di coda
                 }
                 else{
                      H = coordinateSnake[((a-2)*(b-2)-1)][0]; // coordinate elemento di testa
                      W = coordinateSnake[((a-2)*(b-2)-1)][1]; // coordinate elemento di testa
                      h = coordinateSnake[*Pe][0]; // coordinate elemento di coda
                      w = coordinateSnake[*Pe][1]; // coordinate elemento di coda
                      }
                 if( direction == up){
                     if (lastDirection == down) return down;
                     if( campo[H-1][W] == 'O'){ // se perdi (tagli lo snake)
                          haiPerso(campo,a,b,*presenti);
                          return lose;
                              }
                     if( campo[H-1][W] == 0){ // se incontra uno spazio vuoto
                          campo[H-1][W] = 'O';
                          campo[h][w] = 0; // cancella l' ultimo elemento dello snake
                              inserisci( direction, presenti , Pi , coordinateSnake , a , b);
                              estrai( presenti , Pe , coordinateSnake , a , b);
                              }
                     if( campo[H-1][W] == 'Z'){  //se scontra il bordo
                          campo[H+(a-3)][W] = 'O';
                          campo[h][w] = 0; // cancella l' ultimo elemento dello snake
                              inserisciBordo( direction, presenti , Pi , coordinateSnake , a , b);
                              estrai( presenti , Pe , coordinateSnake , a , b);
                              }
                     if( campo[H-1][W] == '#'){ // se trova la frutta
                          campo[H-1][W] = 'O';

                              inserisci( direction, presenti , Pi , coordinateSnake , a , b);
                              generaFrutta (campo,a,b);
                              }
                          return up;
                          }

                 if( direction == down){
                     if (lastDirection == up) return up;
                     if( campo[H+1][W] == 'O'){ // se perdi
                          haiPerso(campo,a,b,*presenti);
                          return lose;
                              }
                     if( campo[H+1][W] == 0){ //se incontra uno spazio vuoto
                          campo[H+1][W] = 'O';
                          campo[h][w] = 0;
                              inserisci( direction, presenti , Pi , coordinateSnake , a , b);
                              estrai( presenti , Pe , coordinateSnake , a , b);
                              }
                     if( campo[H+1][W] == 'Z'){ //se scontra il bordo
                          campo[H-(a-3)][W] = 'O';
                          campo[h][w] = 0;
                              inserisciBordo( direction, presenti , Pi , coordinateSnake , a , b);
                              estrai( presenti , Pe , coordinateSnake , a , b);
                              }
                     if( campo[H+1][W] == '#'){ // se trova la frutta
                          campo[H+1][W] = 'O';
                              inserisci( direction, presenti , Pi , coordinateSnake , a , b);
                              generaFrutta (campo,a,b);
                              }
                          return down;
                          }

                 if( direction == dx){
                     if (lastDirection == sx) return sx;
                     if( campo[H][W+1] == 'O'){ // se perdi
                          haiPerso(campo,a,b,*presenti);
                          return lose;
                              }
                     if( campo[H][W+1] == 0){ //se incontra uno spazio vuoto
                          campo[H][W+1] = 'O';
                          campo[h][w] = 0;
                              inserisci( direction, presenti , Pi , coordinateSnake , a , b);
                              estrai( presenti , Pe , coordinateSnake , a , b);
                              }
                     if( campo[H][W+1] == 'Z'){ // se scontra il bordo
                          campo[H][W-(b-3)] = 'O';
                          campo[h][w] = 0;
                              inserisciBordo( direction, presenti , Pi , coordinateSnake , a , b);
                              estrai( presenti , Pe , coordinateSnake , a , b);
                              }
                     if( campo[H][W+1] == '#'){ // se trova la frutta
                          campo[H][W+1] = 'O';

                              inserisci( direction, presenti , Pi , coordinateSnake , a , b);
                              generaFrutta (campo,a,b);
                              }

                          return dx;
                          }

                  if( direction == sx){
                     if (lastDirection == dx) return dx;
                     if( campo[H][W-1] == 'O'){ // se perdi
                          haiPerso(campo,a,b,*presenti);
                          return lose;
                              }
                     if( campo[H][W-1] == 0){  //se incontra uno spazio vuoto
                          campo[H][W-1] = 'O';
                          campo[h][w] = 0;
                             inserisci( direction, presenti , Pi , coordinateSnake , a , b);
                             estrai( presenti , Pe , coordinateSnake , a , b);
                              }
                     if( campo[H][W-1] == 'Z'){ // se scontra il bordo
                          campo[H][W+(b-3)] = 'O';
                          campo[h][w] = 0;
                              inserisciBordo( direction, presenti , Pi , coordinateSnake , a , b);
                              estrai( presenti , Pe , coordinateSnake , a , b);
                              }
                     if( campo[H][W-1] == '#'){  //se trova la frutta
                          campo[H][W-1] = 'O';

                              inserisci( direction, presenti , Pi , coordinateSnake , a , b);
                              generaFrutta (campo,a,b);
                              }

                          return sx;
                          }
}

//----------------------------------------------------------------
// Inserisce coordinata di testa
//----------------------------------------------------------------
void inserisci( char A , int* B , int* C , int** D , const int E , const int F){ //direzione,elementi,punt ingresso, spazio coordinate, a, b

     if(*C == 0){

           if ( A == up){
                        D[*C][0] = D[((E-2)*(F-2)-1)][0]-1; // coord X
                        D[*C][1] = D[((E-2)*(F-2)-1)][1];
                        }
              if ( A == dx){
                        D[*C][1] =  D[((E-2)*(F-2)-1)][1]+1;  // coord Y
                        D[*C][0] =  D[((E-2)*(F-2)-1)][0];
                        }
              if ( A == sx){
                        D[*C][1] = D[((E-2)*(F-2)-1)][1]-1;  // coord Y
                        D[*C][0] = D[((E-2)*(F-2)-1)][0];
                        }
              if ( A == down){
                        D[*C][0] =  D[((E-2)*(F-2)-1)][0]+1;   // coord X
                        D[*C][1] =  D[((E-2)*(F-2)-1)][1];
                        }
              }
     else{
              if ( A == up){
                        D[*C][0] = D[*C-1][0]-1; // coord X
                        D[*C][1] = D[*C-1][1];
                        }
              if ( A == dx){
                        D[*C][1] =  D[*C-1][1]+1;  // coord Y
                        D[*C][0] =  D[*C-1][0];
                        }
              if ( A == sx){
                        D[*C][1] = D[*C-1][1]-1;  // coord Y
                        D[*C][0] = D[*C-1][0];
                        }
              if ( A == down){
                        D[*C][0] =  D[*C-1][0]+1;   // coord X
                        D[*C][1] =  D[*C-1][1];
                        }
              }
     *B = *B + 1; // aggiorno numero elementi
     *C = (*C+1) % ((E-2)*(F-2)); //aggiorno il puntatore di ingresso

     }

//----------------------------------------------------------------
// Estrae coordinata di coda
//----------------------------------------------------------------
void estrai( int* B , int* C , int** D , const int E , const int F ){ // COME PRIMA MA C è IL PUNTATORE DI USCITA
      D[*C][0] = 0;
      D[*C][1] = 0;
      *B = *B - 1;
      *C = (*C+1) % ((E-2)*(F-2));
}

//---------------------------------------------------------------------
//Inizializza il campo di dimensioni a x b
//---------------------------------------------------------------------
char** initializeCampo( const int a , const int b )
{
 int i,j;
 char **campo;

 campo = (char**)malloc(a*sizeof(char*)); // alloco la memoria per "a" puntatori a char

    for ( i=0 ; i<a ; i++ ){
         campo[i] = (char*)malloc(b*sizeof(char));  // alloco la memoria per "b" puntatori char su ogni campo[i]
         }

       for ( i=0 ; i<a ; i++ ){
           for ( j=0; j<b; j++)
           {
           if ( i == 0 )
              campo[i][j] = 'Z' ; // bordo superiore
           if ( i == a-1)
              campo[i][j] =  'Z' ; // bordo inferiore

           if ( j == 0 )
              campo[i][j] =  'Z' ; // bordo sinistro

           if ( j == b-1 )
               campo[i][j] =  'Z' ;  // bordo destro

           if ( i == a/2 ){
               campo[i][b/2] = 'O';    //coda snake
               campo[i][b/2+1] = 'O';
               campo[i][b/2+2] = 'O';  // testa snake
               }

           if ( i!=0 && i!=a-1 && j!=0 && j!=b-1)
               campo[i][j] = 0;     // spazio centrale

               }
       }
    return campo;
}

//---------------------------------------------------------------------
//Inizializza lo spazio coordinate con le coordinate dello snake di default
//---------------------------------------------------------------------
int** creaSpazioCoordinate( const int a, const int b)
{
      int i,j;
      int **coordinateSnake;

      coordinateSnake = (int**)malloc(((a-2)*(b-2))*sizeof(int*));
         for ( i=0 ; i<(a-2)*(b-2) ; i++)
            coordinateSnake[i] = (int*)malloc(2*sizeof(int));

         for( i=0 ; i<(a-2)*(b-2) ; i++)
            for( j=0 ; j<2 ; j++)
                   coordinateSnake[i][j] = 0;
         //inserisco le coordinate dello snake di default

         coordinateSnake[0][0] = a/2;
         coordinateSnake[0][1] = b/2;
         coordinateSnake[1][0] = a/2;
         coordinateSnake[1][1] = b/2+1;
         coordinateSnake[2][0] = a/2;
         coordinateSnake[2][1] = b/2+2;

       return coordinateSnake;
}

//---------------------------------------------------------------------
//Stampa il campo ad ogni ciclo
//---------------------------------------------------------------------
void stampa( char **campo , const int a , const int b)
{
     int i,j;

         for( i=0 ; i<a ; i++)
         {
              for( j=0 ; j<b ; j++)
              {
                printf("%c ",campo[i][j]);
              }

          printf("\n");
         }
}

//---------------------------------------------------------------------
//Funzione di controllo che stampa le coordinate dello snake
//---------------------------------------------------------------------
void stampaInt( int **coordinateSnake , const int a , const int b)
{
     int i,j;

          for( i=0 ; i<(a-2)*(b-2) ; i++)
         {
              for( j=0 ; j<2 ; j++)
              {
               printf("%d ",coordinateSnake[i][j]);

              }
              printf("\t");

          if((i+1) % 8 == 0 )
          printf("\n");
         }
}

//----------------------------------------------------------------
// Inserisce coordinate in caso di scontro con il bordo
//----------------------------------------------------------------
void inserisciBordo( char A , int* B , int* C , int** D , const int E , const int F){ //direzione elmenti,punt ingresso, spazo coordinate, a, b

    if (*C == 0 ){

           if ( A == up){
                        D[*C][0] = D[((E-2)*(F-2)-1)][0]+(E-3); // coord X
                        D[*C][1] = D[((E-2)*(F-2)-1)][1];
                        }
              if ( A == dx){
                        D[*C][1] =  D[((E-2)*(F-2)-1)][1]-(F-3);  // coord Y
                        D[*C][0] = D[((E-2)*(F-2)-1)][0];
                        }
              if ( A == sx){
                        D[*C][1] = D[((E-2)*(F-2)-1)][1]+(F-3);;  // coord Y
                        D[*C][0] = D[((E-2)*(F-2)-1)][0];
                        }
              if ( A == down){
                        D[*C][0] =  D[((E-2)*(F-2)-1)][0]-(E-3);   // coord X
                        D[*C][1] = D[((E-2)*(F-2)-1)][1];
                        }
           }
    else{

              if ( A == up){
                        D[*C][0] = D[*C-1][0]+(E-3); // coord X
                        D[*C][1] = D[*C-1][1];
                        }
              if ( A == dx){
                        D[*C][1] = D[*C-1][1]-(F-3);  // coord Y
                        D[*C][0] = D[*C-1][0];
                        }
              if ( A == sx){
                        D[*C][1] = D[*C-1][1]+(F-3);;  // coord Y
                        D[*C][0] = D[*C-1][0];
                        }
              if ( A == down){
                        D[*C][0] = D[*C-1][0]-(E-3);   // coord X
                        D[*C][1] = D[*C-1][1];
                        }
                        }
     *B = *B + 1; // aggiorno numero elementi
     *C = (*C+1) % ((E-2)*(F-2)); //aggiorno il puntatore di ingresso
}

//----------------------------------------------------------------
// Genera frutta nel campo
//----------------------------------------------------------------
void generaFrutta( char **campo, const int a , const int b )
{
    int i,j;

    do {
           i = rand() % (a-2);
           j = rand() % (b-2);
           }while( campo[i][j] != 0 );

     campo[i][j] = '#';
}

//----------------------------------------------------------------
// Se perdi stampa a schermo hai perso e chiude il programma
//----------------------------------------------------------------
void haiPerso(char** campo ,const int a ,const int b,int c){

     campo[a/2][b/2-4] = 'H';       campo[a/2][b/2-2] = 'I';   campo[a/2][b/2] = 'P';       campo[a/2][b/2+1] = 'E';   campo[a/2][b/2+4] = 'O';
     campo[a/2][b/2-3] = 'A';       campo[a/2][b/2-1] = ' ';   campo[a/2][b/2+2] = 'R';     campo[a/2][b/2+3] = 'S';

     clean();
     stampa ( campo , a , b );
     printf("\n\tIL TUO PUNTEGGIO E': %d",c);
     pausa();
}

//----------------------------------------------------------------
// Blocca il programma finche non viene premuto un tasto
//--------------------------------------------------------------
void pausa(void)
{
	printf("\n\n\t<o>Premere invio....");
	getchar();
}

//----------------------------------------------------------------
// Pulisce los chemo
//--------------------------------------------------------------
void clean(void){
     
     #if SISTEMA == 1
     system("clear");
     #else
     system("cls");
     #endif     
}

//----------------------------------------------------------------
// Istruzioni
//--------------------------------------------------------------

void istruzioni(void){
    clean();
    printf("\n                          ____  _  _  ____        ____          \n"
    "            /^\\/^\\       [__    |\\ |  |__|  |__/  |___                  \n"
    "          _|__|  O|      ___]   | \\|  |  |  |  \\  |___         \n"
    " \\/     /~     \\_/ \\                                     \n"
    "  \\____|__________/  \\                                    \n"
    "         \\_______      \\                                  \n"
    "                 `\\     \\                 \\           \n"
    "                   |     |                  \\ \n"
    "                  /      /                    \\ \n"
    "                 /     /                       \\\\ \n"
    "               /      /                         \\ \\ \n"
    "              /     /                            \\  \\ \n"
    "            /     /             _----_            \\  \\ \n"
    "           /     /           _-~      ~-_         |   | \n"
    "          (      (        _-~    _--_    ~-_     _/   | \n"
    "           \\      ~-____-~    _-~    ~-_    ~-_-~    /        By\n"
    "            ~-_           _-~          ~-_       _-~      - Antonio - \n"
    "                ~--______-~                ~-___-~ \n\n\n");

    printf("\t<o>Parametri da passare: [righe] [colonne] [diffcolta']\n"
           "\t<o>Numero di righe (10/30): \n"
           "\t<o>Numero di colonne (10/30): \n"
           "\t<o>Livello di diffolta': (1)facile  (2)medio  (3)diffcile\n"
           "\t<o>Pilotare il serpente con i caratteri A-S-D-W ;)\n");
    pausa();
}

#if SISTEMA == 1
char kbhit(void)
{
    struct termios oldt, newt;
    char ch;

    tcgetattr( STDIN_FILENO, &oldt );// scrive i parametri associati all' input terminale e li inserisce in una struttura di tipo termios
    newt = oldt;

    /*se VMIN =0 e VTIME =0 si entra in una modalità in cui la chiamata è soddisfatta dalla coda di input del driver (un buffer)*/
    newt.c_cc[VMIN] = 0;   // VMIN = contatore di caratteri che va da 0-255
    newt.c_cc[VTIME] = 1;  // VTIME = se non è zero è interpretatato come decimi di secondi (tempo di lettura)
    newt.c_lflag &= ~( ICANON | ECHO ); //ICANON imposta la modalità non canonica di input spiegata in basso
    tcsetattr( STDIN_FILENO, TCSANOW, &newt ); //TCSANOW = il cambio avviene immediatamente

    ch = getchar();

    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );

    return ch;
}
#endif
/*
   In canonical input processing mode, terminal input is processed in lines
   terminated by newline ('\n'), EOF, or EOL characters. No input can be read
   until an entire line has been typed by the user, and the read function
   (see I/O Primitives) returns at most a single line of input, no matter how many bytes are requested.

   In noncanonical input processing mode, characters are not grouped into lines,
   and ERASE and KILL processing is not performed. The granularity with which bytes
   are read in noncanonical input mode is controlled by the MIN and TIME settings. See Noncanonical Input.
*/
