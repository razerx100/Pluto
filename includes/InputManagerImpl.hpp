#ifndef INPUT_MANAGER_IMPL_HPP_
#define INPUT_MANAGER_IMPL_HPP_
#include <InputManager.hpp>
#include <unordered_map>
#include <memory>
#include <optional>

class InputManagerImpl final : public InputManager {
public:
	InputManagerImpl() noexcept;

	void AddGamepadSupport(size_t count) noexcept override;

	[[nodiscard]]
	size_t GetGamepadCount() const noexcept override;

	[[nodiscard]]
	IKeyboard& GetKeyboard() const noexcept override;
	[[nodiscard]]
	IMouse& GetMouse() const noexcept override;
	[[nodiscard]]
	IGamepad& GetGamepad(size_t index = 0u) const noexcept override;
	[[nodiscard]]
	IKeyboard* GetKeyboardRef() const noexcept override;
	[[nodiscard]]
	IMouse* GetMouseRef() const noexcept override;
	[[nodiscard]]
	IGamepad* GetGamepadRef(size_t index = 0u) const noexcept override;

	void DisconnectGamepad(size_t index = 0u) noexcept override;

	void ClearInputStates() noexcept override;

private:
	std::unique_ptr<IKeyboard> m_keyboard;
	std::unique_ptr<IMouse> m_mouse;
	std::vector<std::unique_ptr<IGamepad>> m_gamepads;
};
#endif
