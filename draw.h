#pragma once
#include <SDL.h>
#include <iostream>

extern SDL_Renderer* gRenderer;

constexpr int k_PixelSize = 1;

SDL_Window* gWindow;
SDL_Renderer* gRenderer;

bool DrawInit(int sw, int sh)
{
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
                return false;

        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
                printf("Warning: Linear texture filtering not enabled!");

        std::cout << "Initializing window size " << sw << "x" << sh << "px...";
        gWindow = SDL_CreateWindow("Fill v0.0.1", SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED, sw * k_PixelSize, sh * k_PixelSize, SDL_WINDOW_SHOWN);

        if (gWindow == NULL)
                return false;
        std::cout << "Done. " << std::endl;

        gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

        if (gRenderer == NULL)
                return false;

        return true;
}

void SetDrawColor(const SDL_Color& c) { SDL_SetRenderDrawColor(gRenderer, c.r, c.g, c.b, c.a); }
void DrawRect(int x, int y, int w, int h, const SDL_Color& c)
{
        SetDrawColor(c);
        SDL_Rect r = { x * k_PixelSize, y * k_PixelSize, w * k_PixelSize, h * k_PixelSize };
        SDL_RenderFillRect(gRenderer, &r);
}
void FillRect(const SDL_Rect& rect) { SDL_RenderFillRect(gRenderer, &rect); }
void ClearScreen() { SDL_RenderClear(gRenderer); }
void DrawScreen() { SDL_RenderPresent(gRenderer); }

void SetPixel(uint8_t* src_pixel, uint32_t pixel)
{
        *src_pixel = pixel;
}

void SetPixel(SDL_Surface* surface, SDL_Point p, uint8_t pixel)
{
        uint8_t* target_pixel = (uint8_t*) surface->pixels 
                + (uint32_t)(p.y) * (uint32_t)(surface->pitch)
                + (uint32_t)(p.x) * 4L;

        SetPixel(target_pixel, pixel);
}

uint8_t* GetPixel(SDL_Surface* surface, SDL_Point p) 
{
        return (uint8_t*) surface->pixels 
                + p.y * surface->pitch 
                + p.x * surface->format->BytesPerPixel;
}