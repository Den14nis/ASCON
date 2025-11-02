#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ASCON RIDOTTO usa i seguenti parametri :
    Chiave segreta K = 0110  (4 bit)
    Nonce N di 4 bit = 1101
    Dati associati A da 16 bit = 0110111100100101
    Plaintext P di 14 bit = 01110001110101
    5 S-BOX uguali ad Ascon-128
    Stato interno di Ascon S da 20 bit
    Tag di autenitcazione T di 4 bit

    Permutazione di a round = 4 round
    Permutazione di b round = 2 round

*/
  //FUNZIONI
/* prendo la dimensione dell'array e converto il numero in binario */
void int_bin(int *a, int v, int num) {
    int i;
    for (i = 0; i < num; i++) {
        a[i] = (v >> (num - 1 - i)) & 1;
    }
}

/* concateno i quattro array per creare vettore IV */
void concatena ( int *dest, int *a, int *b, int *c, int *d, int num){

        for (int i = 0; i < num; i++) {
        dest[i] = a[i];
        }

        for (int i = 0; i < num; i++) {
        dest[num + i] = b[i];
        }

        for (int i = 0; i < num; i++) {
        dest[2 * num +i] = c[i];
        }

        for (int i = 0; i < num; i++) {
        dest[3 * num +i] = d[i];
        }
    }

    /* funzione per stato iniziale S */
    void concatena2 ( int *dest, int *a, int lenght_a,int *b, int lenght_b, int *c, int lenght_c){

            int i;
            int posizione = 0;
               for (i = 0; i < lenght_a; i++) {
               dest[posizione++] = a[i];
               }

               for (i = 0; i < lenght_b; i++){
                dest[posizione++] = b[i];
               }

               for (i=0; i<lenght_c; i++){
                dest[posizione++] = c[i];
               }
    }

    /* XOR bit a bit */
    void xor(int *dest, int*a,int *b,int  n){
        for(int i=0; i<n; i++){
        dest[i] = a[i] ^ b[i];
    }
    }

    /* per passare da righe a colonne devo fare la matrice trasposta */
    void trasposta(int* righe[5], int *colonne[4]){
         for(int i=0; i<5;i++){
             for (int j=0; j<4;j++){
                 colonne[j][i] = righe[i][j];
                }
            }
    }

    /* Funzione che prende gli array colonne di 5 bit e li converte in interi ( che sono numeri binari da convertire in esadecimale) */
     int ToInt(int array[]){
      int valore = 0;
      for (int i=0;i<5;i++){
        valore = (valore << 1) | array[i];
      }
      return valore;
     }

     /* funzione che passa da binario a esadecimale */
     void BinToHex (int *binario[4],int esadecimale[4]){
      for (int i=0; i< 4;i++){
        esadecimale[i] = ToInt(binario[i]);
      }
     }

    /* S-BOX */
    int  SBOX (int valore){
    const char * SboxINPUT[32] = {"0", "1", "2", "3", "4", "5", "6", "7","8", "9", "a", "b", "c", "d", "e", "f","10", "11", "12", "13", "14", "15", "16", "17","18", "19", "1a", "1b", "1c", "1d", "1e", "1f"};
    const char * SBoxOUTPUT[32] = {"4","b","1f","14","1a","15","9","2","1b","5","8","12","1d","3","6","1c","1e","13","7","e","0","d","11","18","10","c","1","19","16","a","f","17"};

    //visto che in input avrò degli esadecimali salvati in variabili intere devo fare un casting per confrontarle con i char
    char buffer[5];
    sprintf(buffer,"%x",valore); // converte int in esadecimale (minuscolo)

    for(int i=0; i<32; i++) {
        if(strcmp(buffer,SboxINPUT[i])==0){
           //converto la stringa esadecimale da char a intero (così avrò un ritorno da intero)
           int ris;
           sscanf(SBoxOUTPUT[i],"%x",&ris); // da stringa a intero (esadecimale)
           return ris;
           }
    }

    return -1; // valore non trovato

    }



   /* funzione che passa da  esadecimale a binario */
     void HexToBin (int esadecimale, int *binario){
      for (int i=0; i< 5;i++){
        binario[i] = (esadecimale >> (4 - i)) & 1;
      }
     }

   /* per passare da righe a colonne devo fare la matrice trasposta */
    void trasposta2(int* righe[4],int *colonne[5]){
         for(int i=0; i<5;i++){
             for (int j=0; j<4;j++){
                 //righe[j][i] = colonne[i][j];
                    colonne[i][j] = righe[j][i];

                }
            }
    }

 /* diffusione lineare : prendo i registri in input ed eseguo uno shift verso destra pari al numero indicato */
  void diffusione (int *input, int * output, int val){

   val = val %4; // normalizza il risultato visto che ci possono essere numero maggiori di 4 e bisogna ripetere in modo ciclico

   for(int i=0;i<4;i++){
       output[(i+ val) % 4] = input[i];
      }
   }


   /* per aggiornare lo stato S dopo i round di permutazione : concateno i 5 registri per ottenre S  */
 void concatena3 ( int *dest, int *a, int *b, int *c, int *d, int *e, int num){

        for (int i = 0; i < num; i++) {
        dest[i] = a[i];
        }

        for (int i = 0; i < num; i++) {
        dest[num + i] = b[i];
        }

        for (int i = 0; i < num; i++) {
        dest[2 * num +i] = c[i];
        }

        for (int i = 0; i < num; i++) {
        dest[3 * num +i] = d[i];
        }
        for (int i = 0; i < num; i++) {
        dest[4 * num +i] = e[i];
        }
    }

  void concatenaSrSc(int* Sr, int* Sc, int* S) {
    for (int i = 0; i < 4; i++) {
        S[i] = Sr[i];        // Copia i primi 4 bit
    }
    for (int i = 0; i < 16; i++) {
        S[i + 4] = Sc[i];   // Copia i 16 bit successivi
    }
  }

   // Funzione per convertire un array binario in intero
   int binToInt(int bin[], int n) {
    int valore = 0;
    for (int i = 0; i < n; i++) {
        valore = (valore << 1) | bin[i];
    }
    return valore;
   }

   //Funzione ROUND DI PERMUTAZIONE
   void round_permutazione(int S[20], int costante[4]) {

    int X0[4], X1[4], X2[4], X3[4], X4[4]; //registri di stato
    int X0new[4], X1new[4], X2new[4], X3new[4], X4new[4]; //registri aggiornati
    int temp[4];
    int new1[5], new2[5], new3[5], new4[5]; // i 4 puntatori che conterranno le colonne
    int *colonne[4] = {new1, new2, new3, new4}; // array di colonne
    int *righe[5] = {X0, X1, X2new, X3, X4}; //array di puntatori alle righe

    int esadecimale[4], bin1[5], bin2[5], bin3[5], bin4[5];  //contenitore dei 4 valori esadecimali

    int *righe2[4] = {bin1, bin2, bin3, bin4};  //per la seconda trasposta ho bisogno di 4 righe composte da array di 4 bit
    int col0[4], col1[4], col2[4], col3[4], col4[4];// array colonne da 4 bit
    int *colonne2[5] = {col0, col1, col2, col3, col4}; //array che contiene le colonne

    // Divido lo stato in registri
    for (int i = 0; i < 4; i++) {
        X0[i] = S[i];
        X1[i] = S[4 + i];
        X2[i] = S[8 + i];
        X3[i] = S[12 + i];
        X4[i] = S[16 + i];
    }

    // XOR di X2 con costante
    xor(X2new, X2, costante, 4);

    // Trasposizione
    trasposta(righe, colonne);

    //converto i valori in esadecimali
    BinToHex(colonne, esadecimale);

    //chiamo la funzione S-BOX
    int hex1new = SBOX(esadecimale[0]);
    int hex2new = SBOX(esadecimale[1]);
    int hex3new = SBOX(esadecimale[2]);
    int hex4new = SBOX(esadecimale[3]);

    //converto i valori esadecimali in binari
    HexToBin(hex1new, bin1);
    HexToBin(hex2new, bin2);
    HexToBin(hex3new, bin3);
    HexToBin(hex4new, bin4);

    trasposta2(righe2, colonne2);

    for (int i = 0; i < 4; i++) {
        X0[i] = col0[i];
        X1[i] = col1[i];
        X2[i] = col2[i];
        X3[i] = col3[i];
        X4[i] = col4[i];
    }

    // Diffusione Lineare
    diffusione(X0, temp, 19); diffusione(X0, col0, 28); xor(X0, X0, temp, 4); xor(X0new, X0, col0, 4);
    diffusione(X1, temp, 61); diffusione(X1, col1, 39); xor(X1, X1, temp, 4); xor(X1new, X1, col1, 4);
    diffusione(X2, temp, 1);  diffusione(X2, col2, 6);  xor(X2, X2, temp, 4); xor(X2new, X2, col2, 4);
    diffusione(X3, temp, 10); diffusione(X3, col3, 17); xor(X3, X3, temp, 4); xor(X3new, X3, col3, 4);
    diffusione(X4, temp, 7);  diffusione(X4, col4, 41); xor(X4, X4, temp, 4); xor(X4new, X4, col4, 4);

    concatena3(S, X0new, X1new, X2new, X3new, X4new, 4);
}


   //serve per calcolare la costante di round nelle permutazioni con il parametro b
   int *getCostante(int i,int *a_bin, int *b_bin, int *array[4]){
    int a = binToInt(a_bin,3);
    int b = binToInt(b_bin,3);
    int indice = i + a - b;

    return array[indice];

   }

    void PaddingChiave(int chiave[4], int chiavePadding[20]){
     //aggiungo 4 (pari al bit rate) zeri in testa
     for(int i=0;i<4;i++) {
        chiavePadding[i]=0;
     }

     //copio la chiave
       for(int i=0;i<4;i++) {
        chiavePadding[i+4]=chiave[i];
     }

     //aggiungo i 12 (20-k-r = 20-4-4) zeri in coda
       for(int i=8;i<20;i++) {
        chiavePadding[i]=0;
     }

    }

     /* funzione per concatenare i blocchi di ChiperText  */
    void concatenaChiperText (int *dest, int *C0, int lenght_C0,int *C1, int lenght_C1, int *C2, int lenght_C2,int *C3, int lenght_C3){

            int i;
            int posizione = 0;
               for (i = 0; i < lenght_C0; i++) {
               dest[posizione++] = C0[i];
               }

               for (i = 0; i < lenght_C1; i++){
                dest[posizione++] = C1[i];
               }

               for (i=0; i<lenght_C2; i++){
                dest[posizione++] = C2[i];
               }
               for (i=0; i<lenght_C3; i++){
                dest[posizione++] = C3[i];
               }
    }


