#include <GamepadImpl.hpp>

GamepadImpl::GamepadImpl()
	: m_buttonsState{ 0u },
	m_currentLeftTriggerData{ 0.f }, m_previousLeftTriggerData{ 0.f },
	m_currentRightTriggerData{ 0.f }, m_previousRightTriggerData{ 0.f },
	m_currentLeftThumbStickData{ 0.f, 0.f, 0.f }, m_previousLeftThumbStickData{ 0.f, 0.f, 0.f },
	m_currentRightThumbStickData{ 0.f, 0.f, 0.f }, m_previousRightThumbStickData{ 0.f, 0.f, 0.f },
	m_leftThumbStickDeadZone{ 0u }, m_rightThumbStickDeadZone{ 0u }, m_triggerThreshold{ 0u }
{}

bool GamepadImpl::IsButtonPressed(XBoxButton button) const noexcept
{
	return m_buttonsState[static_cast<size_t>(button)];
}

void GamepadImpl::ClearState() noexcept
{
	m_buttonsState.reset();

	m_currentLeftTriggerData      = 0.f;
	m_previousLeftTriggerData     = 0.f;
	m_currentRightTriggerData     = 0.f;
	m_previousRightTriggerData    = 0.f;
	m_currentLeftThumbStickData   = ThumbStickData{ 0.f, 0.f, 0.f };
	m_previousLeftThumbStickData  = ThumbStickData{ 0.f, 0.f, 0.f };
	m_currentRightThumbStickData  = ThumbStickData{ 0.f, 0.f, 0.f };
	m_previousRightThumbStickData = ThumbStickData{ 0.f, 0.f, 0.f };
	m_leftThumbStickDeadZone      = 0u;
	m_rightThumbStickDeadZone     = 0u;
	m_triggerThreshold            = 0u;
}

void GamepadImpl::SetLeftThumbStickData(const ThumbStickData& data) noexcept
{
	m_previousLeftThumbStickData = m_currentLeftThumbStickData;
	m_currentLeftThumbStickData  = data;
}

void GamepadImpl::SetRightThumbStickData(const ThumbStickData& data) noexcept
{
	m_previousRightThumbStickData = m_currentRightThumbStickData;
	m_currentRightThumbStickData  = data;
}

void GamepadImpl::SetLeftTriggerData(float data) noexcept
{
	m_previousLeftTriggerData = m_currentLeftTriggerData;
	m_currentLeftTriggerData  = data;
}

void GamepadImpl::SetRightTriggerData(float data) noexcept
{
	m_previousRightTriggerData = m_currentRightTriggerData;
	m_currentRightTriggerData  = data;
}

void GamepadImpl::SetRawButtonState(std::uint16_t buttonFlags) noexcept
{
	m_buttonsState = buttonFlags;
}
