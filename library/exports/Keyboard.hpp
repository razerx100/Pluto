#ifndef KEYBOARD_HPP_
#define KEYBOARD_HPP_
#include <optional>

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
public:
	virtual ~Keyboard() = default;

	[[nodiscard]]
	virtual bool IsKeyPressed(SKeyCodes keycode) const noexcept = 0;

	template<typename... Keys>
	[[nodiscard]]
	bool AreKeysPressed(Keys... keys) const noexcept
	{
		return (IsKeyPressed(keys) && ...);
	}

	[[nodiscard]]
	virtual std::optional<char> GetCurrentCharacter() const noexcept = 0;
};
#endif
