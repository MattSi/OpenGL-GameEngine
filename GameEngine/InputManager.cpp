#include "EngineCore.h"
#include "InputManager.h"
#include "RenderSystem.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

template<> InputManager* Singleton<InputManager>::instance = nullptr;



void InputManager::CleanUp()
{
	keyNow.fill(false);
	keyPrev.fill(false);
	mouseButtonNow.fill(false);
	mouseButtonPrev.fill(false);

	mouseX = 0;
	mouseY = 0;
	mouseWheelY = 0;
	mouseDeltaX = 0;
	mouseDeltaY = 0;
	quitRequested = false;
	if(relativeMouseMode) {
		SDL_SetRelativeMouseMode(SDL_FALSE);
		relativeMouseMode = false;
	}
}



void InputManager::Update()
{
	keyPrev = keyNow;
	mouseButtonPrev = mouseButtonNow;
	mouseWheelY = 0;
	mouseDeltaX = 0;
	mouseDeltaY = 0;

	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch(event.type) {
			case SDL_QUIT:
				quitRequested = true;
				break;
			case SDL_KEYDOWN:
				keyNow[event.key.keysym.scancode] = true;
				break;
			case SDL_KEYUP:
				keyNow[event.key.keysym.scancode] = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouseButtonNow[event.button.button] = true;
				break;
			case SDL_MOUSEBUTTONUP:
				mouseButtonNow[event.button.button] = false;
				break;
			case SDL_MOUSEMOTION:
				mouseX = event.motion.x;
				mouseY = event.motion.y;
				mouseDeltaX += event.motion.xrel;
				mouseDeltaY += event.motion.yrel;
				break;
			case SDL_MOUSEWHEEL:
				mouseWheelY += event.wheel.y;
				break;
			case SDL_WINDOWEVENT:
					if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
						int newWidth = event.window.data1;
						int newHeight = event.window.data2;
						RenderSystem::GetInstance().OnResize(newWidth, newHeight);
					}
					break;
			default:
				break;
		}
	}
}


void InputManager::SetRelativeMouseMode(bool enabled)
{
	if(relativeMouseMode == enabled) {
		return;
	}

	if(SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE) == 0) {
		relativeMouseMode = enabled;
	}
}