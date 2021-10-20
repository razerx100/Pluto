#include <Keyboard.hpp>
#include <cstdarg>

bool Keyboard::IsKeyPressed(SKeyCodes keycode) const noexcept {
	return m_keystates[static_cast<std::uint32_t>(keycode)];
}

bool Keyboard::AreKeysPressed(int count, ...) const noexcept {
	va_list list;
	va_start(list, count);

	bool result = true;
	for (int _ = 0; _ < count; ++_)
		result = result && IsKeyPressed(va_arg(list, SKeyCodes));

	va_end(list);

	return result;
}

Keyboard::Event Keyboard::ReadKey() noexcept {
	if (!m_keyBuffer.empty()) {
		Keyboard::Event e = m_keyBuffer.front();
		m_keyBuffer.pop();
		return e;
	}
	else
		return Keyboard::Event();
}

bool Keyboard::IsKeyEmpty() const noexcept {
	return m_keyBuffer.empty();
}

char Keyboard::ReadChar() noexcept {
	if (!m_charBuffer.empty()) {
		unsigned char charCode = m_charBuffer.front();
		m_charBuffer.pop();
		return charCode;
	}
	else
		return -1;
}

bool Keyboard::IsCharEmpty() const noexcept {
	return m_charBuffer.empty();
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
	m_keystates[static_cast<std::uint32_t>(keycode)] = true;
	m_keyBuffer.emplace(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	TrimBuffer(m_keyBuffer);
}

void Keyboard::OnKeyReleased(SKeyCodes keycode) noexcept {
	m_keystates[static_cast<std::uint32_t>(keycode)] = false;
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

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept {
	while (buffer.size() > s_bufferSize)
		buffer.pop();
}
