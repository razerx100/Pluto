#include <Gamepad.hpp>
#include <cstdarg>

bool Gamepad::IsButtonPressed(XBoxButton button) const noexcept {
	return m_buttonState & (1u << static_cast<std::uint32_t>(button));
}

bool Gamepad::AreButtonsPressed(size_t count, ...) const noexcept {
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
	m_buttonState = 0u;
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

void Gamepad::OnLeftThumbStickMove(ASData data) noexcept {
	m_eventBuffer.emplace(Gamepad::Event(Gamepad::Event::Type::LeftThumbStick, data));
	TrimBuffer();
}

void Gamepad::OnRightThumbStickMove(ASData data) noexcept {
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
	for (size_t index = 0u; index < static_cast<size_t>(XBoxButton::Invalid); ++index) {
		if (auto flag = 1u << index; buttonFlags & flag) {
			m_eventBuffer.emplace(Gamepad::Event(
				Gamepad::Event::Type::Press, static_cast<XBoxButton>(index)
			));
			TrimBuffer();
		}
		else {
			if (m_buttonState & flag) {
				m_eventBuffer.emplace(Gamepad::Event(
					Gamepad::Event::Type::Release, static_cast<XBoxButton>(index)
				));
				TrimBuffer();
			}
		}
	}

	m_buttonState = buttonFlags;
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
