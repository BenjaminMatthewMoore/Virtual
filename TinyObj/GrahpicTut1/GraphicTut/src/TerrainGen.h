#pragma once
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glm/gtx/transform.hpp"
#include "Grid.h"
#include "Program.h"

#include <assert.h>

struct Texture {
	unsigned int text;
	int imageWidth;
	int imageHeight;
	int imageFormat;
	unsigned char* data;
};

class TerrainGen
{
public:
	TerrainGen();
	~TerrainGen();

	int m_dimensions;
	float *m_perlinData;
	void Draw(mat4 cameraView);
	void GenTerrain(int dimensions);
	void FlattenTerrain();
	void WaterRiseFall(float deltaTime);
	void MountainTest();

	int GetIndex(int x, int y)
	{
		int index = y * m_dimensions + x;
		assert(index < m_dimensions*m_dimensions);
		assert(index >= 0);
		return index;
	}

	//returns highest Vertex
	float& GetHeight(int x, int y)
	{
		return m_perlinData[GetIndex(x, y)];
	}

	//sets the height of a vertex
	void SetHeight(int x, int y, float val)
	{
		m_perlinData[GetIndex(x, y)] = val;
	}

	//returns the coordinate x / y value of vertex in buffer
	glm::ivec2 GetCoords(int index)
	{
		glm::ivec2 result;

		result.x = (index % m_dimensions);
		result.y = (index - result.x) / m_dimensions;

		return result;
	}

private:
	
	
	float scale;
	unsigned int m_perlinTexture;
	Texture	m_waterTexture;
	Texture m_snowTexture;
	Texture m_grassTexture;
	void loadTextures(Texture &tex);
	float waterTimer;


	Program m_program;
	Grid m_grid;
};

