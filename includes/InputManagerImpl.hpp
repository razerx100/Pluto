#ifndef __INPUT_MANAGER_IMPL_HPP__
#define __INPUT_MANAGER_IMPL_HPP__
#include <InputManager.hpp>
#include <unordered_map>
#include <memory>

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

	[[nodiscard]]
	std::vector<IKeyboard*> GetKeyboardRefs() const noexcept override;
	[[nodiscard]]
	std::vector<IMouse*> GetMouseRefs() const noexcept override;
	[[nodiscard]]
	std::vector<IGamepad*> GetGamepadRefs() const noexcept override;

	[[nodiscard]]
	std::uint32_t GetKeyboardsCount() const noexcept override;
	[[nodiscard]]
	std::uint32_t GetMousesCount() const noexcept override;
	[[nodiscard]]
	std::uint32_t GetGamepadsCount() const noexcept override;

	[[nodiscard]]
	IKeyboard* GetKeyboardByIndex(std::uint32_t index) const noexcept override;
	[[nodiscard]]
	IMouse* GetMouseByIndex(std::uint32_t index) const noexcept override;
	[[nodiscard]]
	IGamepad* GetGamepadByIndex(std::uint32_t index) const noexcept override;
	[[nodiscard]]
	IKeyboard* GetKeyboardByHandle(std::uint64_t handle) noexcept override;
	[[nodiscard]]
	IMouse* GetMouseByHandle(std::uint64_t handle) noexcept override;
	[[nodiscard]]
	GamepadData GetGamepadByHandle(std::uint64_t handle) noexcept override;

	void ClearInputStates() noexcept override;

private:
	int FindIndex(const std::vector<bool>& data) const noexcept;

private:
	std::unordered_map<std::uint64_t, HandleData> m_handleMap;
	std::vector<std::unique_ptr<IKeyboard>> m_pKeyboards;
	std::vector<std::unique_ptr<IMouse>> m_pMouses;
	std::vector<std::unique_ptr<IGamepad>> m_pGamepads;
	std::vector<bool> m_availableKeyboardIndices;
	std::vector<bool> m_availableMouseIndices;
	std::vector<bool> m_availableGamepadIndices;
	std::vector<std::uint32_t> m_devicesCount;
};
#endif
