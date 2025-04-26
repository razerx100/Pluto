#ifndef PLUTO_KEYBOARD_HPP_
#define PLUTO_KEYBOARD_HPP_
#include <queue>
#include <bitset>
#include <optional>

namespace Pluto
{
enum class SKeyCodes
{
	Default,
	BackSpace,
	Tab,
	Enter,
	Shift,
	Ctrl,
	Alt,
	CapsLock,
	Esc,
	SpaceBar,
	PageUp,
	PageDown,
	End,
	Home,
	LeftArrow,
	UpArrow,
	RightArrow,
	DownArrow,
	PrintScreen,
	Ins,
	Del,
	Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	Super,
	ZeroNumpad, OneNumpad, TwoNumpad, ThreeNumpad, FourNumpad, FiveNumpad, SixNumpad,
	SevenNumpad, EightNumpad, NineNumpad,
	Multiply, Divide, Add, Subtract, Decimal,
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	NumLock, ScrollLock,
	ShiftLeft, ShiftRight,
	CtrlLeft, CtrlRight,
	AltLeft, AltRight,
	SemiColonUS,
	Plus,
	Comma,
	Hyphen,
	Period,
	SlashUS,
	TildeUS,
	BraceStartUS, BraceEndUS,
	BackSlashUS,
	QuoteUS
};

class Keyboard
{
	friend void PlutoWin32InputCallback(
		class InputManager& inputManager,
		void* hwnd, std::uint32_t message, std::uint64_t wParameter, std::uint64_t lParameter
	);

public:
	Keyboard() : m_keystates{ 0u }, m_currentCharacter{ 0u } {}

	[[nodiscard]]
	bool IsKeyPressed(SKeyCodes keycode) const noexcept;

	[[nodiscard]]
	std::optional<char> GetCurrentCharacter() const noexcept;

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
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	Keyboard(Keyboard&& other) noexcept
		: m_keystates{ std::move(other.m_keystates) },
		m_currentCharacter{ other.m_currentCharacter }
	{}
	Keyboard& operator=(Keyboard&& other) noexcept
	{
		m_keystates        = std::move(other.m_keystates);
		m_currentCharacter = other.m_currentCharacter;

		return *this;
	}
};
}
#endif
