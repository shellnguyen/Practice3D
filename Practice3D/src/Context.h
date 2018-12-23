#ifndef CONTEXT_H
#define CONTEXT_H

#include "GL\glew.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

//typedef SDL_Texture Texture;
typedef SDL_Rect Rect;
typedef SDL_Color Color;
typedef SDL_Point Point;
typedef SDL_BlendMode BlendMode;
typedef TTF_Font Font;
typedef Mix_Chunk Sound;
typedef Mix_Music Music;
typedef SDL_DisplayMode DisplayMode;

bool InitContext(int width, int height);
bool InitGLES(int width, int height);
void SetFPS(int fps);
void GameLoop();

void RegisterUpdateCallback(void(*fnUpdate)(float dt));
void RegisterRenderCallback(void(*fnRender)());
void RegisterKeyDownCallback(void(*fnKeyDown)(int key));
void RegisterKeyUpCallback(void(*fnKeyUp)(int key));
void RegisterMouseDownCallback(void(*fnMouseDown)(int key, int xPos, int yPos));
void RegisterMouseMotionCallback(void(*fnMouseMotion)(int xPos, int yPos));
void RegisterMouseWheelCallback(void(*fnMouseWheel)(int xOffset, int yOffset));

//bool LoadTexture(const char* file, Texture*& tex, int& width, int& height);
//void DestroyTexture(Texture* tex);
//void RenderTexture(Texture* tex, Rect* src, Rect* dst);
//void RotateTexture(Texture* tex, Rect* src, Rect* dst, double angle);
void UpdateRenderer();

void CloseFont(Font* font);
bool GetDisplayMode(int& w, int& h);

void Terminate();

bool IsRectOverlap(const Rect* r1, const Rect* r2);
void SwapWindow();


#endif // !CONTEXT_H

