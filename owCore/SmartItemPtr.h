#pragma once

template< class T > 
class SmartItemPtr
{
public:
	SmartItemPtr(T *ptr = nullptr) : _ptr(ptr) { AddRef(); }
	SmartItemPtr(const SmartItemPtr& smp) : _ptr(smp._ptr) { DelRef(); }
	~SmartItemPtr() { subRef(); }

	T& operator*() const { return *_ptr; }
	T* operator->() const { return _ptr; }
	operator T*() const { return _ptr; }
	operator const T*() const { return _ptr; }
	operator bool() const { return _ptr != nullptr; }
	T* getPtr() const { return _ptr; }

	SmartItemPtr& operator=(const SmartItemPtr& smp)
	{ 
		return *this = smp._ptr; 
	}

	SmartItemPtr& operator=(T *ptr)
	{
		DelRef(); 
		_ptr = ptr; 
		AddRef();
		return *this;
	}

private:
	void AddRef()
	{ 
		if (_ptr != nullptr)
		{
			_ptr->AddRef();
		}
	}
	void DelRef()
	{ 
		if (_ptr != nullptr)
		{
			_ptr->DelRef();
		}
	}

private:
	T  *_ptr;
};