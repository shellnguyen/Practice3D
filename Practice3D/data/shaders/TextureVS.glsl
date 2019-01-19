#version 400 core
in vec3 iPosition;
in vec2 iTexCoord;

out vec2 texCoord;

void main()
{
	texCoord = iTexCoord;
	gl_Position = vec4(iPosition.x, iPosition.y, iPosition.z, 1.0);
}