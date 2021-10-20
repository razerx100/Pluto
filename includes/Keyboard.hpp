#ifndef __KEYBOARD_HPP__
#define __KEYBOARD_HPP__
#include <queue>
#include <bitset>
#include <IKeyboard.hpp>

class Keyboard : public IKeyboard {
public:
	Keyboard() = default;

	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	bool IsKeyPressed(SKeyCodes keycode) const noexcept override;
	bool AreKeysPressed(int count, ...) const noexcept override;
	Event ReadKey() noexcept override;
	bool IsKeyEmpty() const noexcept override;
	void FlushKey() noexcept override;

	// char events
	char ReadChar() noexcept override;
	bool IsCharEmpty() const noexcept override;
	void FlushChar() noexcept override;
	void Flush() noexcept override;

	void OnKeyPressed(SKeyCodes keycode) noexcept override;
	void OnKeyReleased(SKeyCodes keycode) noexcept override;
	void OnChar(char character) noexcept override;
	void ClearState() noexcept override;

private:
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;

private:
	static constexpr std::uint32_t s_nKeys = 256u;
	static constexpr std::uint32_t s_bufferSize = 16u;
	std::bitset<s_nKeys> m_keystates;
	std::queue<Event> m_keyBuffer;
	std::queue<char> m_charBuffer;
};
#endif
