
#include <glad/glad.h>
#include <SDL.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Window.h"
#include "Renderer.h"

// Local functions
namespace {

void resetViewport(SDL_Window* window)
{
	int width, height;

	SDL_GetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

};

Window::Window(Program& program) : m_program(program) {}

int Window::runMainLoop()
{
	// Initialize SDL and prepare SDL for openGL version 3.3 core
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Create SDL window
	SDL_Window* window;
	window = SDL_CreateWindow("Gravity Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	// Create SDL openGL context and make current
	SDL_GLContext glContext;
	glContext = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, glContext);
	SDL_GL_SetSwapInterval(1);

	// Load glad
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	// Set up Dear ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	ImGuiIO& io = ImGui::GetIO();

	// Set up openGL
	resetViewport(window);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Create renderer
	Renderer renderer(window, glContext, io);

	// Run m_program.load
	m_program.load(renderer);

	// main loop
	bool isOpen = true;
	SDL_Event e;

	while (isOpen) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				isOpen = false;
			} else if (e.type == SDL_WINDOWEVENT) {
				if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					resetViewport(window);
					renderer.updateScalingFactor();
				}
			} else if (e.type == SDL_MOUSEBUTTONDOWN && !io.WantCaptureMouse) {
				m_program.mousePressed(renderer, e.button);
			}
			ImGui_ImplSDL2_ProcessEvent(&e);
		}

		// Update program
		m_program.update(renderer);

		// Draw ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		m_program.drawImGui(renderer);

		ImGui::Render();

		glClear(GL_COLOR_BUFFER_BIT);

		// Run m_program.draw
		m_program.draw(renderer);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		SDL_GL_SwapWindow(window);
	}

	// clean up
	renderer.unload();
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
