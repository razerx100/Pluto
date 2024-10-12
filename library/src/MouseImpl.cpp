#include <MouseImpl.hpp>

MouseImpl::MouseImpl()
	: m_mouseState{ 0u }, m_currentCursorCoord{ 0u, 0u },
	m_mouseWheelDelta{ 0.f }, m_wheelRotationCount{ 0u },
	m_isWheelUp{ false }, m_isWheelDown{ false }, m_wheelDeltaCarry{ 0 }, m_inWindow{ false }
{}

bool MouseImpl::IsButtonPressed(MouseButton button) const noexcept
{
	return m_mouseState[static_cast<size_t>(button)];
}

void MouseImpl::SetCurrentCursorCoord(std::int32_t xCoord, std::int32_t yCoord) noexcept
{
	m_currentCursorCoord.x = xCoord;
	m_currentCursorCoord.y = yCoord;
}

void MouseImpl::OnMouseEnter() noexcept
{
	m_inWindow = true;
}

void MouseImpl::OnMouseLeave() noexcept
{
	m_inWindow = false;

	SetCurrentCursorCoord(0, 0);
}

void MouseImpl::SetWheelDelta(std::int16_t delta) noexcept
{
	constexpr std::int32_t deltaUnit = 120;

	m_mouseWheelDelta  = static_cast<float>(delta) / deltaUnit;
	m_wheelDeltaCarry += delta;

	if (!delta)
	{
		m_wheelDeltaCarry    = 0;
		m_isWheelUp          = false;
		m_isWheelDown        = false;
		m_wheelRotationCount = 0u;

		return;
	}

	while (m_wheelDeltaCarry >= deltaUnit)
	{
		m_wheelDeltaCarry -= deltaUnit;
		m_isWheelUp        = true;

		++m_wheelRotationCount;
	}

	while (m_wheelDeltaCarry <= -deltaUnit)
	{
		m_wheelDeltaCarry += deltaUnit;
		m_isWheelDown      = true;

		++m_wheelRotationCount;
	}
}

void MouseImpl::SetPressState(std::uint8_t pressState) noexcept
{
	m_mouseState |= pressState;
}

void MouseImpl::SetReleaseState(std::uint8_t releaseState) noexcept
{
	m_mouseState ^= releaseState;
}

void MouseImpl::ClearState() noexcept
{
	m_mouseState.reset();

	m_wheelDeltaCarry    = 0;
	m_isWheelUp          = false;
	m_isWheelDown        = false;
	m_wheelRotationCount = 0u;

	SetCurrentCursorCoord(0, 0);
}
