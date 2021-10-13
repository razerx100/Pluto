#include <Keyboard.hpp>
#include <Mouse.hpp>
#include <InputManagerImpl.hpp>

static IKeyboard* s_pKeyboard = nullptr;
static IMouse* s_pMouse = nullptr;
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

IKeyboard* GetKeyboardInstance() noexcept {
	return s_pKeyboard;
}

void InitKeyboardInstance() {
	if(!s_pKeyboard)
		s_pKeyboard = new Keyboard();
}

void CleanUpKeyboardInstance() noexcept {
	if (s_pKeyboard)
		delete s_pKeyboard;
}

IMouse* GetMouseInstance() noexcept {
	return s_pMouse;
}

void InitMouseInstance() {
	if (!s_pMouse)
		s_pMouse = new Mouse();
}

void CleanUpMouseInstance() noexcept {
	if (s_pMouse)
		delete s_pMouse;
}
