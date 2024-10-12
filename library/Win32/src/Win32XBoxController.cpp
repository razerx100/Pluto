#include <PlatformSpecificFunctions.hpp>
#include <Win32KeyManager.hpp>
#include <CleanWin.hpp>
#include <Xinput.h>
#include <cmath>

[[nodiscard]]
static float GetMagnitude(std::int16_t x, std::int16_t y) noexcept
{
	return std::sqrtf(
		static_cast<float>(std::pow(x, 2.f) + std::pow(y, 2.f))
	);
}

[[nodiscard]]
static float ProcessDeadZone(
	float magnitude, std::uint32_t maxValue, std::uint32_t deadZone
) noexcept {
	magnitude  = std::min(magnitude, static_cast<float>(maxValue));
	magnitude -= deadZone;

	return magnitude / (maxValue - deadZone);
}

[[nodiscard]]
static ThumbStickData ProcessThumbStickData(
	float magnitude, std::int16_t x, std::int16_t y, std::uint32_t deadZone
) noexcept {
	return ThumbStickData{
		.magnitude  = ProcessDeadZone(magnitude, 32767u, deadZone),
		.xDirection = x / magnitude,
		.yDirection = y / magnitude
	};
}

void CheckXBoxControllerStates(std::vector<GamepadImpl>& gamepads) noexcept
{
	XINPUT_STATE state{};
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	auto gamepadCount = static_cast<DWORD>(std::size(gamepads));

	for (DWORD gamepadIndex = 0u; gamepadIndex < gamepadCount; ++gamepadIndex)
	{
		if (XInputGetState(gamepadIndex, &state) == ERROR_SUCCESS)
		{
			GamepadImpl& gamepad        = gamepads[gamepadIndex];

			const XINPUT_GAMEPAD& xData = state.Gamepad;

			gamepad.SetRawButtonState(ProcessGamepadRawButtons(xData.wButtons));

			{
				ThumbStickData leftThumbStickData{ 0.f, 0.f, 0.f };

				const std::uint32_t leftStickDeadZone = gamepad.GetLeftThumbStickDeadZone();
				const float magnitude                 = GetMagnitude(xData.sThumbLX, xData.sThumbLY);

				if (magnitude > leftStickDeadZone)
					leftThumbStickData = ProcessThumbStickData(
						magnitude, xData.sThumbLX, xData.sThumbLY,
						leftStickDeadZone
					);

				gamepad.SetLeftThumbStickData(leftThumbStickData);
			}

			{
				ThumbStickData rightThumbStickData{ 0.f, 0.f, 0.f };

				const std::uint32_t rightStickDeadZone = gamepad.GetRightThumbStickDeadZone();
				const float magnitude                  = GetMagnitude(xData.sThumbRX, xData.sThumbRY);

				if (magnitude > rightStickDeadZone)
					rightThumbStickData = ProcessThumbStickData(
						magnitude, xData.sThumbRX, xData.sThumbRY,
						rightStickDeadZone
					);

				gamepad.SetRightThumbStickData(rightThumbStickData);
			}

			constexpr std::uint32_t maximumMagnitude = 255u;
			const std::uint32_t threshold            = gamepad.GetTriggerThreshold();

			{
				float leftTriggerData = 0.f;

				if (xData.bLeftTrigger > threshold)
					leftTriggerData = ProcessDeadZone(
						static_cast<float>(xData.bLeftTrigger),
						maximumMagnitude,
						threshold
					);

				gamepad.SetLeftTriggerData(leftTriggerData);
			}

			{
				float rightTriggerData = 0.f;

				if (xData.bRightTrigger > threshold)
					rightTriggerData = ProcessDeadZone(
						static_cast<float>(xData.bRightTrigger),
						255u,
						threshold
					);

				gamepad.SetRightTriggerData(rightTriggerData);
			}
		}
	}
}

void DisconnectXBoxController(std::vector<GamepadImpl>& gamepads) noexcept
{
	XINPUT_STATE state{};
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	auto gamepadCount = static_cast<DWORD>(std::size(gamepads));

	for (DWORD gamepadIndex = 0u; gamepadIndex < gamepadCount; ++gamepadIndex)
		if (XInputGetState(gamepadIndex, &state) == ERROR_DEVICE_NOT_CONNECTED)
			gamepads[gamepadIndex].ClearState();
}
