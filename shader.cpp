#include "shader.h"

unsigned int util::load_shader(const shaderFilePathBundle& filepaths) {
	std::vector<unsigned int> modules;

	if (filepaths.vertex) {
		modules.push_back(util::load_shader_module(filepaths.vertex, GL_VERTEX_SHADER));
	}

	if(filepaths.geometry) {
		modules.push_back(util::load_shader_module(filepaths.geometry, GL_GEOMETRY_SHADER));
	}

	if(filepaths.tcs) {
		modules.push_back(util::load_shader_module(filepaths.tcs, GL_TESS_CONTROL_SHADER));
	}

	if(filepaths.tes) {
		modules.push_back(util::load_shader_module(filepaths.tes, GL_TESS_EVALUATION_SHADER));
	}
	
	if(filepaths.fragment) {
		modules.push_back(util::load_shader_module(filepaths.fragment, GL_FRAGMENT_SHADER));
	}

	// initiate shader
	unsigned int shader = glCreateProgram();

	for (unsigned int shaderModule : modules) {
		glAttachShader(shader, shaderModule);

	}

	glLinkProgram(shader);


	// check link was successful
	int  linkSuccess;
	char linkInfoLog[1024];
	glGetShaderiv(shader, GL_LINK_STATUS, &linkSuccess);

	if (!linkSuccess)
	{
		glGetShaderInfoLog(shader, 512, NULL, linkInfoLog);
		std::cout << "ERROR::SHADER LINKING::LINKING_FAILED\n" << linkInfoLog << std::endl;
	} else {
		glGetShaderInfoLog(shader, 512, NULL, linkInfoLog);
		std::cout << "SHADER LINKING::SUCCESS\n" << linkInfoLog << std::endl;
	}

	// Remove from memory as we dont need the shader object(s) once weve linked in the the program

	for (unsigned int shaderModule : modules) {
		glDeleteShader(shaderModule);
	}

	return shader;
}

unsigned int util::load_shader_module(const char* filepath, unsigned int type) {
	std::ifstream fileReader;
	std::stringstream bufferedLines;
	std::string line;

	fileReader.open(filepath);

	while (std::getline(fileReader, line)) {
		bufferedLines << line << "\n";
	}

	std::string shaderSourceStr = bufferedLines.str();
	const char* shaderSource = shaderSourceStr.c_str();
	bufferedLines.str(std::string());
	fileReader.close();

	unsigned int shaderModule = glCreateShader(type);
	glShaderSource(shaderModule, 1, &shaderSource, NULL);
	glCompileShader(shaderModule);

	//// check compilation was successful
	int  success;
	char infoLog[512];
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shaderModule, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	} else {
		glGetShaderInfoLog(shaderModule, 512, NULL, infoLog);
		std::cout << "SHADER::COMPILATION_SUCCESS\n" << infoLog << std::endl;
	}

	return shaderModule;
}