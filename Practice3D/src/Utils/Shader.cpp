#include "Shader.h"
#include <stdio.h>

Shader::Shader()
{
}

Shader::~Shader()
{
	glDeleteProgram(m_Program);
	glDeleteShader(m_VertexShaderId);
	glDeleteShader(m_FragmentShaderId);
}

GLuint Shader::LoadShader(GLuint type, const char* filePath)
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader(type);

	if (shader == 0)
		return 0;

	// Load the shader source
	FILE * pf;
	if (fopen_s(&pf, filePath, "rb") != 0)
		return NULL;
	fseek(pf, 0, SEEK_END);
	long size = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	char * shaderSrc = new char[size + 1];
	fread(shaderSrc, sizeof(char), size, pf);
	shaderSrc[size] = 0;
	fclose(pf);

	glShaderSource(shader, 1, (const char **)&shaderSrc, NULL);
	delete[] shaderSrc;

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[infoLen];


			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			printf("Error compiling shader:\n%s\n", infoLog);
			//Debug("Error compiling shader:\n%s\n", infoLog);

			delete[] infoLog;
		}

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

GLuint Shader::LoadProgram(GLuint vertexShaderId, GLuint fragmentShaderId)
{
	GLuint programObject;
	GLint linked;

	// Create the program object
	programObject = glCreateProgram();

	if (programObject == 0)
		return 0;

	glAttachShader(programObject, vertexShaderId);
	glAttachShader(programObject, fragmentShaderId);

	// Link the program
	glLinkProgram(programObject);

	// Check the link status
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLint infoLen = 0;

		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[sizeof(char) * infoLen];

			glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
			printf("Error linking program:\n%s\n", infoLog);
			//Debug("Error linking program:\n%s\n", infoLog);

			delete infoLog;
		}

		glDeleteProgram(programObject);
		return 0;
	}

	return programObject;
}

GLuint Shader::Init(const char* vsPath, const char* fsPath)
{
	m_VertexShaderId = LoadShader(GL_VERTEX_SHADER, vsPath);

	if (m_VertexShaderId == 0)
	{
		return -1;
	}

	m_FragmentShaderId = LoadShader(GL_FRAGMENT_SHADER, fsPath);

	if (m_FragmentShaderId == 0)
	{
		return -2;
	}

	m_Program = LoadProgram(m_VertexShaderId, m_FragmentShaderId);

	//finding location of uniforms / attributes
	//positionAttribute = glGetAttribLocation(program, "a_posL");
	m_MvpId = glGetUniformLocation(m_Program, "uMvp");

	return 0;
}

GLuint Shader::GetProgram() const
{
	return m_Program;
}

GLuint Shader::GetVertexShader() const
{
	return m_VertexShaderId;
}

GLuint Shader::GetFragmentShader() const
{
	return m_FragmentShaderId;
}

GLuint Shader::GetMvpMatrix() const
{
	return m_MvpId;
}