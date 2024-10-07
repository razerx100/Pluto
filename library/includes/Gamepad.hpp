#ifndef GAMEPAD_HPP_
#define GAMEPAD_HPP_
#include <queue>
#include <bitset>
#include <IGamepad.hpp>

class Gamepad final : public IGamepad
{
public:
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
	void SetLeftThumbStickDeadZone(std::uint32_t deadzone) noexcept override;
	void SetRightThumbStickDeadZone(std::uint32_t deadzone) noexcept override;
	void SetTriggerThreshold(std::uint32_t threshold) noexcept override;

	[[nodiscard]]
	std::uint32_t GetLeftThumbStickDeadZone() const noexcept override;
	[[nodiscard]]
	std::uint32_t GetRightThumbStickDeadZone() const noexcept override;
	[[nodiscard]]
	std::uint32_t GetTriggerThreshold() const noexcept override;

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
};
#endif
