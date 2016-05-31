#include "TerrainGen.h"
#include <stb_image.h>


TerrainGen::TerrainGen()
{
	std::string vertexShader = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=2) in vec2 TexCoord;\
							out vec2 vTexCoord;\
							uniform sampler2D perlinTexture;\
							uniform mat4 ProjectionView;\
							out float height;\
							void main() { \
							vec4 pos = Position;\
							height =  texture(perlinTexture, TexCoord).r;\
							pos.y += height * 25;\
							gl_Position = ProjectionView *pos;\
							vTexCoord = TexCoord;}";

	std::string fragShader = "#version 410\n \
							in vec4 vNorm;\
							in vec2 vTexCoord;\
							in float height;\
							out vec4 FragColor;\
							uniform sampler2D diffuse;\
							uniform sampler2D Grass;\
							uniform sampler2D Water;\
							uniform sampler2D Snow;\
							vec4 grassColour = texture(Grass, vTexCoord);\
							vec4 waterColour = texture(Water, vTexCoord);\
							vec4 snowColour = texture (Snow, vTexCoord);\
							void main(){\
							float blend = smoothstep(0.3,0.6,height);\
							vec4 grassWaterColour = mix(waterColour, grassColour, blend);\
							blend = smoothstep(0.6,0.9,height);\
							vec4 grassWaterSnowColour = mix(grassWaterColour, snowColour, blend);\
							vec2 uv = vec2(vTexCoord.x, vTexCoord.y);\
							FragColor = grassWaterSnowColour; }";

	m_program.create(vertexShader, fragShader);
	m_waterTexture.data = stbi_load("./data/water.png", &m_waterTexture.imageWidth, &m_waterTexture.imageHeight, &m_waterTexture.imageFormat, STBI_default);
	m_grassTexture.data = stbi_load("./data/grass.png", &m_grassTexture.imageWidth, &m_grassTexture.imageHeight, &m_grassTexture.imageFormat, STBI_default);
	m_snowTexture.data = stbi_load("./data/snow.png", &m_snowTexture.imageWidth, &m_snowTexture.imageHeight, &m_snowTexture.imageFormat, STBI_default);
}


TerrainGen::~TerrainGen()
{
}

void TerrainGen::loadTextures(Texture &tex)
{
	
	assert(tex.data != nullptr);
	glGenTextures(1, &tex.text);
	glBindTexture(GL_TEXTURE_2D, tex.text);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.imageWidth, tex.imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(tex.data);
}

void TerrainGen::GenTerrain(int dimensions)
{
	
	loadTextures(m_waterTexture);
	loadTextures(m_grassTexture);
	loadTextures(m_snowTexture);
	m_grid.startUp(dimensions, dimensions);

	m_dimensions = dimensions;
	m_perlinData = new float[m_dimensions * m_dimensions];
	scale = (1.0f / m_dimensions) * 4.f;
	int octaves = 30.f;
	for (int x = 0; x < m_dimensions; x++)
	{
		for (int y = 0; y < m_dimensions; y++)
		{
			float amplitude = 1.f;
			float persistence = 0.3f;
			//SetHeight(x, y, 0);
			GetHeight(x, y) = 0;

			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				float perlin_sample = glm::perlin(vec2((float)x, (float)y)*scale*freq)*0.5f + 0.5f;
				//assert(perlin_sample > 0);
				m_perlinData[y*m_dimensions + x] += perlin_sample*amplitude;
				amplitude*=persistence;
			}
		}
	}

	glGenTextures(1, &m_perlinTexture);
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_dimensions, m_dimensions, 0, GL_RED, GL_FLOAT, m_perlinData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);	
	glUseProgram(m_program.m_programID);
	unsigned int diffuseLocation = glGetUniformLocation(m_program.m_programID, "diffuse");
//	assert(diffuseLocation != -1);
	glUniform1i(diffuseLocation, 0);
	
	unsigned int perlinLocation = glGetUniformLocation(m_program.m_programID, "perlinTexture");
	assert(perlinLocation != -1);
	glUniform1i(perlinLocation, 1);
	
	
	unsigned int grassLocation = glGetUniformLocation(m_program.m_programID, "Grass");
	assert(grassLocation != -1);
	glUniform1i(grassLocation, 2);


	unsigned int waterLocation = glGetUniformLocation(m_program.m_programID, "Water");
	assert(waterLocation != -1);
	glUniform1i(waterLocation, 3);
	

	unsigned int snowLocation = glGetUniformLocation(m_program.m_programID, "Snow");
	assert(snowLocation != -1);
	glUniform1i(snowLocation, 4);

}

void TerrainGen::Draw(mat4 cameraView)
{
	glUseProgram(m_program.m_programID);
	unsigned int projectionViewUniform = glGetUniformLocation(m_program.m_programID, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(cameraView));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,  m_perlinTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_grassTexture.text);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_waterTexture.text);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_snowTexture.text);

	glBindVertexArray(m_grid.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_grid.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_grid.m_IBO);

	unsigned int indexCount = (m_grid.m_row - 1)*(m_grid.m_col - 1) * 6;
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void TerrainGen::FlattenTerrain()
{



	for (int i = 0; i < 16384; i++)
	{
		if (m_perlinData[i] > 0.7f)
		{
			m_perlinData[i] -= 0.001f;
		}
		else if (m_perlinData[i] < 0.7f)
		{
			m_perlinData[i] += 0.001f;
		}
		else m_perlinData[i] = 0.7f;
	}
	
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, m_dimensions, m_dimensions, GL_RED, GL_FLOAT, m_perlinData);
}

void TerrainGen::WaterRiseFall(float deltaTime)
{
	waterTimer += deltaTime;
	if (waterTimer <=10)
	{
		for (int i = 0; i < 16384; i++)
		{
			if (m_perlinData[i] < 0.45)
			{
				m_perlinData[i] += 0.0001;
			}
		}
	}

	if (waterTimer > 10) 
	{
		for (int i = 0; i < 16384; i++)
		{
			if (0.4501 >= m_perlinData[i])
			{
				m_perlinData[i] -= 0.0001;
			}
		}
		if (waterTimer >= 20)
		{
			waterTimer = 0;
		}
	}

	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_dimensions, m_dimensions, GL_RED, GL_FLOAT, m_perlinData);
}

void TerrainGen::MountainTest()
{
	float highest = 0;
	int highestIndex = 0;
	for (int i = 0; i < 16384; i++)
	{
		if (m_perlinData[i] > highest)
		{
			highest = m_perlinData[i];
			highestIndex = i;
		}
	}

	glm::ivec2 coords = GetCoords(highestIndex);

	for (int x = 0; x <= 30; x++)
	{
		if (coords.x + x < m_dimensions)
		{
			SetHeight(coords.x + x, coords.y, 0.2f);
			for (int y = 0; y <= 30; y++)
			{
				if (coords.x + x < m_dimensions && coords.y + y < m_dimensions)
				{
					SetHeight(coords.x + x, coords.y + y, 0.2f);
				}
			}
		}
	}

	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_dimensions, m_dimensions, GL_RED, GL_FLOAT, m_perlinData);
	//for (int x = 0; x < 200; x++)
	//{
	//	if (coords.y + x <= m_dimensions)
	//	{
	//		SetHeight(coords.x, coords.y + x, 0.2f);
	//	}
	//}
}
