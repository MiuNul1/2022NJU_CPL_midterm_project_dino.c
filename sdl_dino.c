#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include <stdlib.h>
#define WIDTH 1000
#define HEIGHT 800
#define JUMP_SPEED 20
#undef main

SDL_Window *Window = NULL;
SDL_Renderer *Renderer = NULL;
SDL_Texture *dino_left = NULL;
SDL_Texture *dino_midair = NULL;
SDL_Texture *dino_right = NULL;
SDL_Texture *dino_jump = NULL;
SDL_Texture *dino_dead = NULL;
SDL_Texture *table = NULL;
SDL_Texture *horizon = NULL;
SDL_Texture *duck = NULL;
SDL_Texture *cactus = NULL;
SDL_Texture *bird = NULL;
SDL_Texture *text = NULL;
SDL_Texture *cloud = NULL;
SDL_Texture *wasted = NULL;
TTF_Font *font = NULL;
SDL_Color color = {0, 0, 0, 255};
SDL_Surface *surface = NULL;
SDL_Event StopEvent;
int count_horizon = 0;
int count_jumptime = 0;
int count_downtime = 0;
int count_bird = 0;
int count_cactus_1 = 0;
int count_cactus_2 = 0;
int count_cactus_3 = 0;
int record_height = 0;
int downbug = 0;
uint64_t count_time = 0;
int isduck = 0;
int isdown = 0;
int isstop = 0;
int FPS = 40;
int birdfly = 0;
int cactus_1 = 0;
int cactus_2 = 0;
int cactus_3 = 0;
void LOAD();
void QUIT();
void RUN();
void JUMP();
void DOWN(int);
void DUCK();
void CLEAR();
void HORIZON();
void CLOUD();
void DUCK_LEFT();
void DUCK_RIGHT();
void CACTUS_1();
void CACTUS_2();
void CACTUS_3();
void BIRD_0();
void BIRD_1();
void BIRD();
void SCORE(char *);
void ACCELERATE();
void SHOWSPEED(int);
void RANDOMCREATE();
void STOP();
void GAMEOVER();
int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    Window = SDL_CreateWindow("dino", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Init();
    LOAD();
    SDL_Event OpenEvent;
    int running = 1;
    while (SDL_WaitEvent(&OpenEvent) && running)
    {
        switch (OpenEvent.type)
        {
        case SDL_QUIT:
            QUIT();
            break;
        case SDL_KEYDOWN:
            switch (OpenEvent.key.keysym.sym)
            {
            case SDLK_SPACE:
                running = 0;
                break;
            case SDLK_ESCAPE:
                QUIT();
                break;
            default:
                continue;
            }
        default:
            continue;
        }
    }

    SDL_RenderClear(Renderer);
    SDL_RenderPresent(Renderer);
    SDL_Event MainEvent;
    dino_left = IMG_LoadTexture(Renderer, "last/dino_left.png");
    dino_midair = IMG_LoadTexture(Renderer, "last/dino_midair.png");
    dino_right = IMG_LoadTexture(Renderer, "last/dino_right.png");
    dino_jump = IMG_LoadTexture(Renderer, "last/dino_midair.png");
    dino_dead = IMG_LoadTexture(Renderer, "last/dino_dead.png");
    wasted = IMG_LoadTexture(Renderer, "last/wasted.png");
    font = TTF_OpenFont("DroidSans.ttf", 30);
    horizon = IMG_LoadTexture(Renderer, "last/horizon.png");
    cloud = IMG_LoadTexture(Renderer, "last/cloud.png");
    running = 1;
    while (running)
    {

        SDL_PollEvent(&MainEvent);
        switch (MainEvent.type)
        {
        case SDL_QUIT:
            QUIT();
            running = 0;
            break;
        case SDL_KEYDOWN:
            switch (MainEvent.key.keysym.sym)
            {
            case SDLK_q:
                break;
            case SDLK_ESCAPE:
                QUIT();
                running = 0;
                break;
            case SDLK_SPACE:
                count_jumptime++;
                break;
            case SDLK_s:
                if (count_jumptime != 0 && count_downtime == 0)
                {
                    record_height = JUMP_SPEED * count_jumptime - 0.67 * count_jumptime * count_jumptime;
                    count_downtime++;
                    isdown = 1;
                    count_jumptime = 0;
                    downbug = 1;
                }
                if (!isdown)
                    isduck = 1;

                break;
            case SDLK_p:
                STOP();
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (MainEvent.key.keysym.sym)
            {
            case SDLK_s:
                isduck = 0;
                count_jumptime = 0;
                if (downbug && isdown)
                    downbug = 0;
                if (!isdown)
                    downbug = 0;
                break;
            default:
                break;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            printf("(%d,%d)\n", MainEvent.button.x, MainEvent.button.y);
            break;
        case SDL_MOUSEBUTTONUP:
            break;
        default:
            break;
        }

        count_time++;
        char score[20];
        sprintf(score, "SCORE : %d", count_time / 15);
        SCORE(score);
        ACCELERATE();
        if (isduck || (!(isdown) && downbug))
            DUCK();
        else
        {
            if (count_jumptime >= 30)
                count_jumptime = 0;

            if (count_jumptime == 0 && count_downtime == 0)
                RUN();
            else if (count_downtime != 0)
                DOWN(record_height);
            else
                JUMP();
        }
    }
    printf("game over");
    QUIT();
}
void CLEAR()
{
    SDL_DestroyTexture(dino_left);
    SDL_DestroyTexture(dino_midair);
    SDL_DestroyTexture(dino_right);
}
void QUIT()
{
    SDL_DestroyWindow(Window);
    SDL_DestroyRenderer(Renderer);
    SDL_Quit();
    SDL_DestroyTexture(dino_left);
    SDL_DestroyTexture(dino_midair);
    SDL_DestroyTexture(dino_right);
    SDL_DestroyTexture(dino_jump);
    SDL_DestroyTexture(dino_dead);
    SDL_DestroyTexture(horizon);
    SDL_DestroyTexture(cloud);
    SDL_DestroyTexture(duck);
    SDL_DestroyTexture(bird);
    SDL_DestroyTexture(text);
    SDL_DestroyTexture(wasted);
    SDL_FreeSurface(surface);
}
void LOAD()
{
    SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
    SDL_RenderClear(Renderer);
    dino_left = IMG_LoadTexture(Renderer, "last/dino_left.png");
    SDL_Rect src_dino = {250, 200, 500, 600};
    SDL_RenderCopy(Renderer, dino_left, NULL, &src_dino);
    SDL_Rect src_table = {0, 50, 1000, 200};
    font = TTF_OpenFont("DroidSans.ttf", 50);
    surface = TTF_RenderText_Blended(font, "by ymr", color);
    SDL_Rect src_text = {800, 730, surface->w, surface->h};
    text = SDL_CreateTextureFromSurface(Renderer, surface);
    SDL_RenderCopy(Renderer, text, NULL, &src_text);
    font = TTF_OpenFont("DroidSans.ttf", 100);
    surface = TTF_RenderText_Blended(font, "Press space to play", color);
    SDL_Rect src_open = {100, 100, surface->w, surface->h};
    text = SDL_CreateTextureFromSurface(Renderer, surface);
    SDL_RenderCopy(Renderer, text, NULL, &src_open);
    SDL_RenderPresent(Renderer);
}
void RUN()
{
    SDL_Rect src_dino = {5, 400, 50, 60};
    int choice = count_time % 9;
    long fps = 1000 / FPS;

    if (choice < 3)
    {
        HORIZON();

        uint32_t begin = SDL_GetTicks();
        SDL_RenderCopyEx(Renderer, dino_left, NULL, &src_dino, 0, NULL, SDL_FLIP_NONE);
        RANDOMCREATE();
        SDL_RenderPresent(Renderer);
        uint32_t current1 = SDL_GetTicks();
        uint32_t cost = current1 - begin;
        long delay = fps - cost;
        if (delay > 0)
            SDL_Delay(delay);
        SDL_RenderClear(Renderer);
    }
    else if (choice < 6)
    {
        HORIZON();

        uint32_t begin = SDL_GetTicks();
        SDL_RenderCopyEx(Renderer, dino_midair, NULL, &src_dino, 0, NULL, SDL_FLIP_NONE);
        RANDOMCREATE();
        SDL_RenderPresent(Renderer);
        uint32_t current2 = SDL_GetTicks();
        uint32_t cost = current2 - begin;
        long delay = fps - cost;
        if (delay > 0)
            SDL_Delay(delay);
        SDL_RenderClear(Renderer);
    }
    else
    {
        HORIZON();

        uint32_t begin = SDL_GetTicks();
        SDL_RenderCopyEx(Renderer, dino_right, NULL, &src_dino, 0, NULL, SDL_FLIP_NONE);
        RANDOMCREATE();
        SDL_RenderPresent(Renderer);
        uint32_t current3 = SDL_GetTicks();
        uint32_t cost = current3 - begin;
        long delay = fps - cost;
        if (delay > 0)
            SDL_Delay(delay);
        SDL_RenderClear(Renderer);
    }
}
void JUMP()
{

    HORIZON();

    uint32_t begin = SDL_GetTicks();
    int height;
    count_jumptime++;
    height = 400 - (JUMP_SPEED * count_jumptime - 0.67 * count_jumptime * count_jumptime);
    SDL_Rect src_dino = {5, height, 50, 60};
    SDL_RenderCopyEx(Renderer, dino_jump, NULL, &src_dino, 0, NULL, SDL_FLIP_NONE);
    RANDOMCREATE();
    SDL_RenderPresent(Renderer);
    uint32_t current = SDL_GetTicks();
    uint32_t cost = current - begin;
    long fps = 1000 / FPS;
    long delay = fps - cost;
    if (delay > 0)
        SDL_Delay(delay);
    SDL_RenderClear(Renderer);
}
void DOWN(int height)
{
    count_downtime++;
    HORIZON();

    uint32_t begin = SDL_GetTicks();
    int down = 4 * count_downtime * count_downtime;
    if (down > height)
    {
        down = height;
        count_downtime = 0;
        isdown = 0;
    }
    SDL_Rect src_dino = {5, 400 - height + down, 50, 60};
    SDL_RenderCopyEx(Renderer, dino_midair, NULL, &src_dino, 0, NULL, SDL_FLIP_NONE);
    RANDOMCREATE();
    SDL_RenderPresent(Renderer);
    uint32_t current = SDL_GetTicks();
    uint32_t cost = current - begin;
    long fps = 1000 / FPS;
    long delay = fps - cost;
    if (delay > 0)
        SDL_Delay(delay);
    SDL_RenderClear(Renderer);
}
void HORIZON()
{
    int hortime = count_time % 200;
    SDL_Rect src_horizon = {-10 * hortime, 440, 2000, 10};
    SDL_RenderCopy(Renderer, horizon, NULL, &src_horizon);
    if (hortime >= 100)
        src_horizon.x = 2000 - 10 * hortime;
    SDL_RenderCopy(Renderer, horizon, NULL, &src_horizon);
    CLOUD();
}
void CLOUD()
{
    SDL_Rect src_cloud = {100, 100, 82, 26};
    SDL_RenderCopy(Renderer, cloud, NULL, &src_cloud);
    src_cloud.x = 300;
    src_cloud.y = 200;
    SDL_RenderCopy(Renderer, cloud, NULL, &src_cloud);
    src_cloud.x = 500;
    src_cloud.y = 100;
    SDL_RenderCopy(Renderer, cloud, NULL, &src_cloud);
    src_cloud.x = 800;
    src_cloud.y = 250;
    SDL_RenderCopy(Renderer, cloud, NULL, &src_cloud);
}
void DUCK_LEFT()
{

    uint32_t begin = SDL_GetTicks();
    duck = IMG_LoadTexture(Renderer, "last/dino_duck_left.png");
    SDL_Rect src_dino = {5, 420, 60, 40};
    HORIZON();
    SDL_RenderCopy(Renderer, duck, NULL, &src_dino);
    RANDOMCREATE();
    SDL_RenderPresent(Renderer);
    uint32_t current1 = SDL_GetTicks();
    uint32_t cost = current1 - begin;
    long fps = 1000 / FPS;
    long delay = fps - cost;
    if (delay > 0)
        SDL_Delay(delay);
    SDL_RenderClear(Renderer);
    SDL_DestroyTexture(duck);
}
void DUCK_RIGHT()
{
    uint32_t begin = SDL_GetTicks();
    duck = IMG_LoadTexture(Renderer, "last/dino_duck_right.png");
    SDL_Rect src_dino = {5, 420, 60, 40};
    HORIZON();
    SDL_RenderCopy(Renderer, duck, NULL, &src_dino);
    RANDOMCREATE();
    SDL_RenderPresent(Renderer);
    uint32_t current1 = SDL_GetTicks();
    uint32_t cost = current1 - begin;
    long fps = 1000 / FPS;
    long delay = fps - cost;
    if (delay > 0)
        SDL_Delay(delay);
    SDL_RenderClear(Renderer);
    SDL_DestroyTexture(duck);
}
void CACTUS_1()
{
    count_cactus_1++;
    int x = 1000 - 10 * count_cactus_1;
    cactus = IMG_LoadTexture(Renderer, "last/cactus_large_1.png");
    SDL_Rect src_cactus = {x, 400, 26, 66};
    SDL_RenderCopy(Renderer, cactus, NULL, &src_cactus);
    SDL_DestroyTexture(cactus);
    if (x < -40)
    {
        count_cactus_1 = 0;
        cactus_1 = 0;
    }
    int height = 0;
    if (x < 50 && x > -20)
    {
        if (count_jumptime != 0)
        {
            height = 400 - (JUMP_SPEED * count_jumptime - 0.67 * count_jumptime * count_jumptime);
            if (height > 350)
                GAMEOVER();
        }
        else if (isdown)
        {
            height = 400 - record_height + 4 * count_downtime * count_downtime;
            if (height > 370)
                GAMEOVER();
        }
        else
        {
            GAMEOVER();
        }
    }
}
void CACTUS_2()
{
    count_cactus_2++;
    int x = 1000 - 10 * count_cactus_2;
    cactus = IMG_LoadTexture(Renderer, "last/cactus_small_2.png");
    SDL_Rect src_cactus = {x, 410, 46, 47};
    SDL_RenderCopy(Renderer, cactus, NULL, &src_cactus);
    SDL_DestroyTexture(cactus);
    if (x < -40)
    {
        count_cactus_2 = 0;
        cactus_2 = 0;
    }
    int height = 0;
    if (x < 50 && x > -20)
    {
        if (count_jumptime != 0)
        {
            height = 400 - (JUMP_SPEED * count_jumptime - 0.67 * count_jumptime * count_jumptime);
            if (height > 350)
                GAMEOVER();
        }
        else if (isdown)
        {
            height = 400 - record_height + 4 * count_downtime * count_downtime;
            if (height > 370)
                GAMEOVER();
        }
        else
        {
            GAMEOVER();
        }
    }
}
void CACTUS_3()
{
    count_cactus_3++;
    int x = 1000 - 10 * count_cactus_3;
    cactus = IMG_LoadTexture(Renderer, "last/cactus_large_3.png");
    SDL_Rect src_cactus = {x, 390, 100, 75};
    SDL_RenderCopy(Renderer, cactus, NULL, &src_cactus);
    SDL_DestroyTexture(cactus);
    if (x < -100)
    {
        count_cactus_3 = 0;
        cactus_3 = 0;
    }
    int height = 0;
    if (x < 50 && x > -80)
    {
        if (count_jumptime != 0)
        {
            height = 400 - (JUMP_SPEED * count_jumptime - 0.67 * count_jumptime * count_jumptime);
            if (height > 350)
                GAMEOVER();
        }
        else if (isdown)
        {
            height = 400 - record_height + 4 * count_downtime * count_downtime;
            if (height > 370)
                GAMEOVER();
        }
        else
        {
            GAMEOVER();
        }
    }
}
void DUCK()
{
    isduck = 1;
    int choice = count_time % 10;
    if (choice <= 4)
        DUCK_LEFT();
    else
        DUCK_RIGHT();
}
void BIRD_0()
{
    count_bird++;
    int x = 1000 - 12 * count_bird;
    bird = IMG_LoadTexture(Renderer, "last/bird_0.png");
    SDL_Rect src_bird = {x, 380, 45, 45};
    SDL_RenderCopy(Renderer, bird, NULL, &src_bird);
    SDL_DestroyTexture(bird);
    if (x < -30)
    {
        count_bird = 0;
        birdfly = 0;
    }
    int height = 0;
    if (x < 50)
    {
        if (count_jumptime != 0)
        {
            height = 400 - (JUMP_SPEED * count_jumptime - 0.67 * count_jumptime * count_jumptime);
            if (height > 320)
                GAMEOVER();
        }
        else if (isdown)
        {
            height = 400 - record_height + 4 * count_downtime * count_downtime;
            if (height > 340)
                GAMEOVER();
        }
        else if (!isduck)
        {
            GAMEOVER();
        }
    }
}
void BIRD_1()
{
    count_bird++;
    int x = 1000 - 12 * count_bird;
    bird = IMG_LoadTexture(Renderer, "last/bird_1.png");
    SDL_Rect src_bird = {x, 370, 45, 45};
    SDL_RenderCopy(Renderer, bird, NULL, &src_bird);
    SDL_DestroyTexture(bird);
    if (1000 - 12 * count_bird < -30)
    {
        count_bird = 0;
        birdfly = 0;
    }
    int height = 0;
    if (x < 50)
    {
        if (count_jumptime != 0)
        {
            height = 400 - (JUMP_SPEED * count_jumptime - 0.67 * count_jumptime * count_jumptime);
            if (height > 320)
                GAMEOVER();
        }
        else if (isdown)
        {
            height = 400 - record_height + 4 * count_downtime * count_downtime;
            if (height > 340)
                GAMEOVER();
        }
        else if (!isduck)
        {
            GAMEOVER();
        }
    }
}
void BIRD()
{
    int choice = count_time % 10;
    if (choice <= 4)
        BIRD_0();
    else
        BIRD_1();
}
void SCORE(char *score)
{

    surface = TTF_RenderText_Blended(font, score, color);
    SDL_Rect src_text = {800, 100, surface->w, surface->h};
    text = SDL_CreateTextureFromSurface(Renderer, surface);
    SDL_RenderCopy(Renderer, text, NULL, &src_text);
    SDL_DestroyTexture(text);
    SDL_FreeSurface(surface);
}
void ACCELERATE()
{
    int class = (count_time / 15) / 100;
    switch (class)
    {
    case 0:
        SHOWSPEED(0);
        FPS = 40;
        break;
    case 1:
        SHOWSPEED(1);
        FPS = 50;
        break;
    case 2:
        SHOWSPEED(2);
        FPS = 60;
        break;
    case 3:
        SHOWSPEED(3);
        FPS = 70;
        break;
    case 4:
        SHOWSPEED(4);
        FPS = 85;
        break;
    default:
        SHOWSPEED(4);
        break;
    }
}
void SHOWSPEED(int i)
{

    switch (i)
    {
    case 0:
        surface = TTF_RenderText_Blended(font, "SPEED : 1", color);
        break;
    case 1:
        surface = TTF_RenderText_Blended(font, "SPEED : 2", color);
        break;
    case 2:
        surface = TTF_RenderText_Blended(font, "SPEED : 3", color);
        break;
    case 3:
        surface = TTF_RenderText_Blended(font, "SPEED : 4", color);
        break;
    case 4:
        surface = TTF_RenderText_Blended(font, "SPEED : MAX", color);
        break;
    default:
        surface = TTF_RenderText_Blended(font, "SPEED : MAX", color);
        break;
    }
    SDL_Rect src_text = {800, 50, surface->w, surface->h};
    text = SDL_CreateTextureFromSurface(Renderer, surface);
    SDL_RenderCopy(Renderer, text, NULL, &src_text);
    SDL_DestroyTexture(text);
    SDL_FreeSurface(surface);
}
void RANDOMCREATE()
{
    int random;
    if (count_time % 50 == 0)
    {
        random = rand() % 4;
        switch (random)
        {
        case 0:
            birdfly = 1;
            break;
        case 1:
            cactus_1 = 1;
            break;
        case 2:
            cactus_2 = 1;
            break;
        case 3:
            cactus_3 = 1;
            break;
        default:
            break;
        }
    }
    if (birdfly)
        BIRD();
    if (cactus_1)
        CACTUS_1();
    if (cactus_2)
        CACTUS_2();
    if (cactus_3)
        CACTUS_3();
}
void STOP()
{

    isstop = 1;
    while (isstop)
    {
        SDL_PollEvent(&StopEvent);
        if (StopEvent.type == SDL_KEYDOWN)
            isstop = 0;
        SDL_Delay(20);
    }
}
void GAMEOVER()
{
    surface = TTF_RenderText_Blended(font, "GAME OVER", color);
    SDL_Rect src_wasted = {100, 100, surface->w * 2, surface->h * 2};
    text = SDL_CreateTextureFromSurface(Renderer, surface);
    SDL_RenderCopy(Renderer, text, NULL, &src_wasted);
    SDL_DestroyTexture(text);
    SDL_FreeSurface(surface);
    SDL_RenderPresent(Renderer);
    SDL_Delay(2000);
    SDL_RenderClear(Renderer);
    surface = TTF_RenderText_Blended(font, "YOUR FINAL SCORE IS", color);
    SDL_Rect src_final = {100, 200, surface->w * 2, surface->h * 2};
    text = SDL_CreateTextureFromSurface(Renderer, surface);
    SDL_RenderCopy(Renderer, text, NULL, &src_final);
    SDL_RenderPresent(Renderer);
    SDL_Delay(1000);
    char score[20];
    sprintf(score, " : %d", count_time / 15);
    surface = TTF_RenderText_Blended(font, score, color);
    SDL_Rect src_score = {700, 200, surface->w * 2, surface->h * 2};
    text = SDL_CreateTextureFromSurface(Renderer, surface);
    SDL_RenderCopy(Renderer, text, NULL, &src_score);
    SDL_RenderPresent(Renderer);
    SDL_Delay(1000);
    surface = TTF_RenderText_Blended(font, "press any key to restart", color);
    SDL_Rect src_restart = {150, 400, surface->w * 2, surface->h * 2};
    text = SDL_CreateTextureFromSurface(Renderer, surface);
    SDL_RenderCopy(Renderer, text, NULL, &src_restart);
    SDL_RenderPresent(Renderer);
    isstop = 1;
    while (isstop)
    {
        SDL_PollEvent(&StopEvent);
        if (StopEvent.type == SDL_KEYDOWN)
        {
            isstop = 0;
            count_time = 0;
            count_jumptime = 0;
            count_downtime = 0;
            count_bird = 0;
            count_cactus_1 = 0;
            count_cactus_2 = 0;
            count_cactus_3 = 0;
            downbug = 0;
            isdown = 0;
            isduck = 0;
            birdfly = 0;
            cactus_1 = 0;
            cactus_2 = 0;
            cactus_3 = 0;
            SDL_Delay(20);
        }
    }
}