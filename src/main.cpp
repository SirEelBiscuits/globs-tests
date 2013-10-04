#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>

#include "main.h"
#include "version.inc"

#include "graphics/glwrapper.h"
#include "graphics/shader.h"
#include "graphics/shaderloader.h"
#include "graphics/model.h"
#include "graphics/modelloader.h"
#include "graphics/texture.h"
#include "graphics/textureloader.h"
#include "graphics/devilwrapper.h"

#include "util/log.h"

#include "testmain.h"

//Main loop contains code to test current features only :)
int main(int argc, char* argv[]) {
	Init(argc, argv);

	LOG_MSG("INFO", "Loading shader");
	Shader* basicShader = ShaderLoader::LoadShaderFromFiles(
		"basic.frag",
		"basic.vert"
	);

	LOG_GL_ERRORS;
	LOG_MSG("INFO", "loading model assets");
	char const* modelsource = "square.obj";
	Model* testModel = ModelLoader::LoadModelFromFile(modelsource);
	LOG_GL_ERRORS;

	testModel->useShader(basicShader);
	if(!basicShader->set()) {
		LOG_MSG("ERROR", "Shader failed to set");
	}

	LOG_MSG("INFO", "loading texture assets");
	Texture* tex = TextureLoader::LoadTexture(DevILWrapper("test.jpg")
		, TextureType::Diffuse
	);
	tex->set();
	LOG_GL_ERRORS;

	glClearColor(0.,0.,0.,1.);
	while(glfwGetWindowParam(GLFW_OPENED)) {
		glClear(GL_COLOR_BUFFER_BIT);
		testModel->draw();
		glfwSwapBuffers();
		//temporary shutdown command
		if(glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) {
			break;
		}
	}
	glfwTerminate();
	return 0;
}

void Init(int argc, char* argv[]) {
	Gargamel::Process(Arguments, argc, argv);

	if(Gargamel::ArgumentSet[Help].isArgumentPresent) {
		Gargamel::ShowUsage();
		exit(EXIT_SUCCESS);
	}

	Logger::setLogAll(Gargamel::ArgumentSet[LogAll].isArgumentPresent);
	Logger::echo(Gargamel::ArgumentSet[EchoLog].isArgumentPresent);
	//Technically this should go two lines earlier, but there is no way
	// for it to fail before here, and no way for that output to go
	// anywhere either
	LOG_MSG("INFO", "Initialising logger");
	if(Gargamel::ArgumentSet[LogFile].isArgumentPresent) {
		if(!Logger::setFileName(
			Gargamel::ArgumentSet[LogFile].argumentValue
		)) {
			LOG_MSG(
				"ERR",
				"Failed to open log file (%s)",
				Gargamel::ArgumentSet[LogFile]
					.argumentValue
			);
		}
	}
	for(auto s : *(Gargamel::ArgumentSet[LogChannel].argumentArray)) {
		Logger::activateChannel(s);
	}

	LOG_MSG("INFO", "Starting version " VERSION);

	LOG_MSG("INFO", "Calling into GL::Init");
	int w = Gargamel::ArgumentSet[ScreenWidth].intVal();
	int h = Gargamel::ArgumentSet[ScreenHeight].intVal();
	ScreenMode mode = ScreenMode::Windowed;
	if(Gargamel::ArgumentSet[FullScreen].isArgumentPresent)
		mode = ScreenMode::Fullscreen;
	GL::Init(w, h, mode);

	//run tests and exit from here
	if(Gargamel::ArgumentSet[RunUnitTests].isArgumentPresent) {
		exit(UnitTesting::RunTests());
	}
}
