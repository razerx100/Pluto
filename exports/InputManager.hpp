#ifndef __INPUT_MANAGER_HPP__
#define __INPUT_MANAGER_HPP__
#include <IKeyboard.hpp>
#include <IMouse.hpp>
#include <IGamepad.hpp>
#include <vector>

enum class DeviceType {
	Keyboard,
	Mouse,
	Gamepad,
	DeviceTypeCount
};

struct GamepadData {
	IGamepad* pGamepad = nullptr;
	size_t index = 0u;
};

class InputManager {
public:
	virtual ~InputManager() = default;

	virtual void AddDeviceSupport(
		DeviceType device, size_t count = 1u
	) noexcept = 0;
	virtual void DeviceDisconnected(
		std::uint64_t handle
	) noexcept = 0;

	[[nodiscard]]
	virtual std::vector<IKeyboard*> GetKeyboardRefs() const noexcept = 0;
	[[nodiscard]]
	virtual std::vector<IMouse*> GetMouseRefs() const noexcept = 0;
	[[nodiscard]]
	virtual std::vector<IGamepad*> GetGamepadRefs() const noexcept = 0;

	[[nodiscard]]
	virtual size_t GetKeyboardsCount() const noexcept = 0;
	[[nodiscard]]
	virtual size_t GetMousesCount() const noexcept = 0;
	[[nodiscard]]
	virtual size_t GetGamepadsCount() const noexcept = 0;

	[[nodiscard]]
	virtual	IKeyboard* GetKeyboardByIndex(size_t index = 0u) const noexcept = 0;
	[[nodiscard]]
	virtual	IMouse* GetMouseByIndex(size_t index = 0u) const noexcept = 0;
	[[nodiscard]]
	virtual	IGamepad* GetGamepadByIndex(size_t index = 0u) const noexcept = 0;
	[[nodiscard]]
	virtual	IKeyboard* GetKeyboardByHandle(std::uint64_t handle) noexcept = 0;
	[[nodiscard]]
	virtual	IMouse* GetMouseByHandle(std::uint64_t handle) noexcept = 0;
	[[nodiscard]]
	virtual	GamepadData GetGamepadByHandle(std::uint64_t handle) noexcept = 0;

	virtual void ClearInputStates() noexcept = 0;
};

#endif
