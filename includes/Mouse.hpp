#ifndef __MOUSE_HPP__
#define __MOUSE_HPP__
#include <queue>
#include <IMouse.hpp>
#include <bitset>

class Mouse : public IMouse {
public:
	Mouse();

	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	Vector2 GetPos() const noexcept override;
	int GetPosX() const noexcept override;
	int GetPosY() const noexcept override;
	float GetMouseTicks() const noexcept override;
	Event Read() noexcept override;

	bool IsInWindow() const noexcept override;
	bool IsButtonPressed(MouseButtons button) const noexcept override;
	bool AreButtonsPressed(int count, ...) const noexcept override;
	bool IsBufferEmpty() const noexcept override;

	void Flush() noexcept override;

	void SetRawMouseState(std::uint16_t mouseState) noexcept override;
	void OnMouseMove(int x, int y) noexcept override;
	void OnMouseLeave() noexcept override;
	void OnMouseEnter() noexcept override;
	void OnWheelDelta(short delta) noexcept override;

private:
	void TrimBuffer() noexcept;
	void OnWheelUp() noexcept;
	void OnWheelDown() noexcept;
	void ClearState() noexcept;

	std::uint16_t ProcessState(
		std::uint64_t currentState, std::uint16_t newFlag
	) noexcept;

private:
	static constexpr std::uint32_t s_bufferSize = 16u;
	bool m_inWindow;
	float m_mouseTicks;
	Vector2 m_cursorPosition;
	int m_wheelDeltaCarry;

	std::bitset<16u> m_mouseState;
	std::queue<Event> m_buffer;
};
#endif
