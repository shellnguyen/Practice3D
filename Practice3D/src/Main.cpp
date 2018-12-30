#include <stdio.h>
#include "Context.h"
#include "Common.h"
#include "Utils\Shader.h"

bool Init();
void OnRender();
void OnUpdate(float dt);
void OnKeyDown(int keyCode);
void OnKeyUp(int keyCode);
void OnMouseDown(int key, int xPos, int yPos);
void OnMouseMotion(int xPos, int yPos);
void OnMouseWheel(int xOffet, int yOffset);
void OnExit();

//
void CreateBuffers();
//

float vertices[] = {
	0.5f,  0.5f, 0.0f,  // top right
	0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

unsigned int VBO, VAO, EBO;
Shader* myShader;

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
	myShader = new Shader();

	if (myShader->Init("data/shaders/BasicVS.glsl", "data/shaders/BasicFS.glsl") != 0)
	{
		SDL_Log("Failed to load shader !!!");
		return false;
	}

	CreateBuffers();
	
	return true;
}

void OnUpdate(float dt)
{
}

void OnRender()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// draw our first triangle
	glUseProgram(myShader->GetProgram());
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
							//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

void CreateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}