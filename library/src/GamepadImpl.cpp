#include <GamepadImpl.hpp>
#include <cstdarg>

GamepadImpl::GamepadImpl()
	: m_buttonState{ 0u }, m_eventBuffer{},
	m_leftTriggerBuffer{}, m_rightTriggerBuffer{},
	m_leftThumbStickBuffer{}, m_rightThumbStickBuffer{},
	m_leftThumbStickDeadZone{ 0u }, m_rightThumbStickDeadZone{ 0u },
	m_triggerThreshold{ 0u }
{}

bool GamepadImpl::IsButtonPressed(XBoxButton button) const noexcept
{
	return m_buttonState[static_cast<size_t>(button)];
}

bool GamepadImpl::AreButtonsPressed(size_t count, ...) const noexcept
{
	va_list list = nullptr;
	va_start(list, count);

	bool result = true;
	for (int _ = 0; _ < count; ++_)
		result = result && IsButtonPressed(va_arg(list, XBoxButton));

	va_end(list);

	return result;
}

void GamepadImpl::ClearBuffers() noexcept
{
	m_eventBuffer           = std::queue<Event>();
	m_leftTriggerBuffer     = std::queue<float>();
	m_rightTriggerBuffer    = std::queue<float>();
	m_leftThumbStickBuffer  = std::queue<ThumbStickData>();
	m_rightThumbStickBuffer = std::queue<ThumbStickData>();
}

void GamepadImpl::ClearState() noexcept
{
	m_buttonState.reset();
}

std::optional<GamepadImpl::Event> GamepadImpl::ReadEvent() noexcept
{
	if (!std::empty(m_eventBuffer))
	{
		Gamepad::Event _event = m_eventBuffer.front();
		m_eventBuffer.pop();

		return _event;
	}
	else
		return {};
}

void GamepadImpl::OnLeftThumbStickMove(ThumbStickData data) noexcept
{
	m_leftThumbStickBuffer.emplace(data);

	TrimBuffer(m_leftThumbStickBuffer);
}

void GamepadImpl::OnRightThumbStickMove(ThumbStickData data) noexcept
{
	m_rightThumbStickBuffer.emplace(data);

	TrimBuffer(m_rightThumbStickBuffer);
}

void GamepadImpl::OnLeftTriggerMove(float data) noexcept
{
	m_leftTriggerBuffer.emplace(data);

	TrimBuffer(m_leftTriggerBuffer);
}

void GamepadImpl::OnRightTriggerMove(float data) noexcept
{
	m_rightTriggerBuffer.emplace(data);

	TrimBuffer(m_rightTriggerBuffer);
}

void GamepadImpl::SetRawButtonState(std::uint16_t buttonFlags) noexcept
{
	constexpr size_t buttonCount = static_cast<size_t>(XBoxButton::Invalid);
	static std::bitset<16u> buttonCheckBuffer{};
	buttonCheckBuffer            = buttonFlags;

	for (size_t index = 0u; index < buttonCount; ++index)
	{
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

std::optional<float> GamepadImpl::ReadLeftTriggerData() noexcept
{
	if (!std::empty(m_leftTriggerBuffer))
	{
		float magnitude = m_leftTriggerBuffer.front();
		m_leftTriggerBuffer.pop();

		return magnitude;
	}
	else
		return {};
}

std::optional<float> GamepadImpl::ReadRightTriggerData() noexcept
{
	if (!std::empty(m_rightTriggerBuffer))
	{
		float magnitude = m_rightTriggerBuffer.front();
		m_rightTriggerBuffer.pop();

		return magnitude;
	}
	else
		return {};
}

std::optional<ThumbStickData> GamepadImpl::ReadLeftThumbStickData() noexcept
{
	if (!std::empty(m_leftThumbStickBuffer))
	{
		ThumbStickData thumbStickData = m_leftThumbStickBuffer.front();
		m_leftThumbStickBuffer.pop();

		return thumbStickData;
	}
	else
		return {};
}

std::optional<ThumbStickData> GamepadImpl::ReadRightThumbStickData() noexcept
{
	if (!std::empty(m_rightThumbStickBuffer))
	{
		ThumbStickData thumbStickData = m_rightThumbStickBuffer.front();
		m_rightThumbStickBuffer.pop();

		return thumbStickData;
	}
	else
		return {};
}

void GamepadImpl::Flush() noexcept
{
	ClearState();
	ClearBuffers();
}
