#ifndef INPUT_MANAGER_IMPL_HPP_
#define INPUT_MANAGER_IMPL_HPP_
#include <InputManager.hpp>
#include <vector>
#include <KeyboardImpl.hpp>
#include <MouseImpl.hpp>
#include <GamepadImpl.hpp>

class InputManagerImpl final : public InputManager
{
	friend void PlutoWin32InputCallback(
		InputManagerImpl& inputManager,
		void* hwnd, std::uint32_t message, std::uint64_t wParameter, std::uint64_t lParameter
	);

public:
	InputManagerImpl()
		: m_keyboard{}, m_mouse{}, m_gamepads{}
	{}

	void AddGamepadSupport(size_t count) noexcept override;
	void UpdateIndependentInputs() noexcept override;

	[[nodiscard]]
	size_t GetGamepadCount() const noexcept override { return std::size(m_gamepads); }

	[[nodiscard]]
	const Keyboard& GetKeyboard() const noexcept override { return m_keyboard; }

	[[nodiscard]]
	const Mouse& GetMouse() const noexcept override { return m_mouse; }

	[[nodiscard]]
	const Gamepad& GetGamepad(size_t index = 0u) const noexcept override
	{
		return m_gamepads[index];
	}

	void InputCallback(
		void* hwnd, std::uint32_t message, std::uint64_t wParameter, std::uint64_t lParameter
	) override;

private:
	void ClearInputStates() noexcept;

private:
	KeyboardImpl             m_keyboard;
	MouseImpl                m_mouse;
	std::vector<GamepadImpl> m_gamepads;

public:
	InputManagerImpl(const InputManagerImpl&) = delete;
	InputManagerImpl& operator=(const InputManagerImpl&) = delete;

	InputManagerImpl(InputManagerImpl&& other) noexcept
		: m_keyboard{ std::move(other.m_keyboard) },
		m_mouse{ std::move(other.m_mouse) },
		m_gamepads{ std::move(other.m_gamepads) }
	{}
	InputManagerImpl& operator=(InputManagerImpl&& other) noexcept
	{
		m_keyboard = std::move(other.m_keyboard);
		m_mouse    = std::move(other.m_mouse);
		m_gamepads = std::move(other.m_gamepads);

		return *this;
	}
};
#endif
