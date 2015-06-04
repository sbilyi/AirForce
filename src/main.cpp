#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>

#include "Core/Shader_Loader.h"
#include "../ModelViewScene.h"
#include "../Window.h"

using namespace Core;
using namespace gui;

GLuint program;

/*
- Show backgound texture(Video playback)
x keyboard reader
- show text on page
- adjust screen to real pixels
x full screen(enable/disable by pressnging F button)
x draw blinker
x mouse handler
x refactoring
*/


int main(int argc, char **argv) {
	Window &window = Window::getInstance();
	window.setScreen(new ModelViewScene(argc, argv));
	window.getScreen()->init();

	glutMainLoop();
	
	return 0;

}


int getShaderLanguageVersion(void) {
	std::string version = std::string(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	int value = 0;
	int deg = 0;
	for (unsigned int i = 0; i <= version.length(); i++) {
		if (version[i] != ' ') {
			if (version[i] >= '0' && version[i] <= '9') {
				value = value * 10;
				value += version[i] - '0';
			}

		} else {
			break;
		}
	}

	return value;
}

int degree(int v, int d) {
	int res = v;
	if (d == 0) return 1;
	for (int i = 1; i < d; i++) {
		res *= v;
	}

	return res;
}