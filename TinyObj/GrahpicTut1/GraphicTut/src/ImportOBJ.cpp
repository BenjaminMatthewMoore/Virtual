#include "ImportOBJ.h"
#include <stb_image.h>


ImportOBJ::ImportOBJ()
{
	std::string vertexShader = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec4 Norm; \
							layout(location=2) in vec2 TexCoord;\
							out vec2 vTexCoord;\
							out vec4 vNorm;\
							uniform mat4 ProjectionView;\
							void main() { vTexCoord = TexCoord; vNorm = Norm;\
							gl_Position = ProjectionView *Position;}";

	std::string fragShader = "#version 410\n \
							in vec4 vNorm;\
							in vec2 vTexCoord;\
							out vec4 FragColor;\
							uniform vec3 DirectionalLightDir;\
							uniform vec3 DirectionalLightColour;\
							uniform sampler2D diffuse;\
							float d = max(0,dot(normalize(vNorm.xyz), DirectionalLightDir));\
							void main(){\
							vec2 uv = vec2(vTexCoord.x, -vTexCoord.y);\
							FragColor = texture(diffuse, uv)* vec4((DirectionalLightColour*d), 1);}";
	
	m_program.create(vertexShader, fragShader);

}


ImportOBJ::~ImportOBJ()
{
}

void ImportOBJ::createOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes)
{
	m_gl_info.resize(shapes.size());

	for (unsigned int mesh_idx = 0; mesh_idx < shapes.size(); ++mesh_idx)
	{
		glGenVertexArrays(1, &m_gl_info[mesh_idx].m_VAO);
		glGenBuffers(1, &m_gl_info[mesh_idx].m_VBO);
		glGenBuffers(1, &m_gl_info[mesh_idx].m_IBO);
		glBindVertexArray(m_gl_info[mesh_idx].m_VAO);

		unsigned int float_count = shapes[mesh_idx].mesh.positions.size();
		float_count += shapes[mesh_idx].mesh.normals.size();
		float_count += shapes[mesh_idx].mesh.texcoords.size();
		
		std::vector<float> vertex_data;
		vertex_data.reserve(float_count);
		vertex_data.insert(vertex_data.end(), shapes[mesh_idx].mesh.normals.begin(), shapes[mesh_idx].mesh.normals.end());
		vertex_data.insert(vertex_data.end(), shapes[mesh_idx].mesh.texcoords.begin(), shapes[mesh_idx].mesh.texcoords.end());
		vertex_data.insert(vertex_data.end(), shapes[mesh_idx].mesh.positions.begin(), shapes[mesh_idx].mesh.positions.end());

		m_gl_info[mesh_idx].m_index_count = shapes[mesh_idx].mesh.indices.size();

		int positionByteSize = (sizeof(float)*shapes[mesh_idx].mesh.positions.size());
		int normalByteSize = (sizeof(float)*shapes[mesh_idx].mesh.normals.size());
		int texCoordByteSize = (sizeof(float)*shapes[mesh_idx].mesh.texcoords.size());

		glBindBuffer(GL_ARRAY_BUFFER, m_gl_info[mesh_idx].m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), vertex_data.data(), GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_info[mesh_idx].m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[mesh_idx].mesh.indices.size()*sizeof(unsigned int), shapes[mesh_idx].mesh.indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);//position
		glEnableVertexAttribArray(1);//normal data
		glEnableVertexAttribArray(2); //TextureCoordinates
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(normalByteSize+texCoordByteSize));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)normalByteSize);
	
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	


}

void ImportOBJ::SetProgram(mat4 CameraProjectionView, vec3 LightDirection, vec3 LightColour)
{
	glUseProgram(m_program.m_programID);
	unsigned int projectionViewUniform = glGetUniformLocation(m_program.m_programID, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(CameraProjectionView));

	unsigned int DirectionLightDirUniform = glGetUniformLocation(m_program.m_programID, "DirectionalLightDir");
	glUniform3fv(DirectionLightDirUniform, 1, glm::value_ptr(LightDirection));

	unsigned int DirectionalLightColourUniform = glGetUniformLocation(m_program.m_programID, "DirectionalLightColour");
	glUniform3fv(DirectionalLightColourUniform, 1, glm::value_ptr(LightColour));
}

void ImportOBJ::LoadTexture(std::string filePath)
{
	int imageWidth = 0;
	int imageHeight = 0;
	int imageFormat = 0;
	unsigned char* data = stbi_load(filePath.c_str(), &imageWidth, &imageHeight, &imageFormat, STBI_default);
	assert(data != nullptr);
	//printOpenGLError();

	glGenTextures(1, &spearTexture);
	glBindTexture(GL_TEXTURE_2D, spearTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);


}

void ImportOBJ::LoadObject(std::string filePathObj)
{
	std::string err;
	bool successful = tinyobj::LoadObj(shapes, materials, err, filePathObj.c_str());
	createOpenGLBuffers(shapes);
}

void ImportOBJ::LoadObject(std::string filePathObj, std::string filePathTex)
{
	std::string err;
	bool successful = tinyobj::LoadObj(shapes, materials, err, filePathObj.c_str());
	LoadTexture(filePathTex);
	createOpenGLBuffers(shapes);
}

void ImportOBJ::Render(Camera* camera, vec3 lightsDirection, vec3 lightColour)
{
	//texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, spearTexture);

	glUseProgram(m_program.m_programID);
	unsigned int diffuseLocation = glGetUniformLocation(m_program.m_programID, "diffuse");
	assert(diffuseLocation != -1);
	glUniform1i(diffuseLocation,0);
	SetProgram(camera->GetProjectionView(), lightsDirection, lightColour);
	return;

	//draw
	for (unsigned int i = 0; i < m_gl_info.size(); ++i)
	{
		glBindVertexArray(m_gl_info[i].m_VAO);
		glDrawElements(GL_TRIANGLES, m_gl_info[i].m_index_count, GL_UNSIGNED_INT, 0);
	};


}