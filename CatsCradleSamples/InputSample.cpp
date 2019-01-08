//This is a sample of the input possibilites and the namespaces I
//attached to them so I could make a simple type out of them for
//parameters. Below those are two examples of input functions.

using VKType = int;
using XIType = WORD;

namespace GamePad
{
	enum GamepadInputs : XIType
	{
		GPADINVALID = 0x0000, //nothing
		DpadUp = 0x0001,
		DpadDown = 0x0002,
		DpadLeft = 0x0004,
		DpadRight = 0x0008,
		
		...
		
	}
}

namespace Keyboard
{
	enum KeyboardInputs : VKType
	{
		KEYBOARDINVALID = 0x07,

		LeftMouseButton = 0x01,
		RightMouseButton = 0x02,
		MiddleMouseButton = 0x04,
		ForwardMouseButton = 0x05,
		BackwardMouseButton = 0x06,

		Backspace = 0x08,
		Tab = 0x09,
		Clear = 0x0C,
		
		...
		
	}
}

...

bool InputCore::IsPressed(VKType Key)
{
	return m_activeCurrentFrame[Key];
}

bool InputCore::IsPressed(XIType Button)
{
	return (m_controllerActiveCurrentFrame.Gamepad.wButtons & Button) != 0;
}

bool InputCore::IsReleased(VKType Key)
{
	return !m_activeCurrentFrame[Key] && m_activeLastFrame[Key];
}

bool InputCore::IsReleased(XIType Button)
{
	return !((m_controllerActiveCurrentFrame.Gamepad.wButtons & Button) != 0) && (m_controllerActiveLastFrame.Gamepad.wButtons & Button) != 0;
}

...


