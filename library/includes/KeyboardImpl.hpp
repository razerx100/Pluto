#ifndef KEYBOARD_IMPL_HPP_
#define KEYBOARD_IMPL_HPP_
#include <queue>
#include <bitset>
#include <Keyboard.hpp>

class KeyboardImpl final : public Keyboard
{
	friend void PlutoWin32InputCallback(
		class InputManagerImpl& inputManager,
		void* hwnd, std::uint32_t message, std::uint64_t wParameter, std::uint64_t lParameter
	);

public:
	KeyboardImpl() : m_keystates{ 0u }, m_currentCharacter{ 0u } {}

	[[nodiscard]]
	bool IsKeyPressed(SKeyCodes keycode) const noexcept override;

	[[nodiscard]]
	std::optional<char> GetCurrentCharacter() const noexcept override;

	void OnKeyPressed(SKeyCodes keycode) noexcept;
	void OnKeyReleased(SKeyCodes keycode) noexcept;
	void SetChar(char character) noexcept;

	void ClearState() noexcept;

private:
	static constexpr std::uint32_t s_nKeys      = 256u;
	static constexpr std::uint32_t s_bufferSize = 16u;

	std::bitset<s_nKeys> m_keystates;
	char                 m_currentCharacter;

public:
	KeyboardImpl(const KeyboardImpl&) = delete;
	KeyboardImpl& operator=(const KeyboardImpl&) = delete;

	KeyboardImpl(KeyboardImpl&& other) noexcept
		: m_keystates{ std::move(other.m_keystates) },
		m_currentCharacter{ other.m_currentCharacter }
	{}
	KeyboardImpl& operator=(KeyboardImpl&& other) noexcept
	{
		m_keystates        = std::move(other.m_keystates);
		m_currentCharacter = other.m_currentCharacter;

		return *this;
	}
};
#endif
