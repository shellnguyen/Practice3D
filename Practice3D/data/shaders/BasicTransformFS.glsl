#version 400 core
out vec4 color;

in vec2 texCoord;

uniform sampler2D uTexture;

void main()
{
	color = texture(uTexture, texCoord);
}