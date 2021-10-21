#include <InputManagerImpl.hpp>
#include <Keyboard.hpp>
#include <Mouse.hpp>

InputManagerImpl::InputManagerImpl() noexcept
	: m_devicesCount(static_cast<std::uint32_t>(DeviceType::DeviceTypeCount)) {}

void InputManagerImpl::AddDeviceSupport(
	DeviceType device, std::uint32_t count
) noexcept {
	m_devicesCount[static_cast<std::uint32_t>(device)] += count;

	if (device == DeviceType::Keyboard)
		for (std::uint32_t index = 0u; index < count; ++index) {
			m_pKeyboards.emplace_back(std::make_unique<Keyboard>());
			m_availableKeyboardIndices.push(index);
		}
	else if(device == DeviceType::Mouse)
		for (std::uint32_t index = 0u; index < count; ++index) {
			m_pMouses.emplace_back(std::make_unique<Mouse>());
			m_availableMouseIndices.push(index);
		}
}

void InputManagerImpl::DeviceDisconnected(
	std::uint64_t handle
) noexcept {
	if (auto result = m_handleMap.find(handle); result != m_handleMap.end()) {
		m_handleMap.erase(handle);

		std::uint32_t index = result->second.index;
		DeviceType device = result->second.type;

		if (device == DeviceType::Keyboard) {
			m_availableKeyboardIndices.push(index);
			m_pKeyboards[index]->Flush();
		}
		else if (device == DeviceType::Mouse) {
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
		if (!m_availableKeyboardIndices.empty()) {
			std::uint32_t index = 0u;
			index = m_availableKeyboardIndices.front();
			m_availableKeyboardIndices.pop();
			m_handleMap.emplace(handle, HandleData{ index, DeviceType::Keyboard });

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
		if (!m_availableMouseIndices.empty()) {
			std::uint32_t index = 0u;
			index = m_availableMouseIndices.front();
			m_availableMouseIndices.pop();
			m_handleMap.emplace(handle, HandleData{ index, DeviceType::Mouse });

			return m_pMouses[index].get();
		}
		else
			return nullptr;
	}
	else
		return m_pMouses[result->second.index].get();
}

std::uint32_t InputManagerImpl::GetKeyboardCount() const noexcept {
	return static_cast<std::uint32_t>(m_pKeyboards.size());
}

std::uint32_t InputManagerImpl::GetMouseCount() const noexcept {
	return static_cast<std::uint32_t>(m_pMouses.size());
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