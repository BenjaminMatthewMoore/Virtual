#include "Program.h"

#include <iostream>

bool CheckCompileStatus(GLuint shaderId)
{
	GLint result = GL_FALSE;
	int logLength = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE) {
		char* logBuffer = NULL;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
		logBuffer = new char[logLength];
		glGetShaderInfoLog(shaderId, logLength, NULL, logBuffer);
		std::cerr << "Compile Error: " << logBuffer << std::endl;
		delete[] logBuffer;
		return false;
	}
	return true;
}

bool CheckLinkStatus(GLuint programId)
{
	GLint result = GL_FALSE;
	int logLength = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	if (result != GL_TRUE) {
		char* logBuffer = NULL;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
		logBuffer = new char[logLength];
		glGetProgramInfoLog(programId, logLength, NULL, logBuffer);
		std::cerr << "Link Error: " << logBuffer << std::endl;
		delete[] logBuffer;
		return false;
	}
	return true;
}


Program::Program()
{
}


Program::~Program()
{
}

//void Program::LoadShader(char* vertexShdaer, char* fragShader)
//{
//	//TODO:: Load shaders in from file
//	//For now shaders manually made in constructors of objects
//}


bool Program::create(std::string vertShader, std::string fragShader)
{
	// compile, link, etc
	const char* vsSource = vertShader.c_str();
	const char* fsSource = fragShader.c_str();
	int success = GL_FALSE;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	if (!CheckCompileStatus(vertexShader)) return false;

	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	if (!CheckCompileStatus(fragmentShader)) return false;

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);
	if (!CheckLinkStatus(m_programID)) {
		m_programID = -1;
		return false;
	}

	return true;
}
