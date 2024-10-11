#ifndef GAMEPAD_HPP_
#define GAMEPAD_HPP_
#include <cstdint>
#include <optional>

enum class XBoxButton
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	START,
	BACK,
	LEFT_THUMB,
	RIGHT_THUMB,
	LEFT_SHOULDER,
	RIGHT_SHOULDER,
	A,
	B,
	X,
	Y,
	Invalid
};

struct ThumbStickData
{
	float magnitude;
	float xDirection;
	float yDirection;
};

class Gamepad
{
public:
	virtual ~Gamepad() = default;

	[[nodiscard]]
	virtual float GetCurrentLeftTriggerData() const noexcept = 0;
	[[nodiscard]]
	virtual float GetPreviousLeftTriggerData() const noexcept = 0;

	[[nodiscard]]
	virtual float GetCurrentRightTriggerData() const noexcept = 0;
	[[nodiscard]]
	virtual float GetPreviousRightTriggerData() const noexcept = 0;

	[[nodiscard]]
	virtual ThumbStickData GetCurrentLeftThumbStickData() const noexcept = 0;
	[[nodiscard]]
	virtual ThumbStickData GetPreviousLeftThumbStickData() const noexcept = 0;

	[[nodiscard]]
	virtual ThumbStickData GetCurrentRightThumbStickData() const noexcept = 0;
	[[nodiscard]]
	virtual ThumbStickData GetPreviousRightThumbStickData() const noexcept = 0;

	[[nodiscard]]
	// Current button
	virtual bool IsButtonPressed(XBoxButton button) const noexcept = 0;

	template<typename... Keys>
	[[nodiscard]]
	bool AreButtonsPressed(Keys... buttons) const noexcept
	{
		return (IsButtonPressed(buttons) && ...);
	}

	[[nodiscard]]
	// Previous button
	virtual bool WasButtonPressed(XBoxButton button) const noexcept = 0;

	template<typename... Keys>
	[[nodiscard]]
	bool WereButtonsPressed(Keys... buttons) const noexcept
	{
		return (WasButtonPressed(buttons) && ...);
	}

	[[nodiscard]]
	virtual std::uint32_t GetLeftThumbStickDeadZone() const noexcept = 0;
	[[nodiscard]]
	virtual std::uint32_t GetRightThumbStickDeadZone() const noexcept = 0;
	[[nodiscard]]
	virtual std::uint32_t GetTriggerThreshold() const noexcept = 0;
};
#endif
