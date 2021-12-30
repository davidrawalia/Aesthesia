#pragma once

#include <windows.h>
#include <shobjidl.h> 
#include "targetver.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/normal.hpp>
#include <SOIL2/SOIL2.h>
#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/DefaultLogger.hpp>
#include <assimp/Logger.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/fmt/ostr.h>
#include <exception>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/program_options.hpp>
#include <cstdlib>
#include <cstddef>
#include <cassert>
#include <utility>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <locale>
#include <codecvt>