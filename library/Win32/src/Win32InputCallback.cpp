#include <limits>
#include <vector>
#include <CleanWin.hpp>
#include <Win32KeyManager.hpp>
#include <PlatformSpecificFunctions.hpp>

static std::vector<std::uint8_t> s_rawInputBuffer(sizeof(RAWINPUT), 0u);

void PlutoWin32InputCallback(
	InputManagerImpl& inputManager,
	void* hwnd, std::uint32_t message, std::uint64_t wParameter, std::uint64_t lParameter
) {
	[[maybe_unused]] auto hWnd = reinterpret_cast<HWND>(hwnd);

	UINT msg           = message;
	auto wParam        = static_cast<WPARAM>(wParameter);
	auto lParam        = static_cast<LPARAM>(lParameter);

	switch (msg)
	{
	case WM_KILLFOCUS:
	{
		inputManager.ClearInputStates();

		break;
	}
	case WM_INPUT_DEVICE_CHANGE:
	{
		if (wParam == GIDC_REMOVAL)
			DisconnectXBoxController(inputManager.m_gamepads);

		break;
	}
	/************* KEYBOARD MESSAGES *************/
	case WM_SYSKEYDOWN:
	{
		//if ((wParam == VK_RETURN) && (lParam & 0x20000000ul)) // 29th bit checks if Alt is down
			//ToggleFullScreenMode();

		break;
	}
	case WM_CHAR:
	{
		inputManager.m_keyboard.OnChar(static_cast<char>(wParam));

		break;
	}
	/************* END KEYBOARD MESSAGES *************/
	case WM_MOUSEMOVE:
	{
		std::uint16_t xCoord = LOWORD(lParam);
		std::uint16_t yCoord = HIWORD(lParam);

		Mouse& mouse = inputManager.m_mouse;
		mouse.SetCurrentCursorCoord(xCoord, yCoord);

		break;
	}
	/************* RAW MESSAGES *************/
	case WM_INPUT:
	{
		constexpr auto rawInputHeaderSize = static_cast<UINT>(sizeof(RAWINPUTHEADER));

		UINT bufferSize = 0u;

		GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &bufferSize,
			rawInputHeaderSize
		);

		if (bufferSize > std::size(s_rawInputBuffer))
			s_rawInputBuffer.resize(bufferSize);

		// Get raw data by passing buffer
		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, std::data(s_rawInputBuffer),
			&bufferSize, rawInputHeaderSize
		) != bufferSize)
			break;

		auto rawInput = reinterpret_cast<const RAWINPUT*>(std::data(s_rawInputBuffer));

		const RAWINPUTHEADER& rawHeader = rawInput->header;

		if (rawHeader.dwType == RIM_TYPEMOUSE)
		{
			Mouse& mouse = inputManager.m_mouse;

			const RAWMOUSE& rawMouse = rawInput->data.mouse;

			if (rawMouse.usButtonFlags)
			{
				if (rawMouse.usButtonFlags & RI_MOUSE_WHEEL)
					mouse.OnWheelDelta(static_cast<short>(rawMouse.usButtonData));

				auto [pressedButtons, releasedButtons] = ProcessMouseRawButtons(
					rawMouse.usButtonFlags
				);

				mouse.SetPressState(pressedButtons);
				mouse.SetReleaseState(releasedButtons);

				std::int32_t mouseX = 0;
				std::int32_t mouseY = 0;

				if (rawMouse.usFlags & MOUSE_MOVE_ABSOLUTE)
				{
					RECT rect{};

					if (rawMouse.usFlags & MOUSE_VIRTUAL_DESKTOP)
					{
						rect.left   = GetSystemMetrics(SM_XVIRTUALSCREEN);
						rect.top    = GetSystemMetrics(SM_YVIRTUALSCREEN);
						rect.right  = GetSystemMetrics(SM_CXVIRTUALSCREEN);
						rect.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN);
					}
					else
					{
						rect.left   = GetSystemMetrics(SM_XVIRTUALSCREEN);
						rect.top    = GetSystemMetrics(SM_YVIRTUALSCREEN);
						rect.right  = GetSystemMetrics(SM_CXVIRTUALSCREEN);
						rect.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN);
					}

					mouseX = MulDiv(
						rawMouse.lLastX, rect.right,
						std::numeric_limits<std::uint16_t>::max()
					) + rect.left;

					mouseY = MulDiv(
						rawMouse.lLastY, rect.bottom,
						std::numeric_limits<std::uint16_t>::max()
					) + rect.top;
				}
				else if (rawMouse.lLastX != 0l || rawMouse.lLastY != 0l)
				{
					// Relative movement. Can contain negative values.
					mouseX = rawMouse.lLastX;
					mouseY = rawMouse.lLastY;
				}

				mouse.SetCurrentCursorCoord(mouseX, mouseY);
			}
		}
		else if (rawHeader.dwType == RIM_TYPEKEYBOARD)
		{
			Keyboard& keyboard             = inputManager.m_keyboard;
			const RAWKEYBOARD& rawKeyboard = rawInput->data.keyboard;

			UINT legacyMessage = rawKeyboard.Message;

			if (legacyMessage == WM_KEYDOWN || legacyMessage == WM_SYSKEYDOWN)
			{
				switch (rawKeyboard.VKey)
				{
				case VK_SHIFT:
				{
					if (IsKeyDown(VK_LSHIFT))
						keyboard.OnKeyPressed(GetSKeyCodes(VK_LSHIFT));
					else if (IsKeyDown(VK_RSHIFT))
						keyboard.OnKeyPressed(GetSKeyCodes(VK_RSHIFT));

					break;
				}
				case VK_MENU:
				{
					if (IsKeyDown(VK_LMENU))
						keyboard.OnKeyPressed(GetSKeyCodes(VK_LMENU));
					else if (IsKeyDown(VK_RMENU))
						keyboard.OnKeyPressed(GetSKeyCodes(VK_RMENU));

					break;
				}
				case VK_CONTROL:
				{
					if (IsKeyDown(VK_LCONTROL))
						keyboard.OnKeyPressed(GetSKeyCodes(VK_LCONTROL));
					else if (IsKeyDown(VK_RCONTROL))
						keyboard.OnKeyPressed(GetSKeyCodes(VK_RCONTROL));

					break;
				}
				}

				keyboard.OnKeyPressed(GetSKeyCodes(rawKeyboard.VKey));
			}
			else if (legacyMessage == WM_KEYUP || legacyMessage == WM_SYSKEYUP)
			{
				switch (rawKeyboard.VKey)
				{
				case VK_SHIFT:
				{
					if (!IsKeyDown(VK_LSHIFT))
						keyboard.OnKeyReleased(GetSKeyCodes(VK_LSHIFT));
					else if (!IsKeyDown(VK_RSHIFT))
						keyboard.OnKeyReleased(GetSKeyCodes(VK_RSHIFT));

					break;
				}
				case VK_MENU:
				{
					if (!IsKeyDown(VK_LMENU))
						keyboard.OnKeyReleased(GetSKeyCodes(VK_LMENU));
					else if (!IsKeyDown(VK_RMENU))
						keyboard.OnKeyReleased(GetSKeyCodes(VK_RMENU));

					break;
				}
				case VK_CONTROL:
				{
					if (!IsKeyDown(VK_LCONTROL))
						keyboard.OnKeyReleased(GetSKeyCodes(VK_LCONTROL));
					else if (!IsKeyDown(VK_RCONTROL))
						keyboard.OnKeyReleased(GetSKeyCodes(VK_RCONTROL));

					break;
				}
				}

				keyboard.OnKeyReleased(GetSKeyCodes(rawKeyboard.VKey));
			}
		}

		break;
	}
	/************* END RAW MESSAGES *************/
	}
}
