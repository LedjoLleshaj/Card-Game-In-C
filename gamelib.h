typedef enum{ tenebre=1, vita=2, luce=3 }Classe_mago;
typedef enum{ creatura=1, rimuovi_creatura=2, infliggi_danno=3, guarisci_danno=4 }Tipo_carta;
typedef struct Carta_{
    Tipo_carta tipocarta;
    int punti_vita;
    struct Carta_* next;
}Carta;
typedef struct{
    char nomeGiocatore[20];
    int PV;
    Classe_mago classemago;
    Carta* mazzo;
    Carta* mano[6];
    Carta* campo[4];
}Mago;
void imposta_gioco();
void combatti();
void termina_gioco();