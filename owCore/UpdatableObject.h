#pragma once

class UpdatableObject
{
public:
    virtual void Input(double _Time, double _deltaTime) {};
	virtual void Update(double _Time, double _deltaTime) = 0;

protected:
	bool Register();
	void Unregister();
};