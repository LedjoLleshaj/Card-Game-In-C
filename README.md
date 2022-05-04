# Hearthstone-Card-Game simplified In C

Per compilarlo:

$ gcc -c main.c

$ gcc -c gamelib.c

$ gcc -o gioco main.o gamelib.o

Per eseguirlo:

$ ./gioco

Scelte progettuali:

- Nella funzione generaMazzo(), il massimo di punti vita delle carte generate è dim-i+1 (in modo che le ultime siano le più potenti);
- Nella funzione riempiMani(), il massimo di punti vita delle carte generate è 8 indipendentemente dalla loro posizione, in modo da rendere più avvincente il gioco sin dalle prime mosse;
- Nella funzione pesca(), se la mano contiene già 6 carte, quella estratta dal mazzo viene eliminata;
- Nella funzione gioca_carta(), nel case 4 dello switch, è stato scelto di aumentare i punti vita anche oltre il 20, non considerandolo un limite.
