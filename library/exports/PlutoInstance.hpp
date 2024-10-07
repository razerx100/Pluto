#ifndef PLUTO_INSTANCE_HPP_
#define PLUTO_INSTANCE_HPP_
#include <InputManager.hpp>
#include <memory>

#ifdef BUILD_PLUTO
#define PLUTO_DLL __declspec(dllexport)
#else
#define PLUTO_DLL __declspec(dllimport)
#endif

[[nodiscard]]
PLUTO_DLL std::unique_ptr<InputManager> CreatePlutoInstance();
#endif
