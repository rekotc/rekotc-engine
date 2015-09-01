////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


//////////////
// INCLUDES //
//////////////
#include <dinput.h>
#include "graphicsclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	
	void GetKeyboardState(unsigned char(&)[256]);

	//non so se servono
	void KeyDown(unsigned int);
	void KeyUp(unsigned int);
	bool IsKeyDown(unsigned int);

	bool IsEscapePressed();

	BOOL* GetButtons() { return m_pressedButtons; }

	long GetXDelta()    { return m_mouseState.lX; }
	long GetYDelta()    { return m_mouseState.lY; }
	long GetZDelta()    { return m_mouseState.lZ; }

private:
	bool ReadKeyboard();
	bool ReadMouse();
	

private:
	CameraClass* m_Camera;
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	BOOL                  m_pressedButtons[4];
	DIMOUSESTATE m_mouseState;
	
	HWND                  m_hWnd;

	int m_screenWidth, m_screenHeight;
	

	long                  m_x, m_y;
};

#endif