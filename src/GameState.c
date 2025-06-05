#include "../head/GameState.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define GRID_SIZE 50

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
void GR911_affichage2(GameState* state) {
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
void GR911_affichage(SDL_Renderer *renderer, GameState *state) {
    int size = state->size;
    int cell_size = 50; // Taille d'une cellule en pixels
    SDL_Rect cell;

    cell.w = cell_size; // Largeur de la cellule
    cell.h = cell_size; // Hauteur de la cellule
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
SDL_RenderClear(renderer);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int Cellule = state->map[i * size + j];
            // Position de la cellule dans la fenêtre
            cell.x = j * cell_size+(WINDOW_WIDTH-size*cell_size)/2;
            cell.y = i * cell_size+(WINDOW_WIDTH-size*cell_size)/2;

            // Définir la couleur en fonction de la valeur de la cellule
            switch (Cellule) {
                case PLAYER_1:
                    SDL_SetRenderDrawColor(renderer, 171, 30, 0, 255); // Rouge
                    break;
                case PLAYER_2:
                    SDL_SetRenderDrawColor(renderer, 0, 30, 171, 255); // Bleu
                    break;
                case 3:
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge
                    break;
                case 4:
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Vert
                    break;
                case 5:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Bleu
                    break;
                case 6:
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Jaune
                    break;
                case 7:
                    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // Magenta
                    break;
                case 8:
                    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // Cyan
                    break;
                case 9:
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc
                    break;
                default:
                    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Gris
                    break;
            }

            // Dessiner la cellule
            SDL_RenderFillRect(renderer, &cell);

            // Optionnel : dessiner une bordure pour chaque cellule
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Noir
            SDL_RenderDrawRect(renderer, &cell);
        }
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
        printf("Couleurs invalid! Rejoue:%d \n", playedChar);
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


int GR911_Human2(GameState* state,int player){
    printf("Player %d's turnn. Choose [R/G/B/Y/M/C/W]: ", player);

    char playedChar;
    scanf(" %c", &playedChar);

        return playedChar;

}
int GR911_Human(SDL_Renderer *renderer, GameState *state, int player) {
    printf("Player %d's turn. Click on a square to choose [R/G/B/Y/M/C/W]:\n", player);

    SDL_Event event;
    int running = 1;
    int cell_size = 50; // Taille d'une cellule en pixels
    char selectedChar ;

    while (running) {
        // Attendre un événement SDL
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                // Récupérer la position du clic
                int mouseX = event.button.x-(WINDOW_WIDTH-state->size*cell_size)/2;
                int mouseY = event.button.y-(WINDOW_WIDTH-state->size*cell_size)/2;

                // Calculer les indices de la cellule cliquée
                int col = mouseX / cell_size;
                int row = mouseY / cell_size;

                // Valider les indices (éviter de cliquer en dehors de la grille)
                if (row >= 0 && row < state->size && col >= 0 && col < state->size) {
                    // Obtenir la valeur de la cellule cliquée
                    int Cellule = state->map[row * state->size + col];
                    
                    // Associer un caractère selon la cellule
                    switch (Cellule) {
                        case 3: selectedChar = 'R'; break;
                        case 4: selectedChar = 'G'; break;
                        case 5: selectedChar = 'B'; break;
                        case 6: selectedChar = 'Y'; break;
                        case 7: selectedChar = 'M'; break;
                        case 8: selectedChar = 'C'; break;
                        case 9: selectedChar = 'W'; break;
                        default: selectedChar = '?'; break;
                    }
                    // Quitter la boucle après avoir sélectionné une cellule
                    running = 0;
                }
            }
        }


    }

    return selectedChar;
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


void GR911_copyGameState(const GameState *src, GameState *dst)
{
    create_empty_game_state(dst, src->size);          // alloue et init
    int size = src->size;
    for (int i = 0; i < size * size; ++i)
        dst->map[i] = src->map[i];
}

