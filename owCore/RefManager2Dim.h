#pragma once

#include "RefItem.h"

template <class OBJECT_TYPE, class ID_TYPE>
class RefManager2Dim
{
public:
	inline OBJECT_TYPE* Add(cstring name);

	// Delete

	inline void Delete(cstring name);
	inline void Delete(ID_TYPE id);
	inline void Delete(OBJECT_TYPE* item);

	inline void DeleteAll();

	// Exists

	inline bool Exists(cstring name)
	{
		return (names.find(name) != names.end());
	}

	inline bool ExistsID(ID_TYPE id)
	{
		return (items.find(id) != items.end());
	}

	// Getters

	inline ID_TYPE GetIDByName(cstring name);
	inline OBJECT_TYPE* GetItemByID(ID_TYPE id);
	inline OBJECT_TYPE* GetItemByName(cstring name);

	// Console
	inline void PrintAllInfo();

protected:
	virtual ID_TYPE GenerateID() = 0;
	virtual OBJECT_TYPE* CreateAction(cstring name, ID_TYPE id) = 0;
	virtual bool DeleteAction(cstring name, ID_TYPE id) = 0;

	inline void do_add(cstring name, ID_TYPE id, OBJECT_TYPE* item)
	{
		item->AddRef();

		names[name] = id;
		items[id] = item;
	}

private:
	inline void pre_delete(ID_TYPE id, OBJECT_TYPE* item)
	{
		string itemName;

		RefItemNamed* itemAsItemNamed = dynamic_cast<RefItemNamed*>(item);
		if (itemAsItemNamed != nullptr)
		{
			itemName = itemAsItemNamed->GetName();
		}
		else
		{
			for (auto it = names.begin(); it != names.end(); ++it)
			{
				if (it->second == id)
				{
					itemName = it->first;
					break;
				}
			}
		}

		// Delete action
		DeleteAction(itemName, id);

		// Delete from arrays
		do_delete(itemName, id);

		delete item;
	}

	inline void do_delete(cstring name, ID_TYPE id)
	{
		names.erase(names.find(name));
		items.erase(items.find(id));
	}

public:
	map<string, ID_TYPE> names; // names - id
	map<ID_TYPE, OBJECT_TYPE*> items; //id - item
};

#include "RefManager2Dim.inl"