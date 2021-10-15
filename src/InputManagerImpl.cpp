#include <InputManagerImpl.hpp>
#include <Keyboard.hpp>
#include <Mouse.hpp>

InputManagerImpl::InputManagerImpl() noexcept
	: m_devicesCount(DeviceTypeCount) {}

void InputManagerImpl::AddDeviceSupport(
	DeviceType device, std::uint32_t count
) noexcept {
	m_devicesCount[device] += count;

	if (device == KeyboardDiv)
		for (std::uint32_t index = 0u; index < count; ++index) {
			m_pKeyboards.emplace_back(std::make_unique<Keyboard>());
			m_availableKeyboardIndices.push(index);
		}
	else if(device == MouseDiv)
		for (std::uint32_t index = 0u; index < count; ++index) {
			m_pMouses.emplace_back(std::make_unique<Mouse>());
			m_availableMouseIndices.push(index);
		}
}

void InputManagerImpl::DeviceDisconnected(
	DeviceType device, std::uint64_t handle
) noexcept {
	if (auto result = m_handleMap.find(handle); result != m_handleMap.end()) {
		m_handleMap.erase(handle);

		std::uint32_t index = result->second;
		if (device == KeyboardDiv) {
			m_availableKeyboardIndices.push(index);
			m_pKeyboards[index]->Flush();
		}
		else if (device == MouseDiv) {
			m_availableMouseIndices.push(index);
			m_pMouses[index]->Flush();
		}
	}
}

IKeyboard* InputManagerImpl::GetKeyboardByIndex(std::uint32_t index) const noexcept {
	return m_pKeyboards[index].get();
}

IMouse* InputManagerImpl::GetMouseByIndex(std::uint32_t index) const noexcept {
	return m_pMouses[index].get();
}

IKeyboard* InputManagerImpl::GetKeyboardByHandle(std::uint64_t handle) noexcept {
	if (auto result = m_handleMap.find(handle); result == m_handleMap.end()) {
		std::uint32_t index = 0u;
		if (!m_availableKeyboardIndices.empty()) {
			index = m_availableKeyboardIndices.front();
			m_availableKeyboardIndices.pop();
			m_handleMap.emplace(handle, index);
		}

		return m_pKeyboards[index].get();
	}
	else
		return m_pKeyboards[result->second].get();
}

IMouse* InputManagerImpl::GetMouseByHandle(std::uint64_t handle) noexcept {
	if (auto result = m_handleMap.find(handle); result == m_handleMap.end()) {
		std::uint32_t index = 0u;
		if (!m_availableMouseIndices.empty()) {
			index = m_availableMouseIndices.front();
			m_availableMouseIndices.pop();
			m_handleMap.emplace(handle, index);
		}

		return m_pMouses[index].get();
	}
	else
		return m_pMouses[result->second].get();
}
