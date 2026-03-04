#pragma once
#include "main.h"
#include <fstream>
#include <sstream>

namespace util {
	struct shaderFilePathBundle {
		const char* vertex;
		const char* geometry;
		const char* tcs;
		const char* tes;
		const char* fragment;
	};
	unsigned int load_shader(const shaderFilePathBundle& filepaths);
	unsigned int load_shader_module(const char* filePath, unsigned int type);
}