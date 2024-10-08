#include <MouseImpl.hpp>
#include <cstdarg>

MouseImpl::MouseImpl()
	: m_mouseTicks{ 0.f }, m_wheelDeltaCarry{ 0 }, m_mouseState{ 0u },
	m_currentCursorCoord{ 0u, 0u }, m_inWindow{ false }
{}

float MouseImpl::GetMouseTicks() const noexcept
{
	return m_mouseTicks;
}

bool MouseImpl::IsButtonPressed(MouseButtons button) const noexcept
{
	return m_mouseState[static_cast<size_t>(button)];
}

bool MouseImpl::AreButtonsPressed(size_t count, ...) const noexcept
{
	va_list list = nullptr;
	va_start(list, count);

	bool result = true;
	for (int _ = 0; _ < count; ++_)
		result = result && IsButtonPressed(va_arg(list, MouseButtons));

	va_end(list);

	return result;
}

bool MouseImpl::IsInWindow() const noexcept
{
	return m_inWindow;
}

std::optional<Mouse::Event> MouseImpl::ReadEvents() noexcept
{
	if (!std::empty(m_eventBuffer))
	{
		Mouse::Event _event = m_eventBuffer.front();
		m_eventBuffer.pop();

		return _event;
	}
	else
		return {};
}

void MouseImpl::Flush() noexcept
{
	m_eventBuffer        = std::queue<Event>();
	m_currentCursorCoord = { 0u, 0u };
	ClearState();
}

void MouseImpl::SetCurrentCursorCoord(std::int32_t xCoord, std::int32_t yCoord) noexcept
{
	m_currentCursorCoord.x = xCoord;
	m_currentCursorCoord.y = yCoord;
}

void MouseImpl::OnMouseEnter() noexcept
{
	m_inWindow = true;

	m_eventBuffer.emplace(Mouse::Event(Mouse::Event::Type::Enter));
	TrimBuffer(m_eventBuffer);
}

void MouseImpl::OnMouseLeave() noexcept
{
	m_inWindow = false;

	m_eventBuffer.emplace(Mouse::Event(Mouse::Event::Type::Leave));
	TrimBuffer(m_eventBuffer);
}

void MouseImpl::OnWheelUp() noexcept
{
	m_eventBuffer.emplace(Mouse::Event(Mouse::Event::Type::WheelUp));
	TrimBuffer(m_eventBuffer);
}

void MouseImpl::OnWheelDown() noexcept
{
	m_eventBuffer.emplace(Mouse::Event(Mouse::Event::Type::WheelDown));
	TrimBuffer(m_eventBuffer);
}

void MouseImpl::OnWheelDelta(std::int16_t delta) noexcept
{
	m_mouseTicks       = static_cast<float>(delta) / 120;
	m_wheelDeltaCarry += delta;

	while (m_wheelDeltaCarry >= 120)
	{
		m_wheelDeltaCarry -= 120;
		OnWheelUp();
	}
	while (m_wheelDeltaCarry <= -120)
	{
		m_wheelDeltaCarry += 120;
		OnWheelDown();
	}
}

void MouseImpl::SetPressState(std::uint8_t pressState) noexcept
{
	m_mouseState |= pressState;

	constexpr size_t buttonCount = static_cast<std::uint32_t>(MouseButtons::Invalid);
	static std::bitset<8u> pressStateCheck{};
	pressStateCheck = pressState;

	for (size_t index = 0u; index < buttonCount; ++index)
		if (pressStateCheck[index])
		{
			m_eventBuffer.emplace(Mouse::Event(
				Mouse::Event::Type::Press, static_cast<MouseButtons>(index)
			));

			TrimBuffer(m_eventBuffer);
		}
}

void MouseImpl::SetReleaseState(std::uint8_t releaseState) noexcept
{
	m_mouseState ^= releaseState;

	constexpr size_t buttonCount = static_cast<std::uint32_t>(MouseButtons::Invalid);
	static std::bitset<8u> releaseStateCheck{};
	releaseStateCheck = releaseState;

	for (size_t index = 0u; index < buttonCount; ++index)
		if (releaseStateCheck[index]) {
			m_eventBuffer.emplace(Mouse::Event(
				Mouse::Event::Type::Release, static_cast<MouseButtons>(index)
			));

			TrimBuffer(m_eventBuffer);
		}
}

void MouseImpl::ClearState() noexcept
{
	m_mouseState.reset();
}

CursorCoord MouseImpl::GetCurrentCursorCoord() const noexcept
{
	return m_currentCursorCoord;
}
