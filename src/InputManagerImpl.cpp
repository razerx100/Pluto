#include <InputManagerImpl.hpp>
#include <Keyboard.hpp>
#include <Mouse.hpp>
#include <Gamepad.hpp>

InputManagerImpl::InputManagerImpl() noexcept
	: m_devicesCount(static_cast<size_t>(DeviceType::DeviceTypeCount)) {}

void InputManagerImpl::AddDeviceSupport(
	DeviceType device, size_t count
) noexcept {
	m_devicesCount[static_cast<size_t>(device)] += count;

	if (device == DeviceType::Keyboard) {
		for (size_t _ = 0u; _ < count; ++_)
			m_pKeyboards.emplace_back(std::make_unique<Keyboard>());

		m_availableKeyboardIndices.resize(
			m_availableKeyboardIndices.size() + count, true
		);
	}
	else if (device == DeviceType::Mouse) {
		for (size_t _ = 0u; _ < count; ++_)
			m_pMouses.emplace_back(std::make_unique<Mouse>());

		m_availableMouseIndices.resize(
			m_availableMouseIndices.size() + count, true
		);
	}
	else if (device == DeviceType::Gamepad) {
		for (size_t _ = 0u; _ < count; ++_)
			m_pGamepads.emplace_back(std::make_unique<Gamepad>());

		m_availableGamepadIndices.resize(
			m_availableGamepadIndices.size() + count, true
		);
	}
}

void InputManagerImpl::DeviceDisconnected(
	std::uint64_t handle
) noexcept {
	if (auto result = m_handleMap.find(handle); result != m_handleMap.end()) {
		size_t index = result->second.index;
		DeviceType device = result->second.type;

		m_handleMap.erase(handle);

		if (device == DeviceType::Keyboard) {
			m_availableKeyboardIndices[index] = true;
			m_pKeyboards[index]->Flush();
		}
		else if (device == DeviceType::Mouse) {
			m_availableMouseIndices[index] = true;
			m_pMouses[index]->Flush();
		}
		else if (device == DeviceType::Gamepad) {
			m_availableGamepadIndices[index] = true;
			m_pGamepads[index]->ClearState();
		}
	}
}

IKeyboard* InputManagerImpl::GetKeyboardByIndex(size_t index) const noexcept {
	return m_pKeyboards[index].get();
}

IMouse* InputManagerImpl::GetMouseByIndex(size_t index) const noexcept {
	return m_pMouses[index].get();
}

IGamepad* InputManagerImpl::GetGamepadByIndex(size_t index) const noexcept {
	return m_pGamepads[index].get();
}

size_t InputManagerImpl::GetKeyboardsCount() const noexcept {
	return m_pKeyboards.size();
}

size_t InputManagerImpl::GetMousesCount() const noexcept {
	return m_pMouses.size();
}

size_t InputManagerImpl::GetGamepadsCount() const noexcept {
	return m_pGamepads.size();
}

IKeyboard* InputManagerImpl::GetKeyboardByHandle(std::uint64_t handle) noexcept {
	if (auto result = m_handleMap.find(handle); result == m_handleMap.end()) {
		if (std::int64_t index1 = FindIndex(m_availableKeyboardIndices); index1 != -1) {
			size_t index = static_cast<size_t>(index1);

			m_availableKeyboardIndices[index] = false;
			m_handleMap.emplace(handle, HandleData{
					index, DeviceType::Keyboard
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
		if (std::int64_t index1 = FindIndex(m_availableMouseIndices); index1 != -1) {
			size_t index = static_cast<size_t>(index1);

			m_availableMouseIndices[index] = false;
			m_handleMap.emplace(handle, HandleData{
					index, DeviceType::Mouse
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
		if (std::int64_t index1 = FindIndex(m_availableGamepadIndices); index1 != -1) {
			size_t index = static_cast<size_t>(index1);

			m_availableGamepadIndices[index] = false;
			m_handleMap.emplace(handle, HandleData{
					index, DeviceType::Gamepad
				});

			return GamepadData(m_pGamepads[index].get(), index);
		}
		else
			return GamepadData();
	}
	else
		return GamepadData(
			m_pGamepads[result->second.index].get(),
			result->second.index
		);
}

std::vector<IKeyboard*> InputManagerImpl::GetKeyboardRefs() const noexcept {
	std::vector<IKeyboard*> keyboardRefs(m_pKeyboards.size());

	for (size_t index = 0u; index < m_pKeyboards.size(); ++index)
		keyboardRefs[index] = m_pKeyboards[index].get();

	return keyboardRefs;
}

std::vector<IMouse*> InputManagerImpl::GetMouseRefs() const noexcept {
	std::vector<IMouse*> mouseRefs(m_pMouses.size());

	for (size_t index = 0u; index < m_pMouses.size(); ++index)
		mouseRefs[index] = m_pMouses[index].get();

	return mouseRefs;
}

std::vector<IGamepad*> InputManagerImpl::GetGamepadRefs() const noexcept {
	std::vector<IGamepad*> gamepadRefs(m_pGamepads.size());

	for (size_t index = 0u; index < m_pGamepads.size(); ++index)
		gamepadRefs[index] = m_pGamepads[index].get();

	return gamepadRefs;
}

void InputManagerImpl::ClearInputStates() noexcept {
	for (auto& keyboard : m_pKeyboards)
		keyboard->Flush();

	for (auto& mouse : m_pMouses)
		mouse->Flush();

	for (auto& gamepad : m_pGamepads)
		gamepad->ClearState();
}

std::int64_t InputManagerImpl::FindIndex(const std::vector<bool>& data) const noexcept {
	for (size_t index = 0; index < data.size(); ++index)
		if (data[index])
			return static_cast<std::int64_t>(index);

	return -1ll;
}
