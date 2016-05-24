
#pragma once
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glm/gtx/transform.hpp"
#include "Program.h"
struct Vertex;
class Grid
{
public:
	Grid();
	~Grid();

	void Draw();
	void SetProgram(mat4 CameraProjectionView, vec3 LightDirection, vec3 LightColour);
	void GenerateGrid(unsigned int row, unsigned int cols);
	void UploadData();
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	int m_row;
	int m_col;
	Program m_program;
	void startUp(unsigned int row, unsigned int cols);
private:
	int m_vertCount;
	int m_indexCount;
	unsigned int* m_auiIndices;
	Vertex* m_aoVertices;
};

