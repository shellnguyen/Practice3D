#ifndef SHADER_H
#define SHADER_H

#include "../Common.h"

class Shader
{
private:
	GLuint LoadShader(GLuint type, const char* filePath);
	GLuint LoadProgram(GLuint vertexShaderId, GLuint fragmentShaderId);

	GLuint m_Program, m_VertexShaderId, m_FragmentShaderId;
public:
	Shader();
	~Shader();

	GLuint GetProgram() const;
	GLuint GetVertexShader() const;
	GLuint GetFragmentShader() const;

	GLuint Init(const char* vsPath, const char* fsPath);
};

#endif // !SHADER_H

