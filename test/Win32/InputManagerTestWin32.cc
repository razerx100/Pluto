#include <gtest/gtest.h>
#include <InputManagerImpl.hpp>

namespace Constants
{
	constexpr const char* appName = "LunaTest";
}

TEST(InputManagerTest, IOTest)
{
	InputManagerImpl inputManager{};
}