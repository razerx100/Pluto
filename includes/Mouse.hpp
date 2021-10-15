#ifndef __MOUSE_HPP__
#define __MOUSE_HPP__
#include <queue>
#include <IMouse.hpp>

class Mouse : public IMouse {
public:
	Mouse();

	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	Vector2 GetPos() const noexcept override;
	int GetPosX() const noexcept override;
	int GetPosY() const noexcept override;
	Event Read() noexcept override;

	bool IsInWindow() const noexcept override;
	bool IsLeftPressed() const noexcept override;
	bool IsMiddlePressed() const noexcept override;
	bool IsRightPressed() const noexcept override;
	bool IsBufferEmpty() const noexcept override;

	void Flush() noexcept override;

	void OnMouseMove(int x, int y) noexcept override;
	void OnMouseLeave() noexcept override;
	void OnMouseEnter() noexcept override;
	void OnLeftPress() noexcept override;
	void OnMiddlePress() noexcept override;
	void OnRightPress() noexcept override;
	void OnLeftRelease() noexcept override;
	void OnMiddleRelease() noexcept override;
	void OnRightRelease() noexcept override;
	void OnWheelUp() noexcept override;
	void OnWheelDown() noexcept override;
	void OnWheelDelta(int delta) noexcept override;

private:
	void TrimBuffer() noexcept;

private:
	static constexpr unsigned int s_bufferSize = 16u;
	bool m_inWindow;
	int m_wheelDeltaCarry;
	MouseData m_mouseData;

	std::queue<Event> m_buffer;
};
#endif
