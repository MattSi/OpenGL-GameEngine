#pragma once

class Engine;

class InputManager : public Singleton<InputManager>
{
	public:
		// Keyboard input methods
		bool IsKeyDown(SDL_Scancode key) const {
			return keyNow[key];
		}
		bool IsKeyPressed(SDL_Scancode key) const {
			return keyNow[key] && !keyPrev[key];
		}
		bool IsKeyReleased(SDL_Scancode key) const {
			return !keyNow[key] && keyPrev[key];
		}

		// Mouse input methods
		bool IsMouseButtonDown(Uint8 button) const {
			return mouseButtonNow[button];
		}
		bool IsMouseButtonPressed(Uint8 button) const {
			return mouseButtonNow[button] && !mouseButtonPrev[button];
		}
		bool IsMouseButtonReleased(Uint8 button) const {
			return !mouseButtonNow[button] && mouseButtonPrev[button];
		}

		void GetMousePosition(int& x, int& y) const {
			x = mouseX;
			y = mouseY;
		}
		int GetMouseWheelY() const {
			return mouseWheelY;
		}
		void GetMouseDelta(int& dx, int& dy) {
			dx = mouseDeltaX;
			dy = mouseDeltaY;
		}
		bool IsQuitRequested() const {
			return quitRequested;
		}
		void SetRelativeMouseMode(bool enabled);
		bool IsRelativeMouseMode() const {
			return relativeMouseMode;
		}
private:
	friend class Engine;
	friend class Singleton<InputManager>;

	void Initialize() {};
	void CleanUp() override;
	void Update();

	InputManager() = default;
	~InputManager() = default;
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

	// Input handling members
	std::array<bool, SDL_NUM_SCANCODES> keyNow{}, keyPrev{};
	std::array<bool, 8> mouseButtonNow{}, mouseButtonPrev{};
	int mouseX = 0, mouseY = 0;
	int mouseWheelY = 0;
	int mouseDeltaX = 0, mouseDeltaY = 0;

	bool relativeMouseMode = false;

	bool quitRequested = false;
};

