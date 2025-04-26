#ifndef PLUTO_PLATFORM_SPECIFIC_FUNCTIONS_HPP_
#define PLUTO_PLATFORM_SPECIFIC_FUNCTIONS_HPP_
#include <vector>
#include <InputManager.hpp>
#include <Gamepad.hpp>

namespace Pluto
{
#ifdef PLUTO_WIN32
void SetGamepadData(std::vector<Gamepad>& gamepads) noexcept;

void CheckXBoxControllerStates(std::vector<Gamepad>& gamepads) noexcept;
void DisconnectXBoxController(std::vector<Gamepad>& gamepads) noexcept;

void PlutoWin32InputCallback(
	InputManager& inputManager,
	void* hwnd, std::uint32_t message, std::uint64_t wParameter, std::uint64_t lParameter
);
#endif
}
#endif
