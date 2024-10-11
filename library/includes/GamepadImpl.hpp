#ifndef GAMEPAD_IMPL_HPP_
#define GAMEPAD_IMPL_HPP_
#include <queue>
#include <bitset>
#include <Gamepad.hpp>

class GamepadImpl final : public Gamepad
{
public:
	GamepadImpl();

	[[nodiscard]]
	float GetCurrentLeftTriggerData() const noexcept override
	{
		return m_currentLeftTriggerData;
	}
	[[nodiscard]]
	float GetPreviousLeftTriggerData() const noexcept override
	{
		return m_previousLeftTriggerData;
	}

	[[nodiscard]]
	float GetCurrentRightTriggerData() const noexcept override
	{
		return m_currentRightTriggerData;
	}
	[[nodiscard]]
	float GetPreviousRightTriggerData() const noexcept override
	{
		return m_previousRightTriggerData;
	}

	[[nodiscard]]
	ThumbStickData GetCurrentLeftThumbStickData() const noexcept override
	{
		return m_currentLeftThumbStickData;
	}
	[[nodiscard]]
	ThumbStickData GetPreviousLeftThumbStickData() const noexcept override
	{
		return m_previousLeftThumbStickData;
	}

	[[nodiscard]]
	ThumbStickData GetCurrentRightThumbStickData() const noexcept override
	{
		return m_currentRightThumbStickData;
	}
	[[nodiscard]]
	ThumbStickData GetPreviousRightThumbStickData() const noexcept override
	{
		return m_previousRightThumbStickData;
	}

	[[nodiscard]]
	bool IsButtonPressed(XBoxButton button) const noexcept override;
	[[nodiscard]]
	bool WasButtonPressed(XBoxButton button) const noexcept override;

	[[nodiscard]]
	std::uint32_t GetLeftThumbStickDeadZone() const noexcept override
	{
		return m_leftThumbStickDeadZone;
	}
	[[nodiscard]]
	std::uint32_t GetRightThumbStickDeadZone() const noexcept override
	{
		return m_rightThumbStickDeadZone;
	}
	[[nodiscard]]
	std::uint32_t GetTriggerThreshold() const noexcept override
	{
		return m_triggerThreshold;
	}

	void ClearState() noexcept;

	void OnLeftThumbStickMove(const ThumbStickData& data) noexcept;
	void OnRightThumbStickMove(const ThumbStickData& data) noexcept;
	void OnLeftTriggerMove(float data) noexcept;
	void OnRightTriggerMove(float data) noexcept;

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

	std::bitset<s_buttonCount> m_currentButtonState;
	std::bitset<s_buttonCount> m_previousButtonState;
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
	GamepadImpl(const GamepadImpl&) = delete;
	GamepadImpl& operator=(const GamepadImpl&) = delete;

	GamepadImpl(GamepadImpl&& other) noexcept
		: m_currentButtonState{ std::move(other.m_currentButtonState) },
		m_previousButtonState{ std::move(other.m_previousButtonState) },
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
	GamepadImpl& operator=(GamepadImpl&& other) noexcept
	{
		m_currentButtonState          = std::move(other.m_currentButtonState);
		m_previousButtonState         = std::move(other.m_previousButtonState);
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
#endif
