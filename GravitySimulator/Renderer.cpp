
#include <glad/glad.h>

#include "Renderer.h"

Renderer::Renderer(SDL_Window* window, SDL_GLContext glContext, ImGuiIO& io) : m_window(window), m_glContext(glContext), m_io(io), m_scale(1)
{
	setupLineVAO();
	setupLineShaderProgram();
	setupCircleVAO();
	setupCircleShaderProgram();
	updateScalingFactor();
}

void Renderer::updateScalingFactor()
{
	int width, height, scalingFactorLocation;

	SDL_GetWindowSize(m_window, &width, &height);

	glUseProgram(m_lineShaderProgram);
	scalingFactorLocation = glGetUniformLocation(m_lineShaderProgram, "scalingFactor");
	glUniform2f(scalingFactorLocation, static_cast<float>(width) * m_scale, static_cast<float>(height) * m_scale);

	glUseProgram(m_circleShaderProgram);
	scalingFactorLocation = glGetUniformLocation(m_circleShaderProgram, "scalingFactor");
	glUniform2f(scalingFactorLocation, static_cast<float>(width) * m_scale, static_cast<float>(height) * m_scale);
	int scaleLocation = glGetUniformLocation(m_circleShaderProgram, "scale");
	glUniform1f(scaleLocation, m_scale);
}

float Renderer::width()
{
	int width;

	SDL_GetWindowSize(m_window, &width, nullptr);

	return static_cast<float>(width) * m_scale;
}

float Renderer::height()
{
	int height;

	SDL_GetWindowSize(m_window, nullptr, &height);

	return static_cast<float>(height) * m_scale;
}

float Renderer::scale()
{
	return m_scale;
}

void Renderer::setScale(float scale)
{
	m_scale = scale;
	updateScalingFactor();
}

float Renderer::frameRate()
{
	return static_cast<float>(m_io.Framerate);
}

void Renderer::setColor(float r, float g, float b)
{
	int colorLocation;

	glUseProgram(m_lineShaderProgram);
	colorLocation = glGetUniformLocation(m_lineShaderProgram, "myColor");
	glUniform3f(colorLocation, r, g, b);

	glUseProgram(m_circleShaderProgram);
	colorLocation = glGetUniformLocation(m_circleShaderProgram, "myColor");
	glUniform3f(colorLocation, r, g, b);
}

void Renderer::drawLine(float x1, float y1, float x2, float y2)
{
	float vertices[] = { x1, y1, x2, y2 };

	glBindVertexArray(m_lineVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

	glUseProgram(m_lineShaderProgram);
	glDrawArrays(GL_LINES, 0, 2);
}

void Renderer::drawCircle(float x, float y, float radius)
{
	glUseProgram(m_circleShaderProgram);

	int centerLocation = glGetUniformLocation(m_circleShaderProgram, "center");
	glUniform2f(centerLocation, x, y);
	int radiusLocation = glGetUniformLocation(m_circleShaderProgram, "radius");
	glUniform1f(radiusLocation, radius);

	glBindVertexArray(m_circleVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::unload()
{
	glDeleteBuffers(1, &m_lineVBO);
	glDeleteVertexArrays(1, &m_lineVAO);
	glDeleteVertexArrays(1, &m_circleVAO);
	glDeleteProgram(m_lineShaderProgram);
	glDeleteProgram(m_circleShaderProgram);
}

unsigned Renderer::createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	int success;
	char infoLog[512];

	// Compile vertex shader
	unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// Check if it compiled correctly
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), nullptr, infoLog);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Vertex shader failed to compile", infoLog, m_window);
	}

	// Compile fragment shader
	unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// Check if it compiled correctly
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fragment shader failed to compile", infoLog, m_window);
	}

	// Create shader program
	unsigned shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Shader program failed to link", infoLog, m_window);
	}

	// Delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void Renderer::setupLineVAO()
{
	glGenVertexArrays(1, &m_lineVAO);
	glBindVertexArray(m_lineVAO);

	glGenBuffers(1, &m_lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Renderer::setupLineShaderProgram()
{
	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec2 position;"
		"uniform vec2 scalingFactor;"
		"void main()"
		"{"
		"    gl_Position = vec4(position.x * 2.0 / scalingFactor.x - 1.0, position.y * 2.0 / scalingFactor.y - 1.0, 0.0, 1.0);"
		"}";

	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;"
		"uniform vec3 myColor;"
		"void main()"
		"{"
		"    FragColor = vec4(myColor.r, myColor.g, myColor.b, 1.0);"
		"}";

	m_lineShaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void Renderer::setupCircleVAO()
{
	float vertices[] = {
		-1.0, 1.0,
		1.0, 1.0,
		-1.0, -1.0,
		-1.0, -1.0,
		1.0, 1.0,
		1.0, -1.0,
	};

	glGenVertexArrays(1, &m_circleVAO);
	glBindVertexArray(m_circleVAO);

	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Renderer::setupCircleShaderProgram()
{
	const char* vertexShaderSource =
		"#version 330 core\n"
		"uniform vec2 scalingFactor;"
		"uniform vec2 center;"
		"uniform float radius;"
		"layout (location = 0) in vec2 position;"
		"void main()"
		"{"
		"    float positionLength = sqrt(position.x * position.x + position.y * position.y);"
		"    vec2 newPosition = vec2(center.x + position.x * radius * 2 * abs(position.x) / positionLength, center.y + position.y * radius * 2 * abs(position.y) / positionLength);"
		"    gl_Position = vec4(newPosition.x * 2.0 / scalingFactor.x - 1.0, newPosition.y * 2.0 / scalingFactor.y - 1.0, 0.0, 1.0);"
		"}";

	const char* fragmentShaderSource =
		"#version 330 core\n"
		"uniform float scale;"
		"uniform vec3 myColor;"
		"uniform vec2 center;"
		"uniform float radius;"
		"out vec4 FragColor;"
		"void main()"
		"{"
		"    float xDistance = gl_FragCoord.x - center.x / scale;"
		"    float yDistance = gl_FragCoord.y - center.y / scale;"
		"    float distance = sqrt(xDistance * xDistance + yDistance * yDistance);"
		"    if (distance > radius / scale) {"
		"        discard;"
		"    }"
		"    FragColor = vec4(myColor.r, myColor.g, myColor.b, 1.0);"
		"}";

	m_circleShaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
}
