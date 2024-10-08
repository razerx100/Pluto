#ifndef GAMEPAD_IMPL_HPP_
#define GAMEPAD_IMPL_HPP_
#include <queue>
#include <bitset>
#include <Gamepad.hpp>

class GamepadImpl final : public Gamepad
{
public:
	GamepadImpl();

	void ClearState() noexcept override;
	void ClearBuffers() noexcept override;
	void Flush() noexcept override;

	[[nodiscard]]
	std::optional<Event> ReadEvent() noexcept override;
	[[nodiscard]]
	std::optional<float> ReadLeftTriggerData() noexcept override;
	[[nodiscard]]
	std::optional<float> ReadRightTriggerData() noexcept override;
	[[nodiscard]]
	std::optional<ThumbStickData> ReadLeftThumbStickData() noexcept override;
	[[nodiscard]]
	std::optional<ThumbStickData> ReadRightThumbStickData() noexcept override;

	[[nodiscard]]
	bool IsButtonPressed(XBoxButton button) const noexcept override;
	[[nodiscard]]
	bool AreButtonsPressed(size_t count, ...) const noexcept override;

	void OnLeftThumbStickMove(ThumbStickData data) noexcept override;
	void OnRightThumbStickMove(ThumbStickData data) noexcept override;
	void OnLeftTriggerMove(float data) noexcept override;
	void OnRightTriggerMove(float data) noexcept override;

	void SetRawButtonState(std::uint16_t buttonFlags) noexcept override;
	void SetLeftThumbStickDeadZone(std::uint32_t deadzone) noexcept override
	{
		m_leftThumbStickDeadZone = deadzone;
	}
	void SetRightThumbStickDeadZone(std::uint32_t deadzone) noexcept override
	{
		m_rightThumbStickDeadZone = deadzone;
	}
	void SetTriggerThreshold(std::uint32_t threshold) noexcept override
	{
		m_triggerThreshold = threshold;
	}

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

private:
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept
	{
		while (std::size(buffer) > s_bufferSize)
			buffer.pop();
	}

private:
	static constexpr size_t s_bufferSize = 16u;

	std::bitset<16u>           m_buttonState;
	std::queue<Event>          m_eventBuffer;
	std::queue<float>          m_leftTriggerBuffer;
	std::queue<float>          m_rightTriggerBuffer;
	std::queue<ThumbStickData> m_leftThumbStickBuffer;
	std::queue<ThumbStickData> m_rightThumbStickBuffer;
	std::uint32_t              m_leftThumbStickDeadZone;
	std::uint32_t              m_rightThumbStickDeadZone;
	std::uint32_t              m_triggerThreshold;

public:
	GamepadImpl(const GamepadImpl&) = delete;
	GamepadImpl& operator=(const GamepadImpl&) = delete;

	GamepadImpl(GamepadImpl&& other) noexcept
		: m_buttonState{ std::move(other.m_buttonState) },
		m_eventBuffer{ std::move(other.m_eventBuffer) },
		m_leftTriggerBuffer{ std::move(other.m_leftTriggerBuffer) },
		m_rightTriggerBuffer{ std::move(other.m_rightTriggerBuffer) },
		m_leftThumbStickBuffer{ std::move(other.m_leftThumbStickBuffer) },
		m_rightThumbStickBuffer{ std::move(other.m_rightThumbStickBuffer) },
		m_leftThumbStickDeadZone{ other.m_leftThumbStickDeadZone },
		m_rightThumbStickDeadZone{ other.m_rightThumbStickDeadZone },
		m_triggerThreshold{ other.m_triggerThreshold }
	{}
	GamepadImpl& operator=(GamepadImpl&& other) noexcept
	{
		m_buttonState             = std::move(other.m_buttonState);
		m_eventBuffer             = std::move(other.m_eventBuffer);
		m_leftTriggerBuffer       = std::move(other.m_leftTriggerBuffer);
		m_rightTriggerBuffer      = std::move(other.m_rightTriggerBuffer);
		m_leftThumbStickBuffer    = std::move(other.m_leftThumbStickBuffer);
		m_rightThumbStickBuffer   = std::move(other.m_rightThumbStickBuffer);
		m_leftThumbStickDeadZone  = other.m_leftThumbStickDeadZone;
		m_rightThumbStickDeadZone = other.m_rightThumbStickDeadZone;
		m_triggerThreshold        = other.m_triggerThreshold;

		return *this;
	}
};
#endif
