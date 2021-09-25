#ifndef __KEYBOARD_HPP__
#define __KEYBOARD_HPP__
#include <queue>
#include <bitset>
#include <IKeyboard.hpp>

class Keyboard : public IKeyboard {
public:
	Keyboard();

	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	void SetNativeKeyCodeGetter(
		SKeyCodes(*NativeKeyCodeGetter)(std::uint16_t)
	) noexcept override;

	bool IsKeyPressed(SKeyCodes keycode) const noexcept override;
	Event ReadKey() noexcept override;
	bool IsKeyEmpty() const noexcept override;
	void FlushKey() noexcept override;

	// char events
	char ReadChar() noexcept override;
	bool IsCharEmpty() const noexcept override;
	void FlushChar() noexcept override;
	void Flush() noexcept override;

	// auto-repeat control
	void EnableAutoRepeat() noexcept override;
	void DisableAutoRepeat() noexcept override;
	bool IsAutoRepeatEnabled() const noexcept override;

	void OnKeyPressed(std::uint16_t keycode) noexcept override;
	void OnKeyReleased(std::uint16_t keycode) noexcept override;
	void OnChar(char character) noexcept override;
	void ClearState() noexcept override;

private:
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;

private:
	static constexpr unsigned int s_nKeys = 256u;
	static constexpr unsigned int s_bufferSize = 16u;
	bool m_autoRepeatEnabled;
	std::bitset<s_nKeys> m_keystates;
	std::queue<Event> m_keyBuffer;
	std::queue<char> m_charBuffer;
	SKeyCodes(*m_nativeKeyCodeGetter)(std::uint16_t);
};
#endif
