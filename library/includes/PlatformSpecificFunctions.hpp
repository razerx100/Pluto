#ifndef PLATFORM_SPECIFIC_FUNCTIONS_HPP_
#define PLATFORM_SPECIFIC_FUNCTIONS_HPP_
#include <vector>
#include <InputManagerImpl.hpp>
#include <GamepadImpl.hpp>

void SetGamepadData(std::vector<GamepadImpl>& gamepads) noexcept;

void CheckXBoxControllerStates(std::vector<GamepadImpl>& gamepads) noexcept;
void DisconnectXBoxController(std::vector<GamepadImpl>& gamepads) noexcept;

void PlutoWin32InputCallback(
	InputManagerImpl& inputManager,
	void* hwnd, std::uint32_t message, std::uint64_t wParameter, std::uint64_t lParameter
);
#endif
