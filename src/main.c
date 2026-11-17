#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define STD_SIZE_X 25
#define STD_SIZE_Y 25
#define STD_SIZE_BOMB_X 25
#define STD_SIZE_BOMB_Y 25
#define SCREEN_BORDER 40
#define MAX_NAME_LENGTH 7
#define MAX_SNOWFLAKES 100

typedef struct Bomb
{
    Rectangle pos;
    Rectangle explosion_right;
    Rectangle explosion_left;
    Rectangle explosion_up;
    Rectangle explosion_down;
    int isActive;
    int isCharging;
    int distance;
    int time;
    int collision;


} Bomb;
typedef struct Hero
{
    Rectangle pos;
    Color color;
    int speed;
    int special;
    int draw_bomb;
    int put_bomb;
    int num_bombs;
    Bomb bombs[5];

    char *nome_bomberman;
    Image imagem_bomberman;
    Texture2D textura_bomberman;
    unsigned numero_frames_bomberman;
    int largura_frame;
    Rectangle rec_bomberman;
    Vector2 bomberman_posicao;
    int currentFrame;
    int framesSpeed;
    float frameTimer;

    char* nome_bomba;
    Texture2D textura_bomba;
    Rectangle rec_bomba[5];
    Image imagem_bomba;

    char* nome_explosao;
    Texture2D textura_explosao;
    Image imagem_explosao;
    Rectangle rec_explosao[5];


} Hero;
typedef struct Boxes
{
    Rectangle pos;
    int exist;
    int destroyed;

    char* nome_boxes;
    Image imagem_boxes;
    Texture2D textura_boxes;


} Boxes;
typedef struct PowerUp{
    Rectangle pos;
    int exist;

    char* nome_poder;
    Texture2D textura_poder;
    Image imagem_poder;
} PowerUp;
typedef struct Nerf{
    Rectangle pos;
    int exist;

    char* nome_nerf;
    Texture2D textura_nerf;
    Image imagem_nerf;
} Nerf;
typedef struct Map
{
    Rectangle barriers[36];
    int num_barriers;
    Color color;
    int next_map;
    int prev_map;
    Boxes boxes[30];
    int num_boxes;

    char* nome_boxes;
    Image imagem_boxes;
    Texture2D textura_boxes;

    char* nome_barrier1;
    Image imagem_barrier1;
    Texture2D textura_barrier1;

    char* nome_barrier2;
    Image imagem_barrier2;
    Texture2D textura_barrier2;

} Map;
typedef struct Snowflake {
    Vector2 position;
    float speed;
} Snowflake;
typedef struct Game
{
    Map maps[2];
    int num_maps;
    int curr_map;
    Hero hero1;
    Hero hero2;
    int screenWidth;
    int screenHeight;
    int gameover;
    int frameCount;
    Rectangle button1; 
    Rectangle button2; 
    int selectedButton;
    Texture2D cenario1;
    Texture2D cenario2;
    double elapsedTime;
    int tie;
    PowerUp speed;
    PowerUp nBombs;
    PowerUp radius;
    Nerf nerfSpeed;
    Nerf nerfBombs;
    Nerf nerfRadius;
    Image imagem_incial;
    Texture2D textura_incial;
    char* nome_imagem;
    Snowflake snow[MAX_SNOWFLAKES];

} Game;

int winnerScreen = 1;

void InitGame(Game *g);           // Initialize game
void UpdateGame(Game *g, Map *m); // Update game (one frame)
void DrawGame(Game *g);           // Draw game (one frame)
void DrawScreen(Game *g, char *player1Name, char *player2Name, int focusPlayer);
void DrawTie(Game *g);
void DrawWinner(Game *g);
void UpdatePlayerName(char *playerName, int maxChars, int *focusPlayer);
void ChangeMap(Game *g);
void UpdateDrawFrame(Game *g, Map *m); // Update and Draw (one frame)
void draw_borders(Game *g);
void draw_map(Game *g);
void draw_bomb(Game *g);
void drawBoxes(Game *g);
void drawPowerUp(Game *g);
void drawNerf(Game *g);
void update_hero_pos(Game *g);
void update_bomb(Game *g);
int bombCollision(Game *g, Hero *h);
int barrier_collision(Map *m, Rectangle t);
int boxesCollision(Map *m, Rectangle t);
int powerCollision(Game *g);
int nerfCollision(Game *g);
void map0_setup(Game *g);
void map1_setup(Game *g);
void UpdateSnowflake(Game *g);

int main()
{
    Game game;
    Map map;
    game.screenWidth = 800;
    game.screenHeight = 600;
    char player1Name[MAX_NAME_LENGTH] = {0};
    char player2Name[MAX_NAME_LENGTH] = {0};
    int focusPlayer = 1; // 1 para o jogador 1, 2 para o jogador 2
    int selectedButton = 1;
    InitWindow(game.screenWidth, game.screenHeight, "jogo");
    SetTargetFPS(60);

    // Música
    InitAudioDevice();
    Music intro = LoadMusicStream("Once Upon a Time.mp3");
    PlayMusicStream(intro);

    Music megalovania = LoadMusicStream("MEGALOVANIA.mp3");
    PlayMusicStream(megalovania);

    game.curr_map = 0;
    while (!WindowShouldClose()) // Loop principal
    {
        while (!WindowShouldClose())
        {
            telaInicial:
            UpdateMusicStream(intro);
            UpdatePlayerName((focusPlayer == 1) ? player1Name : player2Name, MAX_NAME_LENGTH, &focusPlayer);
            DrawScreen(&game, player1Name, player2Name, focusPlayer);

            ChangeMap(&game);

            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
            {
                InitGame(&game);
                while (1)
                {
                    // Inicia o jogo aqui após a escolha do mapa
                    UpdateMusicStream(megalovania);
                    game.tie = 0;
                    game.frameCount++;
                    game.elapsedTime = game.frameCount / (double)GetFPS();

                    if (game.elapsedTime >= 120.0)
                    {
                        game.hero1.speed = 0;
                        game.hero2.speed = 0;
                        for (int i = 0; i < game.hero1.num_bombs; i++)
                        {
                            game.hero1.bombs[i].isActive = 1;
                        }
                        for (int i = 0; i < game.hero2.num_bombs; i++)
                        {
                            game.hero2.bombs[i].isActive = 1;
                        }
                        DrawTie(&game);
                        if (game.tie == 1)
                        {
                           break;
                        }
                    }
                    if (game.elapsedTime < 120.0)
                        UpdateDrawFrame(&game, &map);
                    if (game.gameover)
                    {
                        while (winnerScreen)
                        {
                            DrawWinner(&game);
                            if (IsKeyPressed(KEY_ENTER))
                            {
                                winnerScreen = 0;
                            }
                        }
                        winnerScreen = 1;
                        game.gameover = 0;
                        goto telaInicial;
                        
                        
                    }
                    if (IsKeyDown(KEY_ESCAPE))
                        game.gameover = 1;
                }
                if (game.gameover)
                {
                    break;
                }
            }
        }
    }

    UnloadMusicStream(megalovania);
    UnloadMusicStream(intro);
    UnloadTexture(game.cenario1);
    UnloadTexture(game.cenario2);
    UnloadTexture(game.hero1.textura_bomberman);
    UnloadTexture(game.hero1.textura_bomba);
    for (int i = 0; i < game.num_maps; i++) {
        UnloadTexture(game.maps[i].textura_boxes);
    }
    UnloadTexture(game.speed.textura_poder);
    UnloadTexture(game.nBombs.textura_poder);
    UnloadTexture(game.radius.textura_poder);

    return 0;
}

