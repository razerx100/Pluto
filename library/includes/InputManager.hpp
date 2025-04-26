#ifndef PLUTO_INPUT_MANAGER_HPP_
#define PLUTO_INPUT_MANAGER_HPP_
#include <array>
#include <vector>
#include <InputEvents.hpp>
#include <Keyboard.hpp>
#include <Mouse.hpp>
#include <Gamepad.hpp>

namespace Pluto
{
class InputManager
{
#ifdef PLUTO_WIN32
	friend void PlutoWin32InputCallback(
		InputManager& inputManager,
		void* hwnd, std::uint32_t message, std::uint64_t wParameter, std::uint64_t lParameter
	);
#endif

public:
	using EventCallback = void(*)(void*, void*);

public:
	InputManager() : m_keyboard{}, m_mouse{}, m_gamepads{}, m_eventCallbacks{} {}

	void AddGamepadSupport(size_t count) noexcept;
	void UpdateIndependentInputs() noexcept;

	void SubscribeToEvent(
		InputEvent event, EventCallback eventCallback, void* extraData = nullptr
	) noexcept;

	void UnsubscribeAllCallbacks(InputEvent event) noexcept;

	[[nodiscard]]
	size_t GetGamepadCount() const noexcept { return std::size(m_gamepads); }

	[[nodiscard]]
	const Keyboard& GetKeyboard() const noexcept { return m_keyboard; }

	[[nodiscard]]
	const Mouse& GetMouse() const noexcept { return m_mouse; }

	[[nodiscard]]
	const Gamepad& GetGamepad(size_t index = 0u) const noexcept
	{
		return m_gamepads[index];
	}

	void InputCallback(
		void* hwnd, std::uint32_t message, std::uint64_t wParameter, std::uint64_t lParameter
	);

private:
	void ClearInputStates() noexcept;

	struct EventData
	{
		EventCallback callback;
		void*         extraData;
	};

	static constexpr size_t s_eventCallbackCount = static_cast<size_t>(InputEvent::Invalid);

	using EventContainer_t = std::vector<EventData>;
	using EventArray_t     = std::array<EventContainer_t, s_eventCallbackCount>;

private:
	Keyboard             m_keyboard;
	Mouse                m_mouse;
	std::vector<Gamepad> m_gamepads;
	EventArray_t         m_eventCallbacks;

public:
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

	InputManager(InputManager&& other) noexcept
		: m_keyboard{ std::move(other.m_keyboard) },
		m_mouse{ std::move(other.m_mouse) },
		m_gamepads{ std::move(other.m_gamepads) },
		m_eventCallbacks{ std::move(other.m_eventCallbacks) }
	{}
	InputManager& operator=(InputManager&& other) noexcept
	{
		m_keyboard       = std::move(other.m_keyboard);
		m_mouse          = std::move(other.m_mouse);
		m_gamepads       = std::move(other.m_gamepads);
		m_eventCallbacks = std::move(other.m_eventCallbacks);

		return *this;
	}
};
}
#endif
