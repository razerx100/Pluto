#ifndef __I_GAMEPAD_HPP__
#define __I_GAMEPAD_HPP__

#ifdef BUILD_PLUTO
#define PLUTO_DLL __declspec(dllexport)
#else
#define PLUTO_DLL __declspec(dllimport)
#endif

enum class PLUTO_DLL XBoxButton {
	X,
	Y,
	A,
	B,
	RB,
	LB,
	Up,
	Down,
	Left,
	Right
};

class PLUTO_DLL IGamepad {
public:
	virtual ~IGamepad() = default;

	virtual bool IsButtonPressed(XBoxButton button) const noexcept = 0;
};
#endif
