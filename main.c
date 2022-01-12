#include <stdio.h>
#include "gamelib.h"

int main(){
    int scelta;
    int flagimpostato=0; // flag per sapere se il gioco e' stato impostato
    do{
        printf("--------------- Menu' ---------------\n"); // Stampo menu'
        printf(" 1) Imposta gioco\n");
        printf(" 2) Combatti\n");
        printf(" 3) Termina gioco\n");
        printf(" Inserisci scelta: ");
        scanf("%d",&scelta); // input scelta
        printf("\n");
	while((getchar()) != '\n');
        switch(scelta){ // in base alla scelta fatta, vedo cosa fare
            case 1:
                imposta_gioco(); // imposto il gioco
                flagimpostato=1; // setto flag a 1
                break;
            case 2:
                if(flagimpostato){ // se flag e' 1 (gioco e' stato impostato)
                    combatti();
                    flagimpostato=0; // la partita e' finita, il gioco deve essere reimpostato
                }
                else // altrimenti
                    printf(" Imposta il gioco prima di combattere!\n");
                break;
            case 3:
                termina_gioco(); // termina gioco ed esci dal programma
                break;
            default:
                printf("Il comando inserito e' sbagliato!\n"); // scelta non valida
                break;
        }
        printf("\n");
    }while(scelta!=3);
    return 0; // terminazione del programma
}