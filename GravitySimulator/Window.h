
#pragma once

#include "Program.h"

class Window {
public:
	explicit Window(Program&);

	int runMainLoop();

private:
	Program& m_program;
};
