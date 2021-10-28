#include <Gamepad.hpp>
#include <cstdarg>

bool Gamepad::IsButtonPressed(XBoxButton button) const noexcept {
	return m_buttonState[static_cast<std::uint32_t>(button)];
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
