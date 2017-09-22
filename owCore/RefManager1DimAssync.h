#pragma once

#include "ContainerAssync.h"

OW_CORE_DLL_API DWORD WINAPI ThreadProc(LPVOID lpParam);

template <class OBJECT_TYPE>
class RefManager1DimAssync
{
public:
	inline void Init()
	{
		m_ObjectsToLoad.Init();

		m_Event_Add = CreateEvent(NULL, TRUE, TRUE, NULL);
		m_Thread_Loader = CreateThread(NULL, 0, &ThreadProc, this, NULL, NULL);
		//SetThreadPriority(m_TextureLoader, THREAD_PRIORITY_TIME_CRITICAL);
	}

	inline void Destroy()
	{
		m_ObjectsToLoad.Destroy();

		ResetEvent(m_Event_Add);
		CloseHandle(m_Event_Add);

		TerminateThread(m_Thread_Loader, 1);
		CloseHandle(m_Thread_Loader);
	}

	inline OBJECT_TYPE* Add(cstring name);

	// Delete
	inline void Delete(cstring name);
	inline void Delete(OBJECT_TYPE* item);

	inline void DeleteAll();

	// Exists
	inline bool Exists(cstring name) const
	{
		return (objects.find(name) != objects.end());
	}
	inline bool IsLoaded(cstring name) const
	{
		return (m_ObjectsToLoad.find(name) != m_ObjectsToLoad.end());
	}

	// Getters
	inline OBJECT_TYPE* GetItemByName(cstring name) const;
	inline string GetNameByItem(OBJECT_TYPE* item) const;

	// Console
	inline void PrintAllInfo();

	//--------------------------------------------------------------

public:
	virtual void LoadAction(string name, OBJECT_TYPE* item) {};

protected:
	virtual OBJECT_TYPE* CreateAction(cstring name) { return nullptr; };
	virtual bool DeleteAction(cstring name) { return false; };

private:
	inline void do_add(cstring name, OBJECT_TYPE* item)
	{
		item->AddRef();

		objects[name] = item;
	}

	inline void pre_delete(OBJECT_TYPE* item)
	{
		string itemName;

		RefItemNamed* itemAsItemNamed = dynamic_cast<RefItemNamed*>(item);
		if (itemAsItemNamed != nullptr)
		{
			itemName = itemAsItemNamed->GetName();
		}
		else
		{
			for (auto it = objects.begin(); it != objects.end(); ++it)
			{
				if (it->second == item)
				{
					itemName = it->first;
					break;
				}
			}
		}

		// Delete action
		DeleteAction(itemName);

		// Delete from arrays
		do_delete(itemName);

		delete item;
	}

	inline void do_delete(cstring name)
	{
		objects.erase(objects.find(name));
	}

	//--------------------------------------------------------------

public:
	map<string, OBJECT_TYPE*> objects; // name - item

protected:

public:
	HANDLE m_Thread_Loader;
	HANDLE m_Event_Add;
	ContainerAssync<string, OBJECT_TYPE*> m_ObjectsToLoad; // name - item
};

#include "RefManager1DimAssync.inl"