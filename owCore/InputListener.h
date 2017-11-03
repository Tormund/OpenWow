#pragma once

#define MOUSE_MOVED_(c)  void c::OnMouseMoved(cvec2 _mousePos)
#define MOUSE_PRESSED(c) bool c::OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos)
#define MOUSE_RELEASE(c) bool c::OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos)
#define MOUSE_WHEEL(c)	 bool c::OnMouseWheel(int _yoffset)
#define KEYBD_PRESSED(c) bool c::OnKeyboardPressed(int _key, int _scancode, int _mods)
#define KEYBD_RELEASE(c) bool c::OnKeyboardReleased(int _key, int _scancode, int _mods)
#define CHAR_INPUT(c)	 bool c::OnCharInput(uint32 _char)

#define V_MOUSE_MOVED	virtual void OnMouseMoved(cvec2 _mousePos) override;
#define V_MOUSE_PRESSED	virtual bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override;
#define V_MOUSE_RELEASE	virtual bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) override;
#define V_MOUSE_WHEEL	virtual bool OnMouseWheel(int _yoffset) override;
#define V_KEYBD_PRESSED	virtual bool OnKeyboardPressed(int _key, int _scancode, int _mods) override;
#define V_KEYBD_RELEASE	virtual bool OnKeyboardReleased(int _key, int _scancode, int _mods) override;
#define V_CHAR_INPUT	virtual bool OnCharInput(uint32 _char) override;

class InputListenerObject
{
public:
    virtual void OnMouseMoved(cvec2 _mousePos)
    {
        return;
    }

    virtual bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos)
    {
        return false;
    }

    virtual bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos)
    {
        return false;
    }

    virtual bool OnMouseWheel(int _yoffset)
    {
        return false;
    }

    virtual bool OnKeyboardPressed(int _key, int _scancode, int _mods)
    {
        return false;
    }

    virtual bool OnKeyboardReleased(int _key, int _scancode, int _mods)
    {
        return false;
    }

    virtual bool OnCharInput(uint32 _char)
    {
        return false;
    }

protected:
    bool Register();
    void Unregister();
};
