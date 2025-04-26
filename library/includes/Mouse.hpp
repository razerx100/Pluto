#ifndef PLUTO_MOUSE_HPP_
#define PLUTO_MOUSE_HPP_
#include <queue>
#include <bitset>
#include <utility>
#include <optional>

namespace Pluto
{
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
	Mouse();

	[[nodiscard]]
	float GetMouseWheelDelta() const noexcept { return m_mouseWheelDelta; }
	[[nodiscard]]
	std::uint16_t GetMouseRotationCount() const noexcept
	{
		return m_wheelRotationCount;
	}
	[[nodiscard]]
	bool IsWheelUp() const noexcept { return m_isWheelUp; }
	[[nodiscard]]
	bool IsWheelDown() const noexcept { return m_isWheelDown; }
	[[nodiscard]]
	CursorCoord GetCurrentCursorCoord() const noexcept
	{
		return m_currentCursorCoord;
	}

	[[nodiscard]]
	bool IsInWindow() const noexcept { return m_inWindow; }
	[[nodiscard]]
	bool IsButtonPressed(MouseButton button) const noexcept;

	void SetPressState(std::uint8_t mouseState) noexcept;
	void SetReleaseState(std::uint8_t mouseState) noexcept;
	void SetCurrentCursorCoord(std::int32_t xCoord, std::int32_t yCoord) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void SetWheelDelta(std::int16_t delta) noexcept;

	void ClearState() noexcept;

private:
	static constexpr size_t s_buttonCount = static_cast<size_t>(MouseButton::Invalid);

	std::bitset<s_buttonCount> m_mouseState;
	CursorCoord                m_currentCursorCoord;
	float                      m_mouseWheelDelta;
	std::uint16_t              m_wheelRotationCount;
	bool                       m_isWheelUp;
	bool                       m_isWheelDown;
	std::int32_t               m_wheelDeltaCarry;
	bool                       m_inWindow;

public:
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	Mouse(Mouse&& other) noexcept
		: m_mouseState{ std::move(other.m_mouseState) },
		m_currentCursorCoord{ other.m_currentCursorCoord },
		m_mouseWheelDelta{ other.m_mouseWheelDelta },
		m_wheelRotationCount{ other.m_wheelRotationCount },
		m_isWheelUp{ other.m_isWheelUp },
		m_isWheelDown{ other.m_isWheelDown },
		m_wheelDeltaCarry{ other.m_wheelDeltaCarry },
		m_inWindow{ other.m_inWindow }
	{}
	Mouse& operator=(Mouse&& other) noexcept
	{
		m_mouseState         = std::move(other.m_mouseState);
		m_currentCursorCoord = other.m_currentCursorCoord;
		m_mouseWheelDelta    = other.m_mouseWheelDelta;
		m_wheelRotationCount = other.m_wheelRotationCount;
		m_isWheelUp          = other.m_isWheelUp;
		m_isWheelDown        = other.m_isWheelDown;
		m_wheelDeltaCarry    = other.m_wheelDeltaCarry;
		m_inWindow           = other.m_inWindow;

		return *this;
	}
};
}
#endif
