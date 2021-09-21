#include <Keyboard.hpp>
#include <Mouse.hpp>

static std::unique_ptr<IKeyboard> sKeyboard;
static std::unique_ptr<IMouse> sMouse;


IKeyboard* GetKeyboardInstance() noexcept {
	return sKeyboard.get();
}

void InitKeyboardInstance() noexcept {
	sKeyboard = std::make_unique<Keyboard>();
}

IMouse* GetMouseInstance() noexcept {
	return sMouse.get();
}

void InitMouseInstance() noexcept {
	sMouse = std::make_unique<Mouse>();
}
