#ifndef INPUT_MANAGER_IMPL_HPP_
#define INPUT_MANAGER_IMPL_HPP_
#include <InputManager.hpp>
#include <unordered_map>
#include <memory>
#include <optional>

class InputManagerImpl final : public InputManager {
public:
	InputManagerImpl() noexcept;

	void AddDeviceSupport(DeviceType device, size_t count) noexcept override;
	void DisconnectDevice(std::uint64_t handle) noexcept override;

	[[nodiscard]]
	std::vector<IKeyboard*> GetKeyboardRefs() const noexcept override;
	[[nodiscard]]
	std::vector<IMouse*> GetMouseRefs() const noexcept override;
	[[nodiscard]]
	std::vector<IGamepad*> GetGamepadRefs() const noexcept override;

	[[nodiscard]]
	IKeyboard* GetKeyboard() const noexcept override;
	[[nodiscard]]
	IMouse* GetMouse() const noexcept override;
	[[nodiscard]]
	IGamepad* GetGamepad() const noexcept override;

	[[nodiscard]]
	size_t GetKeyboardCount() const noexcept override;
	[[nodiscard]]
	size_t GetMouseCount() const noexcept override;
	[[nodiscard]]
	size_t GetGamepadCount() const noexcept override;

	[[nodiscard]]
	IKeyboard* GetKeyboardByIndex(size_t index) const noexcept override;
	[[nodiscard]]
	IMouse* GetMouseByIndex(size_t index) const noexcept override;
	[[nodiscard]]
	IGamepad* GetGamepadByIndex(size_t index) const noexcept override;
	[[nodiscard]]
	IKeyboard* GetKeyboardByHandle(std::uint64_t handle) noexcept override;
	[[nodiscard]]
	IMouse* GetMouseByHandle(std::uint64_t handle) noexcept override;
	[[nodiscard]]
	IGamepad* GetGamepadByHandle(std::uint64_t handle) noexcept override;

	[[nodiscard]]
	bool DoesGamepadHandleExist(std::uint64_t handle) const noexcept override;

	void ClearInputStates() noexcept override;

private:
	struct HandleData {
		size_t deviceIndex;
		DeviceType deviceType;
	};

	std::optional<size_t> GetAvailableIndex(std::vector<bool>& indices) const noexcept;
	void AddAvailableIndices(std::vector<bool>& indices, size_t count) noexcept;

private:
	std::unordered_map<std::uint64_t, HandleData> m_handleMap;

	std::vector<std::unique_ptr<IKeyboard>> m_pKeyboards;
	std::vector<std::unique_ptr<IMouse>> m_pMouses;
	std::vector<std::unique_ptr<IGamepad>> m_pGamepads;

	std::vector<bool> m_availableKeyboardIndices;
	std::vector<bool> m_availableMouseIndices;
	std::vector<bool> m_availableGamepadIndices;
	std::vector<size_t> m_devicesCount;

private:
	template<class T>
	static std::vector<T*> GetVectorOfRefs(
		const std::vector<std::unique_ptr<T>>& ptrs
	) noexcept {
		std::vector<T*> refs;

		for (size_t index = 0u; index < std::size(ptrs); ++index)
			refs.emplace_back(ptrs[index].get());

		return refs;
	}

	template<class T>
	static T* GetPtrByIndex(
		const std::vector<std::unique_ptr<T>>& ptrs, size_t index
	) noexcept {
		if (std::size(ptrs) > index) [[likely]]
			return ptrs[index].get();
		else [[unlikely]]
			return nullptr;
	}

	template<class T>
	T* GetDeviceByHandle(
		std::uint64_t handle, std::vector<bool>& availableIndices,
		const std::vector<std::unique_ptr<T>>& ptrs, DeviceType type
	) noexcept {
		if (auto result = m_handleMap.find(handle);
			result == std::end(m_handleMap)) [[unlikely]] {
			if (auto index = GetAvailableIndex(availableIndices); index) [[likely]] {
				m_handleMap.emplace(handle, HandleData{ *index, type });

				return ptrs[*index].get();
			}
			else [[unlikely]]
				return nullptr;
		}
		else [[likely]]
			return ptrs[result->second.deviceIndex].get();
	}
};
#endif
