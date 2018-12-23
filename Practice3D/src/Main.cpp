#include <stdio.h>
#include "Context.h"
#include "Common.h"

bool Init();
void OnRender();
void OnUpdate(float dt);
void OnKeyDown(int keyCode);
void OnKeyUp(int keyCode);
void OnMouseDown(int key, int xPos, int yPos);
void OnMouseMotion(int xPos, int yPos);
void OnMouseWheel(int xOffet, int yOffset);
void OnExit();


int main(int argc, char *argv[])
{
	srand(NULL);
	if (GetDisplayMode(screen_width, screen_height))
	{
		InitContext(screen_width, screen_height);
	}
	else
	{
		InitContext(DEF_WIDTH, DEF_HEIGHT);
	}
	SetFPS(DEF_FPS);
	RegisterUpdateCallback(OnUpdate);
	RegisterRenderCallback(OnRender);
	RegisterKeyDownCallback(OnKeyDown);
	RegisterKeyUpCallback(OnKeyUp);
	RegisterMouseDownCallback(OnMouseDown);
	RegisterMouseMotionCallback(OnMouseMotion);
	RegisterMouseWheelCallback(OnMouseWheel);

	if (Init() != 1)
	{
		SDL_Log("Init failed !!!");
	}
	else
	{
		GameLoop();
	}

	OnExit();

	return 0;
}

bool Init()
{
	return true;
}

void OnUpdate(float dt)
{
}

void OnRender()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void OnKeyDown(int key)
{
	switch (key)
	{
	case SDLK_w:
	{
		printf("Press W\n");
		break;
	}
	case SDLK_s:
	{
		printf("Press S\n");
		break;
	}
	case SDLK_a:
	{
		printf("Press A\n");
		break;
	}
	case SDLK_d:
	{
		printf("Press D\n");
		break;
	}
	}
}

void OnKeyUp(int key)
{

}

void OnMouseDown(int button, int mouseX, int mouseY)
{
	switch (button)
	{
	case SDL_BUTTON_LEFT:
	{
		break;
	}
	case SDL_BUTTON_RIGHT:
		break;
	case SDL_BUTTON_MIDDLE:
		break;
	}
}

void OnMouseMotion(int xPos, int yPos)
{

	//camera.OnMouseMotion(xoffset, yoffset);
}

void OnMouseWheel(int xOffset, int yOffset)
{
}

void OnExit()
{
	Terminate();
}