int main()
{
    /* dichiarazione array e variabili */

    int K[4] = {0,1,1,0} ;   // chiave segreta
    int N[4] = {1,1,0,1};    // nonce
    int A[16] = {0,1,1,0,1,1,1,1,0,0,1,0,0,1,0,1};    // dati associati
     int P[14] = {0,1,1,1,0,0,0,1,1,1,0,1,0,1} ;       // plaintext

    /* i due indici di permutazione a e b vengono considerati come integer da 3 bit */
    int a[3] = {1,0,0};   // 4 in binario
    int b[3] = {0,1,0};   // 2 in binario

    /* prendo la dim di k, il bit rate e gli scrivo come integer da 3 bit  */
    int sizeK = sizeof(K) / sizeof(K[0]);
    int dimK[3];    // perchè bisogna convertire la dim della chiave in integer da 3 bit
    int_bin(dimK,sizeK,3);

    int sizeN = sizeof(N) / sizeof(N[0]);
    int dimN[3];    // perchè bisogna convertire la dim della chiave in integer da 3 bit
    int_bin(dimN,sizeN,3);

    /* per ottenere IV devo concatenare dim K, bit rate convertiti con a e b */

    int IV [12] ;
    concatena(IV,dimK,dimN,a,b,3);

    /* a questo punto concateno IV con la chiave segreta K e il nonce N per ottenere lo stato iniziale S */

    int S[20]; /* stato iniziale */

    concatena2(S,IV,12,K,4,N,4);

   /* Costanti di round */
   int V0[4] = {0,1,0,0};
   int V1[4] = {0,0,1,0};
   int V2[4] = {1,0,1,0};
   int V3[4] = {1,0,0,0};

   // Creo un vettore con dentro le costanti che poi mi servirà con le permutazioni di round con paramentro b
   int *Vettore[4] = {V0,V1,V2,V3};

  /* FASE DI PERMUTAZIONE  con parametro a */
    round_permutazione(S, V0); // P0
    round_permutazione(S, V1); // P1
    round_permutazione(S, V2); // P2
    round_permutazione(S, V3); // P3

    /* Dopo aver svolto 4 round bisogna effettuare uno xor fra lo stato S ottenuto e la chiave K di 4 bit (alla quale vengono aggiunti 16 zeri) */

    int Knew[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    //inserisco gli zeri prima della chiave
    for(int i=0;i<4;i++){
        Knew[16+i]=K[i];
    }

    xor(S,S,Knew,20);

    /* FINE FASE INIZIALIZZAZIONE e */
    /* INZIO FASE ELABORAZIONE DEI DATI ASSOCIATI */

   //I dati associati  A[16] = {0,1,1,0,1,1,1,1,0,0,1,0,0,1,0,1} vanno divisi in 4 parti da 4 bit;
    int A0[4],A1[4],A2[4],A3[4];

    for (int i = 0; i < 4; i++) {
    A0[i] = A[i];
    A1[i] = A[4 + i];
    A2[i] = A[8 + i];
    A3[i] = A[12 + i];
   }

    //Effettuo  la prima divisione di stato con lo stato aggiornato S
    int  Sr[4];   // parte decisa dal bit rate di 4 bit
    int  Sc[16];   // parte restante

    /* copio i 4 bit in Sr */
    for (int i=0;i < 4; i++) {
        Sr[i] = S[i];
    }
    /* copio i restanti 16 bit in Sc */
    for (int i=0;i < 16; i++) {
        Sc[i] = S[4+i];
    }

    //Biosgna svolgere lo XOR fra la parte di stato Sr e il  blocco di dati associati A0
    int SrNew[4];

    xor(SrNew,Sr,A0,4);

    //Concateno la nuova parte SrNew a Sc per ottenere il nuovo stato S
    concatenaSrSc(SrNew,Sc,S);

   /* PERMUTAZIONE DI ROUND con parametro b  (nel nostro caso b=2)*/
   //costante = i + a − b   (i indica il round)

    /* ROUND DI PERMUTAZIONE  b per A0 */
    round_permutazione(S, getCostante(0, a, b, Vettore));
    round_permutazione(S, getCostante(1, a, b, Vettore));

    /*  Termine operazioni sul  blocco di dati A0 e  Inzio elaborazione blocco dati A1*/


    /* copio i 4 bit in Sr */
    for (int i=0;i < 4; i++) {
        Sr[i] = S[i];
    }
    /* copio i restanti 16 bit in Sc */
    for (int i=0;i < 16; i++) {
        Sc[i] = S[4+i];
    }

     xor(SrNew,Sr,A1,4);

    //Concateno la nuova parte SrNew a Sc per ottenere il nuovo stato S
    concatenaSrSc(SrNew,Sc,S);

    /* ROUND DI PERMUTAZIONE  b per A1 */
    round_permutazione(S, getCostante(0, a, b, Vettore));
    round_permutazione(S, getCostante(1, a, b, Vettore));

    /*  Termine operazioni sul  blocco di dati A1 e Inzio elaborazione blocco dati A2 */
    //Effettuo nuovamente la prima divisione di stato con lo stato aggiornato S

    /* copio i 4 bit in Sr */
    for (int i=0;i < 4; i++) {
        Sr[i] = S[i];
    }
    /* copio i restanti 16 bit in Sc */
    for (int i=0;i < 16; i++) {
        Sc[i] = S[4+i];
    }

     xor(SrNew,Sr,A2,4);

    //Concateno la nuova parte SrNew a Sc per ottenere il nuovo stato S
    concatenaSrSc(SrNew,Sc,S);

    /* ROUND DI PERMUTAZIONE  b per A2 */
    round_permutazione(S, getCostante(0, a, b, Vettore));
    round_permutazione(S, getCostante(1, a, b, Vettore));

     /* Termine operazioni sul  blocco di dati A2 e  Inzio elaborazione blocco dati A3*/
     //Effettuo nuovamente la prima divisione di stato con lo stato aggiornato S

    /* copio i 4 bit in Sr */
    for (int i=0;i < 4; i++) {
        Sr[i] = S[i];
    }

    /* copio i restanti 16 bit in Sc */
    for (int i=0;i < 16; i++) {
        Sc[i] = S[4+i];
    }

     xor(SrNew,Sr,A3,4);

    //Concateno la nuova parte SrNew a Sc per ottenere il nuovo stato S
    concatenaSrSc(SrNew,Sc,S);

    /* ROUND DI PERMUTAZIONE  b per A3 */
    round_permutazione(S, getCostante(0, a, b, Vettore));
    round_permutazione(S, getCostante(1, a, b, Vettore));

   /* Dopo l’elaborazione dell’ultimo blocco di dati associati, viene eseguita
    un'operazione finale di XOR tra  stato  S e un vettore della
   stessa dimensione dello stato contenente un singolo 1 preceduto da 19 zeri.  */

   int Z[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
   xor(S,S,Z,20);

    /* ELABORAZIONE DEL PLAINTEXT */
    //Plaintext P di 14 bit = 01110001110101   va diviso in blocchi di grandezza pari al bit rate ( nel nostro caso 4)

    int P0[4],P1[4],P2[4],P3[2];

    for (int i = 0; i < 4; i++) {
    P0[i] = P[i];
    P1[i] = P[4 + i];
    P2[i] = P[8 + i];
    }

    for (int i = 0; i < 2; i++) {
     P3[i] = P[12 + i];
    }

    //visto che 14 (grandezza plaintext) non è multiplo di 4 ( bit rate) il blocco p3 è di 2 bit e grazie al padding lo rendiamo di 4 bit aggiungendo 1 finale e una serie di zeri prima (in questo caso un solo zero)
      int P3pad[4];

     //salvo P3 in P3pad
     for(int i=0 ; i<2;i++){
        P3pad[i] = P3[i];
     }

     P3pad[2] = 0; //aggiungo uno zero per padding
     P3pad[3] = 1; // setto ad 1 il bit finale

     //Effettuo nuovamente la prima divisione di stato con lo stato aggiornato S

    /* copio i 4 bit in Sr */
    for (int i=0;i < 4; i++) {
        Sr[i] = S[i];
    }

    /* copio i restanti 16 bit in Sc */
    for (int i=0;i < 16; i++) {
        Sc[i] = S[4+i];
    }

    //Blocco di ciphertext C0 = XOR fra P0 e SR
    int C0[4];
    xor(C0,P0,Sr,4);

    //adesso dal blocco C0 concateno Sc e ottengo il nuovo stato S
    concatenaSrSc(C0,Sc,S);


    /* ROUND DI PERMUTAZIONE  b per A0 */
    round_permutazione(S, getCostante(0, a, b, Vettore));
    round_permutazione(S, getCostante(1, a, b, Vettore));

    //ADESSO DIVIDO S IN Sc e Sr

    for (int i=0;i < 4; i++) {
        Sr[i] = S[i];
    }

    for (int i=0;i < 16; i++) {
        Sc[i] = S[4+i];
    }

    /* Adesso bisogna svolgerele stesse operazioni per C1 */

   //Blocco di ciphertext C1 = XOR fra P1 e SR
    int C1[4];
    xor(C1,P1,Sr,4);

    //adesso dal blocco C1 concateno Sc e ottengo il nuovo stato S
    concatenaSrSc(C1,Sc,S);

   /* ROUND DI PERMUTAZIONE  b per A1 */
    round_permutazione(S, getCostante(0, a, b, Vettore));
    round_permutazione(S, getCostante(1, a, b, Vettore));

    //ADESSO DIVIDO S IN Sc e Sr
    for (int i=0;i < 4; i++) {
        Sr[i] = S[i];
    }

    for (int i=0;i < 16; i++) {
        Sc[i] = S[4+i];
    }


    /* Adesso bisogna svolgerele stesse operazioni per C2 */
   //Blocco di ciphertext C2 = XOR fra P2 e SR
    int C2[4];
    xor(C2,P2,Sr,4);


    //adesso dal blocco C2 concateno Sc e ottengo il nuovo stato S
    concatenaSrSc(C2,Sc,S);


    /* ROUND DI PERMUTAZIONE  b per A2 */
    round_permutazione(S, getCostante(0, a, b, Vettore));
    round_permutazione(S, getCostante(1, a, b, Vettore));

    //ADESSO DIVIDO S IN Sc e Sr


    for (int i=0;i < 4; i++) {
        Sr[i] = S[i];
    }

    for (int i=0;i < 16; i++) {
        Sc[i] = S[4+i];
    }


    /* Per il blocco P3pad viene elaborato diversamente, anche se viene generato nel solito modo */

    //Blocco di ciphertext C3pad = XOR fra P3pad e SR
    int C3pad[4];
    xor(C3pad,P3pad,Sr,4);

    //adesso dal blocco C2 concateno Sc e ottengo il nuovo stato S
    concatenaSrSc(C3pad,Sc,S);

    //Visto che il blocco originale P3 ha dim=2 bisogna troncare il blocco C3pad per ottenre C3
    int C3[2];

    //inserisco
    for(int i=0;i<2;i++){
        C3[i] = C3pad[i];
    }

     /* INIZIO FASE DI FINALIZZAZIONE */
     //XOR fra stato S e Chiave segreta K

     /*K = 0110
     per permettere lo xor :
     vengono aggiunti in testa 0 pari al bit rate (in questo caso 4) e
     vengono aggiunti in coda una serie di 0 pari a 20 - r +k, 20 è dim dello stato e k dim chiave
     -> in testa aggiungere 0000
     -> in coda aggiungere : 20-4-4=12 zeri */

     int Kpadding[20];
     PaddingChiave(K,Kpadding);

     xor(S,S,Kpadding,20); //eseguo lo xor

     /* Adesso dopo aver aggiornato lo stato S bisogna eseguire una permutazione di a (4) round */

    round_permutazione(S, V0); // P0
    round_permutazione(S, V1); // P1
    round_permutazione(S, V2); // P2
    round_permutazione(S, V3); // P3

    /*Fine dei 4 round di permutazione */

    /* Generazione tag T di autenticazione */

    int T[4]; // Tag T di autenticazione  ( xor fra K e 4 bit meno significativi di S)

    //salvo i 4 bit meno significativi dell'ultimo stato aggiornato
    int tag[4];

    for(int i=0;i<4;i++){
        tag[i]= S[16+i];
    }

    //faccio lo xor fra K e I primi 4 bit di S
    xor(T,tag,K,4);

    /* Ultima fase consiste nel concatenari i blocchi di chipertext ottenuti */

    int C[14];

    concatenaChiperText(C,C0,4,C1,4,C2,4,C3,2);

    //Stampo Chipertext e tag di autenticazione
    printf("Chipertext C = ");
    for(int i=0;i<14;i++){
        printf("%d",C[i]);
    }
    printf("\nTag T = ");
     for(int i=0;i<4;i++){
        printf("%d",T[i]);
    }
    return 0;
}
