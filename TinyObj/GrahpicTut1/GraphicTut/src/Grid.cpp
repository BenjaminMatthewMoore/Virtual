#include "Grid.h"

struct Vertex {
	glm::vec4 position;
	glm::vec4 colour;
	glm::vec2 uv;
};

Grid::Grid()
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
							vec2 uv = vec2(vTexCoord.x, vTexCoord.y);\
							FragColor = texture(diffuse, uv).rrrr* vec4((DirectionalLightColour*d), 1);}";

	m_program.create(vertexShader, fragShader);
	
}


Grid::~Grid()
{
}

void Grid::GenerateGrid(unsigned int row, unsigned int cols)
{
	m_vertCount = row * cols;
	m_aoVertices = new Vertex[m_vertCount];
	for (unsigned int r = 0; r < row; r++)
	{
		for (unsigned int c = 0; c < cols; c++)
		{
			m_aoVertices[r*cols + c].position = glm::vec4((float)c, 0, (float)r, 1);

			glm::vec3 colour = glm::vec3(sinf((c / (float)(cols - 1)) * (r /
				(float)(row - 1))));
			m_aoVertices[r*cols + c].colour = glm::vec4(colour, 1);
			float rowNorm = r / (float)row;
			float colNorm = c / (float)cols;

			m_aoVertices[r*cols + c].uv = glm::vec2(rowNorm, colNorm);
		}

	}
	//index count
	m_indexCount = (row - 1) * (cols - 1) * 6;
	m_auiIndices = new unsigned int[m_indexCount];

	unsigned int index = 0;
	for (unsigned int r = 0; r < (row - 1); r++)
	{
		for (unsigned int c = 0; c < (cols - 1); c++)
		{
			//tri1
			m_auiIndices[index++] = r* cols + c;
			m_auiIndices[index++] = (r + 1)*cols + c;
			m_auiIndices[index++] = (r + 1)*cols + (c + 1);

			//tri2
			m_auiIndices[index++] = r*cols + c;
			m_auiIndices[index++] = (r + 1)*cols + (c + 1);
			m_auiIndices[index++] = r*cols + (c + 1);
		}
	}
}

void Grid::UploadData()
{

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertCount*sizeof(Vertex), m_aoVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(unsigned int), m_auiIndices, GL_STATIC_DRAW);
	glBindVertexArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	delete[] m_aoVertices;
}

void Grid::SetProgram(mat4 CameraProjectionView, vec3 LightDirection, vec3 LightColour)
{
	glUseProgram(m_program.m_programID);
	unsigned int projectionViewUniform = glGetUniformLocation(m_program.m_programID, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(CameraProjectionView));

	unsigned int DirectionLightDirUniform = glGetUniformLocation(m_program.m_programID, "DirectionalLightDir");
	glUniform3fv(DirectionLightDirUniform, 1, glm::value_ptr(LightDirection));

	unsigned int DirectionalLightColourUniform = glGetUniformLocation(m_program.m_programID, "DirectionalLightColour");
	glUniform3fv(DirectionalLightColourUniform, 1, glm::value_ptr(LightColour));
}

void Grid::Draw()
{
	glBindVertexArray(m_VAO);
	unsigned int indexCount = (m_row - 1)*(m_col - 1) * 6;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}
void Grid::startUp(unsigned int row, unsigned int cols)
{
	this->GenerateGrid(row, cols);
	this->UploadData();
}