void InitGame(Game *g)
{
    g->tie = 0;
    g->num_maps = 2;
    g->hero1.pos = (Rectangle){50, 500, STD_SIZE_X, STD_SIZE_Y};
    g->hero1.color = WHITE;
    g->hero1.speed = 4;
    g->hero1.special = 0;
    g->gameover = 0;
    g->hero1.num_bombs = 1;
    g->hero1.put_bomb = 0;
    g->hero1.draw_bomb = 0;
    g->hero2.pos = (Rectangle){650, 50, STD_SIZE_X, STD_SIZE_Y};
    g->hero2.color = PURPLE;
    g->hero2.speed = 4;
    g->hero2.special = 0;
    g->hero2.num_bombs = 1;
    g->hero2.put_bomb = 0;
    g->hero2.draw_bomb = 0;
    if (g->curr_map == 0)
    {
        map0_setup(g);
    }
    else if (g->curr_map == 1)
    {
        map1_setup(g);
    }
    g->frameCount = 0;
    g->maps->boxes->exist = 1;
    g->maps->boxes->destroyed = 0;
    g->radius.exist = 1;
    g->speed.exist = 1;
    g->nBombs.exist = 1;
    g->nerfBombs.exist = 1;
    g->nerfRadius.exist = 1;
    g->nerfSpeed.exist = 1;
    g->cenario1 = LoadTexture("fundo_teste.png");
    g->cenario2 = LoadTexture("castle.png");
    g->nome_imagem = ("tela_inicial.png");

    
    g->hero1.nome_bomberman = "humano_1.png";
    g->hero1.imagem_bomberman = LoadImage(g->hero1.nome_bomberman);
    g->hero1.textura_bomberman = LoadTextureFromImage(g->hero1.imagem_bomberman);
    g->hero1.bomberman_posicao = (Vector2){g->hero1.pos.x, (g->hero1.pos.y)};
    g->hero1.rec_bomberman = (Rectangle){g->hero1.pos.x, g->hero1.pos.y,  g->hero1.imagem_bomberman.width/12, g->hero1.imagem_bomberman.height};
    g->hero1.currentFrame = 4;
    g->hero1.framesSpeed = 1;
    g->hero1.numero_frames_bomberman = 12;
    g->hero1.largura_frame = g->hero1.imagem_bomberman.width / g->hero1.numero_frames_bomberman;
    g->hero1.frameTimer = 0;


    g->hero2.nome_bomberman = "caveira01.png";
    g->hero2.imagem_bomberman = LoadImage(g->hero2.nome_bomberman);
    g->hero2.textura_bomberman = LoadTextureFromImage(g->hero2.imagem_bomberman);
    g->hero2.bomberman_posicao = (Vector2){g->hero2.pos.x, (g->hero2.pos.y)};
    g->hero2.rec_bomberman = (Rectangle){g->hero2.pos.x, g->hero2.pos.y,  g->hero2.imagem_bomberman.width/12, g->hero2.imagem_bomberman.height};
    g->hero2.currentFrame = 4;
    g->hero2.framesSpeed = 1;
    g->hero2.numero_frames_bomberman = 12;
    g->hero2.largura_frame = g->hero2.imagem_bomberman.width / g->hero2.numero_frames_bomberman;
    g->hero2.frameTimer = 0;


    g->hero1.nome_explosao = "explosão.png";
    g->hero1.imagem_explosao = LoadImage(g->hero1.nome_explosao);
    g->hero1.textura_explosao = LoadTextureFromImage(g->hero1.imagem_explosao);

    g->hero2.nome_explosao = "explosão.png";
    g->hero2.imagem_explosao = LoadImage(g->hero2.nome_explosao);
    g->hero2.textura_explosao = LoadTextureFromImage(g->hero2.imagem_explosao);


    g->maps->nome_barrier1 = "barreira_azul.png";
    g->maps->imagem_barrier1 = LoadImage(g->maps->nome_barrier1);
    g->maps->textura_barrier1 = LoadTextureFromImage(g->maps->imagem_barrier1);

    g->maps->nome_barrier2 = "barreira_vermelha.png";
    g->maps->imagem_barrier2 = LoadImage(g->maps->nome_barrier2);
    g->maps->textura_barrier2 = LoadTextureFromImage(g->maps->imagem_barrier2);


    g->speed.nome_poder = "speed.png";
    g->speed.imagem_poder = LoadImage(g->speed.nome_poder);
    g->speed.textura_poder = LoadTextureFromImage(g->speed.imagem_poder);

    g->nBombs.nome_poder = "n_bombs.png";
    g->nBombs.imagem_poder = LoadImage(g->nBombs.nome_poder);
    g->nBombs.textura_poder = LoadTextureFromImage(g->nBombs.imagem_poder);

    g->radius.nome_poder = "n_radius.png";
    g->radius.imagem_poder = LoadImage(g->radius.nome_poder);
    g->radius.textura_poder = LoadTextureFromImage(g->radius.imagem_poder);

    g->nerfSpeed.nome_nerf = "speed.png";
    g->nerfSpeed.imagem_nerf = LoadImage(g->nerfSpeed.nome_nerf);
    g->nerfSpeed.textura_nerf = LoadTextureFromImage(g->nerfSpeed.imagem_nerf);

    g->nerfBombs.nome_nerf = "n_bombs.png";
    g->nerfBombs.imagem_nerf = LoadImage(g->nerfBombs.nome_nerf);
    g->nerfBombs.textura_nerf = LoadTextureFromImage (g->nerfBombs.imagem_nerf);

    g->nerfRadius.nome_nerf = "n_radius.png";
    g->nerfRadius.imagem_nerf = LoadImage(g->nerfRadius.nome_nerf);
    g->nerfRadius.textura_nerf = LoadTextureFromImage(g->nerfRadius.imagem_nerf);


    g->maps->nome_boxes = "barreira_quebravel.png";
    g->maps->imagem_boxes = LoadImage(g->maps->nome_boxes);
    g->maps->textura_boxes = LoadTextureFromImage(g->maps->imagem_boxes);



    for (int i = 0; i < MAX_SNOWFLAKES; i++) {
        g->snow[i].position.x = GetRandomValue(0, g->screenWidth);
        g->snow[i].position.y = GetRandomValue(0, g->screenHeight);
        g->snow[i].speed = (float)GetRandomValue(1, 5) / 5.0f;
    }

}

