#include <InputManagerImpl.hpp>

void InputManagerImpl::AddGamepadSupport(size_t count) noexcept
{
	for (size_t index = 0u; index < count; ++index)
		m_gamepads.emplace_back(GamepadImpl{});
}

void InputManagerImpl::DisconnectGamepad(size_t index) noexcept
{
	m_gamepads[index].Flush();
}

void InputManagerImpl::ClearInputStates() noexcept
{
	m_keyboard.ClearState();
	m_mouse.ClearState();

	for (auto& gamepad : m_gamepads)
		gamepad.ClearState();
}
