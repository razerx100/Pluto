#ifndef __INPUT_MANAGER_HPP__
#define __INPUT_MANAGER_HPP__
#include <IKeyboard.hpp>
#include <IMouse.hpp>
#include <IGamepad.hpp>
#include <vector>

enum class PLUTO_DLL DeviceType {
	Keyboard,
	Mouse,
	Gamepad,
	DeviceTypeCount
};

struct PLUTO_DLL GamepadData {
	IGamepad* pGamepad = nullptr;
	std::uint8_t index = 0u;
};

class PLUTO_DLL InputManager {
public:
	virtual ~InputManager() = default;

	virtual void AddDeviceSupport(
		DeviceType device, std::uint32_t count = 1u
	) noexcept = 0;
	virtual void DeviceDisconnected(
		std::uint64_t handle
	) noexcept = 0;

	virtual std::vector<IKeyboard*> GetKeyboardRefs() const noexcept = 0;
	virtual std::vector<IMouse*> GetMouseRefs() const noexcept = 0;
	virtual std::vector<IGamepad*> GetGamepadRefs() const noexcept = 0;

	virtual	IKeyboard* GetKeyboardByIndex(std::uint32_t index = 0u) const noexcept = 0;
	virtual	IMouse* GetMouseByIndex(std::uint32_t index = 0u) const noexcept = 0;
	virtual	IGamepad* GetGamepadByIndex(std::uint32_t index = 0u) const noexcept = 0;
	virtual	IKeyboard* GetKeyboardByHandle(std::uint64_t handle) noexcept = 0;
	virtual	IMouse* GetMouseByHandle(std::uint64_t handle) noexcept = 0;
	virtual	GamepadData GetGamepadByHandle(std::uint64_t handle) noexcept = 0;

	virtual void ClearInputStates() noexcept = 0;
};

#endif
