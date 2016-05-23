#pragma once
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glm/gtx/transform.hpp"
#include "tiny_obj_loader.h"
#include "Program.h"
#include "Gizmos.h"
#include <iostream>
#include <cstdio>

struct OpenGLInfo
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_index_count;
};

class ImportOBJ
{
public:
	ImportOBJ();
	~ImportOBJ();;
	void Render(Camera* camera, vec3 lightsDirection, vec3 lightColour);
	Program m_program;
	unsigned int spearTexture;
	void LoadObject(std::string filePathObj, std::string filePathTex);
	void LoadObject(std::string filePath);

private:
	void LoadTexture(std::string filePath);
	void SetProgram(mat4 CameraProjectionView, vec3 LightDirection, vec3 LightColour);
	void createOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes);
	std::vector<OpenGLInfo> m_gl_info;
	

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
};

