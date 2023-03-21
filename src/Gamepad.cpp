#include <Gamepad.hpp>
#include <cstdarg>

bool Gamepad::IsButtonPressed(XBoxButton button) const noexcept {
	return m_buttonState[static_cast<size_t>(button)];
}

bool Gamepad::AreButtonsPressed(size_t count, ...) const noexcept {
	va_list list = nullptr;
	va_start(list, count);

	bool result = true;
	for (int _ = 0; _ < count; ++_)
		result = result && IsButtonPressed(va_arg(list, XBoxButton));

	va_end(list);

	return result;
}

void Gamepad::ClearBuffers() noexcept {
	m_eventBuffer = std::queue<Event>();
	m_leftTriggerBuffer = std::queue<float>();
	m_rightTriggerBuffer = std::queue<float>();
	m_leftThumbStickBuffer = std::queue<ThumbStickData>();
	m_rightThumbStickBuffer = std::queue<ThumbStickData>();
}

void Gamepad::ClearState() noexcept {
	m_buttonState = 0u;
}

std::optional<Gamepad::Event> Gamepad::ReadEvent() noexcept {
	if (!std::empty(m_eventBuffer)) {
		Gamepad::Event _event = m_eventBuffer.front();
		m_eventBuffer.pop();
		return _event;
	}
	else
		return {};
}

void Gamepad::OnLeftThumbStickMove(ThumbStickData data) noexcept {
	m_leftThumbStickBuffer.emplace(data);
	TrimBuffer(m_leftThumbStickBuffer);
}

void Gamepad::OnRightThumbStickMove(ThumbStickData data) noexcept {
	m_rightThumbStickBuffer.emplace(data);
	TrimBuffer(m_rightThumbStickBuffer);
}

void Gamepad::OnLeftTriggerMove(float data) noexcept {
	m_leftTriggerBuffer.emplace(data);
	TrimBuffer(m_leftTriggerBuffer);
}

void Gamepad::OnRightTriggerMove(float data) noexcept {
	m_rightTriggerBuffer.emplace(data);
	TrimBuffer(m_rightTriggerBuffer);
}

void Gamepad::SetRawButtonState(std::uint16_t buttonFlags) noexcept {
	constexpr size_t buttonCount = static_cast<size_t>(XBoxButton::Invalid);
	static std::bitset<16u> buttonCheckBuffer{};
	buttonCheckBuffer = buttonFlags;

	for (size_t index = 0u; index < buttonCount; ++index) {
		if (buttonCheckBuffer[index])
			m_eventBuffer.emplace(Gamepad::Event(
				Gamepad::Event::Type::Press, static_cast<XBoxButton>(index)
			));
		else
			if (m_buttonState[index])
				m_eventBuffer.emplace(Gamepad::Event(
					Gamepad::Event::Type::Release, static_cast<XBoxButton>(index)
				));

		TrimBuffer(m_eventBuffer);
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

std::optional<float> Gamepad::ReadLeftTriggerData() noexcept {
	if (!std::empty(m_leftTriggerBuffer)) {
		float magnitude = m_leftTriggerBuffer.front();
		m_leftTriggerBuffer.pop();

		return magnitude;
	}
	else
		return {};
}

std::optional<float> Gamepad::ReadRightTriggerData() noexcept {
	if (!std::empty(m_rightTriggerBuffer)) {
		float magnitude = m_rightTriggerBuffer.front();
		m_rightTriggerBuffer.pop();

		return magnitude;
	}
	else
		return {};
}

std::optional<ThumbStickData> Gamepad::ReadLeftThumbStickData() noexcept {
	if (!std::empty(m_leftThumbStickBuffer)) {
		ThumbStickData thumbStickData = m_leftThumbStickBuffer.front();
		m_leftThumbStickBuffer.pop();

		return thumbStickData;
	}
	else
		return {};
}

std::optional<ThumbStickData> Gamepad::ReadRightThumbStickData() noexcept {
	if (!std::empty(m_rightThumbStickBuffer)) {
		ThumbStickData thumbStickData = m_rightThumbStickBuffer.front();
		m_rightThumbStickBuffer.pop();

		return thumbStickData;
	}
	else
		return {};
}

void Gamepad::Flush() noexcept {
	ClearState();
	ClearBuffers();
}
