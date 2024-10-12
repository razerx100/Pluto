#ifndef MOUSE_HPP_
#define MOUSE_HPP_
#include <utility>
#include <optional>

struct CursorCoord
{
	std::int32_t x;
	std::int32_t y;
};

enum class MouseButton
{
	Left,
	Right,
	Middle,
	X1,
	X2,
	Invalid
};

class Mouse
{
public:
	virtual ~Mouse() = default;

	[[nodiscard]]
	virtual float GetMouseWheelDelta() const noexcept = 0;
	[[nodiscard]]
	virtual std::uint16_t GetMouseRotationCount() const noexcept = 0;
	[[nodiscard]]
	virtual bool IsWheelUp() const noexcept = 0;
	[[nodiscard]]
	virtual bool IsWheelDown() const noexcept = 0;
	[[nodiscard]]
	virtual CursorCoord GetCurrentCursorCoord() const noexcept = 0;

	[[nodiscard]]
	virtual bool IsInWindow() const noexcept = 0;
	[[nodiscard]]
	virtual bool IsButtonPressed(MouseButton button) const noexcept = 0;

	template<typename... Keys>
	[[nodiscard]]
	bool AreButtonsPressed(Keys... buttons) const noexcept
	{
		return (IsButtonPressed(buttons) && ...);
	}
};
#endif
