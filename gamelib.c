#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "gamelib.h"
// variabili Mago per i due giocatori
Mago* giocatore1;
Mago* giocatore2;
/* funzione per input della classe del mago */
static int setClasseMago(){
    int cm;
    printf("1) Tenebre\n"); // stampo classi
    printf("2) Vita\n");
    printf("3) Luce\n");
    do{ // input con controllo validita'
        printf("Inserisci classe mago: ");
        scanf("%d",&cm);
    }while(cm!=1 && cm!=2 && cm!=3);
    return cm;
}
/* funzione che dato il mago, ritorna la classe */
static char* getClasseMago(Mago* m){
    switch(m->classemago){
        case 1: return "Tenebre"; break;
        case 2: return "Vita"; break;
        case 3: return "Luce"; break;
    }
    return "";
}
/* funzione che data la carta, ritorna il suo tipo */
static char* getTipoCarta(Carta* c){
    switch(c->tipocarta){
        case 1: return "Creatura"; break;
        case 2: return "Rimuovi creatura"; break;
        case 3: return "Infliggi danno"; break;
        case 4: return "Guarisci danno"; break;
    }
    return "";
}
/* funzione che genera casualmente una carta con un massimo di punti vita(maxPuntiCarta) */
static Carta* generaCarta(int maxPuntiCarta){
    Carta* c=(Carta*)malloc(sizeof(Carta)); // alloco dinamicamente spazio per una nuova carta
    int r=rand()%100; // genero numero casuale tra 0 e 99
    // scelgo il tipo della carta in base alle percentuali della consegna
    if(r<40)
        c->tipocarta=1; // creatura
    else if(r<65)
        c->tipocarta=2; // rimuovi_creatura
    else if(r<85)
        c->tipocarta=3; // infliggi_danno
    else
        c->tipocarta=4; // guarisci_danno
    c->punti_vita=(rand()%maxPuntiCarta)+1;
    c->next=NULL;
    return c;
}
/* funzione che data una lista l, aggiunge c in coda a l */
static Carta* inserisciincoda(Carta* l,Carta* c){
    Carta* prec;
    if(l==NULL) // se la lista e' vuota
        l=c; // la lista diventa il nuovo nodo
    else{ // se la lista non e' vuota
        for(prec=l;prec->next!=NULL;prec=prec->next); // scorro tutta la lista fino alla fine
        prec->next=c; // aggancio al nodo finale il nuovo nodo
    }
    return l;
}
/* funzione che genera un mazzo di dimensione dim */
static void generaMazzo(Mago* m,int dim){
    for(int i=0;i<dim;i++){
        Carta* tmp=generaCarta(dim-i+1); // genero carta con max punti vita= dim-i+1
        if(m->classemago==2 && tmp->tipocarta==1) // se il mago ha classe vita
            tmp->punti_vita*=1.5; // le sue carte creatura hanno la meta' dei punti_vita in piu'
        if(m->classemago==1 && tmp->tipocarta==3) // se tenebra
            tmp->punti_vita*=2; // le sue carte infliggi_danno hanno il doppio dei punti_vita
        if(m->classemago==3 && tmp->tipocarta==4) // se luce
            tmp->punti_vita*=3.5; // le sue carte guarisci_danno hanno tre volte e mezzo il valore dei punti_vita
        m->mazzo=inserisciincoda(m->mazzo,tmp);
    }
}
/* funzione che, dato un mago m, gli riempio le mani con 5 carte */
static void riempiMani(Mago* m){
    for(int i=0;i<5;i++)
        m->mano[i]=generaCarta(8); // genero 5 carte con max punti vita= 8
    m->mano[5]=NULL; // l'ultima carta in mano non viene riempita
}
/* funzione che, dato un mago m, ne stampa il mazzo */
static void stampa_mazzo(Mago* m){
    Carta* tmp=m->mazzo;
    while(tmp!=NULL){ // fintanto che ci sono carte
        if(tmp->tipocarta==2)
		    printf(" %spv\n",getTipoCarta(tmp)); // le stampo
        else
            printf(" %s - %dpv\n",getTipoCarta(tmp),tmp->punti_vita); // le stampo
		tmp=tmp->next; // vado al prossimo nodo
	}
}
/* funzione che, dato un mago m, ne stampa la mano */
static int stampa_mano(Mago* m){
    int i=0; // inizializzo il contatore a 0
    while(m->mano[i]!=NULL && i<6){ // fintanto che ci sono carte e il contatore e' minore di 6
        if(m->mano[i]->tipocarta==2) // se la carta e' di tipo rimuovi_creatura, non stampo i pv
            printf(" %d) %s\n",i+1,getTipoCarta(m->mano[i])); // stampo carta
        else // altrimenti si
            printf(" %d) %s - %dpv\n",i+1,getTipoCarta(m->mano[i]),m->mano[i]->punti_vita); // stampo carta
        i++; // incremento contatore
    }
    return i;
}
/* funzione che, dato un mago m, ne stampa le carte in campo */
static int stampa_campo(Mago* m){
    int i=0; // inizializzo contatore a 0
    while(m->campo[i]!=NULL && i<4){// fintanto che ci sono carte e il contatore e' minore di 4
        if(m->campo[i]->tipocarta==2) // se la carta e' di tipo rimuovi_creatura, non stampo i pv
            printf(" %d) %s\n",i+1,getTipoCarta(m->campo[i])); // stampo carta
        else // altrimenti si
            printf(" %d) %s - %dpv\n",i+1,getTipoCarta(m->campo[i]),m->campo[i]->punti_vita); // stampo carta
        i++; // incremento contatore
    }
    return i;
}
/* funzione che, dato una lista di carte, ne estrae l'ultima */
static Carta* estraiultimo(Carta* l){
    if(l==NULL) // la lista e' vuota
        return NULL;
    else if(l->next==NULL) // la lista contiene un solo nodo
        l=NULL;
    else{ // la lista contiene piu' nodi
        Carta* tmp=l;
        Carta* tmp2=l;
        while(tmp->next!=NULL){ // fintanto che ci sono nodi
            tmp2=tmp; // second ultimo
            tmp=tmp->next; // ultimo
        }
        tmp2->next=NULL; // il second ultimo diventa l'ultimo
        tmp=NULL; // elimino l'ultimo
    }
    return l;
}
/* funzione che, dato un mago m, pesca una carta dal mazzo e la mette nella mano */
static void pesca(Mago* m){
    // estraggo dal mazzo una carta e la metto nella mano
    // se non c'e' posto, la dealloco
    Carta *prec,*last;
    if(m->mazzo==NULL) // caso in cui il mazzo e' vuoto
        return;
    for(prec=m->mazzo;prec->next!=NULL;prec=prec->next); // scorro la lista di carte fino alla fine
    last=prec; // salvo la carta estratta
    for(int i=0;i<6;i++){
        if(m->mano[i]==NULL){ // se c'e' spazio in mano
            m->mano[i]=last; // la inserisco in mano
            break;
        }
    }
    prec=NULL;
    m->mazzo=estraiultimo(m->mazzo); // rimuovo dal mazzo la carta estratta
}
/* funzione che, dato un mago m, gioca la carta scelta */
static void gioca_carta(Mago *m){
    int cartaDaGiocare,i=0,scelta,cartaDaSostituire,creaturaDaRimuovere,creaturaDaAttaccare,creaturaDaGuarire;
    i=stampa_mano(m);
    do{ // input con controllo validita'
        printf("> Che carta vuoi giocare? "); // scelta della carta da giocare
        scanf("%d",&cartaDaGiocare);
    }while(cartaDaGiocare<1 || cartaDaGiocare>i);
    cartaDaGiocare--; // decremento l'input perche' l'array inizia da 0
    Carta* tmp=m->mano[cartaDaGiocare]; // salvo la carta da giocare
    while(cartaDaGiocare<6){ // elimino la carta giocata dall'array, sovrascrivendola
        m->mano[cartaDaGiocare]=m->mano[cartaDaGiocare+1];
        cartaDaGiocare++;
    }
    switch(tmp->tipocarta){ // in base al tipo della carta da giocare, scelgo cosa fare
        case 1: // creatura
            i=0; // setto indice a 0
            while(m->campo[i]!=NULL && i<4) i++; // scorro campo finche non trovo un posto libero
            if(i>=4){ // se il campo e' pieno
                printf("> Il campo ha gia' 4 creature presenti!\n");
                do{ // input con controllo
                    printf("> Scegli se (0)sostituire una creatura in campo o (1)eliminare la creatura... ");
                    scanf("%d",&scelta);
                }while(scelta!=0 && scelta!=1);
                if(scelta==0){ // sostituire una creatura in campo
                    stampa_campo(m); // stampo il campo
                    do{ // input con controllo
                        printf("> Quale carta vuoi sostituire? ");
                        scanf("%d",&cartaDaSostituire);
                    }while(i<1 || i>5);
                    m->campo[cartaDaSostituire-1]=tmp; // sostituisco la creatura scelta
                }
                else // altrimenti
                    free(tmp); // elimino la creatura
            }
            else m->campo[i]=tmp; // campo non e' pieno, inserisco creatura nel primo posto libero
            break;
        case 2: // rimuovi creatura
            if(strcmp(m->nomeGiocatore,giocatore1->nomeGiocatore)==0){ // m e' il giocatore 1
                printf("> Campo dell'avversario\n"); // avversario = giocatore 2
                i=stampa_campo(giocatore2);
                do{ // input con controllo sulla creatura da rimuovere
                    printf("> Quale creatura vuoi rimuovere? ");
                    scanf("%d",&creaturaDaRimuovere);
                }while(creaturaDaRimuovere<1 || creaturaDaRimuovere>i);
                creaturaDaRimuovere--;
                while(giocatore2->campo[creaturaDaRimuovere]!=NULL && creaturaDaRimuovere<4){ // elimino la carta scelta dall'array
                    giocatore2->campo[creaturaDaRimuovere]=giocatore2->campo[creaturaDaRimuovere+1];
                    creaturaDaRimuovere++;
                }
                free(giocatore2->campo[creaturaDaRimuovere]); // dealloco la memoria della carta rimossa
            }
            else{ // m e' il giocatore 2
                printf("> Campo dell'avversario\n"); // avversario = giocatore 1
                i=stampa_campo(giocatore1);
                do{ // input con controllo
                    printf("> Quale creatura vuoi rimuovere? ");
                    scanf("%d",&creaturaDaRimuovere);
                }while(creaturaDaRimuovere<1 || creaturaDaRimuovere>i);
                creaturaDaRimuovere--;
                while(giocatore1->campo[creaturaDaRimuovere]!=NULL && creaturaDaRimuovere<4){ // elimino la carta scelta dall'array
                    giocatore1->campo[creaturaDaRimuovere]=giocatore1->campo[creaturaDaRimuovere+1];
                    creaturaDaRimuovere++;
                }
                free(giocatore1->campo[creaturaDaRimuovere]); // dealloco la memoria della carta rimossa
            }
            break;
        case 3: // infliggi danno
            do{ // input con controllo su chi infliggere il danno
                printf("> Scegli se infliggere danno (0)al mago nemico o (1)a una creatura del nemico... ");
                scanf("%d",&scelta);
            }while(scelta!=0 && scelta!=1);
            if(scelta==0) // se viene scelto di infliggere il danno sul mago nemico
                if(strcmp(m->nomeGiocatore,giocatore1->nomeGiocatore)==0) // m = giocatore 1
                    giocatore2->PV-=tmp->punti_vita;
                else // m = giocatore 2
                    giocatore1->PV-=tmp->punti_vita;
            else{ // se viene scelto di infliggere il danno su una creatura del nemico
                i=0;
                if(strcmp(m->nomeGiocatore,giocatore1->nomeGiocatore)==0){ // m = giocatore 1
                    printf("> Campo dell'avversario\n"); // avversario = giocatore 2
                    i=stampa_campo(giocatore2);
                    do{ // input con controllo sulla creatura da attaccare
                        printf("> Quale creatura vuoi attaccare? ");
                        scanf("%d",&creaturaDaAttaccare);
                    }while(creaturaDaAttaccare<1 || creaturaDaAttaccare>i);
                    creaturaDaAttaccare--;
                    giocatore2->campo[creaturaDaAttaccare]->punti_vita-=tmp->punti_vita; // tolgo i punti vita alla creatura scelta
                    if(giocatore2->campo[creaturaDaAttaccare]->punti_vita<=0){ // se ha meno di 0 punti vita, devo eliminarla
                        while(giocatore2->campo[creaturaDaAttaccare]!=NULL && creaturaDaAttaccare<4){ // elimino la creatura dall'array
                            giocatore2->campo[creaturaDaAttaccare]=giocatore2->campo[creaturaDaAttaccare+1];
                            creaturaDaAttaccare++;
                        }
                        free(giocatore2->campo[creaturaDaAttaccare]); // dealloco la memoria della creatura morta
                    }
                }
                else{ // m = giocatore 2
                    printf("> Campo dell'avversario\n"); // avversario = giocatore 1
                    i=stampa_campo(giocatore1);
                    do{ // input con controllo sulla creatura da attaccare
                        printf("> Quale creatura vuoi attaccare? ");
                        scanf("%d",&creaturaDaAttaccare);
                    }while(creaturaDaAttaccare<1 || creaturaDaAttaccare>i);
                    creaturaDaAttaccare--;
                    giocatore1->campo[creaturaDaAttaccare]->punti_vita-=tmp->punti_vita; // tolgo i punti vita alla creatura scelta
                    if(giocatore1->campo[creaturaDaAttaccare]->punti_vita<=0){ // se ha meno di 0 punti vita, devo eliminarla
                        while(giocatore1->campo[creaturaDaAttaccare]!=NULL && creaturaDaAttaccare<4){ // elimino la creatura dall'array
                            giocatore1->campo[creaturaDaAttaccare]=giocatore1->campo[creaturaDaAttaccare+1];
                            creaturaDaAttaccare++;
                        }
                        free(giocatore1->campo[creaturaDaAttaccare]); // dealloco la memoria della creatura morta
                    }
                }
            }
            break;
        case 4: // guarisci danno
            do{ // input con controllo per scegliere chi guarire
                printf("> Scegli se guarire (0)il tuo mago o (1) una tua creatura... ");
                scanf("%d",&scelta);
            }while(scelta!=0 && scelta!=1);
            if(scelta==0) // voglio guarire il mago
                if(strcmp(m->nomeGiocatore,giocatore1->nomeGiocatore)==0) // caso del giocatore 1
                    giocatore1->PV+=tmp->punti_vita;
                else // caso del giocatore 2
                    giocatore2->PV+=tmp->punti_vita;
            else{ // voglio guarire una mia creatura
                if(strcmp(m->nomeGiocatore,giocatore1->nomeGiocatore)==0){ // giocatore 1
                    printf("> Il tuo campo\n");
                    i=stampa_campo(giocatore1); // stampo campo
                    do{ // input con controllo sulla creatura da guarire
                        printf("> Quale creatura vuoi guarire? ");
                        scanf("%d",&creaturaDaGuarire);
                    }while(creaturaDaGuarire<1 || creaturaDaGuarire>i);
                    creaturaDaGuarire--;
                    giocatore1->campo[creaturaDaGuarire]->punti_vita+=tmp->punti_vita; // guarisco creatura scelta
                }
                else{ // giocatore 2
                    printf("> Il tuo campo\n");
                    i=stampa_campo(giocatore2); // stampo campo
                    do{ // input con controllo sulla creatura da guarire
                        printf("> Quale creatura vuoi guarire? ");
                        scanf("%d",&creaturaDaGuarire);
                    }while(creaturaDaGuarire<1 || creaturaDaGuarire>i);
                    creaturaDaGuarire--;
                    giocatore2->campo[creaturaDaGuarire]->punti_vita+=tmp->punti_vita; // guarisco creatura scelta
                }
            }
            break;
    }
}
/* funzione che dato un mago m, attacca in base alle scelte del mago */
static void attacca(Mago *m){
    int i,j,scelta,creaturaDaAttaccare,creaturaDaUsare;
    if(strcmp(m->nomeGiocatore,giocatore1->nomeGiocatore)==0){ // m = giocatore 1, avversario = giocatore 2
        printf("> Il tuo campo\n");
        i=stampa_campo(m); // stampo il campo di m
        printf("> Campo dell'avversario\n");
        j=stampa_campo(giocatore2); // stampo il campo dell'avversario
        do{ // input con controllo
            printf("> Che creatura vuoi usare? ");
            scanf("%d",&creaturaDaUsare); // scelgo che creatura si vuole usare per l'attacco
        }while(creaturaDaUsare<1 || creaturaDaUsare>i);
        creaturaDaUsare--;
        do{ // input con controllo per scegliere chi attaccare
            printf("> Scegliere se attaccare (0)il mago nemico o (1)una creatura avversaria... ");
            scanf("%d",&scelta);
        }while(scelta!=0 && scelta!=1);
        if(scelta==0){ // scelgo di attaccare il mago nemico
            giocatore2->PV-=m->campo[creaturaDaUsare]->punti_vita;
        }
        else{ // scelgo di attaccare una creatura avversaria
            do{ // input con controllo
                printf("> Che creatura vuoi attaccare? ");
                scanf("%d",&creaturaDaAttaccare); // scelgo che creatura attaccare
            }while(creaturaDaAttaccare<1 || creaturaDaAttaccare>j);
            creaturaDaAttaccare--;
            giocatore2->campo[creaturaDaAttaccare]->punti_vita-=m->campo[creaturaDaUsare]->punti_vita; // tolgo punti vita alla creatura scelta
            if(giocatore2->campo[creaturaDaAttaccare]->punti_vita<=0){ // se ha <= di 0pv, e' morta
                while(giocatore2->campo[creaturaDaAttaccare]!=NULL && creaturaDaAttaccare<4){ // elimino la creatura dall'array
                    giocatore2->campo[creaturaDaAttaccare]=giocatore2->campo[creaturaDaAttaccare+1];
                    creaturaDaAttaccare++;
                }
                free(giocatore2->campo[creaturaDaAttaccare]); // dealloco la memoria
            }
        }
    }
    else{ // giocatore 2
        printf("> Il tuo campo\n");
        i=stampa_campo(m); // stampo il campo di m
        printf("> Campo dell'avversario\n");
        j=stampa_campo(giocatore1); // stampo il campo dell'avversario
        do{ // input con controllo
            printf("> Che creatura vuoi usare? ");
            scanf("%d",&creaturaDaUsare); // scelgo che creatura si vuole usare per l'attacco
        }while(creaturaDaUsare<1 || creaturaDaUsare>i);
        creaturaDaUsare--;
        do{ // input con controllo per scegliere chi attaccare
            printf("> Scegliere se attaccare (0)il mago nemico o (1)una creatura avversaria... ");
            scanf("%d",&scelta);
        }while(scelta!=0 && scelta!=1);
        if(scelta==0){ // scelgo di attaccare il mago nemico
            giocatore1->PV-=m->campo[creaturaDaUsare]->punti_vita;
        }
        else{ // scelgo di attaccare una creatura avversaria
            do{ // input con controllo
                printf("> Che creatura vuoi attaccare? ");
                scanf("%d",&creaturaDaAttaccare); // scelgo che creatura attaccare
            }while(creaturaDaAttaccare<1 || creaturaDaAttaccare>j);
            creaturaDaAttaccare--;
            giocatore1->campo[creaturaDaAttaccare]->punti_vita-=m->campo[creaturaDaUsare]->punti_vita; // tolgo punti vita alla creatura scelta
            if(giocatore1->campo[creaturaDaAttaccare]->punti_vita<=0){ // se ha <= di 0pv, e' morta
                while(giocatore1->campo[creaturaDaAttaccare]!=NULL && creaturaDaAttaccare<4){ // elimino la creatura dall'array
                    giocatore1->campo[creaturaDaAttaccare]=giocatore1->campo[creaturaDaAttaccare+1];
                    creaturaDaAttaccare++;
                }
                free(giocatore1->campo[creaturaDaAttaccare]); // dealloco la memoria
            }
        }
    }
}
/* funzione che, dato un mago m, gestisce il suo turno */
static void turno(Mago* m){
    int scelta,fineturno=0;
    int f1=0,f2=0,f3=0; // flag per sapere se ho gia' pescato/ giocato/ attaccato
    printf("-- Turno di %s (%dpv) --\n",m->nomeGiocatore,m->PV); // stampo menu' delle azioni che si possono svolgere
    printf(" 1) Pesca una carta\n");
    printf(" 2) Gioca una carta\n");
    printf(" 3) Attacca\n");
    printf(" 4) Stampa la tua mano\n");
    printf(" 5) Stampa il campo dei due giocatori\n");
    printf(" 6) Passa il turno\n");
    do{ // input con controllo sulla scelta
        printf("Inserisci la tua scelta: ");
        scanf("%d",&scelta); // input scelta
        switch(scelta){ // in base alla scelta fatta, svolgo certe azioni
            case 1:
                if(f1==0){ // se non ho ancora pescato
                    pesca(m); // pesco
                    f1=1; // setto la flag a 1
                    printf("> Operazione effettuata!");
                }
                else printf("> Hai gia' pescato!"); // ho gia' pescato
                break;
            case 2:
                if(f2==0){ // se non ho ancora giocato
                    gioca_carta(m); // gioca una carta
                    f2=1; // setto la flag a 1
                    printf("> Operazione effettuata!");
                }
                else printf("> Hai gia' giocato!"); // ho gia' giocato
                break;
            case 3:
                if(f3==0){ // se non ho ancora attaccato
                    attacca(m); // attacca
                    f3=1; // setto la flag a 1
                    printf("> Operazione effettuata!");
                }
                else printf("> Hai gia' attaccato!"); // ho gia' attaccato
                break;
            case 4:
                printf("> La tua mano\n");
                stampa_mano(m); // stampa la tua mano
                break;
            case 5: // stampo i campi di entrambi i giocatori
                printf("> Campo di %s\n",giocatore1->nomeGiocatore);
                stampa_campo(giocatore1);
                printf("> Campo di %s\n",giocatore2->nomeGiocatore);
                stampa_campo(giocatore2);
                break;
            case 6:
                fineturno=1; // passa il turno
                break;
            default:
                printf("> Scelta non valida!");
                break;
        }
        printf("\n");
    }while(fineturno==0);
}
/* funzione che imposta il gioco e stampa un riepilogo */
void imposta_gioco(){
    int n;
    giocatore1=(Mago*)malloc(sizeof(Mago)); // alloco memoria per i due maghi
    giocatore2=(Mago*)malloc(sizeof(Mago));
    giocatore1->PV=20; // setto punti vita iniziali a 20
    giocatore2->PV=20;
    srand(time(NULL)); // inizializzo il generatore di numeri casuali
    // Input dati giocatori
    printf("Inserisci il nome del giocatore 1: ");
    scanf("%s",giocatore1->nomeGiocatore);
    printf("Inserisci la classe del mago del giocatore 1\n");
    giocatore1->classemago=setClasseMago();
    printf("\nInserisci il nome del giocatore 2: ");
    scanf("%s",giocatore2->nomeGiocatore);
    printf("Inserisci la classe del mago del giocatore 2\n");
    giocatore2->classemago=setClasseMago();
    // Input dimensione mazzo
    printf("\nInserire la dimensione dei mazzi: ");
    scanf("%d",&n);
    // Generare i mazzi di giocatore 1 e giocatore 2
    generaMazzo(giocatore1,n);
    generaMazzo(giocatore2,n);
    // Riempire le mani dei due giocatori con 5 carte e lasciarne una vuota
    riempiMani(giocatore1);
    riempiMani(giocatore2);
    // Campo e' tutto NULL
    for(int i=0;i<4;i++){ // per ogni elemento dell'array, li setto a null
        giocatore1->campo[i]=NULL;
        giocatore2->campo[i]=NULL;
    }
    // Output dati dei due giocatori
    printf("\n---- Dati del giocatore 1 ----\n");
    printf("Nome: %s\n",giocatore1->nomeGiocatore);
    printf("Classe mago: %s\n",getClasseMago(giocatore1));
    printf("  Mazzo:\n");
    stampa_mazzo(giocatore1);
    printf("  Mano:\n");
    stampa_mano(giocatore1);
    printf("\n---- Dati del giocatore 2 ----\n");
    printf("Nome: %s\n",giocatore2->nomeGiocatore);
    printf("Classe mago: %s\n",getClasseMago(giocatore2));
    printf("  Mazzo\n");
    stampa_mazzo(giocatore2);
    printf("  Mano\n");
    stampa_mano(giocatore2);
}
/* funzione che gestisce lo scontro dei due giocatori */
void combatti(){
    int giocatore=rand()%2; // {0,1} = {giocatore1, giocatore2}
    int flagFine=0; // flag per sapere se lo scontro e' finito
    do{
        if(giocatore==0){ // tocca al giocatore 1
            turno(giocatore1);
            if(giocatore2->PV<=0){ // se il giocatore 2 e' morto
                flagFine=1; // il gioco e' finito
                printf("----- %s ha vinto! -----\n",giocatore1->nomeGiocatore);
            }
            giocatore=1; // passo il turno al giocatore 2
        }
        else{ // tocca al giocatore 2
            turno(giocatore2);
            if(giocatore1->PV<=0){ // se il giocatore 1 e' morto
                flagFine=1; // il gioco e' finito
                printf("----- %s ha vinto! -----\n",giocatore2->nomeGiocatore);
            }
            giocatore=0; // passo il turno al giocatore 1
        }
        if(giocatore2->mazzo==NULL || giocatore1->mazzo==NULL){ // se uno dei mazzi dei giocatori non ha piu' carte
            flagFine=1; // il gioco finisce e vince chi ha piu' punti vita
            if(giocatore2->PV>giocatore1->PV)
                printf("----- %s ha vinto! -----\n",giocatore2->nomeGiocatore);
            else if(giocatore1->PV>giocatore2->PV)
                printf("----- %s ha vinto! -----\n",giocatore1->nomeGiocatore);
            else
                printf("----- Pareggio! -----\n");
        }
    }while(flagFine==0);
}
void termina_gioco(){
    free(giocatore1); // dealloco memoria dei due giocatori
    free(giocatore2);
    printf(" Arrivederci!\n"); // fine
}