int GR911_eval(const GameState *state, int player)
{
    int adv = (player == PLAYER_1) ? PLAYER_2 : PLAYER_1;
    return GR911_territoireActuel((GameState *)state, player)
         - GR911_territoireActuel((GameState *)state, adv);
}

int GR911_defensif(GameState *state, int player)
{
    int adv = (player == PLAYER_1) ? PLAYER_2 : PLAYER_1;
    int bestColor      = 3;               /* valeur par défaut*/
    int minAdvMaxGain  = 10000;

    for (int c = 3; c <= 9; ++c) {
        if (!GR911_Couleur_adjacent(state, player, c)) continue;   

        GameState s1;
        GR911_copyGameState(state, &s1);
        GR911_updateWorld(&s1, GR911_NumToLetter(c), player);
        int advInit = GR911_territoireActuel(&s1, adv);

        /* --- Cherche le meilleur (pire) coup de l’adversaire ------------- */
        int advMaxGain = 0;
        for (int d = 3; d <= 9; ++d) {
            if (!GR911_Couleur_adjacent(&s1, adv, d)) continue;

            GameState s2;
            GR911_copyGameState(&s1, &s2);
            GR911_updateWorld(&s2, GR911_NumToLetter(d), adv);
            int gain = GR911_territoireActuel(&s2, adv) - advInit;
            if (gain > advMaxGain) advMaxGain = gain;
            free(s2.map);
        }

        /* --- Choisit le coup qui MINIMISE advMaxGain --------------------- */
        if (advMaxGain < minAdvMaxGain) {
            minAdvMaxGain = advMaxGain;
            bestColor     = c;
        }
        free(s1.map);
    }
    return bestColor;
}

// Structure pour un bouton
typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    const char* text;
} Button;

void render_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        printf("Erreur TTF_RenderText_Solid: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        printf("Erreur SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        return;
    }

    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
}
// Fonction pour dessiner un bouton
void render_button(SDL_Renderer* renderer, TTF_Font* font, Button button) {
    // Dessiner le rectangle
    SDL_SetRenderDrawColor(renderer, button.color.r, button.color.g, button.color.b, 255);
    SDL_RenderFillRect(renderer, &button.rect);

    // Dessiner le texte
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, button.text, white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int text_width = surface->w;
    int text_height = surface->h;
    SDL_Rect text_rect = {
        button.rect.x + (button.rect.w - text_width) / 2,
        button.rect.y + (button.rect.h - text_height) / 2,
        text_width,
        text_height
    };
    SDL_RenderCopy(renderer, texture, NULL, &text_rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Fonction de la page de lancement
void GR911_page_de_lancement(SDL_Renderer* renderer, int* joueur1, int* joueur2) {
    // Initialisation de SDL_ttf
    if (TTF_Init() != 0) {
        printf("Erreur TTF_Init: %s\n", TTF_GetError());
        exit(1);
    }

    TTF_Font* font = TTF_OpenFont("fonts/arial.ttf", 30); // Chemin vers la police (à ajuster)
    if (!font) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        exit(1);
    }
    SDL_Surface *imageSurface = IMG_Load("fonts/hq720.jpg"); // Remplacez par le chemin correct
    if (!imageSurface) {
        printf("Erreur IMG_Load: %s\n", IMG_GetError());
        TTF_CloseFont(font);
        TTF_Quit();
        return;
    }
    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if (!imageTexture) {
        printf("Erreur SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        SDL_FreeSurface(imageSurface);
        TTF_CloseFont(font);
        TTF_Quit();
        return;
    }
    SDL_FreeSurface(imageSurface); // La surface n'est plus nécessaire

    SDL_Rect imageRect = {
        .x = 250,
        .y = 325,
        .w = 300, // Ajustez à la taille de la fenêtre
        .h = 150  // Ajustez à la taille de la fenêtre
    };
    if (SDL_RenderCopy(renderer, imageTexture, NULL, &imageRect) < 0) {
        printf("Erreur SDL_RenderCopy pour l'image : %s\n", SDL_GetError());
        SDL_DestroyTexture(imageTexture);
        TTF_CloseFont(font);
        TTF_Quit();
        return;
    }
    SDL_DestroyTexture(imageTexture); // Libérer la texture de l'image

    Button buttons[6] = {
        {{100, 200, 190, 50}, {0, 128, 255, 255}, "Humain"},
        {{300, 200, 190, 50}, {0, 128, 255, 255}, "Mixte"},
        {{500, 200, 190, 50}, {0, 128, 255, 255}, "Glouton"},
        {{100, 300, 190, 50}, {0, 128, 255, 255}, "Hegemone"},
        {{300, 300, 190, 50}, {0, 128, 255, 255}, "Aleatoire"},
        {{500, 300, 190, 50}, {0, 128, 255, 255}, "Defensif"}
    };

    SDL_Color white = {255, 255, 255, 255};
    int running = 1;
    int selection_joueur = 1;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                for (int i = 0; i < 6; i++) {
                    if (x >= buttons[i].rect.x && x <= (buttons[i].rect.x + buttons[i].rect.w) &&
                        y >= buttons[i].rect.y && y <= (buttons[i].rect.y + buttons[i].rect.h)) {
                        if (selection_joueur == 1) {
                            *joueur1 = i;
                            selection_joueur = 2; // Passer au joueur 2
                        } else {
                            *joueur2 = i;
                            running = 0; // Quitter après sélection des deux joueurs
                        }
                        break;
                    }
                }
            }
        }

        // Dessin de l'écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        //font = TTF_OpenFont("fonts/arial.ttf", 48);
        // Afficher les messages
        render_text(renderer, font, "Bienvenue dans le jeu!", 250, 50, white);
        if (selection_joueur == 1) {
            render_text(renderer, font, "Joueur 1, choisissez votre type :", 200, 100, white);
        } else {
            render_text(renderer, font, "Joueur 2, choisissez votre type :", 200, 100, white);
        }

        // Afficher les boutons
        for (int i = 0; i < 6; i++) {
            render_button(renderer, font, buttons[i]);
        }

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    TTF_Quit();
}

