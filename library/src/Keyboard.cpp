#include <Keyboard.hpp>

namespace Pluto
{
bool Keyboard::IsKeyPressed(SKeyCodes keycode) const noexcept
{
	return m_keystates[static_cast<size_t>(keycode)];
}

std::optional<char> Keyboard::GetCurrentCharacter() const noexcept
{
	constexpr char unusedKey = static_cast<char>(129u);

	return m_currentCharacter == unusedKey ? std::optional<char>{} : m_currentCharacter;
}

void Keyboard::OnKeyPressed(SKeyCodes keycode) noexcept
{
	m_keystates[static_cast<size_t>(keycode)] = true;
}

void Keyboard::OnKeyReleased(SKeyCodes keycode) noexcept
{
	m_keystates[static_cast<size_t>(keycode)] = false;
}

void Keyboard::SetChar(char character) noexcept
{
	m_currentCharacter = character;
}

void Keyboard::ClearState() noexcept
{
	m_keystates.reset();

	m_currentCharacter = static_cast<char>(129u);
}
}
