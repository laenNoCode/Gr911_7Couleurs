#include "../head/GameState.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

    #define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define Couleur_J1 "\e[45m"
#define Couleur_J2 "\e[42m"


GameState state = {.map = NULL, .size = 0};
void create_empty_game_state(GameState* state, int size) {
    // On vérifie la taille
    if (state == NULL || size <= 0) {
        fprintf(stderr, "Erreur : paramètres invalides\n");
        return;
    }
    // On initialise le terrain
    state->size = size;
    // On alloue(heap) la mémoire mémoire pour le tableau map (taille size * size), on réalise tout le jeu en une liste longue au lieu d'un tableau
    state->map = (Color*)malloc(size * size * sizeof(Color));
    if (state->map == NULL) {
        fprintf(stderr, "Erreur : allocation de mémoire échouée\n");
        state->size = 0; // Met à jour la taille pour indiquer une erreur
        return;
    }
    // On initialise les cases à EMPTY
    for (int i = 0; i < size * size; i++) {
        state->map[i] = EMPTY;
    }
}

void set_map_value (GameState* state, int x, int y, Color value) {
    int size = state->size;
    state->map[x * size + y] = value;
}

Color get_map_value (GameState* state, int x, int y){
	if (state -> map == NULL || x > state -> size || y > state -> size || x < 0 || y < 0)
	{
		printf("[ERROR] map not big enough or not initialized %p %i access (%i %i)", state -> map, state -> size, x, y);
		return ERROR;
	}
	return state -> map[y * state -> size + x];
}
void fill_map(GameState* map) {
    int size = map->size;
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            int couleur = rand() % 7 + 3;
            map->map[i * size + j] = couleur;
        }
    }
    map->map[0 * size + (size - 1)] = PLAYER_1;
    map->map[(size - 1) * size + 0] = PLAYER_2;
}

int GR911_letterToNum(char Cellule) {
    // couleurs en numéro pour simplifier le traitement
    switch (Cellule) {
        case 'R': return 3;
        case 'G': return 4;
        case 'B': return 5;
        case 'Y': return 6;
        case 'M': return 7;
        case 'C': return 8;
        case 'W': return 9;
        default:  return ERROR; // Lettre inconnue
    }
}
char GR911_NumToLetter(int Cellule) {
    // couleurs en numéro pour simplifier le traitement
    switch (Cellule) {
        case 3: return 'R';
        case 4: return 'G';
        case 5: return 'B';
        case 6: return 'Y';
        case 7: return 'M';
        case 8: return 'C';
        case 9: return 'W';
        default:  return ERROR; // Lettre inconnue
    }
}

void GR911_affichage(GameState* state) {
    int size = state->size;
    printf("-------------------\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++){
            int Cellule = state->map[i * size + j];
            if (Cellule == PLAYER_1) {
                printf("%s ", Couleur_J1 "v" ANSI_COLOR_RESET);
            }
            else if (Cellule == PLAYER_2) {
                printf("%s ", Couleur_J2 "^" ANSI_COLOR_RESET);
            }
            else if (Cellule == 3) {
                printf("%s ", ANSI_COLOR_RED "R" ANSI_COLOR_RESET);
            }
            else if (Cellule == 4) {
                printf("%s ", ANSI_COLOR_GREEN "G" ANSI_COLOR_RESET);
            }
            else if (Cellule == 5) {
                printf("%s ", ANSI_COLOR_BLUE "B" ANSI_COLOR_RESET);
            }
            else if (Cellule == 6) {
                printf("%s ", ANSI_COLOR_YELLOW "Y" ANSI_COLOR_RESET);
            }
            else if (Cellule == 7) {
                printf("%s ", ANSI_COLOR_MAGENTA "M" ANSI_COLOR_RESET);
            }
            else if (Cellule == 8) {
                printf("%s ", ANSI_COLOR_CYAN "C" ANSI_COLOR_RESET);
            }
            else if (Cellule == 9) {
                printf("%s ", "W");
            }
            else {
                printf("? ");
            }
        }
        printf("\n");
    }
}

int GR911_adjacent(GameState* state,int x , int y , int player){
    int size = state->size;
     int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && nx < size && ny >= 0 && ny < size) {
            if (state->map[nx * size + ny] == player) {
                return 1;
            }
        }
    }

	// Aucune case adjacente n'appartient au joueur
    return 0;
}
int GR911_Couleur_adjacent(GameState* state, int player, int couleur){
    int size=state->size;
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if (state->map[i * size + j] == couleur) {
                if (GR911_adjacent(state, i, j, player)) {
                    return 1;
                }
            }
        }
    }
    return 0;

}

int GR911_updateWorld(GameState* state, char playedChar, int player) {
    int playedNum = GR911_letterToNum(playedChar);
    if (playedNum == ERROR) {
        // Invalid letter
        printf("Couleurs invalid! Rejoue \n");
        return 0;
    }

    int size = state->size;
    int changed = 1;
    while (changed == 1) {
        changed = 0;
        for (int i = 0; i < size; i++){
            for (int j = 0; j < size; j++){
                if (state->map[i * size + j] == playedNum) {
                    if (GR911_adjacent(state, i, j, player)) {
                        state->map[i * size + j] = player;
                        changed = 1;
                    }
                }
            }
        }
    }
	return 1;
}

int GR911_finDuJeu(GameState* state, int player) {
    int sc = 0;
    int size = state->size;

    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if (state->map[i * size + j] == player) {
                sc++;
            }
        }
    }
    
    if (sc > (size * size / 2)) {
        return 1;
    }
    return 0;
}

