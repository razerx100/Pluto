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
	float GetMouseTicks() const noexcept override;
	[[nodiscard]]
	std::optional<Event> ReadEvents() noexcept override;
	[[nodiscard]]
	CursorCoord GetCurrentCursorCoord() const noexcept override;

	[[nodiscard]]
	bool IsInWindow() const noexcept override;
	[[nodiscard]]
	bool IsButtonPressed(MouseButtons button) const noexcept override;
	[[nodiscard]]
	bool AreButtonsPressed(size_t count, ...) const noexcept override;

	void Flush() noexcept override;

	void SetPressState(std::uint8_t mouseState) noexcept override;
	void SetReleaseState(std::uint8_t mouseState) noexcept override;
	void SetCurrentCursorCoord(std::int32_t xCoord, std::int32_t yCoord) noexcept override;
	void OnMouseLeave() noexcept override;
	void OnMouseEnter() noexcept override;
	void OnWheelDelta(std::int16_t delta) noexcept override;
	void ClearState() noexcept override;

private:
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept
	{
		while (std::size(buffer) > s_bufferSize)
			buffer.pop();
	}

	void OnWheelUp() noexcept;
	void OnWheelDown() noexcept;

private:
	static constexpr size_t s_bufferSize = 16u;

	float             m_mouseTicks;
	int               m_wheelDeltaCarry;
	CursorCoord       m_currentCursorCoord;
	std::bitset<8u>   m_mouseState;
	std::queue<Event> m_eventBuffer;
	bool              m_inWindow;

public:
	MouseImpl(const MouseImpl&) = delete;
	MouseImpl& operator=(const MouseImpl&) = delete;

	MouseImpl(MouseImpl&& other) noexcept
		: m_mouseTicks{ other.m_mouseTicks },
		m_wheelDeltaCarry{ other.m_wheelDeltaCarry },
		m_currentCursorCoord{ other.m_currentCursorCoord },
		m_mouseState{ std::move(other.m_mouseState) },
		m_eventBuffer{ std::move(other.m_eventBuffer) },
		m_inWindow{ other.m_inWindow }
	{}
	MouseImpl& operator=(MouseImpl&& other) noexcept
	{
		m_mouseTicks         = other.m_mouseTicks;
		m_wheelDeltaCarry    = other.m_wheelDeltaCarry;
		m_currentCursorCoord = other.m_currentCursorCoord;
		m_mouseState         = std::move(other.m_mouseState);
		m_eventBuffer        = std::move(other.m_eventBuffer);
		m_inWindow           = other.m_inWindow;

		return *this;
	}
};
#endif
