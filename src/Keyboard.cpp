#include <Keyboard.hpp>
#include <cstdarg>

bool Keyboard::IsKeyPressed(SKeyCodes keycode) const noexcept {
	return m_keystates[static_cast<size_t>(keycode)];
}

bool Keyboard::AreKeysPressed(size_t count, ...) const noexcept {
	va_list list = nullptr;
	va_start(list, count);

	bool result = true;
	for (size_t _ = 0; _ < count; ++_)
		result = result && IsKeyPressed(va_arg(list, SKeyCodes));

	va_end(list);

	return result;
}

std::optional<Keyboard::Event> Keyboard::ReadKey() noexcept {
	if (!std::empty(m_keyBuffer)) {
		Keyboard::Event _event = m_keyBuffer.front();
		m_keyBuffer.pop();

		return _event;
	}
	else
		return {};
}

std::optional<char> Keyboard::ReadChar() noexcept {
	if (!std::empty(m_charBuffer)) {
		char charCode = m_charBuffer.front();
		m_charBuffer.pop();

		return charCode;
	}
	else
		return {};
}

void Keyboard::FlushChar() noexcept {
	m_charBuffer = std::queue<char>();
}

void Keyboard::FlushKey() noexcept {
	m_keyBuffer = std::queue<Event>();
}

void Keyboard::Flush() noexcept {
	FlushKey();
	FlushChar();
	ClearState();
}

void Keyboard::OnKeyPressed(SKeyCodes keycode) noexcept {
	m_keystates[static_cast<size_t>(keycode)] = true;
	m_keyBuffer.emplace(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	TrimBuffer(m_keyBuffer);
}

void Keyboard::OnKeyReleased(SKeyCodes keycode) noexcept {
	m_keystates[static_cast<size_t>(keycode)] = false;
	m_keyBuffer.emplace(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	TrimBuffer(m_keyBuffer);
}

void Keyboard::OnChar(char character) noexcept {
	m_charBuffer.emplace(character);
	TrimBuffer(m_charBuffer);
}

void Keyboard::ClearState() noexcept {
	m_keystates.reset();
}
