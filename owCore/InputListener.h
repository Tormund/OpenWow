#pragma once

#define On_Mouse_Moved(c)        void c::OnMouseMoved(cvec2 _mousePos)
#define On_Mouse_Pressed(c)      bool c::OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos)
#define On_Mouse_Released(c)     bool c::OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos)
#define On_Mouse_WScrolled(c)	 bool c::OnMouseWheel(int _yoffset)
#define On_Keyboard_Pressed(c)   bool c::OnKeyboardPressed(int _key, int _scancode, int _mods)
#define On_Keyboard_Released(c)  bool c::OnKeyboardReleased(int _key, int _scancode, int _mods)
#define On_Character_Printed(c)	 bool c::OnCharInput(uint32 _char)

#define V_MOUSE_MOVED	         virtual void OnMouseMoved(cvec2 _mousePos) override;
#define V_MOUSE_PRESSED	         virtual bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override;
#define V_MOUSE_RELEASE	         virtual bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) override;
#define V_MOUSE_WHEEL	         virtual bool OnMouseWheel(int _yoffset) override;
#define V_KEYBD_PRESSED	         virtual bool OnKeyboardPressed(int _key, int _scancode, int _mods) override;
#define V_KEYBD_RELEASE	         virtual bool OnKeyboardReleased(int _key, int _scancode, int _mods) override;
#define V_CHAR_INPUT	         virtual bool OnCharInput(uint32 _char) override;

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
