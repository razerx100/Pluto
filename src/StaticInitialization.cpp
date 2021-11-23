#include <Keyboard.hpp>
#include <Mouse.hpp>
#include <InputManagerImpl.hpp>

static InputManager* s_pInputManager = nullptr;

InputManager* GetInputManagerInstance() noexcept {
	return s_pInputManager;
}

void InitInputManagerInstance() {
	if (!s_pInputManager)
		s_pInputManager = new InputManagerImpl;
}

void CleanUpInputManagerInstance() noexcept {
	if (s_pInputManager) {
		delete s_pInputManager;
		s_pInputManager = nullptr;
	}
}