int GR911_coupAleatoire(GameState* state){
    return rand() % 7 + 3;
}
int GR911_coupAleatoireValable(GameState* state,int player){
    int coupValable = 0;
    int coup;
    int size = state->size;
    while (coupValable == 0)
    {
        coup = rand()%7 + 3;
        int puissanceDuCoup = 0;
        for (int i = 0; i < size; i++){
            for (int j = 0; j < size; j++){
                if (state->map[i * size + j] == coup) {
                    if (GR911_adjacent(state , i , j ,player) == 1)
                    {
                        puissanceDuCoup++;
                    }
                    
                }
            }
        }

        if (puissanceDuCoup>0){
            return coup;
        }

    }
    
}
int GR911_frontiereActuel(GameState* state,int player){
    int front = 0;
    int size = state->size;
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if(state->map[i * size + j] != player && GR911_adjacent(state , i,j, player)){
                front ++;
            }
        }
        }
    return front;
}
int GR911_territoireActuel(GameState* state,int player){
    int front = 0;
    int size = state->size;
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if(state->map[i * size + j] == player){
                front ++;
            }
        }
        }
    return front;
}

int GR911_glouton(GameState* state,int player){
    int coup = 3;
    int territoire = 0;
    int size = state->size;
    for(int i = 3;i<10;i++){
        GameState stateImaginaire;

        create_empty_game_state(&stateImaginaire,size);
        for (int i = 0; i < size; i++){
            for (int j = 0; j < size; j++){
                int couleur = state->map[i * size + j];
                stateImaginaire.map[i * size + j] = couleur;
        }
        }
        GR911_updateWorld(&stateImaginaire, GR911_NumToLetter(i), player);


        if(territoire <= GR911_territoireActuel(&stateImaginaire , player)){
            coup = i;
            territoire=GR911_territoireActuel(&stateImaginaire , player);
        }
        free(stateImaginaire.map);
        stateImaginaire.map = NULL;
    }

    return coup;

}


int GR911_Human(GameState* state,int player){
    printf("Player %d's turnn. Choose [R/G/B/Y/M/C/W]: ", player);

    char playedChar;
    scanf(" %c", &playedChar);

        return playedChar;



}

int GR911_Hegemone(GameState* state,int player){
    int coup = 3;
    int frontiere = 0;
    int size = state->size;
    for(int i = 3;i<10;i++){
        
        GameState stateImaginaire;

        create_empty_game_state(&stateImaginaire,size);
        for (int i = 0; i < size; i++){
            for (int j = 0; j < size; j++){
                int couleur = state->map[i * size + j];
                stateImaginaire.map[i * size + j] = couleur;
        }
        }
        GR911_updateWorld(&stateImaginaire, GR911_NumToLetter(i), player);


        if(frontiere < GR911_frontiereActuel(&stateImaginaire , player) && GR911_Couleur_adjacent(state,player,i)){
            coup = i;
            frontiere=GR911_frontiereActuel(&stateImaginaire , player);
        }
        free(stateImaginaire.map);
        stateImaginaire.map = NULL;
        }    return coup;
}


int GR911_Mixte(GameState* state,int player){
    int coup = 3;
    int frontiere = 0;
    int size = state->size;
    for(int i = 3;i<10;i++){
        GameState stateImaginaire;

        create_empty_game_state(&stateImaginaire,size);
        for (int i = 0; i < size; i++){
            for (int j = 0; j < size; j++){
                int couleur = state->map[i * size + j];
                stateImaginaire.map[i * size + j] = couleur;
        }
        }
        GR911_updateWorld(&stateImaginaire, GR911_NumToLetter(i), player);

        
        if(frontiere < GR911_frontiereActuel(&stateImaginaire , player)&& GR911_Couleur_adjacent(state,player,i)){
            coup = i;
            frontiere=GR911_frontiereActuel(&stateImaginaire , player);
        }
        if(GR911_frontiereActuel(&stateImaginaire , player) ==GR911_frontiereActuel(state , player) ){
            coup = GR911_glouton(state, player);
        }

        free(stateImaginaire.map);
        stateImaginaire.map = NULL;
    }return coup;

}


int main(int argc, char** argv) {
    int windeglouton = 0;
    
        srand(time(NULL));

    GameState state;
    // Taille du terrain

    int size = 11;
	// Initialisation

    create_empty_game_state(&state, size);
	 // Vérification si l'initialisation a réussi
    if (state.map == NULL) {
        printf("Erreur lors de l'initialisation du terrain.\n");
        return 1; 
    }

    fill_map(&state);
    GR911_affichage(&state);

    // Boucle principlae du jeu -----
    int jeuFini = 0;
    int player = 1;

    while (jeuFini == 0) {


        char playedChar = (player == 1)? GR911_NumToLetter(GR911_Hegemone(&state,player)) :GR911_NumToLetter(GR911_Mixte(&state,player));

        int valid = GR911_updateWorld(&state, playedChar, player);
        //GR911_affichage(&state);
		if(valid==1){
			jeuFini = GR911_finDuJeu(&state, player);
        	if (jeuFini == 1) {
            	printf("Player %d wins!\n", player);
                if(player == 2){
                    windeglouton++;
                }
        	} 
        	else {
                printf("%d\n",player);
            	player = (player == 1) ? 2 : 1;
        	}	

		}
        
    }

    free(state.map);
    state.map = NULL;

    printf("%d victoires\n",windeglouton);

    return 0;
}
