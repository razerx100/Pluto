#include <PlutoInstance.hpp>
#include <InputManagerImpl.hpp>

InputManager* CreatePlutoInstance() {
	return new InputManagerImpl();
}
