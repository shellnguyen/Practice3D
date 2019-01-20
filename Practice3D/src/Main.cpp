#include <stdio.h>
#include "Context.h"
#include "Common.h"
#include "Utils\Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

//
bool LoadTexture(const char* file);
//

float vertices[] = {
	0.5f,  0.5f, 0.0f,  // top right
	0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
};

float vertices2[] = {
	// position          // texture coords
	0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
	0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
};

unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

unsigned int VBO, VAO, EBO;
unsigned int texture;
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

	//if (myShader->Init("data/shaders/TextureVS.glsl", "data/shaders/TextureFS.glsl") != 0)
	//{
	//	SDL_Log("Failed to load shader !!!");
	//	return false;
	//}

	if (myShader->Init("data/shaders/BasicTransformVS.glsl", "data/shaders/BasicTransformFS.glsl") != 0)
	{
		SDL_Log("Failed to load shader !!!");
		return false;
	}
	LoadTexture("data/textures/container.jpg");
	CreateBuffers();
	
	return true;
}

void OnUpdate(float dt)
{
	//translation_matrix = glm::translate(translation_matrix, glm::vec3(0.5f, -0.5f, 0.0f));
	translation_matrix = glm::rotate(translation_matrix, dt * glm::radians(2.5f), glm::vec3(0.0f, 0.0f, 1.0f));

	model_matrix = translation_matrix;
}

void OnRender()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// draw our first triangle
	glUseProgram(myShader->GetProgram());
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniformMatrix4fv(myShader->GetMvpMatrix(), 1, GL_FALSE, glm::value_ptr(model_matrix));
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

bool LoadTexture(const char* file)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture !");
		return false;
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
	return true;
}