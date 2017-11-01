#pragma once

class OpenGLAdapter
{
public:
	virtual bool Init() = 0;
	virtual void Destroy() = 0;

	virtual bool SwapWindowBuffers() = 0;
	virtual double GetTime() = 0;

	//

	virtual void SetWindowSize(int32 _width, int32 _height) = 0;
	virtual void SetWindowTitle(cstring _title) = 0;

	virtual void ShowCursor() = 0;
	virtual void HideCursor() = 0;

	virtual void SetMousePosition(cvec2 _mousePosition) = 0;
};