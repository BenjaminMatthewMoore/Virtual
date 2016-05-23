#include "Particles.h"

ParticleEmitter::ParticleEmitter():m_particles(nullptr), m_firstDead(0), m_maxParticles(0),
						m_position(0,0,0), m_vao(0), m_vbo(0), m_ibo(0), m_vertexData(nullptr)
{
	const char* vsSource = "#version 410\n \
							in vec4 Position;\
							in vec4 ColourIn;\
							out vec4 ColourOut;\
							uniform mat4 projectionView;\
							void main() {\
							ColourOut = ColourIn; \
							gl_Position = projectionView * Position;}";
	const char* fsSource = "#version 410\n\
							in vec4 ColourOut;\
							void main() { \
							gl_FragColor = ColourOut;}";

	m_program.create(vsSource, fsSource);

	
}

ParticleEmitter::~ParticleEmitter()
{
	delete[] m_particles;
	delete[] m_vertexData;

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);

}


void ParticleEmitter::initalise(unsigned int a_maxParticles, unsigned int a_emitRate, float a_lifetimeMin, float a_lifetimeMax,
	float a_velocityMin, float a_velocityMax, float a_startSize, float a_endSize, const glm::vec4 &a_startColour, const glm::vec4 &a_endColour)
{
	m_position.x = 100;
	m_position.y = 100;
	m_position.z = 100;

	
	m_emitTimer = 0;
	m_emitRate = 1.0f / a_emitRate;

	m_startColour = a_startColour;
	m_endColour = a_endColour;
	m_startSize = a_startSize;
	m_endSize = a_endSize;
	m_velocityMin = a_velocityMin;
	m_velocityMax = a_velocityMax;
	m_lifespanMin = a_lifetimeMin;
	m_lifespanMax = a_lifetimeMax;
	m_maxParticles = a_maxParticles;

	//create particles array
	m_particles = new Particle[m_maxParticles];
	m_firstDead = 0;
	m_vertexData = new ParticleVertex[m_maxParticles * 4];

	unsigned int* indexData = new unsigned int[m_maxParticles * 6];
	for (unsigned int i = 0; i < m_maxParticles; ++i)
	{
		indexData[i * 6 + 0] = i * 4 + 0;
		indexData[i * 6 + 1] = i * 4 + 1;
		indexData[i * 6 + 2] = i * 4 + 2;

		indexData[i * 6 + 3] = i * 4 + 0;
		indexData[i * 6 + 4] = i * 4 + 2;
		indexData[i * 6 + 5] = i * 4 + 3;
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 4 * sizeof(ParticleVertex), m_vertexData, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxParticles * 6 * sizeof(unsigned int), indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //pos
	glEnableVertexAttribArray(1);//col
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indexData;
}


void ParticleEmitter::emit()
{
	if (m_firstDead >= m_maxParticles)
		return;

	Particle& particle = m_particles[m_firstDead++];

	particle.position = m_position;

	particle.lifetime = 0;
	particle.lifespan = (rand() / (float)RAND_MAX)*(m_lifespanMax - m_lifespanMin) + m_lifespanMin;

	particle.colour = m_startColour;
	particle.size = m_startSize; 

	float velocity = (rand() / (float)RAND_MAX) * (m_velocityMax - m_velocityMin) + m_velocityMin;
	particle.velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity = glm::normalize(particle.velocity)*velocity;
}

void ParticleEmitter::update(float a_deltaTime, const glm::mat4 & a_cameraTransform)
{
	using glm::vec3;
	using glm::vec4;

	m_emitTimer += a_deltaTime;
	while (m_emitTimer > m_emitRate)
	{
		emit();
		m_emitTimer -= m_emitRate;
	}
	unsigned int quad = 0;

	for (unsigned int i = 0; i < m_firstDead; i++)
	{
		Particle *particle = &m_particles[i];

		particle->lifetime += a_deltaTime;
		if (particle->lifetime >= particle->lifespan)
		{
			*particle = m_particles[m_firstDead - 1];
			m_firstDead--;
		}
		else
		{
			particle->position += particle->velocity * a_deltaTime;

			particle->size = glm::mix(m_startSize, m_endSize, particle->lifetime / particle->lifespan);

			particle->colour = glm::mix(m_startColour, m_endColour, particle->lifetime / particle->lifespan);

			float halfSize = particle->size * 0.5f;

			m_vertexData[quad * 4 + 0].position = vec4(halfSize, halfSize, 0, 1);
			m_vertexData[quad * 4 + 0].colour = particle->colour;
			m_vertexData[quad * 4 + 1].position = vec4(-halfSize, halfSize, 0, 1);
			m_vertexData[quad * 4 + 1].colour = particle->colour;
			m_vertexData[quad * 4 + 2].position = vec4(-halfSize, -halfSize, 0, 1);
			m_vertexData[quad * 4 + 2].colour = particle->colour;
			m_vertexData[quad * 4 + 3].position = vec4(halfSize, -halfSize, 0, 1);
			m_vertexData[quad * 4 + 3].colour = particle->colour;

			vec3 zAxis = glm::normalize(vec3(a_cameraTransform[3]) - particle->position);
			vec3 xAxis = glm::cross(vec3(a_cameraTransform[1]), zAxis);
			vec3 yAxis = glm::cross(zAxis, xAxis);
			glm::mat4 billboard(vec4(xAxis, 0),
								vec4(yAxis, 0),
								vec4(zAxis, 0),
								vec4(0, 0, 0, 1));

			m_vertexData[quad * 4 + 0].position = billboard*m_vertexData[quad * 4 + 0].position + vec4(particle->position, 0);

			m_vertexData[quad * 4 + 1].position = billboard*m_vertexData[quad * 4 + 1].position + vec4(particle->position, 0);

			m_vertexData[quad * 4 + 2].position = billboard*m_vertexData[quad * 4 + 2].position + vec4(particle->position, 0);

			m_vertexData[quad * 4 + 3].position = billboard*m_vertexData[quad * 4 + 3].position + vec4(particle->position, 0);

			++quad;
		}
	}
}

void ParticleEmitter::draw(mat4 ProjectionView)
{
	glUseProgram(m_program.m_programID);
	int loc = glGetUniformLocation(m_program.m_programID, "projectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &ProjectionView [0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_firstDead * 4 * sizeof(ParticleVertex), m_vertexData);

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_firstDead * 6, GL_UNSIGNED_INT, 0);
}