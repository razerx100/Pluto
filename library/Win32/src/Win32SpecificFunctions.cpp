#include <PlatformSpecificFunctions.hpp>
#include <CleanWin.hpp>
#include <Xinput.h>

namespace Pluto
{
void SetGamepadData(std::vector<Gamepad>& gamepads) noexcept
{
	for (Gamepad& gamepad : gamepads)
	{
		gamepad.SetLeftThumbStickDeadZone(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		gamepad.SetRightThumbStickDeadZone(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		gamepad.SetTriggerThreshold(XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}
}
}
