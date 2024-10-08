#include <KeyboardImpl.hpp>
#include <cstdarg>

bool KeyboardImpl::IsKeyPressed(SKeyCodes keycode) const noexcept
{
	return m_keystates[static_cast<size_t>(keycode)];
}

bool KeyboardImpl::AreKeysPressed(size_t count, ...) const noexcept
{
	va_list list = nullptr;
	va_start(list, count);

	bool result = true;
	for (size_t _ = 0; _ < count; ++_)
		result = result && IsKeyPressed(va_arg(list, SKeyCodes));

	va_end(list);

	return result;
}

std::optional<KeyboardImpl::Event> KeyboardImpl::ReadKey() noexcept
{
	if (!std::empty(m_keyBuffer))
	{
		Keyboard::Event _event = m_keyBuffer.front();
		m_keyBuffer.pop();

		return _event;
	}
	else
		return {};
}

std::optional<char> KeyboardImpl::ReadChar() noexcept
{
	if (!std::empty(m_charBuffer))
	{
		char charCode = m_charBuffer.front();
		m_charBuffer.pop();

		return charCode;
	}
	else
		return {};
}

void KeyboardImpl::FlushChar() noexcept
{
	m_charBuffer = std::queue<char>();
}

void KeyboardImpl::FlushKey() noexcept
{
	m_keyBuffer = std::queue<Event>();
}

void KeyboardImpl::Flush() noexcept
{
	FlushKey();
	FlushChar();
	ClearState();
}

void KeyboardImpl::OnKeyPressed(SKeyCodes keycode) noexcept
{
	m_keystates[static_cast<size_t>(keycode)] = true;
	m_keyBuffer.emplace(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	TrimBuffer(m_keyBuffer);
}

void KeyboardImpl::OnKeyReleased(SKeyCodes keycode) noexcept
{
	m_keystates[static_cast<size_t>(keycode)] = false;
	m_keyBuffer.emplace(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	TrimBuffer(m_keyBuffer);
}

void KeyboardImpl::OnChar(char character) noexcept
{
	m_charBuffer.emplace(character);
	TrimBuffer(m_charBuffer);
}

void KeyboardImpl::ClearState() noexcept
{
	m_keystates.reset();
}
