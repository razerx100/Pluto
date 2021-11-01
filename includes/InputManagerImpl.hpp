#ifndef __INPUT_MANAGER_IMPL_HPP__
#define __INPUT_MANAGER_IMPL_HPP__
#include <InputManager.hpp>
#include <unordered_map>
#include <memory>
#include <vector>

struct HandleData {
	std::uint32_t index;
	DeviceType type;
};

class InputManagerImpl : public InputManager {
public:
	InputManagerImpl() noexcept;

	void AddDeviceSupport(
		DeviceType device, std::uint32_t count
	) noexcept override;
	void DeviceDisconnected(
		std::uint64_t handle
	) noexcept override;

	std::uint32_t GetKeyboardCount() const noexcept override;
	std::uint32_t GetMouseCount() const noexcept override;
	std::uint32_t GetGamepadCount() const noexcept override;

	std::vector<IKeyboard*> GetKeyboardRefs() noexcept override;
	std::vector<IMouse*> GetMouseRefs() noexcept override;
	std::vector<IGamepad*> GetGamepadRefs() noexcept override;
	IKeyboard* GetKeyboardByIndex(std::uint32_t index) const noexcept override;
	IMouse* GetMouseByIndex(std::uint32_t index) const noexcept override;
	IGamepad* GetGamepadByIndex(std::uint32_t index) const noexcept override;
	IKeyboard* GetKeyboardByHandle(std::uint64_t handle) noexcept override;
	IMouse* GetMouseByHandle(std::uint64_t handle) noexcept override;
	GamepadData GetGamepadByHandle(std::uint64_t handle) noexcept override;

	void ClearInputStates() noexcept override;

private:
	int FindIndex(const std::vector<bool>& data) const noexcept;

private:
	std::unordered_map<std::uint64_t, HandleData> m_handleMap;
	std::vector<std::unique_ptr<IKeyboard>> m_pKeyboards;
	std::vector<std::unique_ptr<IMouse>> m_pMouses;
	std::vector<std::unique_ptr<IGamepad>> m_pGamepads;
	std::vector<bool> m_keyboardIndices;
	std::vector<bool> m_mouseIndices;
	std::vector<bool> m_gamepadIndices;
	std::vector<std::uint32_t> m_devicesCount;
};
#endif
