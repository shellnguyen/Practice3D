#include "Context.h"

SDL_Window*		gWindow = NULL;
SDL_Renderer*	gRenderer = NULL;
Uint32			gFPS = 0;
Uint32			gFrameTime = 0;
Uint32			gDeltaTime = 0;
SDL_GLContext	gContext;

void(*gFnUpdate)(float) = NULL;
void(*gFnRender)() = NULL;
void(*gFnKeyDown)(int) = NULL;
void(*gFnKeyUp)(int) = NULL;
void(*gFnMouseDown)(int, int, int) = NULL;
void(*gFnMouseMotion)(int, int) = NULL;
void(*gFnMouseWheel)(int, int) = NULL;

bool InitContext(int width, int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_Log("SDL Init fail: %d", SDL_GetError());
		return false;
	}

	gWindow = SDL_CreateWindow("SDLES", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	if (gWindow == NULL)
	{
		SDL_Log("SDL Window Init fail: %d", SDL_GetError());
		return false;
	}

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL)
	{
		SDL_Log("SDL Renderer Init fail: %d", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	gContext = SDL_GL_CreateContext(gWindow);
	if (gContext == NULL)
	{
		SDL_Log("OpengGLES context failed: %d", SDL_GetError());
		return false;
	}

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		SDL_Log("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		return false;
	}

	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		SDL_Log("Warning: Unable to set Vsync ! Error %d", SDL_GetError());
	}

	if (!InitGLES(width, height))
	{
		SDL_Log("OpenGLES init failed !");
		return false;
	}

	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if ((initted & flags) != flags)
	{
		SDL_Log("IMG_Init: %s\n", IMG_GetError());
		return false;
	}

	if (TTF_Init() == -1)
	{
		SDL_Log("TTF_Init: %s\n", TTF_GetError());
		return false;
	}

	return true;
}

bool InitGLES(int width, int height)
{
	glViewport(0, 0, width, height);

	return true;
}

void SetFPS(int fps)
{
	gFPS = fps;
	gFrameTime = 1000 / gFPS;
	gDeltaTime = gFrameTime;
}

void GameLoop()
{
	bool isRunning = true;
	SDL_Event e;

	while (isRunning)
	{
		Uint32 startTime = SDL_GetTicks();

		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				gFnKeyDown(e.key.keysym.sym);
				break;
			case SDL_KEYUP:
				gFnKeyUp(e.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				gFnMouseDown(e.button.button, e.motion.x, e.motion.y);
				break;
			case SDL_MOUSEMOTION:
				gFnMouseMotion(e.motion.x, e.motion.y);
				break;
			case SDL_MOUSEWHEEL:
				gFnMouseWheel(e.wheel.x, e.wheel.y);
				break;
			}
		}

		if (gFnUpdate != NULL)
		{
			gFnUpdate(gDeltaTime / 1000.0f);
		}


		if (gFnRender != NULL)
		{
			//SDL_RenderClear(gRenderer);
			gFnRender();
			//SDL_RenderPresent(gRenderer);
			SDL_GL_SwapWindow(gWindow);
		}

		gDeltaTime = SDL_GetTicks() - startTime;
		int sleepTime = gFrameTime - gDeltaTime;

		if (sleepTime > 0)
		{
			SDL_Delay(sleepTime);
			gDeltaTime += sleepTime;
		}
	}
}

void RegisterUpdateCallback(void(*fnUpdate)(float dt))
{
	gFnUpdate = fnUpdate;
}

void RegisterRenderCallback(void(*fnRender)())
{
	gFnRender = fnRender;
}

void RegisterKeyDownCallback(void(*fnKeyDown)(int key))
{
	gFnKeyDown = fnKeyDown;
}

void RegisterKeyUpCallback(void(*fnKeyUp)(int key))
{
	gFnKeyUp = fnKeyUp;
}

void RegisterMouseDownCallback(void(*fnMouseDown)(int key, int xPos, int yPos))
{
	gFnMouseDown = fnMouseDown;
}

void RegisterMouseMotionCallback(void(*fnMouseMotion)(int xPos, int yPos))
{
	gFnMouseMotion = fnMouseMotion;
}

void RegisterMouseWheelCallback(void(*fnMouseWheel)(int xOffset, int yOffset))
{
	gFnMouseWheel = fnMouseWheel;
}

//bool LoadTexture(const char* file, Texture*& tex, int& width, int& height)
//{
//	tex = IMG_LoadTexture(gRenderer, file);
//	if (tex == NULL)
//	{
//		SDL_Log("Texture load failed - File: %s", file);
//		return false;
//	}
//
//	SDL_QueryTexture(tex, NULL, NULL, &width, &height);
//	return true;
//}
//
//void DestroyTexture(Texture* tex)
//{
//	SDL_DestroyTexture(tex);
//}
//
//void RenderTexture(Texture* tex, Rect* src, Rect* des)
//{
//	SDL_RenderCopy(gRenderer, tex, src, des);
//}
//
//void RotateTexture(Texture* tex, Rect* src, Rect* des, double angle)
//{
//	SDL_RenderCopyEx(gRenderer, tex, src, des, angle, NULL, SDL_FLIP_HORIZONTAL);
//}

void Terminate()
{
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

bool IsRectOverlap(const Rect* r1, const Rect* r2)
{
	return r1->x < (r2->x + r2->w) && (r1->x + r1->w) > r2->x &&
		r1->y < (r2->y + r2->h) && (r1->y + r1->h) > r2->y;
}

void UpdateRenderer()
{
	SDL_RenderPresent(gRenderer);
}

void CloseFont(Font* font)
{
	TTF_CloseFont(font);
	//font = NULL;
}

bool GetDisplayMode(int &w, int &h)
{
	SDL_DisplayMode dm;

	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
		return false;
	}

	w = dm.w;
	h = dm.h;

	return true;
}

void SwapWindow()
{
	SDL_GL_SwapWindow(gWindow);
}