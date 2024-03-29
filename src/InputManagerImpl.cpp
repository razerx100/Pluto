#include <InputManagerImpl.hpp>
#include <Keyboard.hpp>
#include <Mouse.hpp>
#include <Gamepad.hpp>

InputManagerImpl::InputManagerImpl() noexcept {
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
}

void InputManagerImpl::AddGamepadSupport(size_t count) noexcept {
	for (size_t index = 0u; index < count; ++index)
		m_gamepads.emplace_back(std::make_unique<Gamepad>());
}

size_t InputManagerImpl::GetGamepadCount() const noexcept {
	return std::size(m_gamepads);
}

IKeyboard& InputManagerImpl::GetKeyboard() const noexcept {
	return *GetKeyboardRef();
}

IMouse& InputManagerImpl::GetMouse() const noexcept {
	return *GetMouseRef();
}

IGamepad& InputManagerImpl::GetGamepad(size_t index) const noexcept {
	return *GetGamepadRef(index);
}

IKeyboard* InputManagerImpl::GetKeyboardRef() const noexcept {
	return m_keyboard.get();
}

IMouse* InputManagerImpl::GetMouseRef() const noexcept {

	return m_mouse.get();
}

IGamepad* InputManagerImpl::GetGamepadRef(size_t index) const noexcept {
	return m_gamepads[index].get();
}

void InputManagerImpl::DisconnectGamepad(size_t index) noexcept {
	m_gamepads[index]->Flush();
}

void InputManagerImpl::ClearInputStates() noexcept {
	m_keyboard->ClearState();
	m_mouse->ClearState();

	for (auto& gamepad : m_gamepads)
		gamepad->ClearState();
}