void UpdateGame(Game *g, Map *m)
{
    UpdateSnowflake(g);

    update_hero_pos(g);
    update_bomb(g);

    Map *map = &g->maps[g->curr_map];



}
void DrawScreen(Game *g, char *player1Name, char *player2Name, int focusPlayer)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Título "Bomberman"
    DrawText("Bomber...tale?", g->screenWidth / 2 - MeasureText("Bomber...tale?", 40) / 2, 50, 40, BLACK);

    // Entrada de Nomes dos Jogadores
    DrawText("Jogador 1:", g->screenWidth / 2 - 100, 150, 20, BLACK);
    DrawRectangle(g->screenWidth / 2 + 10, 150, 100, 30, (focusPlayer == 1) ? DARKGRAY : GRAY);
    DrawText(player1Name, g->screenWidth / 2 - MeasureText(player1Name, 20) / 2 + 60, 155, 20, BLACK);
    if (focusPlayer == 1)
    {
        DrawText(player1Name, g->screenWidth / 2 - MeasureText(player1Name, 20) / 2 + 60, 155, 20, BLACK);
    }

    DrawText("Jogador 2:", g->screenWidth / 2 - 100, 200, 20, BLACK);
    DrawRectangle(g->screenWidth / 2 + 13, 200, 100, 30, (focusPlayer == 2) ? DARKGRAY : GRAY);
    if (focusPlayer == 2)
    {
        DrawText(player2Name, g->screenWidth / 2 - MeasureText(player2Name, 20) / 2 + 63, 205, 20, BLACK);
    }

    // Destacar o mapa selecionado
    DrawText("Escolha um Mapa:", g->screenWidth / 2 - MeasureText("Escolha um Mapa:", 20) / 2, 300, 20, BLACK);
    DrawRectangle(g->screenWidth / 2 - 100, 350 + g->curr_map * 50, 200, 40, GRAY);
    DrawText("Mapa 1", g->screenWidth / 2 - MeasureText("Mapa 1", 20) / 2, 360, 20, BLACK);
    DrawText("Mapa 2", g->screenWidth / 2 - MeasureText("Mapa 2", 20) / 2, 410, 20, BLACK);
    EndDrawing();
}
void DrawTie(Game *g)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Mensagem de empate
    DrawText("EMPATE", GetScreenWidth() / 2 - MeasureText("EMPATE", 20) / 2, GetScreenHeight() / 2 - 50, 20, BLACK);

    // Destacar as opções de botões
    Rectangle button1 = {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 20, 200, 40};
    Rectangle button2 = {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 80, 200, 40};

    // Desenhar os botões
    DrawRectangle(button1.x, button1.y, button1.width, button1.height, (g->selectedButton == 0) ? DARKGRAY : GRAY);
    DrawText("Tela Inicial", button1.x + 40, button1.y + 10, 20, BLACK);

    DrawRectangle(button2.x, button2.y, button2.width, button2.height, (g->selectedButton == 1) ? DARKGRAY : GRAY);
    DrawText("Fechar Jogo", button2.x + 40, button2.y + 10, 20, BLACK);

    // Desenhar um indicador ao redor do botão selecionado
    if (g->selectedButton == 0)
    {
        DrawRectangleLinesEx(button1, 3, RED);
    }
    else
    {
        DrawRectangleLinesEx(button2, 3, RED);
    }

    EndDrawing();

    // Atualizar a seleção com as teclas de seta
    if (IsKeyPressed(KEY_DOWN))
    {
        g->selectedButton = (g->selectedButton + 1) % 2;
    }
    else if (IsKeyPressed(KEY_UP))
    {
        g->selectedButton = (g->selectedButton - 1 + 2) % 2;
    }

    // Verificar se a tecla Enter foi pressionada para selecionar o botão
    if (IsKeyPressed(KEY_ENTER))
    {
        if (g->selectedButton == 0)
        {
            g->elapsedTime = 0;
            g->tie = 1;
            printf("%d", g->tie);
            return;
        }
        else
        {
            // Fechar o jogo
            CloseWindow();
        }
    }
}
void DrawWinner(Game *g)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("GAMEOVER", GetScreenWidth() / 2 - MeasureText("GAMEOVER", 20) / 2, GetScreenHeight() / 2 - 50, 20, BLACK);

    // Destacar as opções de botões
    Rectangle button1 = {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 20, 200, 40};
    Rectangle button2 = {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 80, 200, 40};

    // Desenhar os botões
    DrawRectangle(button1.x, button1.y, button1.width, button1.height, (g->selectedButton == 0) ? DARKGRAY : GRAY);
    DrawText("Revanche", button1.x + 40, button1.y + 10, 20, BLACK);

    DrawRectangle(button2.x, button2.y, button2.width, button2.height, (g->selectedButton == 1) ? DARKGRAY : GRAY);
    DrawText("Fechar Jogo", button2.x + 40, button2.y + 10, 20, BLACK);

    // Desenhar um indicador ao redor do botão selecionado
    if (g->selectedButton == 0)
    {
        DrawRectangleLinesEx(button1, 3, RED);
    }
    else
    {
        DrawRectangleLinesEx(button2, 3, RED);
    }

    EndDrawing();

    // Atualizar a seleção com as teclas de seta
    if (IsKeyPressed(KEY_DOWN))
    {
        g->selectedButton = (g->selectedButton + 1) % 2;
    }
    else if (IsKeyPressed(KEY_UP))
    {
        g->selectedButton = (g->selectedButton - 1 + 2) % 2;
    }

    // Verificar se a tecla Enter foi pressionada para selecionar o botão
    if (IsKeyPressed(KEY_ENTER))
    {
        g->tie = 1;
        winnerScreen = 0;
        if (g->selectedButton == 0)
        {
            g->elapsedTime = 0;
            g->tie = 1;
            InitGame(g);
            return;
        }
        else
        {
            // Fechar o jogo
            CloseWindow();
        }
        
    }
}
void UpdatePlayerName(char *playerName, int maxChars, int *focusPlayer)
{
    if (IsKeyPressed(KEY_RIGHT_SHIFT))
    {
        if (*focusPlayer == 1)
        {
            *focusPlayer = 2;
        }
        else if (*focusPlayer == 2)
        {
            *focusPlayer = 1;
        }
    }
    else if (IsKeyPressed(KEY_BACKSPACE) && TextLength(playerName) > 0)
    {
        playerName[TextLength(playerName) - 1] = '\0';
    }
    else if (IsKeyPressed(KEY_APOSTROPHE) && TextLength(playerName) < maxChars)
    {
        strcat(playerName, "'");
    }
    else
    {
        int key = GetKeyPressed();
        if ((key >= 32) && (key <= 125) && TextLength(playerName) < maxChars)
        {
            char charStr[2] = {(char)key, '\0'};
            strcat(playerName, charStr);
        }
    }
}
void DrawGame(Game *g)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawRectangle(0, 0, g->screenWidth, g->screenHeight, g->maps[g->curr_map].color);
    if(g->curr_map == 0){
        DrawTexture(g->cenario1, 0, 0, WHITE);
    }
    if(g->curr_map == 1){
        DrawTexture(g->cenario2, 0, 0, WHITE);
    }
    //draw_borders(g);
    draw_map(g);

     for (int i = 0; i < g->hero1.num_bombs; i++)
    {
        if (g->hero1.bombs[i].isActive == 1)
            {
                //DrawTexture(g->hero1.textura_bomba, g->hero1.bombs[i].pos.x-5, g->hero1.bombs[i].pos.y-10, WHITE);
            }
    }

    for (int i = 0; i < MAX_SNOWFLAKES; i++) {
            DrawCircleV(g->snow[i].position, 2, WHITE);
        }

    draw_bomb(g);
    drawPowerUp(g);
    drawNerf(g);
    drawBoxes(g);
   

    DrawTextureRec(g->hero2.textura_bomberman, g->hero2.rec_bomberman, (Vector2){g->hero2.pos.x-20,g->hero2.pos.y-30}, WHITE);
    DrawTextureRec(g->hero1.textura_bomberman, g->hero1.rec_bomberman, (Vector2){g->hero1.pos.x-20,g->hero1.pos.y-30}, WHITE);

    EndDrawing();
}
void ChangeMap(Game *g)
{

    if (IsKeyPressed(KEY_UP))
    {
        g->curr_map = 0;
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        g->curr_map = 1;
    }
}
void UpdateDrawFrame(Game *g, Map *m)
{
    UpdateGame(g, m);
    DrawGame(g);
}
void draw_borders(Game *g)
{
    DrawRectangle(0, 0, SCREEN_BORDER, g->screenHeight, BLACK);
    DrawRectangle(0, 0, g->screenWidth, SCREEN_BORDER, BLACK);
    DrawRectangle(g->screenWidth - SCREEN_BORDER, 0, g->screenWidth, g->screenHeight, BLACK);
    DrawRectangle(0, g->screenHeight - SCREEN_BORDER, g->screenWidth, g->screenHeight, BLACK);
}
void draw_map(Game *g)
{
    Map *map = &g->maps[g->curr_map];
    for (int i = 0; i < map->num_barriers; i++)
    {
        if(g->curr_map == 0){
        DrawTexture(g->maps->textura_barrier1, map->barriers[i].x, map->barriers[i].y, WHITE);
        //DrawRectangleRec(map->barriers[i], BLACK);
        }
        if(g->curr_map == 1){
            DrawTexture(g->maps->textura_barrier2, map->barriers[i].x, map->barriers[i].y, WHITE);
        }
    }
}
void draw_bomb(Game *g)
{
    for (int i = 0; i < g->hero1.num_bombs; i++)
    {
        if (g->hero1.bombs[i].isActive == 1)
        {
            DrawRectangleRec(g->hero1.bombs[i].pos, RED);
            DrawRectangleRec(g->hero1.bombs[i].explosion_right, RED);
            DrawRectangleRec(g->hero1.bombs[i].explosion_left, RED);
            DrawRectangleRec(g->hero1.bombs[i].explosion_up, RED);
            DrawRectangleRec(g->hero1.bombs[i].explosion_down, RED);
            DrawTexture(g->hero1.textura_explosao, g->hero1.bombs[i].pos.x-7, g->hero1.bombs[i].pos.y-14, WHITE);
        }
    }
    for (int i = 0; i < g->hero2.num_bombs; i++)
    {
        if (g->hero2.bombs[i].isActive == 1)
        {
            DrawRectangleRec(g->hero2.bombs[i].pos, RED);
            DrawRectangleRec(g->hero2.bombs[i].explosion_right, RED);
            DrawRectangleRec(g->hero2.bombs[i].explosion_left, RED);
            DrawRectangleRec(g->hero2.bombs[i].explosion_up, RED);
            DrawRectangleRec(g->hero2.bombs[i].explosion_down, RED);
            DrawTexture(g->hero2.textura_explosao, g->hero2.bombs[i].pos.x-7, g->hero2.bombs[i].pos.y-14, WHITE);
        }
    }
}
void drawBoxes(Game *g)
{
    Hero *h = &g->hero1;
    Hero *d = &g->hero2;
    Map *map = &g->maps[g->curr_map];

    for (int k = 0; k < h->num_bombs; k++)
    {
        for (int i = 0; i < map->num_boxes; i++)
        {
            if (map->boxes[i].exist == 1 && map->boxes[i].destroyed == 0)
            {
               DrawTexture(g->maps->textura_boxes, map->boxes[i].pos.x, map->boxes[i].pos.y, WHITE);
                //DrawRectangleRec(map->boxes[i].pos, BROWN);

                if (boxesCollision(map, d->bombs[k].explosion_down) &&
                    CheckCollisionRecs(map->boxes[i].pos, d->bombs[k].explosion_down))
                {
                    map->boxes[i].destroyed = 1;
                    g->maps[0].boxes[i] = (Boxes){0, 0, 0, 0, 0};
                    g->maps[1].boxes[i] = (Boxes){0, 0, 0, 0, 0};
                }
                else if (boxesCollision(map, d->bombs[k].explosion_up) &&
                         CheckCollisionRecs(map->boxes[i].pos, d->bombs[k].explosion_up))
                {
                    map->boxes[i].destroyed = 1;
                    g->maps[0].boxes[i] = (Boxes){0, 0, 0, 0, 0};
                    g->maps[1].boxes[i] = (Boxes){0, 0, 0, 0, 0};
                }
                else if (boxesCollision(map, d->bombs[k].explosion_right) &&
                         CheckCollisionRecs(map->boxes[i].pos, d->bombs[k].explosion_right))
                {
                    map->boxes[i].destroyed = 1;
                    g->maps[0].boxes[i] = (Boxes){0, 0, 0, 0, 0};
                    g->maps[1].boxes[i] = (Boxes){0, 0, 0, 0, 0};
                }
                else if (boxesCollision(map, d->bombs[k].explosion_left) &&
                         CheckCollisionRecs(map->boxes[i].pos, d->bombs[k].explosion_left))
                {
                    map->boxes[i].destroyed = 1;
                    g->maps[0].boxes[i] = (Boxes){0, 0, 0, 0, 0};
                    g->maps[1].boxes[i] = (Boxes){0, 0, 0, 0, 0};
                }
            }
        }
    }

    for (int l = 0; l < h->num_bombs; l++)
    {
    for (int j = 0; j < map->num_boxes; j++)
    {
        if (map->boxes[j].exist == 1 && map->boxes[j].destroyed == 0)
        {
            DrawTexture(g->maps->textura_boxes, map->boxes[j].pos.x, map->boxes[j].pos.y, WHITE);
           // DrawRectangleRec(map->boxes[j].pos, BROWN);

            if (boxesCollision(map, h->bombs[l].explosion_down) &&
                CheckCollisionRecs(map->boxes[j].pos, h->bombs[l].explosion_down))
            {
                map->boxes[j].destroyed = 1;
                g->maps[0].boxes[j] = (Boxes){0, 0, 0, 0, 0};
                g->maps[1].boxes[j] = (Boxes){0, 0, 0, 0, 0};
            }
            else if (boxesCollision(map, h->bombs[l].explosion_up) &&
                     CheckCollisionRecs(map->boxes[j].pos, h->bombs[l].explosion_up))
            {
                map->boxes[j].destroyed = 1;
                g->maps[0].boxes[j] = (Boxes){0, 0, 0, 0, 0};
                g->maps[1].boxes[j] = (Boxes){0, 0, 0, 0, 0};
            }
            else if (boxesCollision(map, h->bombs[l].explosion_right) &&
                     CheckCollisionRecs(map->boxes[j].pos, h->bombs[l].explosion_right))
            {
                map->boxes[j].destroyed = 1;
                g->maps[0].boxes[j] = (Boxes){0, 0, 0, 0, 0};
                g->maps[1].boxes[j] = (Boxes){0, 0, 0, 0, 0};
            }
            else if (boxesCollision(map, h->bombs[l].explosion_left) &&
                     CheckCollisionRecs(map->boxes[j].pos, h->bombs[l].explosion_left))
            {
                map->boxes[j].destroyed = 1;
                g->maps[0].boxes[j] = (Boxes){0, 0, 0, 0, 0};
                g->maps[1].boxes[j] = (Boxes){0, 0, 0, 0, 0};
            }
        }
    }
}
}
void drawPowerUp(Game *g){
    if(g->speed.exist == 1){
        DrawTexture(g->speed.textura_poder, g->speed.pos.x, g->speed.pos.y,WHITE);
        //DrawRectangleRec(g->speed.pos, YELLOW);
    }
    if(g->nBombs.exist == 1){
        DrawTexture(g->nBombs.textura_poder, g->nBombs.pos.x, g->nBombs.pos.y,WHITE);
        //DrawRectangleRec(g->nBombs.pos, MAROON);
    }
    if(g->radius.exist == 1){
        DrawTexture(g->radius.textura_poder, g->radius.pos.x, g->radius.pos.y,WHITE);
        //DrawRectangleRec(g->radius.pos, PURPLE);
    }
}
void drawNerf(Game *g){
    if(g->nerfSpeed.exist == 1){
        DrawTexture(g->nerfSpeed.textura_nerf, g->nerfSpeed.pos.x, g->nerfSpeed.pos.y, WHITE);
    }
    if(g->nerfBombs.exist == 1){
        DrawTexture(g->nerfBombs.textura_nerf, g->nerfBombs.pos.x, g->nerfBombs.pos.y, WHITE);
    }
    if(g->nerfRadius.exist == 1){
        DrawTexture(g->nerfRadius.textura_nerf, g->nerfRadius.pos.x, g->nerfRadius.pos.y, WHITE);
    }
}
void update_hero_pos(Game *g)
{
    Hero *h = &g->hero1;
    Hero *d = &g->hero2;
    Map *m = &g->maps[g->curr_map];

    h->frameTimer += GetFrameTime();

    if (h->frameTimer >= 0.2f / h->framesSpeed)
    {
        h->currentFrame++;
        h->frameTimer = 0.0f;
    }

   

    if (IsKeyDown(KEY_D))
    {
        if (h->pos.x + h->pos.width < g->screenWidth - SCREEN_BORDER)
            h->pos.x += h->speed;
        if (barrier_collision(m, h->pos) || bombCollision(g, h) || boxesCollision(m, h->pos) || powerCollision(g) || nerfCollision(g))
        {
            h->pos.x -= h->speed;
        }

        // Quando pressionar D, ir do frame 10 ao 12
        if (h->currentFrame < 10 || h->currentFrame > 12)
        {
            h->currentFrame = 10;
        }
    }
    else if (IsKeyDown(KEY_A))
    {
        if (h->pos.x > SCREEN_BORDER)
        {
            h->pos.x -= h->speed;
        }
        if (barrier_collision(m, h->pos) || bombCollision(g, h) || boxesCollision(m, h->pos) || powerCollision(g) || nerfCollision(g))
        {
            h->pos.x += h->speed;
        }
        h->bomberman_posicao.x -= h->speed;

        // Quando pressionar A, ir do frame 1 ao 3
        if (h->currentFrame < 1 || h->currentFrame > 3)
        {
            h->currentFrame = 1;
        }
    }
    else if (IsKeyDown(KEY_W))
    {
        if (h->pos.y > SCREEN_BORDER)
            h->pos.y -= h->speed;
        if (barrier_collision(m, h->pos) || bombCollision(g, h) || boxesCollision(m, h->pos) || powerCollision(g) || nerfCollision(g))
        {
            h->pos.y += h->speed;
        }

        h->bomberman_posicao.y -= h->speed;

        // Quando pressionar W, ir do frame 7 ao 9
        if (h->currentFrame < 7 || h->currentFrame > 9)
        {
            h->currentFrame = 7;
        }
    }
    else if (IsKeyDown(KEY_S))
    {
        if (h->pos.y + h->pos.height < g->screenHeight - SCREEN_BORDER)
            h->pos.y += h->speed;
        if (barrier_collision(m, h->pos) || bombCollision(g, h) || boxesCollision(m, h->pos) || powerCollision(g) || nerfCollision(g))
        {
            h->pos.y -= h->speed;
        }

        h->bomberman_posicao.y += h->speed;

        // Quando pressionar S, ir do frame 4 ao 6
        if (h->currentFrame < 4 || h->currentFrame > 6)
        {
            h->currentFrame = 4;
        }
    }


    d->frameTimer += GetFrameTime();

    if (d->frameTimer >= 0.2f /d->framesSpeed)
    {
       d->currentFrame++;
       d->frameTimer = 0.0f;
    }

    if (IsKeyDown(KEY_LEFT))
    {
        if (d->pos.x > SCREEN_BORDER)
            d->pos.x -= d->speed;
        if (barrier_collision(m, d->pos)|| bombCollision(g, d) || boxesCollision(m, d->pos) || powerCollision(g) || nerfCollision(g))
            d->pos.x += d->speed;

        // Quando pressionar A, ir do frame 1 ao 3
        if (d->currentFrame < 1 || d->currentFrame > 3)
        {
            d->currentFrame = 1;
        }
    }
    else if (IsKeyDown(KEY_RIGHT))
    {
        if (d->pos.x + d->pos.width < g->screenWidth - SCREEN_BORDER)
            d->pos.x += d->speed;
        if (barrier_collision(m, d->pos)|| bombCollision(g, d) || boxesCollision(m, d->pos) || powerCollision(g) || nerfCollision(g))
            d->pos.x -= d->speed;

        // Quando pressionar D, ir do frame 10 ao 12
        if (d->currentFrame < 10 || d->currentFrame > 12)
        {
            d->currentFrame = 10;
        }
    }
    else if (IsKeyDown(KEY_UP))
    {
        if (d->pos.y > SCREEN_BORDER)
            d->pos.y -= d->speed;
        if (barrier_collision(m, d->pos)|| bombCollision(g, d) || boxesCollision(m, d->pos) || powerCollision(g) || nerfCollision(g))
            d->pos.y += d->speed;

        // Quando pressionar W, ir do frame 7 ao 9
        if (d->currentFrame < 7 || d->currentFrame > 9)
        {
            d->currentFrame = 7;
        }
    }
    else if (IsKeyDown(KEY_DOWN))
    {
        if (d->pos.y + d->pos.height < g->screenHeight - SCREEN_BORDER)
            d->pos.y += d->speed;
        if (barrier_collision(m, d->pos)|| bombCollision(g, d) || boxesCollision(m, d->pos) || powerCollision(g) || nerfCollision(g))
            d->pos.y -= d->speed;

        // Quando pressionar S, ir do frame 4 ao 6
        if (d->currentFrame < 4 || d->currentFrame > 6)
        {
            d->currentFrame = 4;
        }
    }
    
    switch (h->currentFrame) {
        case 1:
            h->rec_bomberman.x = 0;
            break;
        case 2:
            h->rec_bomberman.x = 47.5;
            break;
        case 3:
            h->rec_bomberman.x = 95;
            break;
        case 4:
            h->rec_bomberman.x = 142.5;
            break;
        case 5:
            h->rec_bomberman.x = 190;
            break;
        case 6:
            h->rec_bomberman.x = 237.5;
            break;
        case 7:
            h->rec_bomberman.x = 285;
            break;
        case 8:
            h->rec_bomberman.x = 332.5;
            break;
        case 9:
            h->rec_bomberman.x = 380;
            break;
        case 10:
            h->rec_bomberman.x = 427,5;
            break;
        case 11:
            h->rec_bomberman.x = 475;
            break;
        case 12:
            h->rec_bomberman.x = 522.5;
            break;
        default:
            break;
    }
    
    switch (d->currentFrame) {
        case 1:
            d->rec_bomberman.x = 0;
            break;
        case 2:
            d->rec_bomberman.x = 39;
            break;
        case 3:
            d->rec_bomberman.x = 78;
            break;
        case 4:
            d->rec_bomberman.x = 117;
            break;
        case 5:
            d->rec_bomberman.x = 156;
            break;
        case 6:
            d->rec_bomberman.x = 195;
            break;
        case 7:
            d->rec_bomberman.x = 234;
            break;
        case 8:
            d->rec_bomberman.x = 273;
            break;
        case 9:
            d->rec_bomberman.x = 312;
            break;
        case 10:
            d->rec_bomberman.x = 351;
            break;
        case 11:
            d->rec_bomberman.x = 390;
            break;
        case 12:
            d->rec_bomberman.x = 429;
            break;
        default:
            break;
    }

}
int bombCollision(Game *g, Hero *h)
{

    for (int i = 0; i < g->hero1.num_bombs; i++)
    {
        if (g->hero1.bombs[i].isActive == 1)
        {
            Rectangle bombBounds = {g->hero1.bombs[i].pos.x, g->hero1.bombs[i].pos.y, STD_SIZE_BOMB_X, STD_SIZE_BOMB_Y};
            if (CheckCollisionRecs(h->pos, bombBounds) && !g->hero1.bombs[i].collision)
            {
                return 1;
            }
        }
    }

    for (int i = 0; i < g->hero2.num_bombs; i++)
    {
        if (g->hero2.bombs[i].isActive == 1)
        {
            Rectangle bombBounds = {g->hero2.bombs[i].pos.x, g->hero2.bombs[i].pos.y, STD_SIZE_BOMB_X, STD_SIZE_BOMB_X};
            if (CheckCollisionRecs(h->pos, bombBounds) && !g->hero2.bombs[i].collision)
            {
                return 1;
            }
        }
    }

    return 0;
}
void update_bomb(Game *g)
{
    Map *actual_map = &g->maps[g->curr_map];
    Hero *d = &g->hero2;
    int collision = 0;
    if (IsKeyPressed(KEY_SPACE))
    {
        g->hero1.put_bomb = 1;
    }
    if (g->hero1.put_bomb == 1)
    {
        for (int i = 0; i < g->hero1.num_bombs; i++)
        {
            if (g->hero1.bombs[i].isActive == 0)
            {
                g->hero1.bombs[i].isActive = 1;
                g->hero1.bombs[i].pos = (Rectangle){g->hero1.pos.x, g->hero1.pos.y, STD_SIZE_BOMB_X, STD_SIZE_BOMB_Y};
                g->hero1.bombs[i].explosion_right = (Rectangle){g->hero1.pos.x, g->hero1.pos.y, STD_SIZE_BOMB_X, STD_SIZE_BOMB_Y};
                g->hero1.bombs[i].explosion_left = (Rectangle){g->hero1.pos.x, g->hero1.pos.y, STD_SIZE_BOMB_X, STD_SIZE_BOMB_Y};
                g->hero1.bombs[i].explosion_down = (Rectangle){g->hero1.pos.x, g->hero1.pos.y, STD_SIZE_BOMB_X, STD_SIZE_BOMB_Y};
                g->hero1.bombs[i].explosion_up = (Rectangle){g->hero1.pos.x, g->hero1.pos.y, STD_SIZE_BOMB_X, STD_SIZE_BOMB_Y};
                g->hero1.bombs[i].collision = 1;
                g->hero1.bombs[i].time = GetTime();
                break;
            }
        }
    }
    for (int i = 0; i < g->hero1.num_bombs; i++)
    {
        if (g->hero1.bombs[i].isActive == 1)
        {
            if (!CheckCollisionRecs(g->hero1.bombs[i].pos, g->hero1.pos))
            {
                g->hero1.bombs[i].collision = 0;
            }
            if (fabs(g->hero1.bombs[i].time - GetTime()) > 3 &&
                fabs(g->hero1.bombs[i].time - GetTime()) < 5)
            {
                Rectangle verify_bomb;
                Rectangle rectangle_bomb;
                int grow_tax = g->hero1.bombs[i].distance;
                if (CheckCollisionRecs(g->hero1.bombs[i].explosion_up, g->hero1.pos))
                    g->gameover = 1;
                if (CheckCollisionRecs(g->hero1.bombs[i].explosion_down, g->hero1.pos))
                    g->gameover = 1;
                if (CheckCollisionRecs(g->hero1.bombs[i].explosion_left, g->hero1.pos))
                    g->gameover = 1;
                if (CheckCollisionRecs(g->hero1.bombs[i].explosion_right, g->hero1.pos))
                    g->gameover = 1;

                if (CheckCollisionRecs(g->hero1.bombs[i].explosion_up, g->hero2.pos))
                    g->gameover = 1;
                if (CheckCollisionRecs(g->hero1.bombs[i].explosion_down, g->hero2.pos))
                    g->gameover = 1;
                if (CheckCollisionRecs(g->hero1.bombs[i].explosion_left, g->hero2.pos))
                    g->gameover = 1;
                if (CheckCollisionRecs(g->hero1.bombs[i].explosion_right, g->hero2.pos))
                    g->gameover = 1;
                if (g->hero1.bombs[i].explosion_right.width <
                    g->hero1.bombs[i].distance * STD_SIZE_X)
                {
                    rectangle_bomb = g->hero1.bombs[i].explosion_right;
                    Rectangle verify_bomb = (Rectangle){
                        rectangle_bomb.x, rectangle_bomb.y, rectangle_bomb.width + grow_tax,
                        rectangle_bomb.height};
                    if (!barrier_collision(actual_map, verify_bomb))
                    {
                        g->hero1.bombs[i].explosion_right.width +=
                            grow_tax;
                    }
                }
                if (g->hero1.bombs[i].explosion_left.width <
                    g->hero1.bombs[i].distance * STD_SIZE_X)
                {
                    rectangle_bomb = g->hero1.bombs[i].explosion_left;
                    Rectangle verify_bomb = (Rectangle){
                        rectangle_bomb.x - grow_tax, rectangle_bomb.y, rectangle_bomb.width + grow_tax, rectangle_bomb.height};
                    if (!barrier_collision(actual_map, verify_bomb))
                    {
                        g->hero1.bombs[i].explosion_left.width +=
                            grow_tax;
                        g->hero1.bombs[i].explosion_left.x -= grow_tax;
                    }
                }
                if (g->hero1.bombs[i].explosion_up.height <
                    g->hero1.bombs[i].distance * STD_SIZE_Y)
                {
                    rectangle_bomb = g->hero1.bombs[i].explosion_up;
                    Rectangle verify_bomb = (Rectangle){
                        rectangle_bomb.x, rectangle_bomb.y, rectangle_bomb.width,
                        rectangle_bomb.height + grow_tax};
                    if (!barrier_collision(actual_map, verify_bomb))
                    {
                        g->hero1.bombs[i].explosion_up.height +=
                            grow_tax;
                    }
                }
                if (g->hero1.bombs[i].explosion_down.height <
                    g->hero1.bombs[i].distance * STD_SIZE_Y)
                {
                    rectangle_bomb = g->hero1.bombs[i].explosion_down;
                    Rectangle verify_bomb = (Rectangle){
                        rectangle_bomb.x, rectangle_bomb.y - grow_tax, rectangle_bomb.width,
                        rectangle_bomb.height + grow_tax};
                    if (!barrier_collision(actual_map, verify_bomb))
                    {
                        g->hero1.bombs[i].explosion_down.height +=
                            grow_tax;
                        g->hero1.bombs[i].explosion_down.y -= grow_tax;
                    }
                }
            }
            else if (fabs(g->hero1.bombs[i].time - GetTime()) > 3)
            {
                g->hero1.bombs[i].isActive = 0;
            }
        }
    }
    g->hero1.put_bomb = 0;

    if (IsKeyPressed(KEY_KP_ENTER))
    {
        g->hero2.put_bomb = 1;
    }
    if (g->hero2.put_bomb == 1)
    {
        for (int i = 0; i < g->hero2.num_bombs; i++)
        {
            if (g->hero2.bombs[i].isActive == 0)
            {
                g->hero2.bombs[i].isActive = 1;
                g->hero2.bombs[i].pos = (Rectangle){g->hero2.pos.x, g->hero2.pos.y, STD_SIZE_BOMB_X, STD_SIZE_BOMB_Y};
                g->hero2.bombs[i].explosion_right = (Rectangle){g->hero2.pos.x, g->hero2.pos.y, STD_SIZE_BOMB_X, STD_SIZE_BOMB_Y};
                g->hero2.bombs[i].explosion_left = (Rectangle){g->hero2.pos.x, g->hero2.pos.y, STD_SIZE_BOMB_X, STD_SIZE_BOMB_Y};
                g->hero2.bombs[i].explosion_down = (Rectangle){g->hero2.pos.x, g->hero2.pos.y, STD_SIZE_BOMB_X, STD_SIZE_BOMB_Y};
                g->hero2.bombs[i].explosion_up = (Rectangle){g->hero2.pos.x, g->hero2.pos.y, STD_SIZE_BOMB_X, STD_SIZE_BOMB_Y};
                g->hero2.bombs[i].time = GetTime();
                g->hero2.bombs[i].collision = 1;
                break;
            }
        }
    }
    for (int i = 0; i < g->hero2.num_bombs; i++)
    {
        if (g->hero2.bombs[i].isActive == 1)
        {
            if (!CheckCollisionRecs(g->hero2.bombs[i].pos, g->hero2.pos))
            {
                g->hero2.bombs[i].collision = 0;
            }
            if (fabs(g->hero2.bombs[i].time - GetTime()) > 3 &&
                fabs(g->hero2.bombs[i].time - GetTime()) < 5)
            {
                Rectangle verify_bomb;
                Rectangle rectangle_bomb;
                int grow_tax = g->hero2.bombs[i].distance;
                if (CheckCollisionRecs(g->hero2.bombs[i].explosion_up, g->hero1.pos))
                    g->gameover = 1;
                if (CheckCollisionRecs(g->hero2.bombs[i].explosion_down, g->hero1.pos))
                    g->gameover = 1;
                if (CheckCollisionRecs(g->hero2.bombs[i].explosion_left, g->hero1.pos))
                    g->gameover = 1;
                if (CheckCollisionRecs(g->hero2.bombs[i].explosion_right, g->hero1.pos))
                    g->gameover = 1;

                if (CheckCollisionRecs(g->hero2.bombs[i].explosion_up, g->hero2.pos))
                    g->gameover = 1;
                if (CheckCollisionRecs(g->hero2.bombs[i].explosion_down, g->hero2.pos))
                    g->gameover = 1;
                if (CheckCollisionRecs(g->hero2.bombs[i].explosion_left, g->hero2.pos))
                    g->gameover = 1;
                if (CheckCollisionRecs(g->hero2.bombs[i].explosion_right, g->hero2.pos))
                    g->gameover = 1;
                if (g->hero2.bombs[i].explosion_right.width <
                    g->hero2.bombs[i].distance * STD_SIZE_X)
                {
                    rectangle_bomb = g->hero2.bombs[i].explosion_right;
                    Rectangle verify_bomb = (Rectangle){
                        rectangle_bomb.x, rectangle_bomb.y, rectangle_bomb.width + grow_tax,
                        rectangle_bomb.height};
                    if (!barrier_collision(actual_map, verify_bomb))
                    {
                        g->hero2.bombs[i].explosion_right.width +=
                            grow_tax;
                    }
                }
                if (g->hero2.bombs[i].explosion_left.width <
                    g->hero2.bombs[i].distance * STD_SIZE_X)
                {
                    rectangle_bomb = g->hero2.bombs[i].explosion_left;
                    Rectangle verify_bomb = (Rectangle){
                        rectangle_bomb.x - grow_tax, rectangle_bomb.y, rectangle_bomb.width + grow_tax, rectangle_bomb.height};
                    if (!barrier_collision(actual_map, verify_bomb))
                    {
                        g->hero2.bombs[i].explosion_left.width +=
                            grow_tax;
                        g->hero2.bombs[i].explosion_left.x -= grow_tax;
                    }
                }
                if (g->hero2.bombs[i].explosion_up.height <
                    g->hero2.bombs[i].distance * STD_SIZE_Y)
                {
                    rectangle_bomb = g->hero2.bombs[i].explosion_up;
                    Rectangle verify_bomb = (Rectangle){
                        rectangle_bomb.x, rectangle_bomb.y, rectangle_bomb.width,
                        rectangle_bomb.height + grow_tax};
                    if (!barrier_collision(actual_map, verify_bomb))
                    {
                        g->hero2.bombs[i].explosion_up.height +=
                            grow_tax;
                    }
                }
                if (g->hero2.bombs[i].explosion_down.height <
                    g->hero2.bombs[i].distance * STD_SIZE_Y)
                {
                    rectangle_bomb = g->hero2.bombs[i].explosion_down;
                    Rectangle verify_bomb = (Rectangle){
                        rectangle_bomb.x, rectangle_bomb.y - grow_tax, rectangle_bomb.width,
                        rectangle_bomb.height + grow_tax};
                    if (!barrier_collision(actual_map, verify_bomb))
                    {
                        g->hero2.bombs[i].explosion_down.height +=
                            grow_tax;
                        g->hero2.bombs[i].explosion_down.y -= grow_tax;
                    }
                }
            }
            else if (fabs(g->hero2.bombs[i].time - GetTime()) > 3)
            {
                g->hero2.bombs[i].isActive = 0;
            }
        }
    }
    g->hero2.put_bomb = 0;
}
int barrier_collision(Map *map, Rectangle target)
{
    for (int i = 0; i < map->num_barriers; i++)
    {
        if (CheckCollisionRecs(target, map->barriers[i]))
        {
            return 1;
        }
    }
    return 0;
}
int boxesCollision(Map *map, Rectangle target)
{
    for (int i = 0; i < map->num_boxes; i++)
    {
        if (CheckCollisionRecs(target, map->boxes[i].pos))
        {
            return 1;
        }
    }
    return 0;
}
int powerCollision(Game *g)
{
    Hero *h = &g->hero1;
    Hero *d = &g->hero2;
    {
        if (CheckCollisionRecs(g->hero1.pos, g->speed.pos))
        {
            if(g->speed.exist){
                g->speed.exist = 0;
                if(g->speed.exist == 0){
                    g->hero1.speed = 5;
                }
            }
        }
        if(CheckCollisionRecs(g->hero1.pos, g->nBombs.pos)){
            if(g->nBombs.exist){
                g->nBombs.exist = 0;
                    g->hero1.num_bombs = 2;
                if(g->nBombs.exist == 0){
                }
            }
        }
        if(CheckCollisionRecs(g->hero1.pos, g->radius.pos)){
            if(g->radius.exist){
                g->radius.exist = 0;
                if(g->radius.exist == 0){
                    g->hero1.bombs->distance = 4;
                }
            }
        }
        
        if (CheckCollisionRecs(g->hero2.pos, g->speed.pos))
        {
            if(g->speed.exist){
                g->speed.exist = 0;
                if(g->speed.exist == 0){
                g->hero2.speed = 5;
                }
            }
        }
        if(CheckCollisionRecs(g->hero2.pos, g->nBombs.pos)){
            if(g->nBombs.exist){
                g->nBombs.exist = 0;
                if(g->nBombs.exist == 0){
                g->hero2.num_bombs = 2;
                }
            }
        }
        if(CheckCollisionRecs(g->hero2.pos, g->radius.pos)){
            if(g->radius.exist){
                g->radius.exist = 0;
                if(g->radius.exist == 0){
                    g->hero2.bombs->distance = 4;
                }
            }
        }

    }
    return 0;
}
int nerfCollision(Game *g){
    Hero *h = &g->hero1;
    Hero *d = &g->hero2;
    {
        if (CheckCollisionRecs(g->hero1.pos, g->nerfSpeed.pos))
        {
            if(g->nerfSpeed.exist){
                g->nerfSpeed.exist = 0;
                if(g->nerfSpeed.exist == 0){
                    g->hero1.speed = 3;
                }
            }
        }
        if(CheckCollisionRecs(g->hero1.pos, g->nerfBombs.pos)){
            if(g->nerfBombs.exist){
                g->nerfBombs.exist = 0;
                if(g->nBombs.exist == 0){
                    g->hero1.num_bombs = 1;
                }
            }
        }
        if(CheckCollisionRecs(g->hero1.pos, g->nerfRadius.pos)){
            if(g->nerfRadius.exist){
                g->nerfRadius.exist = 0;
                if(g->nerfRadius.exist == 0){
                    g->hero1.bombs->distance = 2;
                }
            }
        }
        
        if (CheckCollisionRecs(g->hero2.pos, g->nerfSpeed.pos))
        {
            if(g->nerfSpeed.exist){
                g->nerfSpeed.exist = 0;
                if(g->nerfSpeed.exist == 0){
                g->hero2.speed = 3;
                }
            }
        }
        if(CheckCollisionRecs(g->hero2.pos, g->nerfBombs.pos)){
            if(g->nerfBombs.exist){
                g->nerfBombs.exist = 0;
                if(g->nerfBombs.exist == 0){
                g->hero2.num_bombs = 1;
                }
            }
        }
        if(CheckCollisionRecs(g->hero2.pos, g->nerfRadius.pos)){
            if(g->nerfRadius.exist){
                g->nerfRadius.exist = 0;
                if(g->nerfRadius.exist == 0){
                    g->hero2.bombs->distance = 2;
                }
            }
        }

    }
    return 0;

}
void map0_setup(Game *g)
{
    g->maps[0].num_barriers = 36;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            g->maps[0].barriers[i * 6 + j] = (Rectangle){(85 * i) + 160, (85 * j) + 68, 40, 40};
        }
    }
    g->maps[0].num_boxes = 30;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            g->maps[0].boxes[i * 6 + j] = (Boxes){(85 * i) + 202, (85 * j) + 68, 40, 40, 1};
        }
    }
    g->maps[0].color = BLUE;
    g->maps[0].prev_map = -1;
    g->maps[0].next_map = 1;
    g->speed.pos = (Rectangle){205, 157, 30, 30};
    g->nBombs.pos = (Rectangle){290, 415, 30, 30};
    g->radius.pos = (Rectangle){555, 245, 30, 30};

    g->nerfSpeed.pos = (Rectangle){465, 505, 30, 30};
    g->nerfBombs.pos = (Rectangle){465, 157, 30, 30};
    g->nerfRadius.pos = (Rectangle){380, 335, 30, 30};

    g->hero1.num_bombs = 1;
    for (int i = 0; i < 5; i++)
    {
        g->hero1.bombs[i].isActive = 0;
        g->hero1.bombs[i].distance = 3;
    }

    g->hero2.num_bombs = 1;
    for (int i = 0; i < 5; i++)
    {
        g->hero2.bombs[i].isActive = 0;
        g->hero2.bombs[i].distance = 3;
    }
}
void map1_setup(Game *g)
{
    g->maps[1].num_barriers = 36;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            g->maps[1].barriers[i * 6 + j] = (Rectangle){(85 * i) + 160, (85 * j) + 68, 40, 40};
        }
    }
    g->maps[1].num_boxes = 30;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            g->maps[1].boxes[i * 6 + j] = (Boxes){(85 * i) + 202, (85 * j) + 68, 40, 40, 1};
        }
    }
    g->maps[1].color = GREEN;
    g->maps[1].prev_map = -1;
    g->maps[1].next_map = 1;
    g->speed.pos = (Rectangle){380, 70, 30, 30};
    g->nBombs.pos = (Rectangle){205, 415, 30, 30};
    g->radius.pos = (Rectangle){555, 330, 30, 30};

    g->nerfSpeed.pos = (Rectangle){555, 70, 30, 30};
    g->nerfBombs.pos = (Rectangle){290, 245, 30, 30};
    g->nerfRadius.pos = (Rectangle){465, 505, 30, 30};

    g->hero1.num_bombs = 1;
    for (int i = 0; i < 5; i++)
    {
        g->hero1.bombs[i].isActive = 0;
        g->hero1.bombs[i].distance = 3;
    }
    g->hero2.num_bombs = 1;
    for (int i = 0; i < 5; i++)
    {
        g->hero2.bombs[i].isActive = 0;
        g->hero2.bombs[i].distance = 3;
    }
}
void UpdateSnowflake(Game *g) {
    for(int i = 0; i < MAX_SNOWFLAKES; i++){
        g->snow[i].position.y += g->snow[i].speed;

        // Se um floco de neve atingir o chão, reinicie-o no topo
        if (g->snow[i].position.y > GetScreenHeight()) {
            g->snow[i].position.y = -GetRandomValue(0, 100);
            g->snow[i].position.x = GetRandomValue(0, GetScreenWidth());
        }
    }
}


