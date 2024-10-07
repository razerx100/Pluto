#include <PlutoInstance.hpp>
#include <InputManagerImpl.hpp>

std::unique_ptr<InputManager> CreatePlutoInstance()
{
	return std::make_unique<InputManagerImpl>();
}
