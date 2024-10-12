#include <KeyboardImpl.hpp>

bool KeyboardImpl::IsKeyPressed(SKeyCodes keycode) const noexcept
{
	return m_keystates[static_cast<size_t>(keycode)];
}

std::optional<char> KeyboardImpl::GetCurrentCharacter() const noexcept
{
	constexpr char unusedKey = static_cast<char>(129u);

	return m_currentCharacter == unusedKey ? std::optional<char>{} : m_currentCharacter;
}

void KeyboardImpl::OnKeyPressed(SKeyCodes keycode) noexcept
{
	m_keystates[static_cast<size_t>(keycode)] = true;
}

void KeyboardImpl::OnKeyReleased(SKeyCodes keycode) noexcept
{
	m_keystates[static_cast<size_t>(keycode)] = false;
}

void KeyboardImpl::SetChar(char character) noexcept
{
	m_currentCharacter = character;
}

void KeyboardImpl::ClearState() noexcept
{
	m_keystates.reset();

	m_currentCharacter = static_cast<char>(129u);
}
