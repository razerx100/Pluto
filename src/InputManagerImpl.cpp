#include <InputManagerImpl.hpp>
#include <Keyboard.hpp>
#include <Mouse.hpp>
#include <Gamepad.hpp>

InputManagerImpl::InputManagerImpl() noexcept
	: m_devicesCount(static_cast<std::uint32_t>(DeviceType::DeviceTypeCount)) {}

void InputManagerImpl::AddDeviceSupport(
	DeviceType device, std::uint32_t count
) noexcept {
	std::uint32_t deviceIndex = static_cast<std::uint32_t>(device);

	m_devicesCount[deviceIndex] += count;

	if (device == DeviceType::Keyboard)
		for (std::uint32_t index = 0u; index < count; ++index) {
			m_pKeyboards.emplace_back(std::make_unique<Keyboard>());
			m_keyboardIndices.resize(m_keyboardIndices.size() + count, true);
		}
	else if(device == DeviceType::Mouse)
		for (std::uint32_t index = 0u; index < count; ++index) {
			m_pMouses.emplace_back(std::make_unique<Mouse>());
			m_mouseIndices.resize(m_mouseIndices.size() + count, true);
		}
	else if (device == DeviceType::Gamepad)
		for (std::uint32_t index = 0u; index < count; ++index) {
			m_pGamepads.emplace_back(std::make_unique<Gamepad>());
			m_gamepadIndices.resize(m_gamepadIndices.size() + count, true);
		}
}

void InputManagerImpl::DeviceDisconnected(
	std::uint64_t handle
) noexcept {
	if (auto result = m_handleMap.find(handle); result != m_handleMap.end()) {
		std::uint32_t index = result->second.index;
		DeviceType device = result->second.type;

		m_handleMap.erase(handle);

		if (device == DeviceType::Keyboard) {
			m_keyboardIndices[index] = true;
			m_pKeyboards[index]->Flush();
		}
		else if (device == DeviceType::Mouse) {
			m_mouseIndices[index] = true;
			m_pMouses[index]->Flush();
		}
		else if (device == DeviceType::Gamepad) {
			m_gamepadIndices[index] = true;
			m_pGamepads[index]->ClearState();
		}
	}
}

IKeyboard* InputManagerImpl::GetKeyboardByIndex(std::uint32_t index) const noexcept {
	return m_pKeyboards[index].get();
}

IMouse* InputManagerImpl::GetMouseByIndex(std::uint32_t index) const noexcept {
	return m_pMouses[index].get();
}

IGamepad* InputManagerImpl::GetGamepadByIndex(std::uint32_t index) const noexcept {
	return m_pGamepads[index].get();
}

IKeyboard* InputManagerImpl::GetKeyboardByHandle(std::uint64_t handle) noexcept {
	if (auto result = m_handleMap.find(handle); result == m_handleMap.end()) {
		if (int index = FindIndex(m_keyboardIndices); index != -1) {
			m_keyboardIndices[index] = false;
			m_handleMap.emplace(handle, HandleData{
					static_cast<std::uint32_t>(index), DeviceType::Keyboard
				});

			return m_pKeyboards[index].get();
		}
		else
			return nullptr;

	}
	else
		return m_pKeyboards[result->second.index].get();
}

IMouse* InputManagerImpl::GetMouseByHandle(std::uint64_t handle) noexcept {
	if (auto result = m_handleMap.find(handle); result == m_handleMap.end()) {
		if (int index = FindIndex(m_mouseIndices); index != -1) {
			m_mouseIndices[index] = false;
			m_handleMap.emplace(handle, HandleData{
					static_cast<std::uint32_t>(index), DeviceType::Mouse
				});

			return m_pMouses[index].get();
		}
		else
			return nullptr;
	}
	else
		return m_pMouses[result->second.index].get();
}

GamepadData InputManagerImpl::GetGamepadByHandle(std::uint64_t handle) noexcept {
	if (auto result = m_handleMap.find(handle); result == m_handleMap.end()) {
		if (int index = FindIndex(m_gamepadIndices); index != -1) {
			m_gamepadIndices[index] = false;
			m_handleMap.emplace(handle, HandleData{
					static_cast<std::uint32_t>(index), DeviceType::Gamepad
				});

			return GamepadData(m_pGamepads[index].get(), index);
		}
		else
			return GamepadData();
	}
	else
		return GamepadData(
			m_pGamepads[result->second.index].get(), result->second.index
		);
}

std::uint32_t InputManagerImpl::GetKeyboardCount() const noexcept {
	return static_cast<std::uint32_t>(m_pKeyboards.size());
}

std::uint32_t InputManagerImpl::GetMouseCount() const noexcept {
	return static_cast<std::uint32_t>(m_pMouses.size());
}

std::uint32_t InputManagerImpl::GetGamepadCount() const noexcept {
	return static_cast<std::uint32_t>(m_pGamepads.size());
}

std::vector<IKeyboard*> InputManagerImpl::GetKeyboardRefs() noexcept {
	std::vector<IKeyboard*> pKeyboardRefs;
	for (auto& keyboard : m_pKeyboards)
		pKeyboardRefs.emplace_back(keyboard.get());

	return pKeyboardRefs;
}

std::vector<IMouse*> InputManagerImpl::GetMouseRefs() noexcept {
	std::vector<IMouse*> pMouseRefs;
	for (auto& mouse : m_pMouses)
		pMouseRefs.emplace_back(mouse.get());

	return pMouseRefs;
}

std::vector<IGamepad*> InputManagerImpl::GetGamepadRefs() noexcept {
	std::vector<IGamepad*> pGamepadRefs;
	for (auto& gamepad : m_pGamepads)
		pGamepadRefs.emplace_back(gamepad.get());

	return pGamepadRefs;
}

void InputManagerImpl::ClearInputStates() noexcept {
	for (auto& keyboard : m_pKeyboards)
		keyboard->Flush();

	for (auto& mouse : m_pMouses)
		mouse->Flush();

	for (auto& gamepad : m_pGamepads)
		gamepad->ClearState();
}

int InputManagerImpl::FindIndex(const std::vector<bool>& data) const noexcept {
	for (std::uint32_t index = 0; index < data.size(); ++index)
		if (data[index])
			return index;

	return -1;
}
