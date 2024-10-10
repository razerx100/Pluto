#ifndef INPUT_EVENTS_HPP_
#define INPUT_EVENTS_HPP_
#include <cstdint>

enum class InputEvent
{
	Fullscreen,
	Resize,
	Invalid
};

struct FullscreenData{};

struct ResizeData
{
	std::uint32_t width;
	std::uint32_t height;
};
#endif
