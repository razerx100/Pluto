#include <PlatformSpecificFunctions.hpp>
#include <CleanWin.hpp>
#include <Xinput.h>

void SetGamepadData(std::vector<GamepadImpl>& gamepads)
{
	for (GamepadImpl& gamepad : gamepads)
	{
		gamepad.SetLeftThumbStickDeadZone(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		gamepad.SetRightThumbStickDeadZone(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		gamepad.SetTriggerThreshold(XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}
}
