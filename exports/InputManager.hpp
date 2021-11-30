#ifndef __INPUT_MANAGER_HPP__
#define __INPUT_MANAGER_HPP__
#include <IKeyboard.hpp>
#include <IMouse.hpp>
#include <IGamepad.hpp>
#include <ObjectManager.hpp>

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

	virtual std::uint32_t GetKeyboardCount() const noexcept = 0;
	virtual std::uint32_t GetMouseCount() const noexcept = 0;
	virtual std::uint32_t GetGamepadCount() const noexcept = 0;

	virtual void GetKeyboardRefs(
		IKeyboard* keyboards, std::uint32_t& keyboardCount
	) const noexcept = 0;
	virtual void GetMouseRefs(IMouse* mouses, std::uint32_t& mouseCount) const noexcept = 0;
	virtual void GetGamepadRefs(
		IGamepad* gamepads, std::uint32_t& gamepadCount
	) const noexcept = 0;

	virtual	IKeyboard* GetKeyboardByIndex(std::uint32_t index = 0u) const noexcept = 0;
	virtual	IMouse* GetMouseByIndex(std::uint32_t index = 0u) const noexcept = 0;
	virtual	IGamepad* GetGamepadByIndex(std::uint32_t index = 0u) const noexcept = 0;
	virtual	IKeyboard* GetKeyboardByHandle(std::uint64_t handle) noexcept = 0;
	virtual	IMouse* GetMouseByHandle(std::uint64_t handle) noexcept = 0;
	virtual	GamepadData GetGamepadByHandle(std::uint64_t handle) noexcept = 0;

	virtual void ClearInputStates() noexcept = 0;
};

class InputManagerInst : public _ObjectManager<InputManager> {};
#endif
