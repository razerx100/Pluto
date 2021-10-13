#ifndef __INPUT_MANAGER_IMPL_HPP__
#define __INPUT_MANAGER_IMPL_HPP__
#include <InputManager.hpp>
#include <unordered_map>

class InputManagerImpl : public InputManager {
public:
	void AddDeviceSupport(
		DeviceType device, std::uint32_t count
	) noexcept override;
	void DeviceDisconnected(std::uint64_t handle) noexcept override;
	IKeyboard* GetKeyboardByIndex(std::uint32_t index) const noexcept override;
	IMouse* GetMouseByIndex(std::uint32_t index) const noexcept override;
	IKeyboard* GetKeyboardByHandle(std::uint64_t handle) noexcept override;
	IMouse* GetMouseByHandle(std::uint64_t handle) noexcept override;
};
#endif
