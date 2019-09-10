#include "draw.h"
#include <iostream>
#include <queue>
#include <stack>
#include<utility> 

SDL_Surface* surface;

typedef Uint8* Pixel_t;

uint8_t ColorToPixel(SDL_Color& c)
{
        return (uint8_t)SDL_MapRGB(surface->format, c.r, c.g, c.b);
}

bool InBounds(SDL_Point point, Uint32 w, Uint32 h)
{
        int x = point.x, 
            y = point.y;
        return y > 0 && x < (w - 1) && y < (h - 1) && x > 0;
}

bool InBounds2(int x, int y, Uint32 w, Uint32 h)
{
        return y > 0 && x < (w - 1) && y < (h - 1) && x > 0;
}

bool AreSame(Pixel_t src, SDL_Color& dst_color)
{
        return     *(src + 0) == (dst_color.r)
                && *(src + 1) == (dst_color.g)
                && *(src + 2) == (dst_color.b);
}

void SetPixelColor(Pixel_t px, SDL_Color& dst_color)
{
        *(px + 0) = (dst_color.r);
        *(px + 1) = (dst_color.g);
        *(px + 2) = (dst_color.b);
}

int GetPixelIndex(SDL_Point point, SDL_Surface* surface)
{
        int x = point.x,
            y = point.y;
        return surface->format->BytesPerPixel * (y * surface->w + x);
}

void fill(SDL_Surface* surface, int startx, int starty, Uint32 w, Uint32 h, SDL_Color& dst_color)
{
        std::pair<int, int> xy;
        int x, y, pitch;
        std::stack<SDL_Point> stack;
        SDL_Point next_point;
        Uint8* first_px;
        Pixel_t pixels, nor_px, eas_px, sou_px, wes_px;

        unsigned char* next_px;
        unsigned char new_px;

        pitch = surface->pitch;
        pixels = (Pixel_t) surface->pixels;
        stack.push({ startx, starty });
        new_px = ColorToPixel(dst_color);

        while (!stack.empty())
        {
                next_point = stack.top();
                stack.pop();

                if (!InBounds(next_point, w, h))
                        continue;

                x = next_point.x;
                y = next_point.y;

                SetPixelColor(&pixels[GetPixelIndex({ x, y }, surface)], dst_color);
                nor_px = &pixels[GetPixelIndex({ x, y - 1 }, surface)];
                eas_px = &pixels[GetPixelIndex({ x + 1, y }, surface)];
                sou_px = &pixels[GetPixelIndex({ x, y + 1 }, surface)];
                wes_px = &pixels[GetPixelIndex({ x - 1, y }, surface)];

                if (!AreSame(nor_px, dst_color)) stack.push({ x, y - 1 });
                if (!AreSame(eas_px, dst_color)) stack.push({ x + 1, y });
                if (!AreSame(sou_px, dst_color)) stack.push({ x, y + 1 });
                if (!AreSame(wes_px, dst_color)) stack.push({ x - 1, y });
                

        }

        std::cout << " DONE!!!" << std::endl;
}

int main(int argc, char** argv)
{
        int sw = 1000;
        int sh = 1000;

        DrawInit(sw, sh);
        surface = SDL_GetWindowSurface(gWindow);

        int startx = sw / 2;
        int starty = sh / 2;
        int i = 0;

        SDL_Color c = { 0xFF, 0x00, 0xAA };
        fill(surface, startx, starty, sw, sh, c);

        SDL_UpdateWindowSurface(gWindow);

        SDL_Delay(5000);
        SDL_DestroyWindow(gWindow);
        SDL_Quit();
        return 0;
}
