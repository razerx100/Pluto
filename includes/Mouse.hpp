#ifndef MOUSE_HPP_
#define MOUSE_HPP_
#include <queue>
#include <IMouse.hpp>

class Mouse final : public IMouse {
public:
	Mouse();

	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	[[nodiscard]]
	float GetMouseTicks() const noexcept override;
	[[nodiscard]]
	std::optional<Event> ReadEvents() noexcept override;
	[[nodiscard]]
	std::optional<PosDelta> ReadPosDelta() noexcept override;

	[[nodiscard]]
	bool IsInWindow() const noexcept override;
	[[nodiscard]]
	bool IsButtonPressed(MouseButtons button) const noexcept override;
	[[nodiscard]]
	bool AreButtonsPressed(size_t count, ...) const noexcept override;

	void Flush() noexcept override;

	void SetPressState(std::uint8_t mouseState) noexcept override;
	void SetReleaseState(std::uint8_t mouseState) noexcept override;
	void OnMouseMove(std::int64_t dx, std::int64_t dy) noexcept override;
	void OnMouseLeave() noexcept override;
	void OnMouseEnter() noexcept override;
	void OnWheelDelta(std::int16_t delta) noexcept override;

private:
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept {
		while (std::size(buffer) > s_bufferSize)
			buffer.pop();
	}

	void OnWheelUp() noexcept;
	void OnWheelDown() noexcept;
	void ClearState() noexcept;

private:
	static constexpr size_t s_bufferSize = 16u;
	bool m_inWindow;
	float m_mouseTicks;
	int m_wheelDeltaCarry;

	std::uint8_t m_mouseState;
	std::queue<Event> m_eventBuffer;
	std::queue<PosDelta> m_posDeltaBuffer;
};
#endif
