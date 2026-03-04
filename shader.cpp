#include "shader.h"

unsigned int util::load_shader(const shaderFilePathBundle& filepaths) {
	std::vector<unsigned int> modules;

	if (filepaths.vertex) {
		modules.push_back(util::load_shader_module(filepaths.vertex, NULL, NULL, NULL, NULL));
	}

	if(filepaths.geometry) {
		modules.push_back(util::load_shader_module(NULL, filepaths.geometry, NULL, NULL, NULL));
	}

	if(filepaths.tcs) {
		modules.push_back(util::load_shader_module(NULL, NULL, filepaths.tcs, NULL, NULL));
	}

	if(filepaths.tes) {
		modules.push_back(util::load_shader_module(NULL, NULL, NULL, filepaths.tes, NULL));
	}
	
	if(filepaths.fragment) {
		modules.push_back(util::load_shader_module(NULL, NULL, NULL, NULL, filepaths.fragment));
	}

	unsigned int shader = glCreateProgram();

	for (unsigned int shaderModule : modules) {
		glAttachShader(shader, shaderModule);

	}

	glLinkProgram(shader);



	// check compilation was successful
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER LINKING::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

}