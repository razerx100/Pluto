#ifndef __GAMEPAD_HPP__
#define __GAMEPAD_HPP__
#include <IGamepad.hpp>
#include <queue>

class Gamepad : public IGamepad {
public:
	void ClearState() noexcept override;

	Event Read() noexcept override;

	bool IsButtonPressed(XBoxButton button) const noexcept override;
	bool AreButtonsPressed(int count, ...) const noexcept override;

	void OnLeftThumbStickMove(ASData data) noexcept override;
	void OnRightThumbStickMove(ASData data) noexcept override;
	void OnLeftTriggerMove(float data) noexcept override;
	void OnRightTriggerMove(float data) noexcept override;

	void SetRawButtonState(std::uint16_t buttonFlags) noexcept override;
	void SetLeftThumbStickDeadZone(std::uint32_t deadzone) noexcept override;
	void SetRightThumbStickDeadZone(std::uint32_t deadzone) noexcept override;
	void SetTriggerThreshold(std::uint32_t threshold) noexcept override;

	std::uint32_t GetLeftThumbStickDeadZone() const noexcept override;
	std::uint32_t GetRightThumbStickDeadZone() const noexcept override;
	std::uint32_t GetTriggerThreshold() const noexcept override;

private:
	void ClearBuffer() noexcept;
	void TrimBuffer() noexcept;

private:
	static constexpr std::uint32_t s_bufferSize = 16u;
	std::uint16_t m_buttonState;
	std::queue<Event> m_eventBuffer;

	std::uint32_t m_leftThumbStickDeadZone;
	std::uint32_t m_rightThumbStickDeadZone;
	std::uint32_t m_triggerThreshold;
};
#endif
