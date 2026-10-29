// MIT License
// 
// Copyright(c) 2016 William Barry
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#pragma once

#define NOMINMAX
#define _WINSOCKAPI_
#pragma comment(lib, "Rpcrt4.lib")
#include <windows.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <map>
#include <list>
#include <assert.h>
#include <vector>
#include <functional>
#include <memory>
#include <chrono>
#include <thread>
#include <fstream>
#include <math.h>
#include <chrono>
#include <array>
#include <algorithm>
#include <utility>
#include <cassert> 
#include <typeinfo>
#include <filesystem>
#include <cstdlib>
#include <ctime>

// Feel free to add more here!
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rect.h>
#include <SDL_events.h>
#include "json.hpp"
#include "StandardTypes.h"
#include "TypeClass.h"
#include "Singleton.h"
#include "OBJ_Loader.h"
#include "stb_image.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"


#define DEBUG_LOG(indent, log) std::cout << std::setw(indent) << "" << log << std::endl;

#define M_ASSERT(cond, cstr)                                                  \
    do {                                                                      \
      if (!(cond)) {                                                          \
        const char* _msg__ = (cstr);                                          \
        if (_msg__) {                                                         \
          OutputDebugStringA(_msg__);                                         \
          OutputDebugStringA("\n");                                           \
        }                                                                     \
        std::abort();                                                         \
      }                                                                       \
    } while (0)
