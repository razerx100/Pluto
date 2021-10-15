#include <Mouse.hpp>

Mouse::Mouse()
	:
	m_inWindow(false),
	m_wheelDeltaCarry(0),
	m_mouseData{} {}

Vector2 Mouse::GetPos() const noexcept {
	return { m_mouseData.x, m_mouseData.y };
}

int Mouse::GetPosX() const noexcept {
	return m_mouseData.x;
}

int Mouse::GetPosY() const noexcept {
	return m_mouseData.y;
}

bool Mouse::IsInWindow() const noexcept {
	return m_inWindow;
}

bool Mouse::IsLeftPressed() const noexcept {
	return m_mouseData.leftPressed;
}

bool Mouse::IsMiddlePressed() const noexcept {
	return m_mouseData.middlePressed;
}

bool Mouse::IsRightPressed() const noexcept {
	return m_mouseData.rightPressed;
}

Mouse::Event Mouse::Read() noexcept {
	if (!m_buffer.empty()) {
		Mouse::Event e = m_buffer.front();
		m_buffer.pop();
		return e;
	}
	else
		return Mouse::Event();
}

bool Mouse::IsBufferEmpty() const noexcept {
	return m_buffer.empty();
}

void Mouse::Flush() noexcept {
	m_buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int x, int y) noexcept {
	m_mouseData.x = x;
	m_mouseData.y = y;

	m_buffer.emplace(Mouse::Event(Mouse::Event::Type::Move, m_mouseData));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept {
	m_inWindow = true;

	m_buffer.emplace(Mouse::Event(Mouse::Event::Type::Enter, m_mouseData));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept {
	m_inWindow = false;

	m_buffer.emplace(Mouse::Event(Mouse::Event::Type::Leave, m_mouseData));
	TrimBuffer();
}

void Mouse::OnLeftPress() noexcept {
	m_mouseData.leftPressed = true;

	m_buffer.emplace(Mouse::Event(Mouse::Event::Type::LPress, m_mouseData));
	TrimBuffer();
}

void Mouse::OnMiddlePress() noexcept {
	m_mouseData.middlePressed = true;

	m_buffer.emplace(Mouse::Event(Mouse::Event::Type::MPress, m_mouseData));
	TrimBuffer();
}

void Mouse::OnRightPress() noexcept {
	m_mouseData.rightPressed = true;

	m_buffer.emplace(Mouse::Event(Mouse::Event::Type::RPress, m_mouseData));
	TrimBuffer();
}

void Mouse::OnLeftRelease() noexcept {
	m_mouseData.leftPressed = false;

	m_buffer.emplace(Mouse::Event(Mouse::Event::Type::LRelease, m_mouseData));
	TrimBuffer();
}

void Mouse::OnMiddleRelease() noexcept {
	m_mouseData.middlePressed = false;

	m_buffer.emplace(Mouse::Event(Mouse::Event::Type::MRelease, m_mouseData));
	TrimBuffer();
}

void Mouse::OnRightRelease() noexcept {
	m_mouseData.rightPressed = false;

	m_buffer.emplace(Mouse::Event(Mouse::Event::Type::RRelease, m_mouseData));
	TrimBuffer();
}

void Mouse::OnWheelUp() noexcept {
	m_buffer.emplace(Mouse::Event(Mouse::Event::Type::WheelUp, m_mouseData));
	TrimBuffer();
}

void Mouse::OnWheelDown() noexcept {
	m_buffer.emplace(Mouse::Event(Mouse::Event::Type::WheelDown, m_mouseData));
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept {
	while (m_buffer.size() > s_bufferSize)
		m_buffer.pop();
}

void Mouse::OnWheelDelta(int delta) noexcept {
	m_wheelDeltaCarry += delta;

	while (m_wheelDeltaCarry >= 120) {
		m_wheelDeltaCarry -= 120;
		OnWheelUp();
	}
	while (m_wheelDeltaCarry <= -120) {
		m_wheelDeltaCarry += 120;
		OnWheelDown();
	}
}
