#include <Mouse.hpp>
#include <cstdarg>

Mouse::Mouse()
	:
	m_inWindow(false),
	m_mouseTicks(0.0f),
	m_cursorPositionDelta{},
	m_wheelDeltaCarry(0),
	m_mouseState(0u) {}

PosDelta Mouse::GetPosDelta() const noexcept {
	return m_cursorPositionDelta;
}

std::int64_t Mouse::GetPosDX() const noexcept {
	return m_cursorPositionDelta.first;
}

std::int64_t Mouse::GetPosDY() const noexcept {
	return m_cursorPositionDelta.second;
}

float Mouse::GetMouseTicks() const noexcept {
	return m_mouseTicks;
}

bool Mouse::IsButtonPressed(MouseButtons button) const noexcept {
	return m_mouseState & (1u << static_cast<std::uint32_t>(button));
}

bool Mouse::AreButtonsPressed(size_t count, ...) const noexcept {
	va_list list;
	va_start(list, count);

	bool result = true;
	for (int _ = 0; _ < count; ++_)
		result = result && IsButtonPressed(va_arg(list, MouseButtons));

	va_end(list);

	return result;
}

bool Mouse::IsInWindow() const noexcept {
	return m_inWindow;
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

void Mouse::Flush() noexcept {
	m_buffer = std::queue<Event>();
	ClearState();
}

void Mouse::OnMouseMove(std::int64_t dx, int64_t dy) noexcept {
	m_cursorPositionDelta.first = dx;
	m_cursorPositionDelta.second = dy;

	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept {
	m_inWindow = true;

	m_buffer.emplace(Mouse::Event(Mouse::Event::Type::Enter));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept {
	m_inWindow = false;

	m_buffer.emplace(Mouse::Event(Mouse::Event::Type::Leave));
	TrimBuffer();
}

void Mouse::OnWheelUp() noexcept {
	m_buffer.emplace(Mouse::Event(Mouse::Event::Type::WheelUp));
	TrimBuffer();
}

void Mouse::OnWheelDown() noexcept {
	m_buffer.emplace(Mouse::Event(Mouse::Event::Type::WheelDown));
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept {
	while (m_buffer.size() > s_bufferSize)
		m_buffer.pop();
}

void Mouse::OnWheelDelta(std::int16_t delta) noexcept {
	m_mouseTicks = static_cast<float>(delta) / 120;
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

void Mouse::SetPressState(std::uint8_t pressState) noexcept {
	m_mouseState |= pressState;

	for (size_t index = 0u;
		index < static_cast<std::uint32_t>(MouseButtons::Invalid); ++index)
		if (pressState & (1u << index)) {
			m_buffer.emplace(Mouse::Event(
				Mouse::Event::Type::Press, static_cast<MouseButtons>(index)
			));

			TrimBuffer();
		}
}

void Mouse::SetReleaseState(std::uint8_t releaseState) noexcept {
	m_mouseState ^= releaseState;

	for (size_t index = 0u;
		index < static_cast<std::uint32_t>(MouseButtons::Invalid); ++index)
		if (releaseState & (1u << index)) {
			m_buffer.emplace(Mouse::Event(
				Mouse::Event::Type::Release, static_cast<MouseButtons>(index)
			));

			TrimBuffer();
		}
}

void Mouse::ClearState() noexcept {
	m_mouseState = 0u;
}
