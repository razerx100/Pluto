#include <InputManager.hpp>
#include <PlatformSpecificFunctions.hpp>

namespace Pluto
{
void InputManager::AddGamepadSupport(size_t count) noexcept
{
	for (size_t index = 0u; index < count; ++index)
		m_gamepads.emplace_back(Gamepad{});

#ifdef PLUTO_WIN32
	SetGamepadData(m_gamepads);
#endif
}

void InputManager::ClearInputStates() noexcept
{
	m_keyboard.ClearState();
	m_mouse.ClearState();

	for (auto& gamepad : m_gamepads)
		gamepad.ClearState();
}

void InputManager::UpdateIndependentInputs() noexcept
{
#ifdef PLUTO_WIN32
	CheckXBoxControllerStates(m_gamepads);
#endif
}

void InputManager::InputCallback(
	void* hwnd, std::uint32_t message, std::uint64_t wParameter, std::uint64_t lParameter
) {
#ifdef PLUTO_WIN32
	PlutoWin32InputCallback(*this, hwnd, message, wParameter, lParameter);
#endif
}

void InputManager::SubscribeToEvent(
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

void InputManager::UnsubscribeAllCallbacks(InputEvent event) noexcept
{
	EventContainer_t& eventContainer = m_eventCallbacks[static_cast<size_t>(event)];

	eventContainer = EventContainer_t{};
}
}
