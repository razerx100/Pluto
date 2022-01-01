#ifndef __MOUSE_HPP__
#define __MOUSE_HPP__
#include <queue>
#include <IMouse.hpp>

class Mouse : public IMouse {
public:
	Mouse();

	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	[[nodiscard]]
	PosDelta GetPosDelta() const noexcept override;
	[[nodiscard]]
	int GetPosDX() const noexcept override;
	[[nodiscard]]
	int GetPosDY() const noexcept override;
	[[nodiscard]]
	float GetMouseTicks() const noexcept override;
	[[nodiscard]]
	Event Read() noexcept override;

	[[nodiscard]]
	bool IsInWindow() const noexcept override;
	[[nodiscard]]
	bool IsButtonPressed(MouseButtons button) const noexcept override;
	[[nodiscard]]
	bool AreButtonsPressed(int count, ...) const noexcept override;

	void Flush() noexcept override;

	void SetPressState(std::uint16_t mouseState) noexcept override;
	void SetReleaseState(std::uint16_t mouseState) noexcept override;
	void OnMouseMove(int dx, int dy) noexcept override;
	void OnMouseLeave() noexcept override;
	void OnMouseEnter() noexcept override;
	void OnWheelDelta(short delta) noexcept override;

private:
	void TrimBuffer() noexcept;
	void OnWheelUp() noexcept;
	void OnWheelDown() noexcept;
	void ClearState() noexcept;

private:
	static constexpr std::uint32_t s_bufferSize = 16u;
	bool m_inWindow;
	float m_mouseTicks;
	PosDelta m_cursorPositionDelta;
	int m_wheelDeltaCarry;

	std::uint16_t m_mouseState;
	std::queue<Event> m_buffer;
};
#endif
