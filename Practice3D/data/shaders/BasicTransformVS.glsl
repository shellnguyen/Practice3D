#version 400 core
in vec3 iPosition;
in vec2 iTexCoord;

out vec2 texCoord;

uniform mat4 uMvp;

void main()
{
	texCoord = iTexCoord;
	gl_Position = uMvp * vec4(iPosition.x, iPosition.y, iPosition.z, 1.0);
}