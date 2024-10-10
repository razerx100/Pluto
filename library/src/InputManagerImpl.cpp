#include <InputManagerImpl.hpp>
#include <PlatformSpecificFunctions.hpp>

void InputManagerImpl::AddGamepadSupport(size_t count) noexcept
{
	for (size_t index = 0u; index < count; ++index)
		m_gamepads.emplace_back(GamepadImpl{});

	SetGamepadData(m_gamepads);
}

void InputManagerImpl::ClearInputStates() noexcept
{
	m_keyboard.ClearState();
	m_mouse.ClearState();

	for (auto& gamepad : m_gamepads)
		gamepad.ClearState();
}

void InputManagerImpl::UpdateIndependentInputs() noexcept
{
	CheckXBoxControllerStates(m_gamepads);
}

void InputManagerImpl::InputCallback(
	void* hwnd, std::uint32_t message, std::uint64_t wParameter, std::uint64_t lParameter
) {
	PlutoWin32InputCallback(*this, hwnd, message, wParameter, lParameter);
}

void InputManagerImpl::SubscribeToEvent(
	InputEvent event, EventCallback eventCallback, void* extraData /* = nullptr */
) noexcept {
	EventContainer_t& eventContainer = m_eventCallbacks[static_cast<size_t>(event)];

	eventContainer.emplace_back(
		EventData
		{
			.callback  = eventCallback,
			.extraData = extraData
		}
	);
}
