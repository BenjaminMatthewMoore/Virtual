#pragma once
#include "Program.h"
#include "Camera.h"
#include "TerrainGen.h"
// all rendered objects inherit from project
class RenderTargets
{
public:
	RenderTargets();
	~RenderTargets();

	void genBuffer();
	void genTex();
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_fbo;
	unsigned int m_fboTexture;
	unsigned int m_fboDepth;
	unsigned int m_IBO;
	Program m_program;
	void Draw(mat4 camera, TerrainGen terrain);
private:



};

