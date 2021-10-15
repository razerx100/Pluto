#include <Keyboard.hpp>
#include <Mouse.hpp>
#include <InputManagerImpl.hpp>

static InputManager* s_pInputManager = nullptr;

InputManager* _cdecl GetInputManagerInstance() noexcept {
	return s_pInputManager;
}

void _cdecl InitInputManagerInstance() {
	if (!s_pInputManager)
		s_pInputManager = new InputManagerImpl;
}

void _cdecl CleanUpInputManagerInstance() noexcept {
	if (s_pInputManager)
		delete s_pInputManager;
}
