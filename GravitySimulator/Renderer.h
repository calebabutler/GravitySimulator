
#pragma once

#include <SDL.h>
#include "imgui/imgui.h"

class Renderer {
public:
	explicit Renderer(SDL_Window*, SDL_GLContext, ImGuiIO&);

	void updateScalingFactor();

	float width();
	float height();

	float scale();
	void setScale(float);

	float frameRate();

	void setColor(float r, float g, float b);
	void drawLine(float x1, float y1, float x2, float y2);
	void drawCircle(float x, float y, float radius);

	void unload();

private:
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	ImGuiIO& m_io;
	float m_scale;

	unsigned m_lineVAO;
	unsigned m_lineVBO;

	unsigned m_circleVAO;

	unsigned m_lineShaderProgram;
	unsigned m_circleShaderProgram;

	unsigned createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);

	void setupLineVAO();
	void setupLineShaderProgram();
	void setupCircleVAO();
	void setupCircleShaderProgram();
};
