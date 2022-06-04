#include <InputManagerImpl.hpp>
#include <Keyboard.hpp>
#include <Mouse.hpp>
#include <Gamepad.hpp>

InputManagerImpl::InputManagerImpl() noexcept
	: m_devicesCount(static_cast<size_t>(DeviceType::Invalid)) {}

void InputManagerImpl::AddDeviceSupport(DeviceType device, size_t count) noexcept {
	m_devicesCount[static_cast<size_t>(device)] += count;

	if (device == DeviceType::Keyboard) {
		for (size_t _ = 0u; _ < count; ++_)
			m_pKeyboards.emplace_back(std::make_unique<Keyboard>());

		AddAvailableIndices(m_availableKeyboardIndices, count);
	}
	else if (device == DeviceType::Mouse) {
		for (size_t _ = 0u; _ < count; ++_)
			m_pMouses.emplace_back(std::make_unique<Mouse>());

		AddAvailableIndices(m_availableMouseIndices, count);
	}
	else if (device == DeviceType::Gamepad) {
		for (size_t _ = 0u; _ < count; ++_)
			m_pGamepads.emplace_back(std::make_unique<Gamepad>());

		AddAvailableIndices(m_availableGamepadIndices, count);
	}
}

void InputManagerImpl::DisconnectDevice(std::uint64_t handle) noexcept {
	if (auto result = m_handleMap.find(handle); result != std::end(m_handleMap)) {
		auto [index, device] = result->second;

		m_handleMap.erase(handle);

		if (device == DeviceType::Keyboard) {
			m_availableKeyboardIndices[index] = true;
			m_pKeyboards[index]->Flush();
		}
		else if (device == DeviceType::Mouse) {
			m_availableMouseIndices[index] = true;
			m_pMouses[index]->Flush();
		}
	}
}

IKeyboard* InputManagerImpl::GetKeyboardByIndex(size_t index) const noexcept {
	return GetPtrByIndex(m_pKeyboards, index);
}

IMouse* InputManagerImpl::GetMouseByIndex(size_t index) const noexcept {
	return GetPtrByIndex(m_pMouses, index);
}

IGamepad* InputManagerImpl::GetGamepadByIndex(size_t index) const noexcept {
	return GetPtrByIndex(m_pGamepads, index);
}

IKeyboard* InputManagerImpl::GetKeyboard() const noexcept {
	return GetKeyboardByIndex(0u);
}

IMouse* InputManagerImpl::GetMouse() const noexcept {
	return GetMouseByIndex(0u);
}

IGamepad* InputManagerImpl::GetGamepad() const noexcept {
	return GetGamepadByIndex(0u);
}

size_t InputManagerImpl::GetKeyboardCount() const noexcept {
	return std::size(m_pKeyboards);
}

size_t InputManagerImpl::GetMouseCount() const noexcept {
	return std::size(m_pMouses);
}

size_t InputManagerImpl::GetGamepadCount() const noexcept {
	return std::size(m_pGamepads);
}

IKeyboard* InputManagerImpl::GetKeyboardByHandle(std::uint64_t handle) noexcept {
	return GetDeviceByHandle(
		handle, m_availableKeyboardIndices,
		m_pKeyboards, DeviceType::Keyboard
	);
}

IMouse* InputManagerImpl::GetMouseByHandle(std::uint64_t handle) noexcept {
	return GetDeviceByHandle(
		handle, m_availableMouseIndices,
		m_pMouses, DeviceType::Mouse
	);
}

std::vector<IKeyboard*> InputManagerImpl::GetKeyboardRefs() const noexcept {
	return GetVectorOfRefs(m_pKeyboards);
}

std::vector<IMouse*> InputManagerImpl::GetMouseRefs() const noexcept {
	return GetVectorOfRefs(m_pMouses);
}

std::vector<IGamepad*> InputManagerImpl::GetGamepadRefs() const noexcept {
	return GetVectorOfRefs(m_pGamepads);
}

void InputManagerImpl::ClearInputStates() noexcept {
	for (auto& keyboard : m_pKeyboards)
		keyboard->Flush();

	for (auto& mouse : m_pMouses)
		mouse->Flush();

	for (auto& gamepad : m_pGamepads)
		gamepad->ClearState();
}

std::optional<size_t> InputManagerImpl::GetAvailableIndex(
	std::vector<bool>& indices
) const noexcept {
	for (size_t index = 0; index < std::size(indices); ++index)
		if (indices[index]) {
			indices[index] = false;

			return index;
		}

	return {};
}

void InputManagerImpl::AddAvailableIndices(std::vector<bool>& indices, size_t count) noexcept {
	for (size_t _ = 0u; _ < count; ++_)
		indices.emplace_back(true);
}

void InputManagerImpl::ActivateGamepadByIndex(size_t index) noexcept {
	m_availableGamepadIndices[index] = false;
}

void InputManagerImpl::DisconnectGamepadByIndex(size_t index) noexcept {
	m_availableGamepadIndices[index] = true;
	m_pGamepads[index]->Flush();
}
