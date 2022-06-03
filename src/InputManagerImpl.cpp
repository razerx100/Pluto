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
		else if (device == DeviceType::Gamepad) {
			m_availableGamepadIndices[index] = true;
			m_pGamepads[index]->Flush();
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
	if (auto result = m_handleMap.find(handle); result == std::end(m_handleMap)) [[unlikely]] {
		if (auto index = GetAvailableIndex(m_availableKeyboardIndices); index) [[likely]] {
			m_handleMap.emplace(handle, HandleData{ *index, DeviceType::Keyboard });

			return m_pKeyboards[*index].get();
		}
		else [[unlikely]]
			return nullptr;
	}
	else [[likely]]
		return m_pKeyboards[result->second.deviceIndex].get();
}

IMouse* InputManagerImpl::GetMouseByHandle(std::uint64_t handle) noexcept {
	if (auto result = m_handleMap.find(handle); result == std::end(m_handleMap)) [[unlikely]] {
		if (auto index = GetAvailableIndex(m_availableMouseIndices); index) [[likely]] {
			m_handleMap.emplace(handle, HandleData{ *index, DeviceType::Mouse });

			return m_pMouses[*index].get();
		}
		else [[unlikely]]
			return nullptr;
	}
	else [[likely]]
		return m_pMouses[result->second.deviceIndex].get();
}

GamepadData InputManagerImpl::GetGamepadDataByHandle(std::uint64_t handle) noexcept {
	if (auto result = m_handleMap.find(handle); result == std::end(m_handleMap)) [[unlikely]] {
		if (auto index = GetAvailableIndex(m_availableGamepadIndices); index) [[likely]] {
			m_handleMap.emplace(handle, HandleData{ *index, DeviceType::Gamepad });

			return { m_pGamepads[*index].get(), *index };
		}
		else [[unlikely]]
			return GamepadData{ nullptr, 0u };
	}
	else [[likely]] {
		size_t index = result->second.deviceIndex;

		return { m_pGamepads[index].get(), index };
	}
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
