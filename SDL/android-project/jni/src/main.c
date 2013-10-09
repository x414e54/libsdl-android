#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <android/log.h>
#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#include "SDL.h"

typedef struct Sprite
{
    SDL_Texture* texture;
    Uint16 w;
    Uint16 h;
} Sprite;

/* Adapted from SDL's testspriteminimal.c */
Sprite LoadSprite(const char* file, SDL_Renderer* renderer)
{
    Sprite result;
    result.texture = NULL;
    result.w = 0;
    result.h = 0;
    
    SDL_Surface* temp;

    /* Load the sprite image */
    temp = SDL_LoadBMP(file);
    if (temp == NULL)
    {
        fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
        return result;
    }
    result.w = temp->w;
    result.h = temp->h;

    /* Create texture from the image */
    result.texture = SDL_CreateTextureFromSurface(renderer, temp);
    if (!result.texture) {
        fprintf(stderr, "Couldn't create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(temp);
        return result;
    }
    SDL_FreeSurface(temp);

    return result;
}

void draw(SDL_Window *window, SDL_Renderer* renderer, const Sprite sprite)
{
    int w, h;

    SDL_GetWindowSize(window, &w, &h);
    SDL_Rect destRect = {w/2 - sprite.w/2, h/2 - sprite.h/2, sprite.w, sprite.h};
    /* Blit the sprite onto the screen */
    SDL_RenderCopy(renderer, sprite.texture, NULL, &destRect);
}

int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    if(SDL_CreateWindowAndRenderer(0, 0, 0, &window, &renderer) < 0)
        exit(2);

    Sprite sprite = LoadSprite("sample.bmp", renderer);
    if(sprite.texture == NULL)
        exit(2);

    /* Main render loop */
    Uint8 done = 0;
    SDL_Event event;
    while(!done)
    {
        /* Wait for events */
        SDL_WaitEvent(&event);
        do {
            switch (event.type) {
            case SDL_QUIT:
            case SDL_KEYDOWN:
                done = 1;
                break;
            case SDL_WINDOWEVENT:
                if (SDL_GetWindowID(window) == event.window.windowID) {
                    if(event.window.event ==  SDL_WINDOWEVENT_RESIZED) {
                        /* SDL_WINDOWEVENT_RESIZED will trigger SDL_WINDOWEVENT_SIZE_CHANGED,
                        so ignore it in order to avoid duplicate redraws */
                        continue;
                    } else if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
#ifdef ANDROID /* SDL/Android bug https://bugzilla.libsdl.org/show_bug.cgi?id=1849 */
                        SDL_GL_SwapWindow(window);
#endif
                    }
                }
                break;
            }
        } while (!done && SDL_PollEvent(&event));
        if (done) break;
        
        /* Draw a gray background */
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
        
        draw(window, renderer, sprite);
    
        /* Update the screen! */
        SDL_RenderPresent(renderer);
    }

    exit(0);
}
