#include <Keyboard.hpp>
#include <Mouse.hpp>

static IKeyboard* s_pKeyboard = nullptr;
static IMouse* s_pMouse = nullptr;

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
