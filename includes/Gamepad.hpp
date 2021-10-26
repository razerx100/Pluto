#ifndef __GAMEPAD_HPP__
#define __GAMEPAD_HPP__
#include <IGamepad.hpp>

class Gamepad : public IGamepad {
public:
	bool IsButtonPressed(XBoxButton button) const noexcept override;
};
#endif
