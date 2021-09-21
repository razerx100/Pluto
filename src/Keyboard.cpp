#include<Keyboard.hpp>

Keyboard::Keyboard()
	: m_autoRepeatEnabled(false), m_nativeKeyCodeGetter(nullptr) {}

bool Keyboard::IsKeyPressed(SKeyCodes keycode) const noexcept {
	return m_keystates[keycode];
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
}

void Keyboard::EnableAutoRepeat() noexcept {
	m_autoRepeatEnabled = true;
}

void Keyboard::DisableAutoRepeat() noexcept {
	m_autoRepeatEnabled = false;
}

bool Keyboard::IsAutoRepeatEnabled() const noexcept {
	return m_autoRepeatEnabled;
}

void Keyboard::OnKeyPressed(std::uint16_t keycode) noexcept {
	SKeyCodes sKeyCode = m_nativeKeyCodeGetter(keycode);

	m_keystates[sKeyCode] = true;
	m_keyBuffer.emplace(Keyboard::Event(Keyboard::Event::Type::Press, sKeyCode));
	TrimBuffer(m_keyBuffer);
}

void Keyboard::OnKeyReleased(std::uint16_t keycode) noexcept {
	SKeyCodes sKeyCode = m_nativeKeyCodeGetter(keycode);

	m_keystates[sKeyCode] = false;
	m_keyBuffer.emplace(Keyboard::Event(Keyboard::Event::Type::Release, sKeyCode));
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

void Keyboard::SetNativeKeyCodeGetter(
	SKeyCodes(*NativeKeyCodeGetter)(std::uint16_t)
) noexcept {
	m_nativeKeyCodeGetter = NativeKeyCodeGetter;
}