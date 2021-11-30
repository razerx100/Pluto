#include <PlutoInstance.hpp>
#include <InputManagerImpl.hpp>

InputManager* CreateInputManagerInstance() noexcept {
	return new InputManagerImpl();
}