void GR911_ecranVictoire(SDL_Renderer *renderer, int player) {
    // Couleur de fond : noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
  
    // Initialisation de SDL_ttf
    if (TTF_Init() != 0) {
        printf("Erreur TTF_Init: %s\n", TTF_GetError());
        return;
    }

    // Charger une police
    TTF_Font *font = TTF_OpenFont("fonts/arial.ttf", 48); // Remplacez par le chemin correct
    if (!font) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        TTF_Quit();
        return;
    }

    // Charger l'image
    SDL_Surface *imageSurface = IMG_Load("fonts/top1.jpg"); // Remplacez par le chemin correct
    if (!imageSurface) {
        printf("Erreur IMG_Load: %s\n", IMG_GetError());
        TTF_CloseFont(font);
        TTF_Quit();
        return;
    }

    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if (!imageTexture) {
        printf("Erreur SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        SDL_FreeSurface(imageSurface);
        TTF_CloseFont(font);
        TTF_Quit();
        return;
    }
    SDL_FreeSurface(imageSurface); // La surface n'est plus nécessaire

    SDL_Rect imageRect = {
        .x = 250,
        .y = 325,
        .w = 300, // Ajustez à la taille de la fenêtre
        .h = 150  // Ajustez à la taille de la fenêtre
    };
    if (SDL_RenderCopy(renderer, imageTexture, NULL, &imageRect) < 0) {
        printf("Erreur SDL_RenderCopy pour l'image : %s\n", SDL_GetError());
        SDL_DestroyTexture(imageTexture);
        TTF_CloseFont(font);
        TTF_Quit();
        return;
    }
    SDL_DestroyTexture(imageTexture); // Libérer la texture de l'image

    // Préparer le texte
    char message[50];
    snprintf(message, sizeof(message), u8"Victoire du joueur %d  !", player);
    SDL_Color textColor = {255, 255, 255, 255}; // Blanc
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, message, textColor);
    if (!textSurface) {
        printf("Erreur TTF_RenderText_Solid: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        TTF_Quit();
        return;
    }

    // Créer une texture à partir de la surface
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        printf("Erreur SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        TTF_Quit();
        return;
    }

    // Calculer la position du texte pour qu'il soit centré
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface); // La surface n'est plus nécessaire

    SDL_Rect textRect = {
        .x = (800 - textWidth) / 2, // Centrer horizontalement (800 = largeur de la fenêtre)
        .y = (600 - textHeight) / 2, // Centrer verticalement (600 = hauteur de la fenêtre)
        .w = textWidth,
        .h = textHeight
    };

    // Dessiner le texte
    if (SDL_RenderCopy(renderer, textTexture, NULL, &textRect) < 0) {
        printf("Erreur SDL_RenderCopy pour le texte : %s\n", SDL_GetError());
    }

    SDL_DestroyTexture(textTexture); // Libérer la texture

    // Présenter le rendu
    SDL_RenderPresent(renderer);

    // Nettoyer
    TTF_CloseFont(font);
    TTF_Quit();
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
	srand(time(NULL));
    int joueur1, joueur2;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Jeu du groupe 911", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    GR911_page_de_lancement(renderer, &joueur1, &joueur2);

    printf("Joueur 1: %d\n", joueur1);
    printf("Joueur 2: %d\n", joueur2);

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
	GR911_affichage2(&state);
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }


    int running = 1;
    SDL_Event event;
    int jeuFini = 0;
    int player = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir

        SDL_RenderPresent(renderer);
        while (jeuFini == 0) {
            char playedChar;
             if(player == 1){switch (joueur1)
             {
             case 0: playedChar=GR911_Human(renderer,&state,player); break;
             case 1:  playedChar=GR911_NumToLetter(GR911_Mixte(&state,player)); break;
             case 2:  playedChar=GR911_NumToLetter(GR911_glouton(&state,player)); break;
             case 3:  playedChar=GR911_NumToLetter(GR911_Hegemone(&state,player)); break;
             case 4:  playedChar=GR911_NumToLetter(GR911_coupAleatoireValable(&state,player)); break;
             case 5:  playedChar=GR911_NumToLetter(GR911_defensif(&state,player)); break;
             default: playedChar=GR911_Human(renderer,&state,player); break;
                break;
             }}
             else{ switch (joueur2)
                {
                case 0:  playedChar=GR911_Human(renderer,&state,player); break;
                case 1:  playedChar=GR911_NumToLetter(GR911_Mixte(&state,player)); break;
                case 2:  playedChar=GR911_NumToLetter(GR911_glouton(&state,player)); break;
                case 3:  playedChar=GR911_NumToLetter(GR911_Hegemone(&state,player)); break;
                case 4:  playedChar=GR911_NumToLetter(GR911_coupAleatoireValable(&state,player)); break;
                default: playedChar=GR911_Human(renderer,&state,player); break;
                   break;
                }}
            
        printf("Joueur sélectionné:%d",joueur1);
        
        printf("Joueur sélectionné:%d",joueur2);
            int valid = GR911_updateWorld(&state, playedChar, player);
     
            
            GR911_affichage2(&state);


            if(valid==1){
                jeuFini = GR911_finDuJeu(&state, player);
                if (jeuFini == 1) {
                    printf("Player %d wins!\n", player);
                    GR911_ecranVictoire(renderer, player);
                    SDL_RenderPresent(renderer);
                } 
                else {
                    printf("%d\n",player);
                    player = (player == 1) ? 2 : 1;
                    GR911_affichage(renderer,&state);
                    SDL_RenderPresent(renderer);
                }	
    
            }
            
        }
        
    GR911_ecranVictoire(renderer, player);
    }


   

    free(state.map);
    state.map = NULL;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
