#include <PlutoInstance.hpp>
#include <InputManagerImpl.hpp>

InputManager* CreatePlutoInstance() noexcept {
	return new InputManagerImpl();
}
