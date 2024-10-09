#ifndef WIN32_KEY_MANAGER_HPP_
#define WIN32_KEY_MANAGER_HPP_
#include <cstdint>
#include <Keyboard.hpp>

struct MouseStateFlags
{
	std::uint8_t pressFlags;
	std::uint8_t releaseFlags;
};

[[nodiscard]]
bool IsKeyDown(std::int32_t vKey) noexcept;
[[nodiscard]]
SKeyCodes GetSKeyCodes(std::uint16_t nativeKeycode) noexcept;
[[nodiscard]]
std::uint16_t ProcessGamepadRawButtons(std::uint16_t state) noexcept;
[[nodiscard]]
MouseStateFlags ProcessMouseRawButtons(std::uint16_t newState) noexcept;
#endif
