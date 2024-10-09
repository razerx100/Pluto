#ifndef INPUT_MANAGER_HPP_
#define INPUT_MANAGER_HPP_
#include <Keyboard.hpp>
#include <Mouse.hpp>
#include <Gamepad.hpp>

class InputManager
{
public:
	virtual ~InputManager() = default;

	virtual void AddGamepadSupport(size_t count) noexcept = 0;
	virtual void UpdateIndependentInputs() noexcept = 0;

	[[nodiscard]]
	virtual size_t GetGamepadCount() const noexcept = 0;

	[[nodiscard]]
	virtual	const Keyboard& GetKeyboard() const noexcept = 0;

	[[nodiscard]]
	virtual	const Mouse& GetMouse() const noexcept = 0;

	[[nodiscard]]
	virtual	const Gamepad& GetGamepad(size_t index = 0u) const noexcept = 0;

	// Win32
	virtual void InputCallback(
		void* hwnd, std::uint32_t message, std::uint64_t wParameter, std::uint64_t lParameter
	) = 0;
};
#endif
