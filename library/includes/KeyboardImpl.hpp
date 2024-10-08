#ifndef KEYBOARD_IMPL_HPP_
#define KEYBOARD_IMPL_HPP_
#include <queue>
#include <bitset>
#include <Keyboard.hpp>

class KeyboardImpl final : public Keyboard
{
public:
	KeyboardImpl()
		: m_keystates{}, m_keyBuffer{}, m_charBuffer{}
	{}

	[[nodiscard]]
	bool IsKeyPressed(SKeyCodes keycode) const noexcept override;
	[[nodiscard]]
	bool AreKeysPressed(size_t count, ...) const noexcept override;
	[[nodiscard]]
	std::optional<Event> ReadKey() noexcept override;
	void FlushKey() noexcept override;

	// char events
	[[nodiscard]]
	std::optional<char> ReadChar() noexcept override;
	void FlushChar() noexcept override;
	void Flush() noexcept override;

	void OnKeyPressed(SKeyCodes keycode) noexcept override;
	void OnKeyReleased(SKeyCodes keycode) noexcept override;
	void OnChar(char character) noexcept override;
	void ClearState() noexcept override;

private:
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept
	{
		while (std::size(buffer) > s_bufferSize)
			buffer.pop();
	}

private:
	static constexpr std::uint32_t s_nKeys      = 256u;
	static constexpr std::uint32_t s_bufferSize = 16u;

	std::bitset<s_nKeys> m_keystates;
	std::queue<Event>    m_keyBuffer;
	std::queue<char>     m_charBuffer;

public:
	KeyboardImpl(const KeyboardImpl&) = delete;
	KeyboardImpl& operator=(const KeyboardImpl&) = delete;

	KeyboardImpl(KeyboardImpl&& other) noexcept
		: m_keystates{ std::move(other.m_keystates) },
		m_keyBuffer{ std::move(other.m_keyBuffer) },
		m_charBuffer{ std::move(other.m_charBuffer) }
	{}
	KeyboardImpl& operator=(KeyboardImpl&& other) noexcept
	{
		m_keystates  = std::move(other.m_keystates);
		m_keyBuffer  = std::move(other.m_keyBuffer);
		m_charBuffer = std::move(other.m_charBuffer);

		return *this;
	}
};
#endif
