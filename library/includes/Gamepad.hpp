#ifndef PLUTO_GAMEPAD_HPP_
#define PLUTO_GAMEPAD_HPP_
#include <cstdint>
#include <queue>
#include <bitset>
#include <optional>

namespace Pluto
{
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
	Gamepad();

	[[nodiscard]]
	float GetCurrentLeftTriggerData() const noexcept
	{
		return m_currentLeftTriggerData;
	}
	[[nodiscard]]
	float GetPreviousLeftTriggerData() const noexcept
	{
		return m_previousLeftTriggerData;
	}

	[[nodiscard]]
	float GetCurrentRightTriggerData() const noexcept
	{
		return m_currentRightTriggerData;
	}
	[[nodiscard]]
	float GetPreviousRightTriggerData() const noexcept
	{
		return m_previousRightTriggerData;
	}

	[[nodiscard]]
	ThumbStickData GetCurrentLeftThumbStickData() const noexcept
	{
		return m_currentLeftThumbStickData;
	}
	[[nodiscard]]
	ThumbStickData GetPreviousLeftThumbStickData() const noexcept
	{
		return m_previousLeftThumbStickData;
	}

	[[nodiscard]]
	ThumbStickData GetCurrentRightThumbStickData() const noexcept
	{
		return m_currentRightThumbStickData;
	}
	[[nodiscard]]
	ThumbStickData GetPreviousRightThumbStickData() const noexcept
	{
		return m_previousRightThumbStickData;
	}

	[[nodiscard]]
	bool IsButtonPressed(XBoxButton button) const noexcept;

	[[nodiscard]]
	std::uint32_t GetLeftThumbStickDeadZone() const noexcept
	{
		return m_leftThumbStickDeadZone;
	}
	[[nodiscard]]
	std::uint32_t GetRightThumbStickDeadZone() const noexcept
	{
		return m_rightThumbStickDeadZone;
	}
	[[nodiscard]]
	std::uint32_t GetTriggerThreshold() const noexcept
	{
		return m_triggerThreshold;
	}

	void ClearState() noexcept;

	void SetLeftThumbStickData(const ThumbStickData& data) noexcept;
	void SetRightThumbStickData(const ThumbStickData& data) noexcept;
	void SetLeftTriggerData(float data) noexcept;
	void SetRightTriggerData(float data) noexcept;

	void SetRawButtonState(std::uint16_t buttonFlags) noexcept;
	void SetLeftThumbStickDeadZone(std::uint32_t deadzone) noexcept
	{
		m_leftThumbStickDeadZone = deadzone;
	}
	void SetRightThumbStickDeadZone(std::uint32_t deadzone) noexcept
	{
		m_rightThumbStickDeadZone = deadzone;
	}
	void SetTriggerThreshold(std::uint32_t threshold) noexcept
	{
		m_triggerThreshold = threshold;
	}

private:
	static constexpr size_t s_buttonCount = static_cast<size_t>(XBoxButton::Invalid);

	std::bitset<s_buttonCount> m_buttonsState;
	float                      m_currentLeftTriggerData;
	float                      m_previousLeftTriggerData;
	float                      m_currentRightTriggerData;
	float                      m_previousRightTriggerData;
	ThumbStickData             m_currentLeftThumbStickData;
	ThumbStickData             m_previousLeftThumbStickData;
	ThumbStickData             m_currentRightThumbStickData;
	ThumbStickData             m_previousRightThumbStickData;
	std::uint32_t              m_leftThumbStickDeadZone;
	std::uint32_t              m_rightThumbStickDeadZone;
	std::uint32_t              m_triggerThreshold;

public:
	Gamepad(const Gamepad&) = delete;
	Gamepad& operator=(const Gamepad&) = delete;

	Gamepad(Gamepad&& other) noexcept
		: m_buttonsState{ std::move(other.m_buttonsState) },
		m_currentLeftTriggerData{ other.m_currentLeftTriggerData },
		m_previousLeftTriggerData{ other.m_previousLeftTriggerData },
		m_currentRightTriggerData{ other.m_currentRightTriggerData },
		m_previousRightTriggerData{ other.m_previousRightTriggerData },
		m_currentLeftThumbStickData{ other.m_currentLeftThumbStickData },
		m_previousLeftThumbStickData{ other.m_previousLeftThumbStickData },
		m_currentRightThumbStickData{ other.m_currentRightThumbStickData },
		m_previousRightThumbStickData{ other.m_previousRightThumbStickData },
		m_leftThumbStickDeadZone{ other.m_leftThumbStickDeadZone },
		m_rightThumbStickDeadZone{ other.m_rightThumbStickDeadZone },
		m_triggerThreshold{ other.m_triggerThreshold }
	{}
	Gamepad& operator=(Gamepad&& other) noexcept
	{
		m_buttonsState                = std::move(other.m_buttonsState);
		m_currentLeftTriggerData      = other.m_currentLeftTriggerData;
		m_previousLeftTriggerData     = other.m_previousLeftTriggerData;
		m_currentRightTriggerData     = other.m_currentRightTriggerData;
		m_previousRightTriggerData    = other.m_previousRightTriggerData;
		m_currentLeftThumbStickData   = other.m_currentLeftThumbStickData;
		m_previousLeftThumbStickData  = other.m_previousLeftThumbStickData;
		m_currentRightThumbStickData  = other.m_currentRightThumbStickData;
		m_previousRightThumbStickData = other.m_previousRightThumbStickData;
		m_leftThumbStickDeadZone      = other.m_leftThumbStickDeadZone;
		m_rightThumbStickDeadZone     = other.m_rightThumbStickDeadZone;
		m_triggerThreshold            = other.m_triggerThreshold;

		return *this;
	}
};
}
#endif
