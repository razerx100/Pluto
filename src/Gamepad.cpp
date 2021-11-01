#include <Gamepad.hpp>
#include <cstdarg>

static const std::vector<std::uint16_t> buttonMap = {
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	12,
	13,
	14,
	15
};

bool Gamepad::IsButtonPressed(XBoxButton button) const noexcept {
	return m_buttonState[buttonMap[static_cast<std::uint32_t>(button)]];
}

bool Gamepad::AreButtonsPressed(int count, ...) const noexcept {
	va_list list;
	va_start(list, count);

	bool result = true;
	for (int _ = 0; _ < count; ++_)
		result = result && IsButtonPressed(va_arg(list, XBoxButton));

	va_end(list);

	return result;
}

void Gamepad::TrimBuffer() noexcept {
	while (m_eventBuffer.size() > s_bufferSize)
		m_eventBuffer.pop();
}

void Gamepad::ClearBuffer() noexcept {
	m_eventBuffer = std::queue<Event>();
}

void Gamepad::ClearState() noexcept {
	m_buttonState.reset();
	ClearBuffer();
}

Gamepad::Event Gamepad::Read() noexcept {
	if (!m_eventBuffer.empty()) {
		Gamepad::Event e = m_eventBuffer.front();
		m_eventBuffer.pop();
		return e;
	}
	else
		return Gamepad::Event();
}

bool Gamepad::IsBufferEmpty() const noexcept {
	return m_eventBuffer.empty();
}

void Gamepad::OnLeftThumbStickMove(float data) noexcept {
	m_eventBuffer.emplace(Gamepad::Event(Gamepad::Event::Type::LeftThumbStick, data));
	TrimBuffer();
}

void Gamepad::OnRightThumbStickMove(float data) noexcept {
	m_eventBuffer.emplace(Gamepad::Event(Gamepad::Event::Type::RightThumbStick, data));
	TrimBuffer();
}

void Gamepad::OnLeftTriggerMove(float data) noexcept {
	m_eventBuffer.emplace(Gamepad::Event(Gamepad::Event::Type::LeftTrigger, data));
	TrimBuffer();
}

void Gamepad::OnRightTriggerMove(float data) noexcept {
	m_eventBuffer.emplace(Gamepad::Event(Gamepad::Event::Type::RightTrigger, data));
	TrimBuffer();
}

void Gamepad::SetRawButtonState(std::uint16_t buttonFlags) noexcept {
	m_buttonState = std::bitset<16u>(buttonFlags);
}

void Gamepad::SetLeftThumbStickDeadZone(std::uint32_t deadzone) noexcept {
	m_leftThumbStickDeadZone = deadzone;
}

void Gamepad::SetRightThumbStickDeadZone(std::uint32_t deadzone) noexcept {
	m_rightThumbStickDeadZone = deadzone;
}

void Gamepad::SetTriggerThreshold(std::uint32_t threshold) noexcept {
	m_triggerThreshold = threshold;
}

std::uint32_t Gamepad::GetLeftThumbStickDeadZone() const noexcept {
	return m_leftThumbStickDeadZone;
}

std::uint32_t Gamepad::GetRightThumbStickDeadZone() const noexcept {
	return m_rightThumbStickDeadZone;
}

std::uint32_t Gamepad::GetTriggerThreshold() const noexcept {
	return m_triggerThreshold;
}
