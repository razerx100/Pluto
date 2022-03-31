#ifndef __PLUTO_INSTANCE_HPP__
#define __PLUTO_INSTANCE_HPP__
#include <InputManager.hpp>

#ifdef BUILD_PLUTO
#define PLUTO_DLL __declspec(dllexport)
#else
#define PLUTO_DLL __declspec(dllimport)
#endif

[[nodiscard]]
PLUTO_DLL InputManager* __cdecl CreatePlutoInstance();
#endif
