#pragma once

#include "RefItem.h"

template <class OBJECT_TYPE>
class RefManager1Dim
{
public:
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

	// Getters
	inline OBJECT_TYPE* GetItemByName(cstring name) const;
	inline string GetNameByItem(OBJECT_TYPE* item) const;

	// Console
	inline void PrintAllInfo();

protected:
	virtual OBJECT_TYPE* CreateAction(cstring name) = 0;
	virtual bool DeleteAction(cstring name) = 0;

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
			for (auto it : objects)
			{
				if (it.second == item)
				{
					itemName = it.first;
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

public:
	map<string, OBJECT_TYPE*> objects; // name - item
};

#include "RefManager1Dim.inl"