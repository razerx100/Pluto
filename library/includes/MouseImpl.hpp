#ifndef MOUSE_IMPL_HPP_
#define MOUSE_IMPL_HPP_
#include <queue>
#include <bitset>
#include <Mouse.hpp>

class MouseImpl final : public Mouse
{
public:
	MouseImpl();

	[[nodiscard]]
	float GetMouseWheelDelta() const noexcept override { return m_mouseWheelDelta; }
	[[nodiscard]]
	std::uint16_t GetMouseRotationCount() const noexcept override
	{
		return m_wheelRotationCount;
	}
	[[nodiscard]]
	bool IsWheelUp() const noexcept override { return m_isWheelUp; }
	[[nodiscard]]
	bool IsWheelDown() const noexcept override { return m_isWheelDown; }
	[[nodiscard]]
	CursorCoord GetCurrentCursorCoord() const noexcept override
	{
		return m_currentCursorCoord;
	}

	[[nodiscard]]
	bool IsInWindow() const noexcept override { return m_inWindow; }
	[[nodiscard]]
	bool IsButtonPressed(MouseButton button) const noexcept override;

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
	MouseImpl(const MouseImpl&) = delete;
	MouseImpl& operator=(const MouseImpl&) = delete;

	MouseImpl(MouseImpl&& other) noexcept
		: m_mouseState{ std::move(other.m_mouseState) },
		m_currentCursorCoord{ other.m_currentCursorCoord },
		m_mouseWheelDelta{ other.m_mouseWheelDelta },
		m_wheelRotationCount{ other.m_wheelRotationCount },
		m_isWheelUp{ other.m_isWheelUp },
		m_isWheelDown{ other.m_isWheelDown },
		m_wheelDeltaCarry{ other.m_wheelDeltaCarry },
		m_inWindow{ other.m_inWindow }
	{}
	MouseImpl& operator=(MouseImpl&& other) noexcept
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
#endif
